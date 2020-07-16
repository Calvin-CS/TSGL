/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW on Windows
#ifdef _WIN32
  #define GLEW_STATIC
#endif

#include "Array.h"          // Our own array for buffering drawing operations
#include "Arrow.h"	    // Our own array for drawing arrows
#include "Color.h"          // Our own interface for converting color types
#include "Cone.h"           // Our own class for drawing cones
#include "Cube.h"           // Our own class for drawing cubes
#include "Cuboid.h"         // Our own class for drawing cuboids
#include "Cylinder.h"       // Our own class for drawing cylinders
#include "TriangleStrip.h" // Our own class for drawing polygons with colored vertices
#include "Ellipse.h"        // Our own class for drawing ellipses
#include "Ellipsoid.h"      // Our own class for drawing ellipsoids
#include "Circle.h" 	    // Our own class for drawing circles
#include "ConcavePolygon.h" // Our own class for concave polygons with colored vertices
#include "ConvexPolygon.h"  // Our own class for convex polygons with colored vertices
#include "Image.h"          // Our own class for drawing images / textured quads
#include "Keynums.h"        // Our enums for key presses
#include "Line.h"           // Our own class for drawing straight lines
#include "Polyline.h"       // Our own class for drawing polylines
// #include "ProgressBar.h"    // Our own class for drawing progress bars
#include "Pyramid.h"        // Our own class for drawing pyramids
#include "Rectangle.h"      // Our own class for drawing rectangles
#include "RegularPolygon.h" // Our own class for drawing regular polygons
#include "Sphere.h"         // Our own class for drawing spheres
#include "Square.h"         // Our own class for drawing squares
#include "Star.h"           // Our own class for drawing stars
#include "Text.h"           // Our own class for drawing text
#include "Timer.h"          // Our own timer for steady FPS
#include "Triangle.h"       // Our own class for drawing triangles
#include "Background.h"     // Our own class for drawing a background
#include "TextureHandler.h" // Currently used for screenshots, might change this
#include "Util.h"           // Needed constants and has cmath for performing math operations

#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <unistd.h>
#include <fstream>
#include <sys/stat.h>

#include <functional>       // For callback upon key presses
#include <iostream>         // DEBUGGING
#include <mutex>            // Needed for locking the Canvas for thread-safety
#include <sstream>          // For string building
#include <string>           // For window titles
#include <algorithm>
#include <omp.h>
#ifdef __APPLE__
  #include <pthread.h>
#else
  #include <thread>           // For spawning rendering in a different thread
#endif

#include "gl_includes.h"

#ifdef _WIN32
  #define round(x) ((x-floor(x))>0.5 ? ceil(x) : floor(x))      // round is not defined in Visual Studio
  #define Rectangle tsgl::Rectangle
  #define Polyline tsgl::Polyline
#endif

namespace tsgl {

/*! \class Canvas
 *  \brief A GL window with numerous built-in, thread-safe drawing operations.
 *  \details Canvas provides an easy-to-set-up, easy-to-use class for drawing various shapes.
 *  \details Using STB, Canvas also supports the drawing of images.
 *  \details On top of being easy to use, Canvas is also thread-safe, so any number of images may be drawn at once.
 *  \note <b>OS X:</b> Due to the way OS X handles I/O, either sleep() or handleIO() must be manually called
 *       whenever the user wants to handle any input/output events (keyboard/mouse presses). Whenever a window is
 *       created using OpenGL, OS X requires the main thread to handle I/O calls.
 *  \note <b>OS X:</b> OS X also uses p_thread instead of std::thread for threading.
 *  \bug <b>Linux:</b> X forwarding does not work properly with TSGL.
 */
class Canvas {
private:
    typedef GLFWvidmode const*                      displayInfo;
    typedef std::function<void(double, double)>     doubleFunction;
    typedef std::function<void()>                   voidFunction;

