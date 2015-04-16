#include "Canvas.h"

// Shader sources
static const GLchar* vertexSource = "#version 150 core\n"
    "in vec2 position;"
    "in vec4 color;"
    "out vec4 Color;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "void main() {"
    "   Color = color;"
    "   gl_Position = proj * view * model * vec4(position, 0.0, 1.0);"
    "}";
static const GLchar* fragmentSource = "#version 150\n"
    "in vec4 Color;"
    "out vec4 outColor;"
    "void main() {"
    "    outColor = vec4(Color);"
    "}";
static const GLchar* textureVertexSource = "#version 150 core\n"
    "in vec2 position;"
    "in vec4 color;"
    "in vec2 texcoord;"
    "out vec4 Color;"
    "out vec2 Texcoord;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "void main() {"
    "    Texcoord = texcoord;"
    "   Color = color;"
    "   gl_Position = proj * view * model * vec4(position, 0.0, 1.0);"
    "}";
static const GLchar* textureFragmentSource = "#version 150\n"
    "in vec4 Color;"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D tex;"
    "void main() {"
    "    outColor = texture(tex, Texcoord) * vec4(Color);"
    "}";

std::mutex Canvas::glfwMutex;
int Canvas::drawBuffer = GL_LEFT;

Canvas::Canvas(unsigned int b) {
    init(0, 0, 400*3, 300*3, b, "");
}

Canvas::Canvas(int xx, int yy, int w, int h, unsigned int b, std::string title) {
    init(xx, yy, w, h, b, title);
}

Canvas::~Canvas() {
    // Free our pointer memory
    delete clearRectangle;
    delete myShapes;
    delete myBuffer;
    delete timer;
    delete[] vertexData;
}

void Canvas::bindToButton(Key button, Action a, voidFunction f) {
    boundKeys[button + a * (GLFW_KEY_LAST + 1)] = f;
}

void Canvas::bindToScroll(std::function<void(double, double)> f) {
    scrollFunction = f;
}

void Canvas::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_REPEAT) return;
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    int index = button + action * (GLFW_KEY_LAST + 1);
    if (&(can->boundKeys[index]) != nullptr) if (can->boundKeys[index]) can->boundKeys[index]();
}

void Canvas::clear() {
    toClear = true;
}

int Canvas::close() {
    if (!started) return -1;  // If we haven't even started yet, return error code -1
    renderThread.join();      // Blocks until ready to join, which will be when the window is closed
    return 0;
}

