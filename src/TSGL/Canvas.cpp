#include "Canvas.h"

// From stb_image.h:
// Do this:
//   #define STB_IMAGE_IMPLEMENTATION
// before you include this file in *one* C or C++ file to create the implementation.
  #ifndef STB_DEFINE
    #define STB_IMAGE_WRITE_IMPLEMENTATION
    #include "stb/stb_image_write.h"
    #define STB_IMAGE_IMPLEMENTATION
    #include "stb/stb_image.h"
    #define STB_DEFINE
  #endif

namespace tsgl {

// Shader sources

static const GLchar* shapeVertexShader = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "layout (location = 1) in vec4 aColor;"
  "out vec4 color;"
  "uniform mat4 projection;"
  "uniform mat4 view;"
  "uniform mat4 model;"
  "void main() {"
  "gl_Position = projection * view * model * vec4(aPos, 1.0);"
	"color = aColor;"
  "}";

static const GLchar* shapeFragmentShader =
  "#version 330 core\n"
  "out vec4 FragColor;"
  "in vec4 color;"
  "void main() {"
	"FragColor = color;"
  "}";

static const GLchar* textVertexShader = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "layout (location = 1) in vec2 aTexCoord;"
  "out vec2 TexCoords;"
  "uniform mat4 aspect;"
  "uniform mat4 projection;"
  "uniform mat4 view;"
  "uniform mat4 model;"
  "void main() {"
  "gl_Position = projection * view * model * aspect * vec4(aPos, 1.0);"
  "TexCoords = vec2(aTexCoord.x, aTexCoord.y);"
  "}";

static const GLchar* textFragmentShader =
  "#version 330 core\n"
  "in vec2 TexCoords;"
  "out vec4 FragColor;"
  "uniform sampler2D text;"
  "uniform vec4 textColor;"
  "void main() {"
  "vec4 sampled = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);"
  "FragColor = textColor * sampled;"
  "}";

static const GLchar* textureVertexShader = 
  "#version 330 core\n"
  "layout (location = 0) in vec3 aPos;"
  "layout (location = 1) in vec2 aTexCoord;"
  "out vec2 TexCoords;"
  "uniform mat4 projection;"
  "uniform mat4 view;"
  "uniform mat4 model;"
  "void main() {"
	"gl_Position = projection * view * model * vec4(aPos, 1.0);"
	"TexCoords = vec2(aTexCoord.x, aTexCoord.y);"
  "}";

static const GLchar* textureFragmentShader = 
  "#version 330 core\n"
  "out vec4 FragColor;"
  "in vec2 TexCoords;"
  "uniform sampler2D texture1;"
  "uniform float alpha;"
  "void main() {"
	"FragColor = texture(texture1, TexCoords) * vec4(1.0,1.0,1.0,alpha);"
  "}";

bool Canvas::glfwIsReady = false;
std::mutex Canvas::glfwMutex;
GLFWvidmode const* Canvas::monInfo;
unsigned Canvas::openCanvases = 0;

 /*!
  * \brief Default Canvas constructor method.
  * \details This is the default constructor for the Canvas class.
  *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
  *     A value less than or equal to 0 sets it to automatic.
  * \return A new Canvas in the middle of the screen with no title.
  *   The created Canvas will take up approximately 90% of the monitor's height, and will
  *   have a 4:3 aspect ratio.
  */
Canvas::Canvas(double timerLength, Background * bg) {
    init(-1, -1, -1, -1, "", GRAY, bg, timerLength);
}

 /*!
  * \brief Explicit Canvas constructor method.
  * \details This is the explicit constructor for the Canvas class.
  *   \param x The x position of the Canvas window.
  *   \param y The y position of the Canvas window.
  *   \param width The x dimension of the Canvas window.
  *   \param height The y dimension of the Canvas window.
  *   \param title The title of the window.
  *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
  *     A value less than or equal to 0 sets it to automatic.
  * \return A new Canvas with the specified position, dimensions, title, and draw cycle length.
  */
Canvas::Canvas(int x, int y, int width, int height, std::string title, ColorFloat backgroundColor, Background * background, double timerLength) {
    init(x, y, width, height, title, backgroundColor, background, timerLength);
}

