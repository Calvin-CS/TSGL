#include "Canvas.h"

namespace tsgl {

// Shader sources
static const GLchar* vertexSource =
    "#version 150 core\n"
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
static const GLchar* fragmentSource =
    "#version 150\n"
    "in vec4 Color;"
    "out vec4 outColor;"
    "void main() {"
    "    outColor = vec4(Color);"
    "}";
static const GLchar* textureVertexSource =
    "#version 150 core\n"
    "in vec2 position;"
    "in vec4 color;"
    "in vec2 texcoord;"
    "out vec4 Color;"
    "out vec2 Texcoord;"
    "uniform mat4 model;"
    "uniform mat4 view;"
    "uniform mat4 proj;"
    "void main() {"
    "   Texcoord = texcoord;"
    "   Color = color;"
    "   gl_Position = proj * view * model * vec4(position, 0.0, 1.0);"
    "}";
static const GLchar* textureFragmentSource =
    "#version 150\n"
    "in vec4 Color;"
    "in vec2 Texcoord;"
    "out vec4 outColor;"
    "uniform sampler2D tex;"
    "void main() {"
    "    outColor = texture(tex, Texcoord) * vec4(Color);"
    "}";

std::mutex Canvas::glfwMutex;
int Canvas::drawBuffer = GL_FRONT_LEFT;
unsigned Canvas::openCanvases = 0;

//Negative timerLength
Canvas::Canvas(double timerLength) {
    int w = 1200, h = 900;
    init(0, 0, w, h, w*h*2, "", timerLength);
}

Canvas::Canvas(int xx, int yy, int w, int h, std::string title, double timerLength) {
    init(xx, yy, w, h, w*h*2, title, timerLength);
}

Canvas::~Canvas() {
    // Free our pointer memory
    delete myShapes;
    delete myBuffer;
    delete drawTimer;
    delete[] vertexData;
    if (--openCanvases == 0)
        glfwTerminate();  // Terminate GLFW
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

void Canvas::stop() {
    close();
    wait();
}

int Canvas::wait() {
    if (!started) return -1;  // If we haven't even started yet, return error code -1
  #ifndef __APPLE__
    renderThread.join();
  #endif
    return 0;
}

void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    TsglDebug("Window closed successfully.");
}

void Canvas::draw() {
    // Reset the window
    glfwSetWindowShouldClose(window, GL_FALSE);

    if (hasStereo)
      Canvas::setDrawBuffer(hasBackbuffer ? GL_FRONT_AND_BACK : GL_FRONT);
    else
      Canvas::setDrawBuffer(hasBackbuffer ? GL_LEFT : GL_FRONT_LEFT);

    setBackgroundColor(bgcolor); //Set our initial clear / background color

    // Start the drawing loop
    for (framecounter = 0; !glfwWindowShouldClose(window); framecounter++) {
        drawTimer->sleep(true);

      #ifdef __APPLE__
        windowMutex.lock();
      #endif
        glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created
        if (toClear) glClear(GL_COLOR_BUFFER_BIT);
        toClear = false;

        realFPS = round(1 / drawTimer->getTimeBetweenSleeps());
        if (showFPS) std::cout << realFPS << "/" << FPS << std::endl;
        std::cout.flush();

        bufferMutex.lock();  // Time to flush our buffer
        if (myBuffer->size() > 0) {     // But only if there is anything to flush
            for (unsigned int i = 0; i < myBuffer->size(); i++)
              myShapes->push((*myBuffer)[i]);
            myBuffer->shallowClear();  // We want to clear the buffer but not delete those objects as we still need to draw them
        }
        bufferMutex.unlock();

        int pos = pointBufferPosition;
        int posLast = pointLastPosition;
        pointLastPosition = pos;

        if (loopAround) {
            int toend = myShapes->capacity() - posLast;
            glBufferData(GL_ARRAY_BUFFER, toend * 6 * sizeof(float),
                         &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, toend);
            posLast = 0;
            loopAround = false;
        }
        int pbsize = pos - posLast;
        glBufferData(GL_ARRAY_BUFFER, pbsize * 6 * sizeof(float), &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
        glDrawArrays(GL_POINTS, 0, pbsize);

        unsigned int size = myShapes->size();
        for (unsigned int i = 0; i < size; i++) {
            if (!(*myShapes)[i]->getIsTextured()) {
                (*myShapes)[i]->draw();  // Iterate through our queue until we've made it to the end
            } else {
                textureShaders(true);
                (*myShapes)[i]->draw();
                textureShaders(false);
            }
        }

        // Update our screenBuffer copy with the screen
        glReadPixels(0, 0, winWidth, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
        if (toRecord > 0) {
          screenShot();
          --toRecord;
        }

        myShapes->clear();                           // Clear our buffer of shapes to be drawn
        glFlush();                                   // Flush buffer data to the actual draw buffer
        glfwSwapBuffers(window);                     // Swap out GL's back buffer and actually draw to the window

      #ifndef __APPLE__
        glfwPollEvents();                            // Handle any I/O
      #endif
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwMakeContextCurrent(NULL);                // We're drawing to window as soon as it's created
      #ifdef __APPLE__
        windowMutex.unlock();
      #endif

        if (toClose) glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

//Workaround for OS X
void Canvas::handleIO() {
  #ifdef __APPLE__
	if (pthread_main_np() == 0)
		return;  //If we're not the main thread, we can't call this
	windowMutex.lock();
	glfwMakeContextCurrent(window);
	glfwPollEvents();
	glfwMakeContextCurrent(NULL);
	windowMutex.unlock();
  #endif
}

//Negative radii?
//Invalid color?
void Canvas::drawCircle(int x, int y, int radius, int res, ColorFloat color, bool filled) {
    float delta = 2.0f / res * 3.1415926585f;
    if (filled) {
        ConvexPolygon *s = new ConvexPolygon(res);
        for (int i = 0; i < res; ++i)
          s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color);
          drawShape(s);
    } else {
        float oldX = 0, oldY = 0, newX = 0, newY = 0;
        Polyline *p = new Polyline(res+1);
        for (int i = 0; i <= res; ++i) {
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

//Negative size?
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

//Negative size?
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    if (filled) {
        ConcavePolygon* p = new ConcavePolygon(size);
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

//Negative size?
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    if (filled) {
        ConvexPolygon* p = new ConvexPolygon(size);
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

//Invalid width and/or height?
void Canvas::drawImage(std::string fname, int x, int y, int w, int h, float a) {
    Image* im = new Image(fname, loader, x, y, w, h, a);  // Creates the Image with the specified coordinates
    drawShape(im);                                        // Push it onto our drawing buffer
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

    vertexData[tempPos] = x;
    vertexData[tempPos + 1] = y;
    vertexData[tempPos + 2] = color.R;
    vertexData[tempPos + 3] = color.G;
    vertexData[tempPos + 4] = color.B;
    vertexData[tempPos + 5] = color.A;
    pointArrayMutex.unlock();
}

void Canvas::drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color, bool filled) {
    if (filled) {
        if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
        if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
        Rectangle* rec = new Rectangle(x1, y1, x2-x1, y2-y1, color);  // Creates the Rectangle with the specified coordinates and color
        drawShape(rec);                                     // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(5);
        p->addNextVertex(x1, y1, color);
        p->addNextVertex(x1, y2, color);
        p->addNextVertex(x2, y2, color);
        //TODO: This should not have to be decremented by 1...
        p->addNextVertex(x2, y1, color);
        p->addNextVertex(x1, y1, color);
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

ColorFloat Canvas::getBackgroundColor() {
  return bgcolor;
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
    return drawTimer->getTime();
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
    glfwMakeContextCurrent(window);         // We're drawing to window as soon as it's created
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

    bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this]() {
        glfwSetWindowShouldClose(window, GL_TRUE);
    });

    unsigned char stereo[1] = {5}, dbuff[1] = {5};
    int aux[1] = {5};
    glGetBooleanv(GL_STEREO,stereo);
    glGetBooleanv(GL_DOUBLEBUFFER,dbuff);
    glGetIntegerv(GL_AUX_BUFFERS,aux);
    hasStereo = ((int)stereo[0] > 0);
    hasBackbuffer = ((int)dbuff[0] > 0);

    glfwMakeContextCurrent(NULL);   // Reset the context
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
}

void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title, double timerLength) {
    if (openCanvases == 0)
        glfwInit();  // Initialize GLFW
    ++openCanvases;

    title_ = title;
    winWidth = ww, winHeight = hh;
    aspect = (float) winWidth / winHeight;
    keyDown = false;
    toClose = false;
    framecounter = 0;

    int padwidth = winWidth % 4;
    if (padwidth > 0)
       padwidth = 4-padwidth;
    unsigned buffersize = 3 * (winWidth+padwidth+1) * winHeight;
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

    bgcolor = GREY;
    window = nullptr;

    if (timerLength == 0.0f) {
      drawTimer = new Timer(FRAME);
//      drawTimer = new Timer(FRAME);
    } else {
      drawTimer = new Timer(timerLength);
//      drawTimer = new Timer(timerLength);
    }

    for (int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
        boundKeys[i++] = nullptr;

    glfwSetErrorCallback(errorCallback);

     // Create a Window and the Context
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set target GL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // Set target GL minor version to 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We're using the standard GL Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Don't use methods that are deprecated in the target version
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);                    // Disable the back buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first

    monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());

    glfwMutex.lock();                                  // GLFW crashes if you try to make more than once window at once
    window = glfwCreateWindow(winWidth, winHeight, title_.c_str(), NULL, NULL);  // Windowed
 //   window = glfwCreateWindow(monInfo->width, monInfo->height, title_.c_str(), glfwGetPrimaryMonitor(), NULL);  // Fullscreen
    if (!window) {
        fprintf(stderr, "GLFW window creation failed. Was the library correctly initialized?\n");
    }
    glfwMutex.unlock();

    if (!monInfo) {
        fprintf(stderr, "GLFW failed to return monitor information. Was the library correctly initialized?\n");
    }
    if (monitorX == -1)
      monitorX = (monInfo->width - winWidth) / 2;
    if (monitorY == -1)
      monitorY = (monInfo->height - winHeight) / 2;
    glfwSetWindowPos(window, monitorX, monitorY);

    glfwMakeContextCurrent(window);
    glfwShowWindow(window);                 // Show the window
    glfwSetWindowUserPointer(window, this);

    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);

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

    glfwMakeContextCurrent(NULL);   // Reset the context
}

void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.png", framecounter);  // TODO: Make this save somewhere not in root

    loader.saveImageToFile(filename, screenBuffer, winWidth, winHeight);
}

void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    if (can->scrollFunction) can->scrollFunction(xpos, ypos);
}

void Canvas::setBackgroundColor(ColorFloat color) {
    bgcolor = color;
    if (window != nullptr) {
      glfwMakeContextCurrent(window);
      glClearColor(color.R,color.G,color.B,color.A);
      glfwMakeContextCurrent(NULL);
    }
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

void Canvas::setupCamera() {
    // Set up camera positioning
    // Note: (winWidth-1) is a dark voodoo magic fix for some camera issues
    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0, -(winWidth-1) / 2.0f, winHeight / 2.0f, -winHeight / 2.0f,
        1 };
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

    // Set up camera zooming
    float projF[] = { 1.0f / aspect, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.0f, -1, 0, 0, -0.02f, 0 };
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

    // Set up camera transformation
    float modelF[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
}

int Canvas::start() {
    if (started) return -1;
    started = true;
  #ifdef __APPLE__
    pthread_create(&renderThread,NULL,startDrawing,(void*)this);
  #else
    renderThread = std::thread(Canvas::startDrawing, this);  // Spawn the rendering thread
  #endif
    return 0;
}

#ifdef __APPLE__
void* Canvas::startDrawing(void* cPtr) {
    Canvas* c = (Canvas*)cPtr;
    c->glInit();
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
    pthread_exit(NULL);
}
#else
void Canvas::startDrawing(Canvas *c) {
    c->glInit();
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
}
#endif

void Canvas::sleep() {
  #ifdef __APPLE__
	handleIO();
  #endif
    drawTimer->sleep(false);
}

void Canvas::sleepFor(float seconds) {
#ifdef __APPLE__
	handleIO();
#endif
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (seconds * 1000000000)));
}

void Canvas::reset() {
    drawTimer->reset();
}

unsigned int Canvas::getReps() const {
    return drawTimer->getReps();
}

double Canvas::getTimeBetweenSleeps() const {
    return drawTimer->getTimeBetweenSleeps();
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
    setupCamera();
}

//-----------------Unit testing-------------------------------------------------------
void Canvas::runTests() {
  TsglDebug("Testing Canvas class...");
  Canvas c1(0, 0, 500, 500, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  tsglAssert(testFilledDraw(c1), "Unit test for filled draw failed!");
//  tsglAssert(testPerimeter(c1), "Unit test for non filled draw failed!");
  tsglAssert(testLine(c1), "Unit test for line failed!");
  tsglAssert(testAccessors(c1), "Unit test for accessors failed!");
  tsglAssert(testDrawImage(c1), "Unit test for drawing images failed!");
  c1.stop();
  TsglDebug("Unit tests for Canvas complete.");
  std::cout << std::endl;
}

//Similar format is used for the remaining unit tests
bool Canvas::testFilledDraw(Canvas& can) {
  int passed = 0;   //Passed tests
  int failed = 0;   //Failed tests
  ColorInt red(255, 0, 0);   //Fill color
  can.drawCircle(250, 250, 50, 32, red, true);  //Draw filled shape
  can.sleepFor(1);

  //Test 1: Get middle pixel and see if its red.
  if(can.getPixel(250, 250) == red) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, middle pixel for testFilledDraw() failed!");
  }

  //Test 2: Get leftmost and rightmost pixel of the circle
  //Have to add or subtract 1 from the y so that you can get the correct pixel (center radius is 1. No 0 radius).
  if(can.getPixel(201, 250) == red && can.getPixel(299, 250) == red) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 2, leftmost and rightmost pixel for testFilledDraw() failed!");
  }

  //Test 3: Outside pixels shouldn't equal inside pixels
  int test = 0;
  //Single pixel....
  if(can.getPixel(1, 1) != red) {
    //Multiple pixels....
    for(int i = 201; i <= 299; i++) {
      if(can.getPixel(1, i) != red) {
        test++;
      }
    }
    //Results of multiple pixels...
    if(test == 99) {
      passed++;
    } else {
    failed++;
    TsglErr("Test 3, outside != inside, Multiple pixels for testFilledDraw() failed!");
  }
  } else {
    failed++;
    TsglErr("Test 3, outside != inside, Single pixel for testFilledDraw() failed!");
  }

  //Test 4: A LOT of the pixels on the inside should be red
  int count = 0;
  for(int i = 201; i <= 299; i++) {
    if(can.getPixel(i, 250) == red) {
      count++;
    }
  }

  //Now check the count, should be 99
  if(count == 99) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 4, multiple pixels for testFilledDraw() failed!");
  }