void Canvas::draw() {
    // Reset the window
    glfwSetWindowShouldClose(window, GL_FALSE);

    // Start the drawing loop
    for (framecounter = 0; !glfwWindowShouldClose(window); framecounter++) {
        timer->sleep();


        glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created
        if (toClear) {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            clearRectangle->draw();

            glDrawBuffer(drawBuffer);  // See: http://www.opengl.org/wiki/Default_Framebuffer#Color_buffers

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
            clearRectangle->draw();

            toClear = false;
        } else {
            glDrawBuffer(drawBuffer);
        }

        realFPS = round(1 / timer->getTimeBetweenSleeps());
        if (showFPS) std::cout << realFPS << "/" << FPS << std::endl;
//        fflush(stdout);
        std::cout.flush();

        bufferMutex.lock();  // Time to flush our buffer
        if (myBuffer->size() > 0) {     // But only if there is anything to flush
            for (unsigned int i = 0; i < myBuffer->size(); i++) {
                myShapes->push(myBuffer->operator[](i));
            }
            myBuffer->shallowClear();  // We want to clear the buffer but not delete those objects as we still need to draw them
        }
        bufferMutex.unlock();

        int pos = pointBufferPosition;
        int posLast = pointLastPosition;
        pointLastPosition = pos;

        if (loopAround) {
            glBufferData(GL_ARRAY_BUFFER, (myShapes->capacity() - posLast) * 6 * sizeof(float),
                         &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, myShapes->capacity() - posLast);
            posLast = 0;
            loopAround = false;
        }
        glBufferData(GL_ARRAY_BUFFER, (pos - posLast) * 6 * sizeof(float), &vertexData[posLast * 6],
                     GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pos - posLast);

        unsigned int size = myShapes->size();
//        if (size == myShapes->capacity()) std::cerr << "BUFFER OVERFLOW" << std::endl;
        for (unsigned int i = 0; i < size; i++) {
            if (!myShapes->operator[](i)->getIsTextured()) {
                myShapes->operator[](i)->draw();  // Iterate through our queue until we've made it to the end
            } else {
                textureShaders(true);
                myShapes->operator[](i)->draw();
                textureShaders(false);
            }
        }

        // Update our screenBuffer copy with the screen
        glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
        if (toRecord > 0) {
            toRecord--;
            screenShot();
        }

        myShapes->clear();                           // Clear our buffer of shapes to be drawn
        glFlush();
        glDrawBuffer(GL_BACK_LEFT);
        glfwSwapBuffers(window);                     // Swap out GL's back buffer and actually draw to the window

        glfwPollEvents();                            // Handle any I/O
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwMakeContextCurrent(NULL);                // We're drawing to window as soon as it's created

        if (toClose) glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Canvas::drawCircle(int x, int y, int radius, int res, ColorFloat color, bool filled) {
    float delta = 2.0f / res * 3.1415926585f;
    float oldX = 0, oldY = 0, newX = 0, newY = 0;
    if (filled) {
        ColoredPolygon *s = new ColoredPolygon(res*3);
        for (int i = 0; i <= res; i++ ) {
            oldX = newX; oldY = newY;
            newX = x+radius*cos(i*delta);
            newY = y+radius*sin(i*delta);
            if (i > 0) {
                s->addVertex(x,y,color);
                s->addVertex(oldX, oldY,color);
                s->addVertex(newX, newY,color);
            }
        }
        drawShape(s);
    } else {
        Polyline *p = new Polyline(res+1);
        for (int i = 0; i <= res; i++ ) {
            oldX = newX; oldY = newY;
            newX = x+radius*cos(i*delta);
            newY = y+radius*sin(i*delta);
            if (i > 0)
                p->addNextVertex(oldX, oldY,color);
        }
        p->addNextVertex(newX, newY,color);
        drawShape(p);
    }
}

void Canvas::drawColoredPolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    if (filled) {
        ColoredPolygon* p = new ColoredPolygon(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(x[i], y[i], color[i]);
        }
        drawShape(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size);
        for (int i = 0; i < size; i++) {
            p->addNextVertex(x[i], y[i], color[i]);
        }
        drawShape(p);  // Push it onto our drawing buffer
    }
}

void Canvas::drawImage(std::string fname, int x, int y, int w, int h, float a) {
//    glfwMakeContextCurrent(window);                       // We're drawing to window as soon as it's created
    Image* im = new Image(fname, loader, x, y, w, h, a);  // Creates the Image with the specified coordinates
    drawShape(im);                                        // Push it onto our drawing buffer
//    glfwMakeContextCurrent(NULL);                         // We're drawing to window as soon as it's created
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorFloat color) {
    Line* l = new Line(x1, y1, x2, y2, color);  // Creates the Line with the specified coordinates and color
    drawShape(l);                               // Push it onto our drawing buffer
}

inline void Canvas::drawPixel(int row, int col, ColorFloat color) {
    drawPoint(col, row, color);
}

void Canvas::drawPoint(int x, int y, ColorFloat color) {
    pointArrayMutex.lock();
    if (pointBufferPosition >= myShapes->capacity()) {
        loopAround = true;
        pointBufferPosition = 0;
    }
    int tempPos = pointBufferPosition * 6;
    pointBufferPosition++;
    // mlock.unlock();

    vertexData[tempPos] = x;
    vertexData[tempPos + 1] = y;
    vertexData[tempPos + 2] = color.R;
    vertexData[tempPos + 3] = color.G;
    vertexData[tempPos + 4] = color.B;
    vertexData[tempPos + 5] = color.A;
    pointArrayMutex.unlock();
}

void Canvas::drawRectangle(int x, int y, int w, int h, ColorFloat color, bool filled) {
    if (filled) {
        Rectangle* rec = new Rectangle(x, y, w, h, color);  // Creates the Rectangle with the specified coordinates and color
        drawShape(rec);                                     // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(5);
        p->addNextVertex(x, y, color);
        p->addNextVertex(x + w - 1, y, color);
        p->addNextVertex(x + w - 1, y + h - 1, color);
        p->addNextVertex(x, y + h - 1, color);
        p->addNextVertex(x, y, color);
        drawShape(p);
    }
}

void Canvas::drawShape(Shape* s) {
    bufferMutex.lock();
    myBuffer->push(s);  // Push it onto our drawing buffer
    bufferMutex.unlock();
}

void Canvas::drawText(std::wstring s, int x, int y, unsigned int size, ColorFloat color) {
    Text* t = new Text(s, loader, x, y, size, color);  // Creates the Point with the specified coordinates and color
    drawShape(t);                                // Push it onto our drawing buffer
}

void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled) {
    if (filled) {
        Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color);  // Creates the Triangle with the specified vertices and color
        drawShape(t);                                               // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(4);
        p->addNextVertex(x1,y1,color);
        p->addNextVertex(x2,y2,color);
        p->addNextVertex(x3,y3,color);
        p->addNextVertex(x1,y1,color);
        drawShape(p);
    }
}