 /*!
  * \brief Canvas destructor method.
  * \details This is the destructor for the Canvas class.
  * \details Frees up memory that was allocated to a Canvas instance.
  */
Canvas::~Canvas() {
    // Free our pointer memory
    delete drawTimer;
    delete camera;
    delete [] screenBuffer;
    if (defaultBackground) {
      delete myBackground;
    }
    if (--openCanvases == 0) {
        glfwIsReady = false;
        glfwTerminate();  // Terminate GLFW
    }
}

 /*!
  * \brief Binds a key or button to a function.
  * \details This function binds a key or mouse button to a function pointer.
  * \details Upon pressing or releasing the given key, Canvas will call the specified function.
  *   \param button The key or button to bind, as specified in Keynums.h.
  *   \param action The action to look out for (TSGL_PRESS or TSGL_RELEASE).
  *   \param function The function to call upon action <code>a</code> on button.
  * \warning <b>TSGL_KEY_ESCAPE is automatically bound to closing the window. Overriding TSGL_KEY_ESCAPE will
  *   likely make you unable to close the window through the escape key.</b>
  */
void Canvas::bindToButton(Key button, Action action, voidFunction function) {
    boundKeys[button + action * (GLFW_KEY_LAST + 1)] = function;
}

 /*!
  * \brief Binds the mouse wheel to a function.
  * \details This function binds the mouse wheel to a function pointer.
  * \details Upon scrolling, Canvas will call the specified function.
  *   \param function A function taking x and y parameters to be called when the mouse is scrolled.
  */
void Canvas::bindToScroll(std::function<void(double, double)> function) {
    scrollFunction = function;
}

void Canvas::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_REPEAT) return;
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    int index = button + action * (GLFW_KEY_LAST + 1);
    if (&(can->boundKeys[index]) != nullptr) if (can->boundKeys[index]) can->boundKeys[index]();
}

 /*!
  * \brief Clears the Canvas.
  * \details This function clears the screen to the color specified in setBackgroundColor().
  */
void Canvas::clearBackground() {
  backgroundMutex.lock();
  myBackground->clear();
  backgroundMutex.unlock();
}

 /*!
  * \brief Closes the Canvas window.
  * \details This function tells the Canvas to stop rendering and to close its rendering window.
  * \details Any threads that have called wait() will continue.
  * \see start(), stop(), wait()
  */
void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    toClose = true;
    TsglDebug("Window closed successfully.");
}

///////////////////////////////////////////////
// Object Interface
///////////////////////////////////////////////

/**
 * \brief Adds a Drawable to the Canvas.
 * \details Adds the parameter drawable to objectBuffer.
 *    \param shapePtr Pointer to the Drawable to add to this Canvas.
 */
void Canvas::add(Drawable * shapePtr) {
  objectMutex.lock();
  objectBuffer.push_back(shapePtr);
  objectMutex.unlock();
}

/**
 * \brief Removes a Drawable from the Canvas.
 * \details Removes shapePtr from the Canvas's drawing buffer.
 *    \param shapePtr Pointer to the Drawable to remove from this Canvas.
 * \warning The Drawable being deleted or going out of scope before remove() is called will cause a segmentation fault.
 * \warning If shapePtr is not in the drawing buffer, behavior is undefined.
 */
void Canvas::remove(Drawable * shapePtr) {
  objectMutex.lock();
  objectBuffer.erase(std::remove(objectBuffer.begin(), objectBuffer.end(), shapePtr), objectBuffer.end());
  objectMutex.unlock();
}

/**
 * \brief Removes all Drawables from the Canvas.
 * \details Clears all Drawables from the drawing buffer.
 *    \param shouldFreeMemory Whether the pointers will be deleted as well as removed and free their memory. (Defaults to false.)
 * \warning Setting shouldFreeMemory to true will cause a segmentation fault if the user continues to access the pointer to a
 *  Drawable that has been added to the Canvas.
 * \warning Setting shouldFreeMemory to false will leak memory from any objects created in Canvas draw methods.
 */
void Canvas::clearObjectBuffer(bool shouldFreeMemory) {
  if( shouldFreeMemory ) {
    for(unsigned i = 0; i < objectBuffer.size(); i++) {
      delete objectBuffer[i]; //TODO fix this, causes to crash
    }
  }
  objectBuffer.clear();
}

