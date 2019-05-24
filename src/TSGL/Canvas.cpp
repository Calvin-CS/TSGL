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

int Canvas::drawBuffer = GL_FRONT_LEFT;
bool Canvas::glfwIsReady = false;
std::mutex Canvas::glfwMutex;
GLFWvidmode const* Canvas::monInfo;
unsigned Canvas::openCanvases = 0;

Canvas::Canvas(double timerLength) {
    init(-1, -1, -1, -1, -1, "", timerLength);
}

Canvas::Canvas(int x, int y, int width, int height, std::string title, double timerLength) {
    init(x, y, width, height, width*height*2, title, timerLength);
}

Canvas::~Canvas() {
    // Free our pointer memory
    delete myDrawables;
    delete drawableBuffer;
    delete drawTimer;
    delete[] vertexData;
    delete [] screenBuffer;
    if (--openCanvases == 0) {
        glfwIsReady = false;
        glfwTerminate();  // Terminate GLFW
    }
}

void Canvas::bindToButton(Key button, Action action, voidFunction function) {
    boundKeys[button + action * (GLFW_KEY_LAST + 1)] = function;
}

void Canvas::bindToScroll(std::function<void(double, double)> function) {
    scrollFunction = function;
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

void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    TsglDebug("Window closed successfully.");
}