void Canvas::end() {
    toClose = true;
}

void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
}

int Canvas::getFrameNumber() {
    return framecounter;
}

float Canvas::getFPS() {
    return realFPS;
}

bool Canvas::getIsOpen() {
    return !isFinished;
}

int Canvas::getMouseX() {
    return mouseX;
}

int Canvas::getMouseY() {
    return mouseY;
}

ColorInt Canvas::getPixel(int row, int col) {
    return getPoint(col,row);
//    int padding = winWidth % 4;  // Apparently, the array is automatically padded to four bytes. Go figure.
//    int offset = 3 * (row * winWidth + col) + padding * row;
//    return ColorInt(screenBuffer[offset],
//                    screenBuffer[offset + 1],
//                    screenBuffer[offset + 2],
//                    255);
}

ColorInt Canvas::getPoint(int x, int y) {
    int padding = winWidth % 4;  // Apparently, the array is automatically padded to four bytes. Go figure.
    int yy = winHeight - y;      // TODO: glReadPixels starts from the bottom left, and we have no way to change that...
    int offset = 3 * (yy * winWidth + x) + padding * yy;
    return ColorInt(screenBuffer[offset],
                    screenBuffer[offset + 1],
                    screenBuffer[offset + 2],
                    255);
}

uint8_t* Canvas::getScreenBuffer() {
    return screenBuffer;
}

double Canvas::getTime() {
    return timer->getTime();
}

int Canvas::getWindowWidth() {
    return winWidth;
}

int Canvas::getWindowHeight() {
    return winHeight;
}

int Canvas::getWindowX() {
    return monitorX;
}

int Canvas::getWindowY() {
    return monitorY;
}

void Canvas::glDestroy() {
    // Free up our resources
    glDetachShader(shaderProgram, shaderFragment);
    glDetachShader(shaderProgram, shaderVertex);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteProgram(shaderProgram);
    glDetachShader(textureShaderProgram, textureShaderFragment);
    glDetachShader(textureShaderProgram, textureShaderVertex);
    glDeleteShader(textureShaderFragment);
    glDeleteShader(textureShaderVertex);
    glDeleteProgram(textureShaderProgram);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
}

