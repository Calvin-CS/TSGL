/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW on Windows
#ifdef _WIN32
  #define GLEW_STATIC
#endif

#include "glad/glad.h"      // New loader for GL function calls TODO: fix the path here


// #include "Array.h"          // Our own array for buffering drawing operations
#include "Color.h"          // Our own interface for converting color types
#include "TriangleStrip.h" // Our own class for drawing polygons with colored vertices
#include "Circle.h"         // Our own class for drawing circles
#include "RegularPolygon.h" //Our own class for drawing regular polygons
#include "ConcavePolygon.h" // Our own class for concave polygons with colored vertices
#include "ConvexPolygon.h"  // Our own class for convex polygons with colored vertices
#include "Circle.h" // Our own class for concave polygons with colored vertices
#include "UnfilledCircle.h" //Our own class for unfilled circles
#include "Ellipse.h"        //Our own class for drawing ellipses
#include "UnfilledEllipse.h" //Our own class for unfilled ellipses
#include "UnfilledRegularPolygon.h" //Our own class for unfilled regular polygons
#include "Image.h"          // Our own class for drawing images / textured quads
#include "Keynums.h"        // Our enums for key presses
#include "Line.h"           // Our own class for drawing straight lines
#include "Arrow.h"          // Our own class for drawing arrows
#include "PointLayer.h"     // Class that adds a layer for drawing raster points
#include "Polygon.h"        // Our own class for drawing polygons
#include "Polyline.h"       // Our own class for drawing polylines
#include "ProgressBar.h"    // Our own class for drawing progress bars
#include "Rectangle.h"      // Our own class for drawing rectangles
#include "UnfilledRectangle.h" //Our own class for drawing unfilled rectangles
#include "Star.h"           //Our own class for drawing stars
#include "UnfilledStar.h" //Our own class for drawing unfilled stars
#include "Text.h"           // Our own class for drawing text
#include "Timer.h"          // Our own timer for steady FPS
#include "Triangle.h"       // Our own class for drawing triangles
#include "UnfilledTriangle.h" //Our own class for drawing unfilled triangles
#include "Util.h"           // Needed constants and has cmath for performing math operations

#include <functional>       // For callback upon key presses
#include <iostream>         // DEBUGGING
#include <mutex>            // Needed for locking the Canvas for thread-safety
#include <sstream>          // For string building
#include <string>           // For window titles
#include <algorithm>
// #ifdef __APPLE__
//   #include <pthread.h>
// #else
  #include <thread>           // For spawning rendering in a different thread
// #endif