    // float           aspect;                                             // Aspect ratio used for setting up the window
    Background *    myBackground;
    voidFunction    boundKeys    [(GLFW_KEY_LAST+1)*2];                 // Array of function objects for key binding
    std::mutex      bufferMutex;                                        // Mutex for locking the render buffer so that only one thread can read/write at a time
    unsigned        bufferSize;                                         // Size of the screen buffer
    bool            defaultBackground;                                  // Boolean indicating whether myBackground has been set by an external source
    std::string     defaultFontFileName;
    Timer*          drawTimer;                                          // Timer to regulate drawing frequency
    int             frameCounter;                                       // Counter for the number of frames that have elapsed in the current session (for animations)
    bool            isFinished;                                         // If the rendering is done, which will signal the window to close
    bool            keyDown;                                            // If a key is being pressed. Prevents an action from happening twice
    TextureHandler  loader;                                             // The TextureHandler that holds all our already loaded textures
    bool            loopAround;                                         // Whether our point buffer has looped back to the beginning this
    int             monitorX, monitorY;                                 // Monitor position for upper left corner
    double          mouseX, mouseY;                                     // Location of the mouse once HandleIO() has been called
    std::vector<Drawable*> objectBuffer;                                    // Holds a list of pointers to objects drawn each frame
    std::mutex	    objectMutex;
    std::mutex      pointArrayMutex;                                    // Mutex for the allPoints array
    unsigned int    pointBufferPosition, pointLastPosition;             // Holds the position of the allPoints array
	  bool            readyToDraw;                                        // Whether a Canvas is ready to start drawing
    int             realFPS;                                            // Actual FPS of drawing
  #ifdef __APPLE__
    pthread_t     renderThread;                                         // Thread dedicated to rendering the Canvas
  #else
    std::thread   renderThread;                                         // Thread dedicated to rendering the Canvas
  #endif
    uint8_t*        screenBuffer;                                       // Array that is a copy of the screen
    doubleFunction  scrollFunction;                                     // Single function object for scrolling                                    // Address of the vertex shader
    Shader *        textShader;                                         // Shader for Text class
    Shader *        shapeShader;                                        // Shader for Shape class
    Shader *        textureShader;                                      // Shader for Background and Image classes
    std::mutex      shapesMutex;                                        // Mutex for locking the render array so that only one thread can read/write at a time
    bool            showFPS;                                            // Flag to show DEBUGGING FPS
    bool            started;                                            // Whether our canvas is running and the frame counter is counting
    std::mutex      syncMutex;                                          // Mutex for syncing the rendering thread with a computational thread
    int             syncMutexLocked;                                    // Whether the syncMutex is currently locked
	  int             syncMutexOwner;                                     // Thread ID of the owner of the syncMutex                               // Address of the textured vertex shader
    bool            toClose;                                            // If the Canvas has been asked to close
    unsigned int    toRecord;                                           // To record the screen each frame
    GLint           uniModel,                                           // Model perspective of the camera
                    uniView,                                            // View perspective of the camera
                    uniProj;                                            // Projection of the camera
    GLuint          VAO,                                                // Address of GL's vertex array object
                    VBO;                                                // Address of GL's vertex buffer object
    float*          vertexData;                                         // The allPoints array
    GLFWwindow*     window;                                             // GLFW window that we will draw to
    bool            windowClosed;                                       // Whether we've closed the Canvas' window or not
    std::mutex      windowMutex;                                        // (OS X) Mutex for handling window contexts
    int             winHeight;                                          // Height of the Canvas' window
    std::string     winTitle;                                           // Title of the window
    GLint           winWidth;                                           // Width of the Canvas' window
    GLint           winWidthPadded;                                     // Window width padded to a multiple of 4 (necessary for taking screenshots)

    static int          drawBuffer;                                     // Buffer to use for drawing (set to GL_LEFT or GL_RIGHT)
    static bool         glfwIsReady;                                    // Whether or not we have info about our monitor
    static std::mutex   glfwMutex;                                      // Keeps GLFW createWindow from getting called at the same time in multiple threads
    static displayInfo  monInfo;                                        // Info about our display
    static unsigned     openCanvases;                                   // Total number of open Canvases

    static void  buttonCallback(GLFWwindow* window, int key,
                   int action, int mods);                               // GLFW callback for mouse buttons
    void         draw();                                                // Draw loop for the Canvas
    static void  errorCallback(int error, const char* string);          // Display where an error is coming from
    void         glDestroy();                                           // Destroys the GL and GLFW things that are specific for this canvas
    void         init(int xx,int yy,int ww,int hh,
                   unsigned int b, std::string title, 
                   ColorFloat backgroundColor, double timerLength);     // Method for initializing the canvas
    void         initBackground(ColorFloat bgcolor);                    // Initializes myBackground
    void         initGl();                                              // Initializes the GL things specific to the Canvas
    void         initGlew();                                            // Initialized the GLEW things specific to the Canvas
    static void  initGlfw();                                            // Initalizes GLFW for all future canvases.
    void         initWindow();                                          // Initalizes the window specific to the Canvas
    static void  keyCallback(GLFWwindow* window, int key,
                   int scancode, int action, int mods);                 // GLFW callback for keys
    void         screenShot();                                          // Takes a screenshot
    static void  scrollCallback(GLFWwindow* window, double xpos,
                   double ypos);                                        // GLFW callback for scrolling
    static void  setDrawBuffer(int buffer);                             // Sets the buffer used for drawing
    void         setupCamera();                                         // Setup the 2D camera for smooth rendering
  #ifdef __APPLE__
    static void* startDrawing(void* cPtr);
  #else
    static void  startDrawing(Canvas *c);                               // Static method that is called by the render thread
  #endif
    void         selectShaders(unsigned int choice);                    // Select appropriate shader for type of Drawable
    // static bool  testFilledDraw(Canvas& can);                           // Unit test for drawing shapes and determining if fill works
    // static bool testLine(Canvas& can);                                  // Unit tester for lines
    static bool testAccessors(Canvas& can);                             // Unit tester for accessor methods
    // static bool testDrawImage(Canvas& can);                             // Unit tester for drawing images (simultaneously a Unit test for Image)

protected:
    bool        atiCard;                                                // Whether the vendor of the graphics card is ATI
    void        drawDrawable(Drawable* s);                              // Draw a drawable type
public:

    Canvas(double timerLength = 0.0f);

    Canvas(int x, int y, int width, int height, std::string title, ColorFloat backgroundColor = GRAY, double timerLength = 0.0f);

    virtual ~Canvas();

    void bindToButton(Key button, Action action, voidFunction function);

    void bindToScroll(std::function<void(double, double)> function);

    void add(Drawable * shapePtr);

    void clearBackground();

    void close();

    void clearObjectBuffer(bool shouldFreeMemory = false);

    // virtual void drawArrow(float x1, float y1, float x2, float y2, const ColorFloat color, bool doubleArrow = false);

    // virtual void drawArrow(float x1, float y1, float x2, float y2, const ColorFloat color[], bool doubleArrow = false);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat color, bool filled = true);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat color[], bool filled = true);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat fillColor, ColorFloat outlineColor);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat fillColor[], ColorFloat outlineColor);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat fillColor, ColorFloat outlineColor[]);

    // virtual void drawCircle(int x, int y, int radius, ColorFloat fillColor[], ColorFloat outlineColor[]);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat color, bool filled, float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat color[], bool filled, float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawImage(std::string filename, int x, int y, int width, int height, float alpha = 1.0f, float rotation = 0);

    // virtual void drawLine(int x1, int y1, int x2, int y2, ColorFloat color = BLACK, float rotation = 0);

    // virtual void drawLine(int x1, int y1, int x2, int y2, ColorFloat color[], float rotation = 0);

    // virtual void drawPixel(int row, int col, ColorFloat color = BLACK);

    // virtual void drawPoint(int x, int y, ColorFloat color = BLACK);

    // virtual void drawPolyline(int size, int x[], int y[], ColorFloat color, float rotation = 0);

    // virtual void drawPolyline(int size, int x[], int y[], ColorFloat color[], float rotation = 0);

    // virtual void drawProgress(ProgressBar* p);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawRectangle(float x, float y, float w, float h, ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat color = BLACK, bool filled = true, float rotation = 0);

    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);
    
    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);
    
    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat color, bool filled = true, bool ninja = false, float rotation = 0);

    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat color[], bool filled = true, bool ninja = false, float rotation = 0);

    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat fillColor, ColorFloat outlineColor, bool ninja = false, float rotation = 0);

    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat fillColor[], ColorFloat outlineColor, bool ninja = false, float rotation = 0);

    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat fillColor, ColorFloat outlineColor[], bool ninja = false, float rotation = 0);

    // virtual void drawStar(int x1, int y1, int radius, int points, ColorFloat fillColor[], ColorFloat outlineColor[], bool ninja = false, float rotation = 0);

    // virtual void drawText(std::string text, int x, int y, unsigned size, ColorFloat color = BLACK, std::string fontFileName = "", float rotation = 0);

    // virtual void drawText(std::wstring text, int x, int y, unsigned int size, ColorFloat color = BLACK, std::string fontFileName = "", float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat color, bool filled = true, float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat color[], bool filled = true, float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor, float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor, float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[], float rotation = 0);

    // virtual void drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[], float rotation = 0);

    Background * getBackground();

    ColorFloat getBackgroundColor();

    static int getDisplayHeight();

    static int getDisplayWidth();

    int getFrameNumber();

    float getFPS();

    int getMouseX();

    int getMouseY();

    ColorInt getPixel(int row, int col);

    ColorInt getPoint(int x, int y);

    unsigned int getReps() const;

    uint8_t* getScreenBuffer();

    double getTime();

    double getTimeBetweenSleeps() const;

    int getWindowHeight();

    int getWindowWidth();

    int getWindowX();

    int getWindowY();

    void handleIO();

    bool isOpen();

    void pauseDrawing();

    void recordForNumFrames(unsigned int num_frames);

    void remove(Drawable * shapePtr);

    void reset();

    void resumeDrawing();

    virtual void run(void (*myFunction)(Canvas&));

    virtual void run(void (*myFunction)(Canvas&, int), int i);

    virtual void run(void (*myFunction)(Canvas&, unsigned), unsigned u);

    virtual void run(void (*myFunction)(Canvas&, int, int), int i1, int i2);
    
    virtual void run(void (*myFunction)(Canvas&, int, int, int), int i1, int i2, int i3);

    virtual void run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2);

    virtual void run(void (*myFunction)(Canvas&, std::string),std::string s);

    virtual void run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s);

    virtual void run(void (*myFunction)(Canvas&, std::string, int), std::string s, int i);

    virtual void run(void (*myFunction)(Canvas&, int, std::string, bool), int i, std::string s, bool b);

    void setBackground(Background * background, bool previouslySet = false);

    void setBackgroundColor(ColorFloat color);

    void setFont(std::string filename);

    void setShowFPS(bool b);

    void sleep();

    void sleepFor(float seconds);

    int start();

    void stop();

    void stopRecording();

    void takeScreenShot();

    int wait();

    // static void runTests();
};

}

#endif /* CANVAS_H_ */