void Canvas::glInit() {
    glfwSetErrorCallback(errorCallback);

    // Create a Window and the Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set target GL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // Set target GL minor version to 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We're using the standard GL Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Don't use methods that are deprecated in the target version
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first

    glfwMutex.lock();                                  // GLFW crashes if you try to make more than once window at once
    window = glfwCreateWindow(winWidth, winHeight, title_.c_str(), NULL, NULL);  // Windowed
    if (!window) {
        fprintf(stderr, "GLFW window creation failed. Was the library correctly initialized?\n");
    }
    glfwMutex.unlock();

    const GLFWvidmode* monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
    if (!monInfo) {
        fprintf(stderr, "GLFW failed to return monitor information. Was the library correctly initialized?\n");
    }
    glfwSetWindowPos(window, (monInfo->width - winWidth) / 2, (monInfo->height - winHeight) / 2);

    glfwMakeContextCurrent(window);         // We're drawing to window as soon as it's created
    glfwShowWindow(window);                 // Show the window
    glfwSetWindowUserPointer(window, this);

    // Enable and disable necessary stuff
    glDisable(GL_DEPTH_TEST);                           // Disable depth testing because we're not drawing in 3d
    glDisable(GL_DITHER);                               // Disable dithering because pixels do not (generally) overlap
    glDisable(GL_CULL_FACE);                            // Disable culling because the camera is stationary
    glEnable(GL_BLEND);                                 // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blending mode to standard alpha blending

#ifdef DEBUG
    printf("Vendor:         %s %s\n", glGetString(GL_VENDOR), glGetString(GL_RENDERER));
    printf("OpenGL version: %s\n", glGetString(GL_VERSION));
    printf("GLFW version:   %s\n", glfwGetVersionString());
#endif

    // Needed?
//    glEnable(GL_TEXTURE_2D);
//    glShadeModel(GL_FLAT);

    // Enable Experimental GLEW to Render Properly
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
    }

    GLint status;

    // Create and bind our Vertex Array Object
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Create and bind our Vertex Buffer Object
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

    // Create / compile vertex shader
    shaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(shaderVertex, 1, &vertexSource, NULL);
    glCompileShader(shaderVertex);
    glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &status);

    // Create / compile fragment shader
    shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(shaderFragment, 1, &fragmentSource, NULL);
    glCompileShader(shaderFragment);
    glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &status);

    // Attach both shaders to a shader program, link the program
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, shaderVertex);
    glAttachShader(shaderProgram, shaderFragment);
    glBindFragDataLocation(shaderProgram, 0, "outColor");

    // Specify the layout of the vertex data in our standard shader
    glLinkProgram(shaderProgram);

    // Create / compile textured vertex shader
    textureShaderVertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(textureShaderVertex, 1, &textureVertexSource, NULL);
    glCompileShader(textureShaderVertex);
    glGetShaderiv(textureShaderVertex, GL_COMPILE_STATUS, &status);

    // Create / compile textured fragment shader
    textureShaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(textureShaderFragment, 1, &textureFragmentSource, NULL);
    glCompileShader(textureShaderFragment);
    glGetShaderiv(textureShaderFragment, GL_COMPILE_STATUS, &status);

    // Attach both shaders to another shader program, link the program
    textureShaderProgram = glCreateProgram();
    glAttachShader(textureShaderProgram, textureShaderVertex);
    glAttachShader(textureShaderProgram, textureShaderFragment);
    glBindFragDataLocation(textureShaderProgram, 0, "outColor");

    // Specify the layout of the vertex data in our textured shader
    glLinkProgram(textureShaderProgram);

    textureShaders(false);

    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

    bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this]() {
        glfwSetWindowShouldClose(window, GL_TRUE);
    });

    glfwMakeContextCurrent(NULL);   // Reset the context
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
}

void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title) {
    title_ = title;
    winWidth = ww, winHeight = hh;
    aspect = (float) winWidth / winHeight;
    keyDown = false;
    toClose = false;
    framecounter = 0;

    unsigned buffersize = 3 * (winWidth+(winWidth%4)) * winHeight;
    screenBuffer = new uint8_t[buffersize];
    for (unsigned i = 0; i < buffersize; ++i)
      screenBuffer[i] = 0;

    toClear = true;                   // Don't need to clear at the start
    started = false;                  // We haven't started the window yet
    monitorX = xx;
    monitorY = yy;
    winWidth = ww;
    winHeight = hh;                   // Initialize translation
    myShapes = new Array<Shape*>(b);  // Initialize myShapes
    myBuffer = new Array<Shape*>(b);
    vertexData = new float[6 * b];    // Buffer for vertexes for points
    showFPS = false;                  // Set debugging FPS to false
    isFinished = false;               // We're not done rendering
    pointBufferPosition = pointLastPosition = 0;
    loopAround = false;
    toRecord = 0;

    clearRectangle = new Rectangle(0, 0, winWidth, winHeight, GREY);

    timer = new Timer(FRAME);
    for (int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
        boundKeys[i++] = nullptr;
}

void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.bmp", framecounter);  // TODO: Make this save somewhere not in root

    loader.saveImageToFile(filename, screenBuffer, winWidth, winHeight);
}