void Canvas::draw() {
    // Reset the window
    glfwSetWindowShouldClose(window, GL_FALSE);

    // Get actual framebuffer size and adjust scaling accordingly
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    int scaling = round((1.0f*fbw)/winWidth);

    if (hasStereo)
      Canvas::setDrawBuffer(hasBackbuffer ? GL_FRONT_AND_BACK : GL_FRONT);
    else
      Canvas::setDrawBuffer(hasBackbuffer ? GL_LEFT : GL_FRONT_LEFT);


    setBackgroundColor(bgcolor); //Set our initial clear / background color
    glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    readyToDraw = true;
    bool nothingDrawn = false;  //Always draw the first frame

    // Start the drawing loop
    for (frameCounter = 0; !glfwWindowShouldClose(window); frameCounter++) {
        drawTimer->sleep(true);

        syncMutex.lock();

      #ifdef __APPLE__
        windowMutex.lock();
      #endif
        glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created

        realFPS = round(1 / drawTimer->getTimeBetweenSleeps());
        if (showFPS) std::cout << realFPS << "/" << FPS << std::endl;
        std::cout.flush();

        bufferMutex.lock();  // Time to flush our buffer
        if (drawableBuffer->size() > 0) {     // But only if there is anything to flush
          nothingDrawn = false;
          for (unsigned int i = 0; i < drawableBuffer->size(); i++)
            myDrawables->push((*drawableBuffer)[i]);
          drawableBuffer->shallowClear();  // We want to clear the buffer but not delete those objects as we still need to draw them
        }
        bufferMutex.unlock();

        int pos = pointBufferPosition;
        int posLast = pointLastPosition;
        pointLastPosition = pos;

        if (loopAround || pos != posLast)
          nothingDrawn = false;

        if (!nothingDrawn) {

          if (hasEXTFramebuffer)
            glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
          else
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
          glDrawBuffer(GL_COLOR_ATTACHMENT0);

          glViewport(0,0,winWidth,winHeight);

          if (toClear) glClear(GL_COLOR_BUFFER_BIT);
          toClear = false;

          unsigned int size = myDrawables->size();
          for (unsigned int i = 0; i < size; i++) {
            Drawable* d = (*myDrawables)[i];
            if(d->isProcessed())
              if (!d->getIsTextured()) {
                d->draw();
              } else {
                textureShaders(true);
                d->draw();
                textureShaders(false);
              }
          }

          if (loopAround) {
            nothingDrawn = false;
            int toend = myDrawables->capacity() - posLast;
            glBufferData(GL_ARRAY_BUFFER, toend * 6 * sizeof(float),
                   &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, toend);
            posLast = 0;
            loopAround = false;
          }
          int pbsize = pos - posLast;
          if (pbsize > 0) {
            nothingDrawn = false;
            glBufferData(GL_ARRAY_BUFFER, pbsize * 6 * sizeof(float), &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, pbsize);
          }
        }

        // Reset drawn status for the next frame
        nothingDrawn = true;

        // Update our screenBuffer copy with the screen
        glViewport(0,0,winWidth*scaling,winHeight*scaling);
        myDrawables->clear();                           // Clear our buffer of shapes to be drawn

        if (hasEXTFramebuffer)
          glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
        else
          glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        glReadPixels(0, 0, winWidthPadded, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
        if (toRecord > 0) {
          screenShot();
          --toRecord;
        }

        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        glDrawBuffer(drawBuffer);

        textureShaders(true);
        const float vertices[32] = {
          0,       0,        1,1,1,1,0,1,
          winWidth,0,        1,1,1,1,1,1,
          0,       winHeight,1,1,1,1,0,0,
          winWidth,winHeight,1,1,1,1,1,0
        };
        glBindTexture(GL_TEXTURE_2D,renderedTexture);
        glPixelStorei(GL_UNPACK_ALIGNMENT,4);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        glBufferData(GL_ARRAY_BUFFER,32*sizeof(float),vertices,GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glFlush();                                   // Flush buffer data to the actual draw buffer
        glfwSwapBuffers(window);                     // Swap out GL's back buffer and actually draw to the window

        textureShaders(false);

      #ifndef __APPLE__
        glfwPollEvents();                            // Handle any I/O
      #endif
        glfwGetCursorPos(window, &mouseX, &mouseY);
        glfwMakeContextCurrent(NULL);                // We're drawing to window as soon as it's created
      #ifdef __APPLE__
        windowMutex.unlock();
      #endif

        syncMutex.unlock();

        if (toClose) glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

void Canvas::drawArrow(float x1, float y1, float x2, float y2, const ColorFloat &color) {
  Arrow * arrow = new Arrow(x1, y1, x2, y2, color);
  drawDrawable(arrow);
}

void Canvas::drawCircle(int xverts, int yverts, int radius, int sides, ColorFloat color, bool filled) {
    // version 1
    // Circle* c = new Circle(xverts, yverts, radius, color);  // Creates the Line with the specified coordinates and color
    // drawDrawable(c);                               // Push it onto our drawing buffer

    // version 2
    RegularPolygon *c = new RegularPolygon(xverts, yverts, radius, sides, color);
    drawDrawable(c);

    //version 3
    // float delta = 2.0f / sides * PI;
    // if (filled) {
    //     ConvexPolygon *s = new ConvexPolygon(sides);
    //     for (int i = 0; i < sides; ++i)
    //         s->addVertex(xverts+radius*cos(i*delta), yverts+radius*sin(i*delta),color);
    //     drawDrawable(s);
    // } else {
    //     float oldX = 0, oldY = 0, newX = 0, newY = 0;
    //     Polyline *p = new Polyline(sides+1);
    //     for (int i = 0; i <= sides; ++i) {
    //         oldX = newX; oldY = newY;
    //         newX = xverts+radius*cos(i*delta);
    //         newY = yverts+radius*sin(i*delta);
    //         if (i > 0)
    //             p->addVertex(oldX, oldY,color);
    //     }
    //     p->addVertex(newX, newY,color);
    //     drawDrawable(p);
    // }
}

void Canvas::drawConcavePolygon(int size, int xverts[], int yverts[], ColorFloat color[], bool filled) {
    if (filled) {
        ConcavePolygon* p = new ConcavePolygon(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
}



void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    if (filled) {
        ConvexPolygon* p = new ConvexPolygon(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(x[i], y[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(x[i], y[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
}

void Canvas::drawImage(std::string filename, int x, int y, int width, int height, float alpha) {
    Image* im = new Image(filename, loader, x, y, width, height, alpha);  // Creates the Image with the specified coordinates
    drawDrawable(im);                                       // Push it onto our drawing buffer
}

void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorFloat color) {
    Line* l = new Line(x1, y1, x2, y2, color);  // Creates the Line with the specified coordinates and color
    drawDrawable(l);                               // Push it onto our drawing buffer
}

inline void Canvas::drawPixel(int row, int col, ColorFloat color) {
    drawPoint(col, row, color);
}

void Canvas::drawPoint(int x, int y, ColorFloat color) {
    pointArrayMutex.lock();
    if (pointBufferPosition >= myDrawables->capacity()) {
        loopAround = true;
        pointBufferPosition = 0;
    }
    int tempPos = pointBufferPosition * 6;
    pointBufferPosition++;

    float atioff = atiCard ? 0.5f : 0.0f;
    vertexData[tempPos] = x;
    vertexData[tempPos + 1] = y+atioff;
    vertexData[tempPos + 2] = color.R;
    vertexData[tempPos + 3] = color.G;
    vertexData[tempPos + 4] = color.B;
    vertexData[tempPos + 5] = color.A;
    pointArrayMutex.unlock();
}

void Canvas::drawProgress(ProgressBar* p) {
    for (int i = 0; i < p->getSegs(); ++i) {
      drawText(to_string(i),p->getSegX(i)+8,p->getSegY()-8,32,BLACK);
      drawDrawable(p->getRect(i));
      drawDrawable(p->getBorder(i));
    }
}

void Canvas::drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color, bool filled) {
    if (filled) {
        if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
        if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
        Rectangle* rec = new Rectangle(x1, y1, x2-x1, y2-y1, color);  // Creates the Rectangle with the specified coordinates and color
        drawDrawable(rec);                                     // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(5);
        p->addVertex(x1, y1, color);
        p->addVertex(x1, y2, color);
        p->addVertex(x2, y2, color);
        p->addVertex(x2, y1, color);
        p->addVertex(x1, y1, color);
        drawDrawable(p);
    }
}

void Canvas::drawDrawable(Drawable* d) {
	if (!started) {
	  TsglDebug("No drawing before Canvas is started! Ignoring draw request.");
	  return;
	}
	while (!readyToDraw)
	  sleep();
    bufferMutex.lock();
    drawableBuffer->push(d);  // Push it onto our drawing buffer
    bufferMutex.unlock();
}

void Canvas::drawText(std::string text, int x, int y, unsigned size, ColorFloat color) {
    std::wstring wsTmp(text.begin(), text.end());
    std::wstring ws = wsTmp;
    drawText(ws, x, y, size, color);
}

void Canvas::drawText(std::wstring text, int x, int y, unsigned size, ColorFloat color) {
    Text* t = new Text(text, loader, x, y, size, color);  // Creates the Point with the specified coordinates and color
    drawDrawable(t);                                // Push it onto our drawing buffer
}

void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled) {
    if (filled) {
        Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color);  // Creates the Triangle with the specified vertices and color
        drawDrawable(t);                                               // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(4);
        p->addVertex(x1,y1,color);
        p->addVertex(x2,y2,color);
        p->addVertex(x3,y3,color);
        p->addVertex(x1,y1,color);
        drawDrawable(p);
    }
}

void Canvas::drawTriangleStrip(int size, int xverts[], int yverts[], ColorFloat color[], bool filled) {
    if (filled) {
        TriangleStrip* p = new TriangleStrip(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i], color[i]);
        }
        drawDrawable(p);  // Push it onto our drawing buffer
    }
}

void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
}

ColorFloat Canvas::getBackgroundColor() {
  return bgcolor;
}

int Canvas::getDisplayHeight() {
  initGlfw();
  return monInfo->height;
}

int Canvas::getDisplayWidth() {
  initGlfw();
  return monInfo->width;
}

float Canvas::getFPS() {
    return realFPS;
}

int Canvas::getFrameNumber() {
    return frameCounter;
}

bool Canvas::isOpen() {
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
    int yy;
    //if (atiCard)
    //  yy = (winHeight) - y; //glReadPixels starts from the bottom left, and we have no way to change that...
    //else
      yy = (winHeight-1) - y;
    int off = 3 * (yy * winWidthPadded + x);
    return ColorInt(screenBuffer[off], screenBuffer[off + 1], screenBuffer[off + 2], 255);
}

unsigned int Canvas::getReps() const {
    return drawTimer->getReps();
}

uint8_t* Canvas::getScreenBuffer() {
    return screenBuffer;
}

double Canvas::getTime() {
    return drawTimer->getTime();
}

double Canvas::getTimeBetweenSleeps() const {
    return drawTimer->getTimeBetweenSleeps();
}

int Canvas::getWindowHeight() {
    return winHeight;
}

int Canvas::getWindowWidth() {
    return winWidth;
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

//Workaround for OS X
void Canvas::handleIO() {
  #ifdef __APPLE__
    if (isFinished)
        return;
    if (pthread_main_np() == 0)
        return;  //If we're not the main thread, we can't call this
    windowMutex.lock();
    glfwMakeContextCurrent(window);
    glfwPollEvents();
    glfwMakeContextCurrent(NULL);
    windowMutex.unlock();
    if (toClose && !windowClosed) {
        windowClosed = true;
        while (!isFinished)
          sleepFor(0.1f);
        glfwDestroyWindow(window);  //We have to do this on the main thread for OS X
        glDestroy();
    }
  #endif
}

void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title, double timerLength) {
    ++openCanvases;

    if (ww == -1)
      ww = 1.2*Canvas::getDisplayHeight();
    if (hh == -1)
      hh = 0.75*ww;
    b = ww*hh*2;

    winTitle = title;
    winWidth = ww, winHeight = hh;
    aspect = (float) winWidth / winHeight;
    keyDown = false;
    toClose = false;
    windowClosed = false;
	  readyToDraw = false;
    frameCounter = 0;
    syncMutexLocked = 0;
	  syncMutexOwner = -1;

    int padwidth = winWidth % 4;
    if (padwidth > 0)
       padwidth = 4-padwidth;
    winWidthPadded = winWidth + padwidth;
    bufferSize = 3 * (winWidthPadded+1) * winHeight;
    screenBuffer = new uint8_t[bufferSize];
    for (unsigned i = 0; i < bufferSize; ++i) {
      screenBuffer[i] = 0;
    }

    toClear = true;                   // Don't need to clear at the start
    started = false;                  // We haven't started the window yet
    monitorX = xx;
    monitorY = yy;
    myDrawables = new Array<Drawable*>(b);  // Initialize myDrawables
    drawableBuffer = new Array<Drawable*>(b);
    vertexData = new float[6 * b];    // Buffer for vertexes for points
    showFPS = false;                  // Set debugging FPS to false
    isFinished = false;               // We're not done rendering
    pointBufferPosition = pointLastPosition = 0;
    loopAround = false;
    toRecord = 0;

    bgcolor = GRAY;
    window = nullptr;

    drawTimer = new Timer((timerLength > 0.0f) ? timerLength : FRAME);

    for (int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
        boundKeys[i++] = nullptr;

    initGlfw();
#ifndef _WIN32
    initWindow();
    initGlew();
    glfwMakeContextCurrent(NULL);   // Reset the context
#endif
}

void Canvas::initGl() {
#ifdef _WIN32
    initWindow();
    initGlew();
#else
    glfwMakeContextCurrent(window);         // We're drawing to window as soon as it's created
    glfwSetWindowUserPointer(window, this);
#endif

    // Enable and disable necessary stuff
    glDisable(GL_DEPTH_TEST);                           // Disable depth testing because we're not drawing in 3d
    glDisable(GL_DITHER);                               // Disable dithering because pixels do not (generally) overlap
    glDisable(GL_CULL_FACE);                            // Disable culling because the camera is stationary
    glEnable(GL_BLEND);                                 // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blending mode to standard alpha blending

    bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this]() {
        glfwSetWindowShouldClose(window, GL_TRUE);
        toClose = true;
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

void Canvas::initGlew() {
    // Enable Experimental GLEW to Render Properly
    glewExperimental = GL_TRUE;
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        // Problem: glewInit failed, something is seriously wrong.
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
        exit(102);
    }

    hasEXTFramebuffer = false;

    GLint n, i;
    glGetIntegerv(GL_NUM_EXTENSIONS, &n);
    for (i = 0; i < n; i++) {
      std::string s = reinterpret_cast< char const * >(glGetStringi(GL_EXTENSIONS, i));
      if (s == "GL_EXT_framebuffer_object") {
        hasEXTFramebuffer = true;
        break;
      }
    }
    const GLubyte* gfxVendor = glGetString(GL_VENDOR);
    std::string gfx(gfxVendor, gfxVendor + strlen((char*)gfxVendor));
    atiCard = (gfx.find("ATI") != std::string::npos);
//    #define DEBUG
    #ifdef DEBUG
        printf("Vendor:         %s %s\n", gfx.c_str(), glGetString(GL_RENDERER));
        printf("OpenGL version: %s\n", glGetString(GL_VERSION));
        printf("GLFW version:   %s\n", glfwGetVersionString());
        printf("GL Extension: ");
        for (i = 0; i < n; i++)
          printf("%s, ", glGetStringi(GL_EXTENSIONS, i));
        if (hasEXTFramebuffer)
          TsglDebug("EXT Framebuffer available");
    #endif

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

    /****** NEW ******/
    // Create a framebuffer
    frameBuffer = 0;
    glGenFramebuffersEXT(1, &frameBuffer);
    if (hasEXTFramebuffer)
      glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, frameBuffer);
    else
      glBindFramebuffer(GL_FRAMEBUFFER_EXT, frameBuffer);
    // The texture we're going to render to
    glGenTextures(1, &renderedTexture);
    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, renderedTexture);
    // Give an empty image to OpenGL ( the last "0" )
    // Note: Using RGBA here creates a texture with alpha, which causes weird redrawing problems
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, winWidth+1, winHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
    // Poor filtering. Needed !
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    // Set "renderedTexture" as our colour attachement #0
    glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D,renderedTexture, 0);
    // Set the list of draw buffers.
    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      TsglErr("FRAMEBUFFER CREATION FAILED");

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Canvas::initGlfw() {
  if (!glfwIsReady) {
    glfwInit();  // Initialize GLFW
    monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwIsReady = true;
  }
}

void Canvas::initWindow() {
    glfwSetErrorCallback(errorCallback);

     // Create a Window and the Context
  #ifdef __arm__                                                    // Tentative fix for the Raspberry Pi
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);                  // Set target GL major version to 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);                  // Set target GL minor version to 2.0
    glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);             // Pi uses OpenGL ES
  #else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set target GL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // Set target GL minor version to 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We're using the standard GL Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Don't use methods that are deprecated in the target version
  #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);                    // Disable the back buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first
    glfwWindowHint(GLFW_SAMPLES,4);

    glfwMutex.lock();                                  // GLFW crashes if you try to make more than once window at once
    window = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), NULL, NULL);  // Windowed
 //   window = glfwCreateWindow(monInfo->width, monInfo->height, title_.c_str(), glfwGetPrimaryMonitor(), NULL);  // Fullscreen
    if (!window) {
        fprintf(stderr, "GLFW window creation failed. Was the library correctly initialized?\n");
        exit(100);
    }
    glfwMutex.unlock();

    if (!monInfo) {
        fprintf(stderr, "GLFW failed to return monitor information. Was the library correctly initialized?\n");
        exit(101);
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
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
}