void Canvas::draw()
{
    windowMutex.lock();
    glfwMakeContextCurrent(window);
    // Reset the window
    glfwSetWindowShouldClose(window, GL_FALSE);

    // Get actual framebuffer size and adjust scaling accordingly
    // NOTE: framebuffer stuff seems purposeless, at least with pure OO on MacOS.
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    int scaling = round((1.0f*fbw)/winWidth);
    glfwMakeContextCurrent(NULL);
    windowMutex.unlock();

    bool captureScreen = false;

    for (frameCounter = 0; !glfwWindowShouldClose(window); frameCounter++)
    {
        // this if, and the capturescreen variable, are necessary for screenshots to be 100% correct.
        if (toRecord > 0) {
          captureScreen = true;
          --toRecord;
        }
        drawTimer->sleep(true);

        syncMutex.lock();

      #ifdef __APPLE__
        windowMutex.lock();
      #endif
        glfwMakeContextCurrent(window);

        realFPS = round(1 / drawTimer->getTimeBetweenSleeps());
        if (showFPS) std::cout << realFPS << "/" << FPS << std::endl;
        std::cout.flush();

        // clear default framebuffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // if background initialized draw it using its multisampled framebuffer
        backgroundMutex.lock();
        if (myBackground)
          if (myBackground->isInitialized()) {
            myBackground->draw();
          }
        backgroundMutex.unlock();

        // Scale to window size
        GLint windowWidth, windowHeight;
        glfwGetWindowSize(window, &windowWidth, &windowHeight);
        glViewport(0, 0, windowWidth, windowHeight);
        winWidth = windowWidth;
        winHeight = windowHeight;

        objectMutex.lock();
        if (objectBuffer.size() > 0) {
          // sort between opaques and transparents and then sort by center z. depth buffer takes care of the rest. not perfect, but good.
          std::stable_sort(objectBuffer.begin(), objectBuffer.end(), [this](Drawable * a, Drawable * b)->bool {
            if (a->getAlpha() == 1.0 && b->getAlpha() != 1.0)
              return true;
            else if (a->getAlpha() != 1.0 && b->getAlpha() == 1.0)
              return false;
            else
              return (distanceBetween(a->getCenterX(), a->getCenterY(), a->getCenterZ(), camera->getPositionX(), camera->getPositionY(), camera->getPositionZ())
                    > distanceBetween(b->getCenterX(), b->getCenterY(), b->getCenterZ(), camera->getPositionX(), camera->getPositionY(), camera->getPositionZ()));
          });
          for (unsigned int i = 0; i < objectBuffer.size(); i++) {
            Drawable* d = objectBuffer[i];
            if(d->isProcessed()) {
              selectShaders(d->getShaderType());
              if (d->getShaderType() == SHAPE_SHADER_TYPE) {
                d->draw(shapeShader);
              } else if (d->getShaderType() == TEXTURE_SHADER_TYPE) {
                d->draw(textureShader);
              } else if (d->getShaderType() == TEXT_SHADER_TYPE) {
                d->draw(textShader);
              }
            }
          }
        }
        objectMutex.unlock();

        if (captureScreen) {
          // Update our screenBuffer copy with the default framebuffer
          screenBufferMutex.lock();
          glViewport(0,0,winWidth*scaling,winHeight*scaling);
          glBindFramebuffer(GL_READ_FRAMEBUFFER, 0);
          glReadPixels(0, 0, winWidthPadded, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
          screenBufferMutex.unlock();
          screenShot();
          captureScreen = false;
        }
      
        // Update Screen
        glfwSwapBuffers(window);

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

//  /*!
//   * \brief Draw a string of text.
//   * \details This function draws a given string of Text at the given coordinates with the given color.
//   *   \param text The string to draw.
//   *   \param x The x coordinate of the text's left bound.
//   *   \param y The y coordinate of the text's left bound.
//   *   \param size The size of the text in pixels.
//   *   \param color The color of the Text (set to BLACK by default).
//   *   \param rotation Rotation of the Text in radians clockwise.
//   */
// void Canvas::drawText(std::string text, int x, int y, unsigned size, ColorFloat color, std::string fontFileName, float rotation) {
//     std::wstring wsTmp(text.begin(), text.end());
//     std::wstring ws = wsTmp;
//     drawText(ws, x, y, size, color, fontFileName, rotation);
// }

void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
}

 /*!
  * \brief Accessor for the current background.
  * \return The Background that the Canvas draws when draw() is called.
  */
Background * Canvas::getBackground() {
  return myBackground;
}

 /*!
  * \brief Accessor for the current background color.
  * \return The color that the Canvas clears to when clear() is called.
  */
ColorFloat Canvas::getBackgroundColor() {
  return myBackground->getClearColor();
}

 /*!
  * \brief Accessor for the height of the user's primary monitor.
  * \return The height of the user's primary monitor.
  */
int Canvas::getDisplayHeight() {
  initGlfw();
  return monInfo->height;
}

 /*!
  * \brief Accessor for the width of the user's primary monitor.
  * \return The width of the user's primary monitor.
  */
int Canvas::getDisplayWidth() {
  initGlfw();
  return monInfo->width;
}

 /*!
  * \brief Accessor for the current frame number.
  * \return The number of actual draw cycles / frames the Canvas has rendered so far.
  * \see getReps()
  */
int Canvas::getFrameNumber() {
    return frameCounter;
}

 /*!
  * \brief Accessor for the current FPS.
  * \return The average number of frames being rendered per second.
  */
float Canvas::getFPS() {
    return realFPS;
}

 /*!
  * \brief Accessor for the mouse's x-position.
  * \return The x coordinates of the mouse on the Canvas.
  */
float Canvas::getMouseX() {
    return mouseX - winWidth/2;
}

 /*!
  * \brief Accessor for the mouse's y-position.
  * \return The y coordinates of the mouse on the Canvas.
  */
float Canvas::getMouseY() {
    return (float) winHeight/2 - mouseY;
}

 /*!
  * \brief Accessor for the number of theoretical draw cycles that have elapsed
  * \details This function returns the time elapsed since the Canvas has been opened divided
  *   by the drawTimer's period.
  * \return The number of times the drawTimer has expired since starting the Canvas.
  * \see getFrameNumber()
  */
unsigned int Canvas::getReps() const {
    return drawTimer->getReps();
}

 /*!
  * \brief Accessor for the Canvas's currently drawn image.
  * \return A pointer to the RGB pixel buffer for the current Canvas.
  * \note The array starts in the bottom left corner of the image, and is in row-major ordering.
  * \deprecated <b>This function returns a pointer directly to the Canvas' screen buffer. This
  *   function may be removed in future versions of TSGL. Please use getPixel() or getPoint()
  *   get individual pixels.
  */
uint8_t* Canvas::getScreenBuffer() {
    return screenBuffer;
}

 /*!
  * \brief Accessor for the time since the Canvas was initialized.
  * \return The elapsed time in microseconds since the Canvas has started drawing.
  */
double Canvas::getTime() {
    return drawTimer->getTime();
}

 /*!
  * \brief Accessor that gets the time between two sleep times of the internal drawing timer of a Canvas object.
  * \return The time between two sleep cycles of the internal drawing timer.
  */
double Canvas::getTimeBetweenSleeps() const {
    return drawTimer->getTimeBetweenSleeps();
}

 /*!
  * \brief Accessor for the Canvas's window height.
  * \return The height in pixels of the Canvas window.
  */
int Canvas::getWindowHeight() {
    return winHeight;
}

 /*!
  * \brief Accessor for the Canvas's window width.
  * \return The width in pixels of the Canvas window.
  */
int Canvas::getWindowWidth() {
    return winWidth;
}

 /*!
  * \brief Accessor for the Canvas's x-position.
  * \return The x coordinate in pixels of the left of the Canvas (0 = left of monitor).
  */
int Canvas::getWindowX() {
    return monitorX;
}

 /*!
  * \brief Accessor for the Canvas's y-position.
  * \return The y coordinate in pixels of the top of the Canvas (0 = top of monitor).
  */
int Canvas::getWindowY() {
    return monitorY;
}

void Canvas::glDestroy() {
    // Free up our resources
    delete textShader;
    delete shapeShader;
    delete textureShader;
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

 /*!
  * \brief Manually handles keyboard/mouse I/O.
  * \note This function will return immediately if not called from the main thread.
  * \note <b>OS X:</b> This function exists for OS X compatibility purposes.
  *   This function does nothing on Linux or Windows.
  */
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

void Canvas::init(int xx, int yy, int ww, int hh, std::string title, ColorFloat backgroundColor, Background * background, double timerLength) {
    ++openCanvases;

    if (ww == -1)
      ww = 1.2*Canvas::getDisplayHeight();
    if (hh == -1)
      hh = 0.75*ww;

    winTitle = title;
    winWidth = ww, winHeight = hh;
    // aspect = (float) winWidth / winHeight;
    keyDown = false;
    toClose = false;
    windowClosed = false;
    frameCounter = 0;
    syncMutexLocked = 0;
	  syncMutexOwner = -1;

    int padwidth = winWidth % 4;
    if (padwidth > 0)
       padwidth = 4-padwidth;
    winWidthPadded = winWidth + padwidth;
    screenBufferMutex.lock();
    screenBuffer = new uint8_t[3 * (winWidthPadded+1) * winHeight];
    for (int i = 0; i < 3 * (winWidthPadded+1) * winHeight; ++i) {
      screenBuffer[i] = 0;
    }
    screenBufferMutex.unlock();

    started = false;                  // We haven't started the window yet
    monitorX = xx;
    monitorY = yy;
    showFPS = false;                  // Set debugging FPS to false
    isFinished = false;               // We're not done rendering
    toRecord = 0;

    window = nullptr;

    defaultBackground = true;
    myBackground = nullptr;

    drawTimer = new Timer((timerLength > 0.0f) ? timerLength : FRAME);
    camera = new Camera(glm::vec3(0.0f, 0.0f, (winHeight / 2) / tan(glm::pi<float>()/6)),glm::vec3(0.0f,1.0f,0.0f),glm::vec3(0.0f,0.0f,0.0f));

    for (int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
        boundKeys[i++] = nullptr;

    windowMutex.lock();
    initGlfw();
#ifndef _WIN32
    initWindow();
    initGlew();
    glfwMakeContextCurrent(NULL);   // Reset the context
#endif
    initGl();
    initBackground(background, backgroundColor);
    windowMutex.unlock();
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
    // glEnable(GL_DEPTH_TEST); // Depth Testing
    glDisable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDisable(GL_CULL_FACE);
    // glCullFace(GL_BACK);
    glEnable(GL_BLEND);                                 // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blending mode to standard alpha blending

    bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this]() {
        glfwSetWindowShouldClose(window, GL_TRUE);
        toClose = true;
    });

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
    #endif

    // Create our Vertex Array Object
    glGenVertexArrays(1, &VAO);

    // Create our Vertex Buffer Object
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    textShader = new Shader(textVertexShader, textFragmentShader);

    shapeShader = new Shader(shapeVertexShader, shapeFragmentShader); 

    textureShader = new Shader(textureVertexShader, textureFragmentShader);

    // char buf[PATH_MAX]; /* PATH_MAX incudes the \0 so +1 is not required */
    // char *res = realpath(".", buf);
    // if (res) {
    //     printf("This source is at %s.\n", buf);
    // } else {
    //     perror("realpath");
    //     exit(EXIT_FAILURE);
    // }
}

void Canvas::initGlfw() {
  if (!glfwIsReady) {
    glfwInit();  // Initialize GLFW
    monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwIsReady = true;
  }
}

void Canvas::initBackground(Background * background, ColorFloat bgcolor) {
    backgroundMutex.lock();
    if (!background) {
      myBackground = new Background(winWidth, winHeight, bgcolor);
    } else {
      myBackground = background;
      background->setClearColor(bgcolor);
    }
    myBackground->init(shapeShader, textShader, textureShader, camera, window);
    backgroundMutex.unlock();
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
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);                  // Set target GL minor version to 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We're using the standard GL Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Don't use methods that are deprecated in the target version
  #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    // glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);                    // Disable the back buffer
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first
    glfwWindowHint(GLFW_SAMPLES,4);

    glfwMutex.lock();                                  // GLFW crashes if you try to make more than one window at once
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

    // Scale to window size
    GLint windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    winWidth = windowWidth;
    winHeight = windowHeight;
    
    // Get info of GPU and supported OpenGL version
    // printf("Renderer: %s\n", glGetString(GL_RENDERER));
    // printf("OpenGL version supported %s\n", glGetString(GL_VERSION));
}

 /*!
  * \brief Accessor for window's closed status.
  * \return Whether the window is still open (that is, the user has not closed it).
  */
bool Canvas::isOpen() {
    return !isFinished;
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
}

 /*!
  * \brief Pauses the rendering thread of the Canvas
  * \details This function forces the calling thread to wait until the Canvas finishes its draw cycle,
  *   them prevents the Canvas from rendering further updates until resumeDrawing is called.
  * \note This method may be called from any number of threads, so long as a matching number of calls
  *   to resumeDrawing() are made.
  * \warning <b>This function makes use of a mutex lock. Do not call this without later calling
  *   resumeDrawing().</b>
  * \see resumeDrawing()
  */
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

 /*!
  * \brief Records the Canvas for a specified number of frames.
  * \details This function starts dumping screenshots of the Canvas to the working directory every draw
  *   cycle.
  * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
  * \details The function automatically terminates after num_frames cycles have completed.
  *   \param num_frames The number of frames to dump screenshots for.
  */
void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
}

 /*!
  * \brief Resets the internal drawing timer of a Canvas instance.
  * \details This function resets the starting time of the Canvas' draw timer
  *   to the current time.
  */
