/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the TSGL library.
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW
#define GLEW_STATIC

#include "Array.h"          // Our own array for buffering drawing operations
#include "Color.h"          // Our own interface for converting color types
#include "ColoredPolygon.h" // Our own class for drawing polygons with colored vertices
#include "ConcavePolygon.h" // Our own class for concave polygons with colored vertices
#include "ConvexPolygon.h"  // Our own class for convex polygons with colored vertices
#include "Image.h"          // Our own class for drawing images / textured quads
#include "Keynums.h"        // Our enums for key presses
#include "Line.h"           // Our own class for drawing straight lines
#include "Polyline.h"       // Our own class for drawing polylines
#include "ProgressBar.h"    // Our own class for drawing progress bars
#include "Rectangle.h"      // Our own class for drawing rectangles
#include "Text.h"           // Our own class for drawing text
#include "Timer.h"          // Our own timer for steady FPS
#include "Triangle.h"       // Our own class for drawing triangles
//#include "TsglAssert.h"     // For unit testing purposes

#include <cmath>            // For performing math operations
#include <functional>       // For callback upon key presses
#include <iostream>         // DEBUGGING
#include <mutex>            // Needed for locking the Canvas for thread-safety
#include <sstream>          // For string building
#include <string>           // For window titles
#ifdef __APPLE__
  #include <pthread.h>
#else
  #include <thread>           // For spawning rendering in a different thread
#endif


#include <GL/glew.h>        // Needed for GL function calls
#include <GLFW/glfw3.h>     // For window creation and management

#define FPS 60              // Frames per second
#define FRAME 1.0f/FPS      // Number of seconds between frames

#ifdef _WIN32
  #define round(x) ((x-floor(x))>0.5 ? ceil(x) : floor(x))      // round is not defined in Visual Studio
  #define Rectangle tsgl::Rectangle
  #define Polyline tsgl::Polyline
#endif

namespace tsgl {

/*! \class Canvas
 *  \brief A GL window with numerous built-in, thread-safe drawing operations.
 *  \details Canvas provides an easy-to-set-up, easy-to-use class for drawing various shapes.
 *  \details Using stb, Canvas also supports the drawing of images.
 *  \details On top of being easy to use, Canvas is also thread-safe, so any number of images may be drawn at once.
 *  \note <b>OS X:</b> Due to the way OS X handles I/O, either sleep() or handleIO() must be manually called
 *       whenever the user wants to handle any input/output events (keyboard/mouse presses). Whenever a window is
 *       created using OpenGL, OS X requires the main thread to handle I/O calls.
 *  \note <b>OS X:</b> OS X also uses p_thread instead of std::thread for threading.
 */
class Canvas {
private:
    typedef std::function<void()>                   voidFunction;
    typedef std::function<void(double, double)>     doubleFunction;
    typedef GLFWvidmode const*                      displayInfo;

    float           aspect;                                             // Aspect ratio used for setting up the window
    ColorFloat      bgcolor;                                            // Color of the Canvas' clearRectangle
    voidFunction    boundKeys    [(GLFW_KEY_LAST+1)*2];                 // Array of function objects for key binding
    std::mutex      bufferMutex;                                        // Mutex for locking the render buffer so that only one thread can read/write at a time
    Timer*          drawTimer;                                          // Timer to regulate drawing frequency
    uint8_t*        focusBuffer;                                        // Holds a copy of the Canvas's window when it loses focus
    GLuint          frameBuffer;
    int             framecounter;                                       // Counter for the number of frames that have elapsed in the current session (for animations)
    bool            hasBackbuffer;                                      // Whether or not the hardware supports double-buffering
    bool            hasStereo;                                          // Whether or not the hardware supports stereoscopic rendering
    bool            isFinished;                                         // If the rendering is done, which will signal the window to close
    bool            keyDown;                                            // If a key is being pressed. Prevents an action from happening twice
    TextureHandler  loader;                                             // The ImageLoader that holds all our already loaded textures
    bool            loopAround;                                         // Whether our point buffer has looped back to the beginning this
    int             monitorX, monitorY;                                 // Monitor position for upper left corner
    double          mouseX, mouseY;                                     // Location of the mouse once HandleIO() has been called
    Array<Shape*> * myBuffer;                                           // Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
    Array<Shape*> * myShapes;                                           // Our buffer of shapes to draw
    std::mutex      pointArrayMutex;                                    // Mutex for the allPoints array
    unsigned int    pointBufferPosition, pointLastPosition;             // Holds the position of the allPoints array
    int             realFPS;                                            // Actual FPS of drawing
  #ifdef __APPLE__
    pthread_t     renderThread;                                         // Thread dedicated to rendering the Canvas
  #else
    std::thread   renderThread;                                         // Thread dedicated to rendering the Canvas
  #endif
    uint8_t*        screenBuffer;                                       // Array that is a copy of the screen
    doubleFunction  scrollFunction;                                     // Single function object for scrolling
    GLtexture       shaderFragment,                                     // Address of the fragment shader
                    shaderProgram,                                      // Addres of the shader program to send to the GPU
                    shaderVertex;                                       // Address of the vertex shader
    std::mutex      shapesMutex;                                        // Mutex for locking the render array so that only one thread can read/write at a time
    bool            showFPS;                                            // Flag to show DEBUGGING FPS
    bool            started;                                            // Whether our canvas is running and the frame counter is counting
    GLtexture       textureShaderFragment,                              // Address of the textured fragment shader
                    textureShaderProgram,                               // Addres of the textured shader program to send to the GPU
                    textureShaderVertex;                                // Address of the textured vertex shader
    bool            toClose;                                            // If the Canvas has been asked to close
    unsigned int    toRecord;                                           // To record the screen each frame
    std::string     title_;                                             // Title of the window
    bool            toClear;                                            // Flag for clearing the canvas
    GLint           uniModel,                                           // Model perspective of the camera
                    uniView,                                            // View perspective of the camera
                    uniProj;                                            // Projection of the camera
    GLtexture       vertexArray,                                        // Address of GL's array buffer object
                    vertexBuffer;                                       // Address of GL's vertex buffer object
    float*          vertexData;                                         // The allPoints array
    GLFWwindow*     window;                                             // GLFW window that we will draw to
    std::mutex      windowMutex;                                        // (OS X) Mutex for handling window contexts
    int             winWidth, winHeight;                                // Window sizes used for setting up the window