void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    if (can->scrollFunction) can->scrollFunction(xpos, ypos);
}

void Canvas::setBackgroundColor(ColorFloat color) {
    delete clearRectangle;
    clearRectangle = new Rectangle(0, 0, winWidth, winHeight, color);
}

void Canvas::setDrawBuffer(int buffer) {
	Canvas::drawBuffer = buffer;
}

void Canvas::setFont(std::string filename) {
    loader.loadFont(filename);
}

void Canvas::setShowFPS(bool b) {
    showFPS = b;
}

void Canvas::SetupCamera() {
//    static float time = 0;
//    // Set up camera positioning
//    glm::mat4 view = glm::lookAt(
//        glm::vec3(cameraPanX, cameraPanY, -cameraDistance),  // Camera position
//        glm::vec3(cameraPanX, cameraPanY, 0.0f),  // On-screen center
//        glm::vec3(-0.0f, -1.0f, 0.0f)             // "Up" axis (y = 0.01 because undefined when same as camera position)
//    );
//    glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
//    // Set up camera zooming
//    glm::mat4 proj = glm::perspective(90.0f, 800.0f / 600.0f, 0.01f, 10000.0f);
//    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
//    // Set up camera transformation
//    glm::mat4 model;                 //Create a new (identity matrix)
//    model = glm::rotate(
//        model,                       //Rotate the model (identity) matrix...
//        0.0f,                        //...actually, don't rotate it at all
//        glm::vec3(0.0f, 0.0f, 1.0f)  //...along the Z-axis.
//    );
//    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

    // Set up camera positioning
    // Note: (winWidth-1) is a dark voodoo magic fix for some camera issues
    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, -(winWidth-1) / 2.0f, winHeight / 2.0f, -winHeight / 2.0f,
        1 };
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

    // Set up camera zooming
    float projF[] = { 1.0f / aspect, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.00000191f, -1, 0, 0, -0.02000002f, 0 };
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

    // Set up camera transformation
    float modelF[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
}

int Canvas::start() {
    if (started) return -1;
    started = true;
    renderThread = std::thread(Canvas::startDrawing, this);  // Spawn the rendering thread
    return 0;
}

void Canvas::startDrawing(Canvas *c) {
    c->glInit();
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
}

void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
}

void Canvas::stopRecording() {
    toRecord = 0;
}

void Canvas::takeScreenShot() {
    if (toRecord == 0) toRecord = 1;
}

void Canvas::textureShaders(bool on) {
    GLint program;
    if (!on) {
        program = shaderProgram;

        // Relocate the shader attributes
        GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
        GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));

    } else {
        program = textureShaderProgram;

        // Relocate the shader attributes
        GLint texturePosAttrib = glGetAttribLocation(textureShaderProgram, "position");
        glEnableVertexAttribArray(texturePosAttrib);
        glVertexAttribPointer(texturePosAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
        GLint textureColAttrib = glGetAttribLocation(textureShaderProgram, "color");
        glEnableVertexAttribArray(textureColAttrib);
        glVertexAttribPointer(textureColAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*) (2 * sizeof(float)));
        GLint textureTexAttrib = glGetAttribLocation(textureShaderProgram, "texcoord");
        glEnableVertexAttribArray(textureTexAttrib);
        glVertexAttribPointer(textureTexAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
                              (void*) (6 * sizeof(float)));
    }

    // Reallocate the shader program for use
    glUseProgram(program);

    // Recompute the camera matrices
    uniModel = glGetUniformLocation(program, "model");
    uniView = glGetUniformLocation(program, "view");
    uniProj = glGetUniformLocation(program, "proj");

    // Update the camera
    SetupCamera();
}