void Canvas::reset() {
    drawTimer->reset();
}

 /*!
  * \brief Resumes the rendering thread of the Canvas
  * \details This function should be called after pauseDrawing to let the Canvas' rendering thread
  *   know that it may resume rendering.
  * \note This method may be called from any number of threads, so long as a matching number of calls
  *   to pauseDrawing() are made.
  * \warning <b>This function makes use of a mutex lock. Do not call this without having
  *   first called pauseDrawing().</b>
  * \see pauseDrawing()
  */
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

 /*!
  * \brief Start the Canvas, run a function on it, and wait for the user to close it
  * \details This function binds another function to the current Canvas, waits until that function is
  *   complete, and waits for the user to close the Canvas.  This function effectively calls start(),
  *   <code>myFunction</code>(), and wait() in sequence.
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  */
void Canvas::run(void (*myFunction)(Canvas&) ) {
  start(); myFunction(*this); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int), int i) {
  start(); myFunction(*this, i); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param u An unsigned integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, unsigned), unsigned u) {
  start(); myFunction(*this, u); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i1 An integer argument to myFunction
  * \param i2 An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, int), int i1, int i2) {
  start(); myFunction(*this, i1, i2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i1 An integer argument to myFunction
  * \param i2 An integer argument to myFunction
  * \param i3 An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, int, int), int i1, int i2, int i3) {
  start(); myFunction(*this, i1, i2, i3); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param u1 An unsigned integer argument to myFunction
  * \param u2 An unsigned integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
  start(); myFunction(*this, u1, u2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param s A string argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, std::string),std::string s) {
  start(); myFunction(*this, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i An integer argument to myFunction
  * \param s A string argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s) {
  start(); myFunction(*this, i, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param s A string argument to myFunction
  * \param i An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, std::string, int), std::string s, int i) {
  start(); myFunction(*this, s, i); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i An integer argument to myFunction
  * \param s A string argument to myFunction
  * \param b A boolean argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, std::string, bool), int i, std::string s, bool b) {
  start(); myFunction(*this, i, s, b); wait();
}