    static displayInfo  monInfo;                                        // Info about our display
    static int          drawBuffer;                                     // Buffer to use for drawing (set to GL_LEFT or GL_RIGHT)
    static bool         glfwIsReady;                                    // Whether or not we have info about our monitor
    static std::mutex   glfwMutex;                                      // Keeps GLFW createWindow from getting called at the same time in multiple threads
    static unsigned     openCanvases;                                   // Total number of open Canvases

    static void buttonCallback(GLFWwindow* window, int key,
                  int action, int mods);                                // GLFW callback for mouse buttons
    void        draw();                                                 // Draw loop for the Canvas
    static void errorCallback(int error, const char* string);           // Display where an error is coming from
    void        glDestroy();                                            // Destroys the GL and GLFW things that are specific for this canvas
    void        init(int xx,int yy,int ww,int hh,
                  unsigned int b, std::string title,
                  double timerLength);                                  // Method for initializing the canvas
    void        initGl();                                               // Initializes the GL things specific to the Canvas
    void        initGlew();                                             // Initialized the GLEW things specific to the Canvas
    static void initGlfw();                                             // Initalizes GLFW for all future canvases.
    void        initWindow();                                           // Initalizes the window specific to the Canvas
    static void keyCallback(GLFWwindow* window, int key,
                  int scancode, int action, int mods);                  // GLFW callback for keys
    void        screenShot();                                           // Takes a screenshot
    static void scrollCallback(GLFWwindow* window, double xpos,
                  double ypos);                                         // GLFW callback for scrolling
    static void setDrawBuffer(int buffer);                              // Sets the buffer used for drawing
    void        setupCamera();                                          // Setup the 2D camera for smooth rendering
  #ifdef __APPLE__
    static void* startDrawing(void* cPtr);
  #else
    static void startDrawing(Canvas *c);                                // Static method that is called by the render thread
  #endif
    void        textureShaders(bool state);                             // Turn textures on or off
    static bool testFilledDraw(Canvas& can);                            // Unit test for drawing shapes and determining if fill works
    //Theory why it doesn't work:
    //Different graphics cards; Nvidia vs. ATI.
    //It works for Patrick's machine, which is has an Nvidia.
    //It doesn't work for the Ulab machines, which has an ATI.
    //The only way to prove this theory is to wait until the new
    //lab machines come in which have Nvidia graphics cards.
    //If they work, then hooray!
    //If not, our theory is wrong and its something totally different.
    static bool testPerimeter(Canvas& can);                             // Unit tester for drawing shapes and determining if the shape is still drawn correctly but not filled
    static bool testLine(Canvas& can);                                  // Unit tester for lines
    static bool testAccessors(Canvas& can);                             // Unit tester for accessor methods
    static bool testDrawImage(Canvas& can);                             // Unit tester for drawing images (simultaneously a Unit test for Image)

protected:
    void        drawShape(Shape* s);                                    // Draw a shape type
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
     *   \param xx The x position of the Canvas window.
     *   \param yy The y position of the Canvas window.
     *   \param w The x dimension of the Canvas window.
     *   \param h The y dimension of the Canvas window.
     *   \param title The title of the window.
     *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
     *     A value less than or equal to 0 sets it to automatic.
     * \return A new Canvas with the specified position, dimensions, title, and draw cycle length.
     */
    Canvas(int xx, int yy, int w, int h, std::string title, double timerLength = 0.0f);

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
     *   \param a The action to look out for (TSGL_PRESS or TSGL_RELEASE).
     *   \param f The function to call upon action <code>a</code> on button.
     * \warning <b>TSGL_KEY_ESCAPE is automatically bound to closing the window. Overriding TSGL_KEY_ESCAPE will
     *   likely make you unable to close the window through the escape key.</b>
     */
    void bindToButton(Key button, Action a, voidFunction f);