void Canvas::pauseDrawing() {
  #pragma omp critical (pauseResume)
  {
    if (syncMutexLocked == 0 && syncMutexOwner == -1) {
      syncMutex.lock();
      syncMutexOwner = omp_get_thread_num();
    }
    #pragma omp critical (syncMutLock)
    {
      ++syncMutexLocked;
    }
  }
}

void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
}

void Canvas::reset() {
    drawTimer->reset();
}

void Canvas::resumeDrawing() {
  #pragma omp critical (syncMutLock)
  {
    --syncMutexLocked;
  }
  #pragma omp critical (pauseResume)
  {
    if (syncMutexOwner == omp_get_thread_num()) {
      while (syncMutexLocked > 0)
        sleepFor(FRAME/2);
      syncMutex.unlock();
      syncMutexOwner = -1;
    }
  }
  while (syncMutexOwner != -1)
    sleepFor(FRAME/2);
}

void Canvas::run(void (*myFunction)(Canvas&) ) {
  start(); myFunction(*this); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, int), int i) {
  start(); myFunction(*this, i); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, unsigned), unsigned u) {
  start(); myFunction(*this, u); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, int, int), int i1, int i2) {
  start(); myFunction(*this, i1, i2); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
  start(); myFunction(*this, u1, u2); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, std::string),std::string s) {
  start(); myFunction(*this, s); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s) {
  start(); myFunction(*this, i, s); wait();
}
void Canvas::run(void (*myFunction)(Canvas&, std::string, int), std::string s, int i) {
  start(); myFunction(*this, s, i); wait();
}