void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.png", frameCounter);  // TODO: Make this save somewhere not in root

    screenBufferMutex.lock();
    // loader.saveImageToFile(filename, screenBuffer, winWidthPadded, winHeight);
    for (int j = 0; j < winHeight - (winHeight / 2); j++) {
        for (int i = 0; i <  3 * winWidthPadded; i++) {
            int s1 =  3 * winWidthPadded * j + i;
            int s2 =  3 * winWidthPadded * (winHeight - 1 - j) + i;  // This needs to be height *MINUS ONE* minus j
            char tmp = screenBuffer[s1];
            screenBuffer[s1] = screenBuffer[s2];
            screenBuffer[s2] = tmp;
        }
    }
    stbi_write_png(filename, winWidthPadded, winHeight, 3, screenBuffer, 0);
    screenBufferMutex.unlock();
}

void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    if (can->scrollFunction) can->scrollFunction(xpos, ypos);
}

 /*!
  * \brief Mutator for the Canvas Background.
  * \details This function sets the Background which Canvas will draw.
  *   \param background A pointer to the new Background value which will be assigned to myBackground.
  *   \param previouslySet Boolean indicating if background has been previously set for this Canvas.
  */
void Canvas::setBackground(Background * background, bool previouslySet) {
  backgroundMutex.lock();
  if (myBackground != background) {
    defaultBackground = false;
    myBackground = background;
    if (!previouslySet) {
      windowMutex.lock();
      background->init(shapeShader, textShader, textureShader, camera, window);
      windowMutex.unlock();
    }
  }
  backgroundMutex.unlock();
}

 /*!
  * \brief Mutator for the background color.
  * \details This function sets the clear color for when Canvas::clear() is called.
  *   \param color The color to clear to.
  * \note The alpha channel of the color is ignored.
  */