  //Determine if we passed all four tests or not, Results:
  if(passed == 4 && failed == 0) {
    can.clear();
    TsglDebug("Unit test for drawing filled shapes passed!");
    return true;
  } else {
    can.clear();
    TsglErr("This many passed for testFilledDraw(): ");
    std::cerr << " " << passed << std::endl;
    TsglErr("This many failed for testFilledDraw(): ");
    std::cerr << " " << failed << std::endl;
    return false;
  }
}

bool Canvas::testPerimeter(Canvas& can) {
  int passed = 0;  //Passed tests
  int failed = 0;  //Failed tests
  can.drawRectangle(200, 350, 300, 400, BLACK, false);   //Test 1
  can.drawCircle(250, 250, 50, 32, BLACK, false);  //Test 2
  can.drawTriangle(50, 80, 150, 80, 50, 150, BLACK, false);  //Test 3
  can.sleepFor(1);
  ColorInt black(0, 0, 0);

  //Test 1: Rectangle
  //Four corners make a rectangle, so check the corners, then perimeter.
  //Interesting...it appears as if though sometimes the exact coordinate works and sometimes I have to either subtract 1 from one of them or add 1.
  //Not sure if that's a bug, or if i'm missing something.
  //Rectangle should look like this:
  //  (200, 350)-------------------------------(300, 350)
  //            |                             |
  //            |                             |
  //            |                             |
  //            |                             |
  //            |                             |
  //            |                             |
  //            |                             |
  //            |                             |
  //  (200, 400)-------------------------------(300, 400)
  // The call: can.drawRectangle(200, 350, 300, 400, BLACK, false);
  if(can.getPoint(200, 350) == black && can.getPoint(200, 400) == black && can.getPoint(300, 350) == black && can.getPoint(300, 400) == black) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, Four corners for testPerimeter() failed!");
  }

  //Left to right, top
  int topCount = 0;
  for(int i = 200; i <= 300; i++) {
    if(can.getPoint(i, 350) == black) {
      topCount++;
    }
  }

  //Results of Left to right top
  if(topCount == 101) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, Left to right, top for testPerimeter() failed!");
  }

  //Top to bottom, left
  int leftCount = 0;
  for(int j = 350; j <= 400; j++) {
    if(can.getPoint(200, j) == black) {
      leftCount++;
    }
  }

  //Results of Top to bottom, left
  if(leftCount == 51) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, Top to bottom, left for testPerimeter() failed!");
  }

  //Left to right, bottom
  int bottomCount = 0;
  for(int k = 200; k <= 300; k++) {
    if(can.getPoint(k, 400) == black) {
      bottomCount++;
    }
  }

  //Results of Left to right, bottom
  if(bottomCount == 101) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 1, Left to right, bottom for testPerimeter() failed!");
  }

  //Top to bottom, right
  int rightCount = 0;
  for(int l = 350; l <= 400; l++) {
    if(can.getPoint(300, l) == black) {
      rightCount++;
    }
  }

  //Results of Top to bottom, right
  if(rightCount == 51) {
   passed++;
  } else {
   failed++;
   TsglErr("Test 1, Top to bottom, right for testPerimeter() failed!");
  }

  //Test 2: Circle
  //Check the leftmost, rightmost, top, and bottom coordinates.
  //They should all be the same color
  if(can.getPoint(250, 200) == black && can.getPoint(250, 300) == black && can.getPoint(200, 250) == black && can.getPoint(300, 250) == black) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 2 for testPerimeter() failed!");
  }

  //Test 3: Triangle
  //Check the vertices, and a point in from their line segments
  //NOTE: The vertices themselves aren't necessarily drawn, as the points taper off due to the sharp
  //  angles of the triangle (Unless we are dealing with a right triangle).
  //Vertices
  if(can.getPoint(50, 80) == black && can.getPoint(150, 80) == black && can.getPoint(50, 150) == black) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 3 for testPerimeter() failed!");
  }

  //Point from line segment (Test 3, part 2)
  if(can.getPoint(50, 109) == black && can.getPoint(100, 80) == black && can.getPoint(100, 115) == black) {
    passed++;
  } else {
    failed++;
    TsglErr("Test 3, part 2 for testPerimeter() failed!");
  }

  //Results:
  if(passed == 8 && failed == 0) {
  //  can.clear();
    TsglDebug("Unit test for drawing non-filled shapes passed!");
    return true;
  } else {
  //  can.clear();
    TsglErr("This many passed for testPerimeter(): ");
    std::cerr << " " << passed << std::endl;
    TsglErr("This many failed for testPerimeter(): ");
    std::cerr << " " << failed << std::endl;
    return false;
  }
}