// #include <GL/glew.h>        // Needed for GL function calls
#include <GLFW/glfw3.h>     // For window creation and management

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

    std::vector<Drawable *> objectBuffer;                                  // Holds a list of pointers to objects drawn each frame
    int currentNewShapeLayerDefault = 0;                                // Layer value set for objects that haven't received an explicit layer designation (default 0)

    float           aspect;                                             // Aspect ratio used for setting up the window
    ColorFloat      bgcolor;                                            // Color of the Canvas' clearRectangle
    voidFunction    boundKeys    [(GLFW_KEY_LAST+1)*2];                 // Array of function objects for key binding
    std::mutex      objectMutex;                                        // Mutex for locking the objectBuffer so that only one thread can add/remove objects at a time
    //TODO do we have too many mutexes?  Probably.
    std::mutex      bufferMutex;                                        // Mutex for locking the render buffer so that only one thread can read/write at a time
    unsigned        bufferSize;                                         // Size of the screen buffer
    Timer*          drawTimer;                                          // Timer to regulate drawing frequency
    GLuint          frameBuffer;                                        // Target buffer for rendering to renderedTexture
    int             frameCounter;                                       // Counter for the number of frames that have elapsed in the current session (for animations)
    bool            hasBackbuffer;                                      // Whether or not the hardware supports double-buffering
    bool            hasEXTFramebuffer;                                  // Whether or not the hardware supports EXT FBOs
    bool            hasStereo;                                          // Whether or not the hardware supports stereoscopic rendering
    bool            isFinished;                                         // If the rendering is done, which will signal the window to close
    bool            keyDown;                                            // If a key is being pressed. Prevents an action from happening twice
    TextureHandler  loader;                                             // The ImageLoader that holds all our already loaded textures
    bool            loopAround;                                         // Whether our point buffer has looped back to the beginning this
    int             monitorX, monitorY;                                 // Monitor position for upper left corner
    double          mouseX, mouseY;                                     // Location of the mouse once HandleIO() has been called
    // Array<Drawable*> * myBuffer;                                           // Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
    // Array<Drawable*> * myShapes;                                           // Our buffer of shapes to draw
    std::mutex      pointArrayMutex;                                    // Mutex for the allPoints array
    unsigned int    pointBufferPosition, pointLastPosition;             // Holds the position of the allPoints array
	bool            readyToDraw;                                        // Whether a Canvas is ready to start drawing
    int             realFPS;                                            // Actual FPS of drawing
    GLuint          renderedTexture;                                    // Texture to which we render to every frame
  // #ifdef __APPLE__
  //   pthread_t     renderThread;                                         // Thread dedicated to rendering the Canvas
  // #else
    std::thread   renderThread;                                         // Thread dedicated to rendering the Canvas
  // #endif
    uint8_t*        screenBuffer;                                       // Array that is a copy of the screen
    doubleFunction  scrollFunction;                                     // Single function object for scrolling
    GLtexture       shaderFragment,                                     // Address of the fragment shader
                    shaderProgram,                                      // Addres of the shader program to send to the GPU
                    shaderVertex;                                       // Address of the vertex shader
    bool            showFPS;                                            // Flag to show DEBUGGING FPS
    bool            started;                                            // Whether our canvas is running and the frame counter is counting
    std::mutex      syncMutex;                                          // Mutex for syncing the rendering thread with a computational thread
    int             syncMutexLocked;                                    // Whether the syncMutex is currently locked
	int             syncMutexOwner;                                     // Thread ID of the owner of the syncMutex
    GLtexture       textureShaderFragment,                              // Address of the textured fragment shader
                    textureShaderProgram,                               // Addres of the textured shader program to send to the GPU
                    textureShaderVertex;                                // Address of the textured vertex shader
    bool            toClose;                                            // If the Canvas has been asked to close
    unsigned int    toRecord;                                           // To record the screen each frame
    // GLint           uniModel,                                           // Model perspective of the camera
    //                 uniView,                                            // View perspective of the camera
    //                 uniProj;                                            // Projection of the camera
    GLtexture       vertexArray,                                        // Address of GL's array buffer object
                    vertexBuffer;                                       // Address of GL's vertex buffer object
    float*          vertexData;                                         // The allPoints array
    GLFWwindow*     window;                                             // GLFW window that we will draw to
    bool            windowClosed;                                       // Whether we've closed the Canvas' window or not
    std::mutex      windowMutex;                                        // (OS X) Mutex for handling window contexts
    int             winHeight;                                          // Height of the Canvas' window
    std::string     winTitle;                                           // Title of the window
    int             winWidth;                                           // Width of the Canvas' window
    int             winWidthPadded;                                     // Window width padded to a multiple of 4 (necessary for taking screenshots)

    static int          drawBuffer;                                     // Buffer to use for drawing (set to GL_LEFT or GL_RIGHT)
    static bool         glfwIsReady;                                    // Whether or not we have info about our monitor
    static std::mutex   glfwMutex;                                      // Keeps GLFW createWindow from getting called at the same time in multiple threads
    static displayInfo  monInfo;                                        // Info about our display
    static unsigned     openCanvases;                                   // Total number of open Canvases

    static void  buttonCallback(GLFWwindow* window, int key,
                   int action, int mods);                               // GLFW callback for mouse buttons
    void         draw();                                                // Draw loop for the Canvas
    static void  errorCallback(int error, const char* string);          // Display where an error is coming from
    void         init(int xx,int yy,int ww,int hh,
                   unsigned int b, std::string title,
                   double timerLength);                                 // Method for initializing the canvas
    void         initGl();                                              // Initializes the GL things specific to the Canvas
    void         initShaders();                                         // Initialize shaders, framebuffers, the works TODO
    void         initGLAD();                                            // Initialize the GL pipeline, that kinda stuff TODO
    static void  initGlfw();                                            // Initalizes GLFW for all future canvases.
    void         initWindow();                                          // Initalizes the window specific to the Canvas
    static void  keyCallback(GLFWwindow* window, int key,
                   int scancode, int action, int mods);                 // GLFW callback for keys
    void         screenShot();                                          // Takes a screenshot
    static void  scrollCallback(GLFWwindow* window, double xpos,
                   double ypos);                                        // GLFW callback for scrolling
    static void  setDrawBuffer(int buffer);                             // Sets the buffer used for drawing
    void         setupCamera();                                         // Setup the 2D camera for smooth rendering
  // #ifdef __APPLE__
  //   static void* startDrawing(void* cPtr);
  // #else
    static void  startDrawing(Canvas *c);                               // Static method that is called by the render thread
  // #endif
    void         textureShaders(bool state);                            // Turn textures on or off
    static bool  testFilledDraw(Canvas& can);                           // Unit test for drawing shapes and determining if fill works
    static bool testLine(Canvas& can);                                  // Unit tester for lines
    static bool testAccessors(Canvas& can);                             // Unit tester for accessor methods
    static bool testDrawImage(Canvas& can);                             // Unit tester for drawing images (simultaneously a Unit test for Image)