void Canvas::setBackgroundColor(ColorFloat color) {
    windowMutex.lock();
    if (window != nullptr) {
      glfwMakeContextCurrent(window);
      backgroundMutex.lock();
      myBackground->setClearColor(color);
      backgroundMutex.unlock();
      glfwMakeContextCurrent(NULL);
    }
    windowMutex.unlock();
}

 /*!
  * \brief Mutator for showing the FPS.
  *   \param b Whether to print the FPS to stdout every draw cycle (for debugging purposes).
  */
void Canvas::setShowFPS(bool b) {
    showFPS = b;
}

 /*!
  * \brief Sleeps the calling thread to sync with the Canvas.
  * \details Tells the calling thread to sleep until the Canvas' drawTimer expires.
  * \note It is recommened that you call sleep() at least once before doing any drawing;
  *   otherwise, the Canvas may not render the first frame of drawing.
  * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
  */
void Canvas::sleep() {
  #ifdef __APPLE__
    handleIO();
  #endif
    drawTimer->sleep(false);
}

 /*!
  * \brief Sleeps the calling thread for a set amount of time
  * \details Tells the calling thread to sleep for <code>seconds</code> seconds.
  *   \param seconds Number of seconds to sleep for
  * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
  */
void Canvas::sleepFor(float seconds) {
#ifdef __APPLE__
    handleIO();
#endif
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (seconds * 1000000000)));
}

 /*!
  * \brief Opens the Canvas.
  * \details This function opens a GL window and tells the Canvas to start rendering.
  * \return 0 if start is successful, -1 if the canvas has already started.
  * \see wait(), stop(), close()
  */
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
    c->draw();
    c->isFinished = true;
    pthread_exit(NULL);
}
#else
void Canvas::startDrawing(Canvas *c) {
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
}
#endif

 /*!
  * \brief Begins the process of closing the Canvas.
  * \details This function calls close() followed by wait() to gracefully close the Canvas
  *   at the earliest available opportunity.
  * \see start(), wait(), close()
  */