void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.png", frameCounter);  // TODO: Make this save somewhere not in root

    loader.saveImageToFile(filename, screenBuffer, winWidthPadded, winHeight);
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
    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
      -(winWidth-1) / 2.0f, (winHeight) / 2.0f, -(winHeight) / 2.0f, 1 };
//    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
//      -(winWidth-1) / 2.0f, (winHeight+0.5f) / 2.0f, -(winHeight-0.5f) / 2.0f, 1 };
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

    // Set up camera zooming
    float projF[] = { 1.0f / aspect, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.0f, -1, 0, 0, -0.02f, 0 };
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

    // Set up camera transformation
    float modelF[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
}

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
    c->initGl();
    c->draw();
    c->isFinished = true;
    pthread_exit(NULL);
}
#else
void Canvas::startDrawing(Canvas *c) {
    c->initGl();
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
}
#endif

void Canvas::stop() {
    close();
    wait();
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

int Canvas::wait() {
    if (!started) return -1;  // If we haven't even started yet, return error code -1
  #ifdef __APPLE__
    while(!isFinished)
      sleepFor(0.1f);
    pthread_join(renderThread, NULL);
  #else
    renderThread.join();
  #endif
    return 0;
}

//-----------------Unit testing-------------------------------------------------------
void Canvas::runTests() {
  TsglDebug("Testing Canvas class...");
  Canvas c1(0, 0, 500, 500, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  tsglAssert(testFilledDraw(c1), "Unit test for filled draw failed!");
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
  if(can.getPixel(250, 201) == red && can.getPixel(250, 299) == red) {
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
    if(can.getPixel(250, i) == red) {
      count++;
    }
  }

  //Now check the count, should be 99
  if(count == 99) {
    passed++;
  } else {
    failed++;
    std::cout << "Count: " << count << std::endl;
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

bool Canvas::testLine(Canvas & can) {
   int passed = 0;
   int failed = 0;
   can.drawLine(0, 0, 250, 250, BLACK);  //Diagonal line
   can.drawLine(253, 253, 400, 253);  //Straight line
   can.sleepFor(1);
   ColorInt black(0, 0, 0);
   //Test 1: Near the ending endpoint? (Diagonal)
   if(can.getPoint(249, 249) == black) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 1, Near the ending endpoint? for testLine() failed!");
   }

   //Test 2: Somewhere in the middle? (Diagonal)
   if(can.getPoint(155, 155) == black) {
     passed++;
   } else {
     failed++;
     TsglErr("Test 2, Somewhere in the middle? for testLine() failed!");
   }

   //Test 3: Near the starting endpoint? (Diagonal)
   if(can.getPoint(15, 15) == black) {
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
    if(can.isOpen() == true) {
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
    can.drawImage("../assets/pics/ff0000.png", 0, 0, 200, 200);
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
    for(int i = 0; i < 200; i++) {
      if(can.getPoint(1, i) == red) {
        count++;
      }
    }

    //Results of Test 2:
    if(count == 200) {
      passed++;
    } else {
      failed++;
      std::cout << "Count: " << count << std::endl;
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