bool Canvas::testLine(Canvas & can) {
   int passed = 0;
   int failed = 0;
   can.drawLine(0, 0, 250, 250, BLACK);  //Diagonal line
   can.drawLine(253, 253, 400, 253);  //Straight line
   can.sleepFor(1);
   ColorInt black(0, 0, 0);
   //Test 1: Near the ending endpoint? (Diagonal)
   if(can.getPoint(249, 250) == black) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 1, Near the ending endpoint? for testLine() failed!");
   }

   //Test 2: Somewhere in the middle? (Diagonal)
   if(can.getPoint(154, 155) == black) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 2, Somewhere in the middle? for testLine() failed!");
   }

   //Test 3: Near the starting endpoint? (Diagonal)
   if(can.getPoint(14, 15) == black) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 3, Near the starting endpoint? for testLine() failed!");
   }

   //Test 4: An entire line? (Straight)
   int count = 0;
   for(int i = 253; i <= 399; i++) {
     if(can.getPoint(i, 253) == black) {
       count++;
     }
   }

   //Check the results of the Straight line test
   if(count == 147) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 4, An entire line? (Straight) for testLine() failed!");
   }

   //Results:
   if(passed == 4 && failed == 0) {
     can.clear();
     TsglDebug("Unit test for line passed!");
     return true;
   } else {
     can.clear();
     TsglErr("This many passed testLine(): ");
     std::cerr << " " << passed << std::endl;
     TsglErr("This many failed for testLine(): ");
     std::cerr << " " << failed << std::endl;
     return false;
   }
}