void Canvas::stop() {
    close();
    wait();
}

 /*!
  * \brief Stops recording the Canvas.
  * \details This function tells the Canvas to stop dumping images to the file system.
  */
void Canvas::stopRecording() {
    toRecord = 0;
}

 /*!
  * \brief Takes a screenshot.
  * \details This function saves a screenshot of the current Canvas to the working directory.
  * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
  * \bug Multiple calls to this function in rapid succession render the FPS counter inaccurate.
  */
void Canvas::takeScreenShot() {
    if (toRecord == 0) toRecord = 1;
}

void Canvas::selectShaders(unsigned int sType) {
    Shader * program = 0;
    if (sType == TEXT_SHADER_TYPE) {
        program = textShader;
        // position attribute
        GLint posAttrib = glGetAttribLocation(textShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint texAttrib = glGetAttribLocation(textShader->ID, "aTexCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
        GLint aspectLoc = glGetUniformLocation(textShader->ID, "aspect");
        glUniformMatrix4fv(aspectLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
    } else if (sType == SHAPE_SHADER_TYPE)  {
        program = shapeShader;
        // position attribute
        GLint posAttrib = glGetAttribLocation(shapeShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint colAttrib = glGetAttribLocation(shapeShader->ID, "aColor");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
    } else if (sType == TEXTURE_SHADER_TYPE) {
        program = textureShader;
        GLint posAttrib = glGetAttribLocation(textureShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint texAttrib = glGetAttribLocation(textureShader->ID, "aTexCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
    }
 
    // Recompute the camera matrices
    uniModel = glGetUniformLocation(program->ID, "model");
    uniView = glGetUniformLocation(program->ID, "view");
    uniProj = glGetUniformLocation(program->ID, "projection");

    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)winWidth/(float)winHeight, 0.1f, 5000.0f);
    glm::mat4 view = camera->getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));
}

 /*!
  * \brief Waits for the user to close the Canvas.
  * \details This function blocks the calling thread until the user closes the Canvas or until
  *   close() is called. This function has no effect if the Canvas has not started.
  * \return 0 if exit is successful, -1 if the Canvas has not started yet.
  * \see start(), end(), close(), stop().
  */
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
}