    /*!
     * \brief Binds the mouse wheel to a function.
     * \details This function binds the mouse wheel to a function pointer.
     * \details Upon scrolling, Canvas will call the specified function.
     *   \param f A function taking x and y parameters to be called when the mouse is scrolled.
     */
    void bindToScroll(std::function<void(double, double)> f);

    /*!
     * \brief Clears the Canvas.
     * \details This function clears the screen to the color specified in setBackgroundColor().
     */
    void clear();

    /*!
     * \brief Waits for the user to close the Canvas.
     * \details This function blocks the calling thread until the user closes the Canvas or until
     *   close() is called. This function has no effect if the Canvas has not started.
     * \return 0 if exit is successful, -1 if the Canvas has not started yet.
     * \see start(), end(), close(), stop().
     */
    int wait();

    /*!
     * \brief Draws a circle.
     * \details This function draws a circle with the given center, radius, resolution
     *   (number of sides), color, and fill status.
     *   \param x The x coordinate of the circle's center.
     *   \param y The y coordinate of the circle's center.
     *   \param radius The radius of the circle in pixels.
     *   \param res The number of sides to use in the circle.
     *   \param color The color of the circle
     *     (set to BLACK by default).
     *   \param filled Whether the circle should be filled
     *     (set to true by default).
     */
    virtual void drawCircle(int x, int y, int radius, int res, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draws an arbitrary polygon with colored vertices.
     * \details This function draws a ColoredPolygon with the given vertex data, specified as
     *   a triangle strip.
     *   \param size The number of vertices in the polygon.
     *   \param x An array of x positions of the vertices.
     *   \param y An array of y positions of the vertices.
     *   \param color An array of colors for the vertices.
     *   \param filled Whether the colored polygon should be filled (true) or not (false)
     *     (set to true by default).
     */
    virtual void drawColoredPolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws a concave polygon with colored vertices.
     * \details This function draws a ConcavePolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param x An array of x positions of said vertices.
     *   \param y An array of y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the Concave polygon should be filled in or not
     *     (set to true by default).
     * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
     *   that you draw convex polygons with this function.
     * \see drawConvexPolygon().
     */
    virtual void drawConcavePolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws a convex polygon with colored vertices.
     * \details This function draws a ConvexPolygon with the given vertex data, specified as the
     *   outer perimeter of the polygon.
     *   \param size The number of vertices in the polygon.
     *   \param x An array of the x positions of said vertices.
     *   \param y An array of the y positions of said vertices.
     *   \param color An array of colors for the said vertices.
     *   \param filled Whether the ConvexPolygon should be filled in or not
     *     (set to true by default).
     * \note The difference between a convex polygon and a concave polygon
     *   is that a convex polygon has all interior angles less than
     *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
     */
    virtual void drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draws an image.
     * \details This function draws an Image with the given coordinates and dimensions.
     *   \param fname The name of the file to load the image from.
     *   \param x The x coordinate of the Image's left edge.
     *   \param y The y coordinate of the Image's top edge.
     *   \param w The width of the Image.
     *   \param h The height of the Image.
     *   \param a The alpha with which to draw the Image
     *     (set to 1.0f by default).
     */
    virtual void drawImage(std::string fname, int x, int y, int w, int h, float a = 1.0f);

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
     *   a ProgressBar on a CartesianCanvas, you may still use this function, but you must
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
     */
    virtual void drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \param s The string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     */
    virtual void drawText(std::string s, int x, int y, unsigned size, ColorFloat color = BLACK);

     /*!
     * \brief Draws a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *   \param s The UTF8-encoded string to draw.
     *   \param x The x coordinate of the text's left bound.
     *   \param y The y coordinate of the text's left bound.
     *   \param size The size of the text in pixels.
     *   \param color The color of the Text (set to BLACK by default).
     * \note Identical to the drawText(std::string, ...) aside from the first parameter.
     * \see drawText(std::string s, int x, int y, unsigned size, ColorFloat color = BLACK)
     */
    virtual void drawText(std::wstring s, int x, int y, unsigned int size, ColorFloat color);

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
     * \brief Accessor for the current background color.
     * \return The color that the Canvas clears to when clear() is called.
     */
    ColorFloat getBackgroundColor();

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
    bool getIsOpen();

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
     * \note getPixel will return the current status of the screen. Any object waiting to be drawn
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
     * \note getPixel will return the current status of the screen. Any object waiting to be drawn
     *  will not affect what is returned.
     *      \param x The x position of the pixel to grab.
     *      \param y The y position of the pixel to grab.
     * \return A ColorInt containing the color of the pixel at (x, y).
     */
    ColorInt getPoint(int x, int y);

    /*!
     * \brief Accessor for the Canvas's currently drawn image.
     * \note This array starts in the bottom left corner of the image.
     * \return A pointer to the RGB pixel buffer for the current Canvas.
     */
    uint8_t* getScreenBuffer();

    /*!
     * \brief Accessor for the time since the Canvas was initialized.
     * \return The elapsed time in microseconds since the Canvas has started drawing.
     */
    double getTime();

    /*!
     * \brief Accessor for the Canvas's window width.
     * \return The width in pixels of the Canvas window.
     */
    int getWindowWidth();

    /*!
     * \brief Accessor for the Canvas's window height.
     * \return The height in pixels of the Canvas window.
     */
    int getWindowHeight();

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
     * \brief Records the Canvas for a specified number of frames.
     * \details This function starts dumping screenshots of the Canvas to the working directory every draw
     *   cycle.
     * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
     * \details The function automatically terminates after num_frames cycles have completed.
     *   \param num_frames The number of frames to dump screenshots for.
     */
    void recordForNumFrames(unsigned int num_frames);

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
     * \note Supports all font types that FreeType supports, which is almost anything.
     */
    void setFont(std::string filename);

    /*!
     * \brief Mutator for showing the FPS.
     *   \param b Whether to print the FPS to stdout every draw cycle (for debugging purposes).
     */
    void setShowFPS(bool b);

    /*!
     * \brief Stops recording the Canvas.
     * \details This function tells the Canvas to stop dumping images to the file system.
     */
    void stopRecording();

    /*!
     * \brief Opens the Canvas.
     * \details This function opens a GL window and tells the Canvas to start rendering.
     * \return 0 if start is successful, -1 if the canvas has already started.
     * \see wait(), stop(), close()
     */
    int start();

    /*!
     * \brief Sleeps the calling thread to sync with the Canvas.
     * \details Tells the calling thread to sleep until the Canvas' drawTimer expires.
     * \note <b>OS X:</b> This function calls handleIO() on OS X.
     */
    void sleep();

    /*!
     * \brief Sleeps the calling thread for a set amount of time
     * \details Tells the calling thread to sleep for <code>seconds</code> seconds.
     *   \param seconds Number of seconds to sleep for
     * \note <b>OS X:</b> This function calls handleIO() on OS X.
     */
    void sleepFor(float seconds);

    /*!
     * \brief Resets the internal drawing timer of a Canvas instance.
     */
    void reset();

    /*!
     * \brief Accessor for the number of theoretical draw cycles that have elapsed
     * \details This function returns the time elapsed since the Canvas has been opened divided
     *   by the drawTimer's period.
     * \return The number of times the drawTimer has expired since starting the Canvas.
     * \see getFrameNumber()
     */
    unsigned int getReps() const;

    /*!
     * \brief Accessor that gets the time between two sleep times of the internal drawing timer of a Canvas object.
     * \return The time between two sleep cycles of the internal drawing timer.
     */
    double getTimeBetweenSleeps() const;

    /*!
     * \brief Takes a screenshot.
     * \details This function saves a screenshot of the current Canvas to the working directory.
     * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
     * \bug Multiple calls to this function in rapid succession seem to make the FPS counter inaccurate.
     */
    void takeScreenShot();

    /*!
     * \brief Runs unit tests for the Canvas.
     */
    static void runTests();

    /*!
     * \brief Begins the process of closing the Canvas.
     * \details This function calls close() followed by wait() to gracefully close the Canvas
     *   at the earliest available opportunity.
     * \see start(), wait(), close()
     */
    void stop();

    /*!
     * \brief Closes the Canvas window.
     * \details This function tells the Canvas to stop rendering and to close its rendering window.
     * \details Any threads that have called wait() will continue.
     * \see start(), stop(), wait()
     */
    void close();
};

}

#endif /* CANVAS_H_ */