bool Canvas::testAccessors(Canvas& can) {
    int passed = 0;
    int failed = 0;
    ColorFloat white = WHITE;  //Have to set these to new variables so that I can compare them
    ColorFloat black = BLACK;

    //Test 1: Background color
    if(can.getBackgroundColor() == white) {
      can.setBackgroundColor(BLACK);
      if(can.getBackgroundColor() == black) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 1, Background color for testAccessors() failed!");
      }
    }

    //Test 2: Window width/height
    //width
    if(can.getWindowWidth() == 500) {
      //height
      if(can.getWindowHeight() == 500) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 2 for testAccessors() failed! (height)");
      }
    } else {
      failed++;
      TsglErr("Test 2 for testAccessors() failed! (width)");
    }

    //Test 3: Window x/y
    //x
    if(can.getWindowX() == 0) {
      //y
      if(can.getWindowY() == 0) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 3 for testAccessors() failed! (y)");
      }
    } else {
      failed++;
      TsglErr("Test 3 for testAccessors() failed! (x)");
    }

    //Test 4: Window open?
    if(can.getIsOpen() == true) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 4, Window open? for testAccessors() failed!");
    }

    //Results:
    if(passed == 4 && failed == 0) {
      can.clear();
      TsglDebug("Unit test for accessors/mutators passed!");
      return true;
    } else {
      can.clear();
      TsglErr("This many passed for testAccessors(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testAccessors(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
}

bool Canvas::testDrawImage(Canvas& can) {
    can.drawImage("assets/ff0000.png", 0, 0, 200, 200);
    can.sleepFor(1);
    int passed = 0;
    int failed = 0;
    ColorInt red(255, 0, 0);
    //Test 1: Single pixel
    if(can.getPoint(1, 1) == red) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Single pixel for testDrawImage() failed!");
    }

    //Test 2: Multiple pixels
    int count = 0;
    for(int i = 1; i <= 200; i++) {
      if(can.getPoint(1, i) == red) {
        count++;
      }
    }

    //Results of Test 2:
    if(count == 200) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Multiple pixels for testDrawImage() failed!");
    }

    //Results of entire Unit test:s
    if(passed == 2 && failed == 0) {
      TsglDebug("Unit test for drawing images passed!");
      return true;
    } else {
      TsglErr("This many passed for testDrawImage(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testDrawImage(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
}
//------------End Unit testing--------------------------------------------------------
}