public:

    /*!
     * \brief Default Canvas constructor method.
     * \details This is the default constructor for the Canvas class.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new Canvas in the middle of the screen with no title.
     *   The created Canvas will take up approximately 90% of the monitor's height, and will
     *   have a 4:3 aspect ratio.
     */
    Canvas(double timerLength = 0.0f);

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
    Canvas(int x, int y, int width, int height, std::string title, double timerLength = 0.0f);

    /*!
     * \brief Canvas destructor method.
     * \details This is the destructor for the Canvas class.
     * \details Frees up memory that was allocated to a Canvas instance.
     */
    virtual ~Canvas();

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
    void bindToButton(Key button, Action action, voidFunction function);

    /*!
     * \brief Binds the mouse wheel to a function.
     * \details This function binds the mouse wheel to a function pointer.
     * \details Upon scrolling, Canvas will call the specified function.
     *   \param function A function taking x and y parameters to be called when the mouse is scrolled.
     */
    void bindToScroll(std::function<void(double, double)> function);

    /*!
     * \brief Clears the Canvas.
     * \details This function clears the screen to the color specified in setBackgroundColor().
     */
    void clear();

    /*!
     * \brief Closes the Canvas window.
     * \details This function tells the Canvas to stop rendering and to close its rendering window.
     * \details Any threads that have called wait() will continue.
     * \see start(), stop(), wait()
     */
    void close();

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
// NEW OOP STUFF
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    /**
     * \brief Adds a Drawable to the Canvas.
     * \details If the Drawable's layer has not been set, it will default to <code>currentNewShapeLayerDefault</code>,
     * which can be set through <code>setDefaultLayer()</code>.
     *    \param shapePtr Pointer to the Drawable to add to this Canvas.
     */
    void add(Drawable * shapePtr);

    /**
     * \brief Removes a Drawable from the Canvas.
     * \details Removes shapePtr from the Canvas's drawing buffer.
     *    \param shapePtr Pointer to the Drawable to remove from this Canvas.
     * \warning The Drawable being deleted or going out of scope before remove() is called will cause a segmentation fault.
     * \warning If shapePtr is not in the drawing buffer, behavior is undefined.
     */
    void remove(Drawable * shapePtr);

    /**
     * \brief Removes all Drawables from the Canvas.
     * \details Clears all Drawables from the drawing buffer.
     *    \param shouldFreeMemory Whether the pointers will be deleted as well as removed and free their memory. (Defaults to false.)
     * \warning Setting shouldFreeMemory to true will cause a segmentation fault if the user continues to access the pointer to a
     *  Drawable that has been added to the Canvas.
     * \warning Setting shouldFreeMemory to false will leak memory from any objects created in Canvas draw methods.
     */
    void clearObjectBuffer(bool shouldFreeMemory = false);

    /**
     * \brief Prints the number of and pointers to items in the drawing buffer.
     */
    void printBuffer();

    // Layering
    /**
     * \brief Accessor for <code>currentNewShapeLayerDefault</code>.
     * \return The default layer for Drawables added to this Canvas.
     * New Drawables will be set to this layer only if their layer has not been previously specified.
     */
    int getDefaultLayer();

    /**
     * \brief Sets the default layer.
     * \details Sets <code>currentNewShapeLayerDefault</code> to n if n >= 0.
     *    \param n The new default layer.
     */
    void setDefaultLayer(int n);


////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////









//TODO: remove this stuff, just for trying things!
float data[9] = {1.0, 0.0, 1.0, 0.0, 0.0, -1.0, -1.0, 0.0, 1.0};
GLuint triangleVBO;

/* This is a handle to the shader program */
// GLuint shaderProgram;

/* These pointers will receive the contents of our shader source code files */
GLchar *vertexSource, *fragmentSource;

/* These are handles used to reference the shaders */
GLuint vertexShader, fragmentShader;
const unsigned int shaderAttribute = 0;

GLuint VertexArrayID;




























////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
//NOTE: we're keeing these here to be backwards compaitble with the old version
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////



    /*!
     * \brief Draws a circle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color, and fill status.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param sides The number of sides to use in the circle.
     *   \param color The color of the circle
     *     (set to BLACK by default).
     *   \param filled Whether the circle should be filled
     *     (set to true by default).
     */
    virtual void drawCircle(int x, int y, int radius, int sides, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draws a concave polygon with colored vertices.
     * \details This function draws a ConcavePolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of x positions of said vertices.
     *   \param yverts An array of y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the Concave polygon should be filled in or not
     *     (set to true by default).
     * \warning <b>This function is significantly slower than drawConvexPolygon().</b> It is not recommended
     *   that you draw convex polygons with this function.
     * \see drawConvexPolygon().
     */
    virtual void drawConcavePolygon(int size, int xverts[], int yverts[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws a convex polygon with colored vertices.
     * \details This function draws a ConvexPolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of the x positions of said vertices.
     *   \param yverts An array of the y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the ConvexPolygon should be filled in or not
     *     (set to true by default).
     * \note The difference between a convex polygon and a concave polygon
     *   is that a convex polygon has all interior angles less than
     *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
     */
    virtual void drawConvexPolygon(int size, int xverts[], int yverts[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws an image.
     * \details This function draws an Image with the given coordinates and dimensions.
     *   \param filename The name of the file to load the image from.
     *   \param x The x coordinate of the Image's left edge.
     *   \param y The y coordinate of the Image's top edge.
     *   \param width The width of the Image.
     *   \param height The height of the Image.
     *   \param alpha The alpha with which to draw the Image
     */
    virtual void drawImage(std::string filename, int x, int y, int width, int height, float alpha = 1.0f);

    /*!
     * \brief Draws a line.
     * \details This function draws a Line at the given coordinates with the given color.
     *   \param x1 The x position of the start of the line.
     *   \param y1 The y position of the start of the line.
     *   \param x2 The x position of the end of the line.
     *   \param y2 The y position of the end of the line.
     *   \param color The color of the line
     *     (set to BLACK by default).
     */
    virtual void drawLine(int x1, int y1, int x2, int y2, ColorFloat color = BLACK);

    /*!
     * \brief Draws a single pixel, specified in row,column format.
     * \details This function draws a pixel at the given screen coordinates with the given color.
     * \note (0,0) signifies the <b>top-left</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>bottom-left</b> of the screen when working with a CartesianCanvas object.
     *   \param row The row (y-position) of the pixel.
     *   \param col The column (x-position) of the pixel.
     *   \param color The color of the point (set to BLACK by default).
     * \see drawPoint()
     */
    virtual void drawPixel(int row, int col, ColorFloat color = BLACK);

    /*!
     * \brief Draws a single pixel, specified in x,y format.
     * \details This function draws a pixel at the given Cartesian coordinates with the given color.
     * \note (0,0) signifies the <b>left-top</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>left-bottom</b> of the screen when working with a CartesianCanvas object.
     *   \param x The x position of the point.
     *   \param y The y position of the point.
     *   \param color The color of the point (set to BLACK by default).
     * \see drawPixel()
     */
    virtual void drawPoint(int x, int y, ColorFloat color = BLACK);

    /*!
     * \brief Draws a progress bar.
     * \details This function draws a previously created ProgressBar to the Canvas, as
     *   specified in that ProgressBar's constructor.
     *   \param p A pointer to a ProgressBar.
     * \note There is no equivalent function for CartesianCanvas. If you'd like to draw
     *   a ProgressBar on a CartesianCanvas, you can still use this function, but you must
     *   use absolute Canvas coordinates rather than the scaled CartesianCanvas coordinates.
     */
    virtual void drawProgress(ProgressBar* p);

    /*!
     * \brief Draws a rectangle.
     * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
     *   \param x1 The x coordinate of the Rectangle's left edge.
     *   \param y1 The y coordinate of the Rectangle's top edge.
     *   \param x2 The x coordinate of the Rectangle's right edge.
     *   \param y2 The y coordinate of the Rectangle's bottom edge.
     *   \param color The color of the rectangle
     *     (set to BLACK by default).
     *   \param filled Whether the rectangle should be filled
     *     (set to true by default).
     * \bug The bottom-right pixel of a non-filled rectangle may not get drawn on some machines.
     */
    virtual void drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \param text The string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     */
    virtual void drawText(std::string text, int x, int y, unsigned size, ColorFloat color = BLACK);

     /*!
     * \brief Draws a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \param text The UTF8-encoded string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     * \note Identical to the drawText(std::string, ...) aside from the first parameter.
     * \see drawText(std::string s, int x, int y, unsigned size, ColorFloat color = BLACK)
     */
    virtual void drawText(std::wstring text, int x, int y, unsigned int size, ColorFloat color = BLACK);

    /*!
     * \brief Draws a triangle.
     * \details This function draws a Triangle with the given vertices.
     *   \param x1 The x coordinate of the first vertex of the Triangle.
     *   \param y1 The y coordinate of the first vertex of the Triangle.
     *   \param x2 The x coordinate of the second vertex of the Triangle.
     *   \param y2 The y coordinate of the second vertex of the Triangle.
     *   \param x3 The x coordinate of the third vertex of the Triangle.
     *   \param y3 The y coordinate of the third vertex of the Triangle.
     *   \param color The color of the Triangle (set to BLACK by default).
     *   \param filled Whether the Triangle should be filled (set to true by default).
     */
    virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color = BLACK,
                              bool filled = true);

    /*!
     * \brief Draws an arbitrary triangle strip with colored vertices.
     * \details This function draws a TriangleStrip with the given vertex data, specified as
     *   a triangle strip.
     *   \param size The number of vertices in the polygon.
     *   \param xverts An array of x positions of the vertices.
     *   \param yverts An array of y positions of the vertices.
     *   \param color An array of colors for the vertices.
     *   \param filled Whether the triangle strip should be filled (true) or not (false).
     */
    virtual void drawTriangleStrip(int size, int xverts[], int yverts[], ColorFloat color[], bool filled = true);

    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////
    //     //TODO move ^ to another location?
    ////////////////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////////////////

































    /*!
     * \brief Accessor for the current background color.
     * \return The color that the Canvas clears to when clear() is called.
     */
    ColorFloat getBackgroundColor();

    /*!
     * \brief Accessor for the height of the user's primary monitor.
     * \return The height of the user's primary monitor.
     */
    static int getDisplayHeight();

    /*!
     * \brief Accessor for the width of the user's primary monitor.
     * \return The width of the user's primary monitor.
     */
    static int getDisplayWidth();

    /*!
     * \brief Accessor for the current frame number.
     * \return The number of actual draw cycles / frames the Canvas has rendered so far.
     * \see getReps()
     */
    int getFrameNumber();

    /*!
     * \brief Accessor for the current FPS.
     * \return The average number of frames being rendered per second.
     */
    float getFPS();

    /*!
     * \brief Accessor for window's closed status.
     * \return Whether the window is still open (that is, the user has not closed it).
     */
    bool isOpen();

    /*!
     * \brief Accessor for the mouse's x-position.
     * \return The x coordinates of the mouse on the Canvas.
     */
    int getMouseX();

    /*!
     * \brief Accessor for the mouse's y-position.
     * \return The y coordinates of the mouse on the Canvas.
     */
    int getMouseY();

    /*!
     * \brief Gets the color of the pixel drawn on the current Canvas at the given screen coordinates,
     *   specified in row,column format.
     * \note (0,0) signifies the <b>top-left</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>bottom-left</b> of the screen when working with a CartesianCanvas.
     * \note getPixel() will return only what is currently drawn the screen. Any object waiting to be drawn
     *  will not affect what is returned.
     *      \param row The row (y-position) of the pixel to grab.
     *      \param col The column (x-position) of the pixel to grab.
     * \return A ColorInt containing the color of the pixel at (col,row).
     */
    ColorInt getPixel(int row, int col);

    /*!
     * \brief Gets the color of the pixel drawn on the current Canvas at the given screen coordinates,
     *   specified in x,y format.
     * \note (0,0) signifies the <b>left-top</b> of the screen when working with a Canvas object.
     * \note (0,0) signifies the <b>left-bottom</b> of the screen when working with a CartesianCanvas.
     * \note getPoint() will return only what is currently drawn the screen. Any object waiting to be drawn
     *  will not affect what is returned.
     *      \param x The x position of the pixel to grab.
     *      \param y The y position of the pixel to grab.
     * \return A ColorInt containing the color of the pixel at (x, y).
     */
    ColorInt getPoint(int x, int y);

    /*!
     * \brief Accessor for the number of theoretical draw cycles that have elapsed
     * \details This function returns the time elapsed since the Canvas has been opened divided
     *   by the drawTimer's period.
     * \return The number of times the drawTimer has expired since starting the Canvas.
     * \see getFrameNumber()
     */
    unsigned int getReps() const;

    /*!
     * \brief Accessor for the Canvas's currently drawn image.
     * \return A pointer to the RGB pixel buffer for the current Canvas.
     * \note The array starts in the bottom left corner of the image, and is in row-major ordering.
     * \deprecated <b>This function returns a pointer directly to the Canvas' screen buffer. This
     *   function may be removed in future versions of TSGL. Please use getPixel() or getPoint()
     *   get individual pixels.
     */
    uint8_t* getScreenBuffer();

    /*!
     * \brief Accessor for the time since the Canvas was initialized.
     * \return The elapsed time in microseconds since the Canvas has started drawing.
     */
    double getTime();

    /*!
     * \brief Accessor that gets the time between two sleep times of the internal drawing timer of a Canvas object.
     * \return The time between two sleep cycles of the internal drawing timer.
     */
    double getTimeBetweenSleeps() const;

    /*!
     * \brief Accessor for the Canvas's window height.
     * \return The height in pixels of the Canvas window.
     */
    int getWindowHeight();

    /*!
     * \brief Accessor for the Canvas's window width.
     * \return The width in pixels of the Canvas window.
     */
    int getWindowWidth();

    /*!
     * \brief Accessor for the Canvas's x-position.
     * \return The x coordinate in pixels of the left of the Canvas (0 = left of monitor).
     */
    int getWindowX();

    /*!
     * \brief Accessor for the Canvas's y-position.
     * \return The y coordinate in pixels of the top of the Canvas (0 = top of monitor).
     */
    int getWindowY();

    /*!
     * \brief Manually handles keyboard/mouse I/O.
     * \note This function will return immediately if not called from the main thread.
     * \note <b>OS X:</b> This function exists for OS X compatibility purposes.
     *   This function does nothing on Linux or Windows.
     */
    void handleIO();

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
    void pauseDrawing();

    /*!
     * \brief Records the Canvas for a specified number of frames.
     * \details This function starts dumping screenshots of the Canvas to the working directory every draw
     *   cycle.
     * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
     * \details The function automatically terminates after num_frames cycles have completed.
     *   \param num_frames The number of frames to dump screenshots for.
     */
    void recordForNumFrames(unsigned int num_frames);

    /*!
     * \brief Resets the internal drawing timer of a Canvas instance.
     * \details This function resets the starting time of the Canvas' draw timer
     *   to the current time.
     */
    void reset();

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
    void resumeDrawing();

    /*!
     * \brief Start the Canvas, run a function on it, and wait for the user to close it
     * \details This function binds another function to the current Canvas, waits until that function is
     *   complete, and waits for the user to close the Canvas.  This function effectively calls start(),
     *   <code>myFunction</code>(), and wait() in sequence.
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     */
    virtual void run(void (*myFunction)(Canvas&));

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param i An integer argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, int), int i);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param u An unsigned integer argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, unsigned), unsigned u);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param i1 An integer argument to myFunction
     * \param i2 An integer argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, int, int), int i1, int i2);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param u1 An unsigned integer argument to myFunction
     * \param u2 An unsigned integer argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param s A string argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, std::string),std::string s);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param i An integer argument to myFunction
     * \param s A string argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s);

    /*!
     * \brief Overload for run()
     * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
     *   which is a reference to the Canvas to render to.
     * \param s A string argument to myFunction
     * \param i An integer argument to myFunction
     */
    virtual void run(void (*myFunction)(Canvas&, std::string, int), std::string s, int i);

    /*!
     * \brief Mutator for the background color.
     * \details This function sets the clear color for when Canvas::clear() is called.
     *   \param color The color to clear to.
     * \note The alpha channel of the color is ignored.
     */
    void setBackgroundColor(ColorFloat color);

    /*!
     * \brief Mutator for the currently loaded font.
     * \details This function sets the font with the specified filename into memory.
     *   Subsequent calls to drawText() will use this font to print.
     *   \param filename The filename of the font to load.
     * \note Supports all font types that FreeType supports.
     */
    void setFont(std::string filename);

    /*!
     * \brief Mutator for showing the FPS.
     *   \param b Whether to print the FPS to stdout every draw cycle (for debugging purposes).
     */
    void setShowFPS(bool b);

    /*!
     * \brief Sleeps the calling thread to sync with the Canvas.
     * \details Tells the calling thread to sleep until the Canvas' drawTimer expires.
     * \note It is recommened that you call sleep() at least once before doing any drawing;
     *   otherwise, the Canvas may not render the first frame of drawing.
     * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
     */
    void sleep();

    /*!
     * \brief Sleeps the calling thread for a set amount of time
     * \details Tells the calling thread to sleep for <code>seconds</code> seconds.
     *   \param seconds Number of seconds to sleep for
     * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
     */
    void sleepFor(float seconds);

    /*!
     * \brief Opens the Canvas.
     * \details This function opens a GL window and tells the Canvas to start rendering.
     * \return 0 if start is successful, -1 if the canvas has already started.
     * \see wait(), stop(), close()
     */
    int start();

    /*!
     * \brief Begins the process of closing the Canvas.
     * \details This function calls close() followed by wait() to gracefully close the Canvas
     *   at the earliest available opportunity.
     * \see start(), wait(), close()
     */
    void stop();

    /*!
     * \brief Stops recording the Canvas.
     * \details This function tells the Canvas to stop dumping images to the file system.
     */
    void stopRecording();

    /*!
     * \brief Takes a screenshot.
     * \details This function saves a screenshot of the current Canvas to the working directory.
     * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
     * \bug Multiple calls to this function in rapid succession render the FPS counter inaccurate.
     */
    void takeScreenShot();

    /*!
     * \brief Waits for the user to close the Canvas.
     * \details This function blocks the calling thread until the user closes the Canvas or until
     *   close() is called. This function has no effect if the Canvas has not started.
     * \return 0 if exit is successful, -1 if the Canvas has not started yet.
     * \see start(), end(), close(), stop().
     */
    int wait();

    /*!
     * \brief Runs unit tests for the Canvas.
     */
    static void runTests();
};

}

#endif /* CANVAS_H_ */
