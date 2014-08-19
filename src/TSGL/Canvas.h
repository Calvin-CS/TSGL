/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the TSGL library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 7/21/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW
#define GLEW_STATIC

#ifdef _WIN32
#define round(x) ((x-floor(x))>0.5 ? ceil(x) : floor(x))      // round is not defined in Visual Studio
#endif

#include "Array.h"          // Our own array for buffering drawing operations
#include "Color.h"          // Our own interface for converting color types
#include "ColoredPolygon.h" // Our own class for drawing polygons with colored vertices
#include "Image.h"          // Our own class for drawing images / textured quads
#include "Keynums.h"        // Our enums for key presses
#include "Line.h"           // Our own class for drawing straight lines
#include "Polyline.h"       // Our own class for drawing polylines
#include "Rectangle.h"      // Our own class for drawing rectangles
#include "Text.h"           // Our own class for drawing text
#include "Timer.h"          // Our own timer for steady FPS
#include "Triangle.h"       // Our own class for drawing triangles

#include <cmath>
#include <functional>       // For callback upon key presses
#include <iostream>         // DEBUGGING
#include <mutex>            // Needed for locking the Canvas for thread-safety
#include <sstream>          // For string building
#include <string>           // For window titles
#include <thread>           // For spawning rendering in a different thread

#include <GL/glew.h>        // Needed for GL function calls
#include <GLFW/glfw3.h>     // For window creation and management

#define FPS 60              // Frames per second
#define FRAME 1.0f/FPS      // Number of seconds between frames

/*! \class Canvas
 *  \brief A GL window with numerous built-in, thread-safe drawing operations
 *  \details Canvas provides an easy-to-set-up, easy-to-use class for drawing various shapes.
 *  \details With libpng and libjpeg, Canvas also supports the drawing of images.
 *  \details On top of being easy to use, Canvas is also thread-safe, so any number of images may be drawn at once.
 */
class Canvas {
private:
    typedef std::function<void()>                   voidFunction;
    typedef std::function<void(double, double)>     doubleFunction;
    typedef std::unique_lock<std::mutex>            mutexLock;

    float           aspect;                                             // Aspect ratio used for setting up the window
    voidFunction    boundKeys    [(GLFW_KEY_LAST+1)*2];                 // Array of function objects for key binding
    std::mutex      buffer;                                             // Mutex for locking the render buffer so that only one thread can read/write at a time
    Rectangle*      clearRectangle;                                     // Rectangle for clearing to the background color
    int             framecounter;                                       // Counter for the number of frames that have elapsed in the current session (for animations)
    bool            isFinished;                                         // If the rendering is done, which will signal the window to close
    bool            keyDown;                                            // If a key is being pressed. Prevents an action from happening twice
    TextureHandler  loader;                                             // The ImageLoader that holds all our already loaded textures
    bool            loopAround;                                         // Whether our point buffer has looped back to the beginning this
    int             monitorX, monitorY;                                 // Monitor position for upper left corner
    double          mouseX, mouseY;                                     // Location of the mouse once HandleIO() has been called
    Array<Shape*> * myBuffer;                                           // Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
    Array<Shape*> * myShapes;                                           // Our buffer of shapes to draw
    std::mutex      pointArray;                                         // Mutex for the allPoints array
    unsigned int    pointBufferPosition, pointLastPosition;             // Holds the position of the allPoints array
    int             realFPS;                                            // Actual FPS of drawing
    std::thread     renderThread;                                       // Thread dedicated to rendering the Canvas
    uint8_t*        screenBuffer;                                       // Array that is a copy of the screen
    doubleFunction  scrollFunction;                                     // Single function object for scrolling
    GLtexture       shaderFragment,                                     // Address of the fragment shader
                    shaderProgram,                                      // Addres of the shader program to send to the GPU
                    shaderVertex;                                       // Address of the vertex shader
    std::mutex      shapes;                                             // Mutex for locking the render array so that only one thread can read/write at a time
    bool            showFPS;                                            // Flag to show DEBUGGING FPS
    bool            started;                                            // Whether our canvas is running and the frame counter is counting
    GLtexture       textureShaderFragment,                              // Address of the textured fragment shader
                    textureShaderProgram,                               // Addres of the textured shader program to send to the GPU
                    textureShaderVertex;                                // Address of the textured vertex shader
    bool            toClose;                                            // If the Canvas has been asked to close
    unsigned int    toRecord;                                           // To record the screen each frame
    bool            toUpdateScreenCopy;                                 // Whether we copy the screen to our buffer next draw cycle
    Timer*          timer;                                              // Timer for steady FPS
    std::string     title_;                                             // Title of the window
    bool            toClear;                                            // Flag for clearing the canvas
    GLint           uniModel,                                           // Model perspective of the camera
                    uniView,                                            // View perspective of the camera
                    uniProj;                                            // Projection of the camera
    GLtexture       vertexArray,                                        // Address of GL's array buffer object
                    vertexBuffer;                                       // Address of GL's vertex buffer object
    float*          vertexData;                                         // The allPoints array
    GLFWwindow*     window;                                             // GLFW window that we will draw to
    int             winWidth, winHeight;                                // Window sizes used for setting up the window

    static std::mutex glfwMutex;                                        // Keeps GLFW createWindow from getting called at the same time in multiple threads

    static void buttonCallback(GLFWwindow* window, int key,
                               int action, int mods);                   // GLFW callback for mouse buttons

    void        draw();                                                 // Draw loop for the Canvas
    static void errorCallback(int error, const char* string);

    void        init(int xx,int yy,int ww,int hh,
                     unsigned int b,std::string title);                 // Method for initializing the canvas
    void        glDestroy();                                            // Destroys the GL and GLFW things that are specific for this canvas
    void        glInit();                                               // Initializes the GL and GLFW things that are specific for this canvas
    static void keyCallback(GLFWwindow* window, int key,
                            int scancode, int action, int mods);        // GLFW callback for keys
    void        screenShot();                                           // Takes a screenshot
    static void scrollCallback(GLFWwindow* window, double xpos,
                               double ypos);                            // GLFW callback for scrolling
    void        SetupCamera();                                          // Setup the 2D camera for smooth rendering
    static void startDrawing(Canvas *c);                                // Static method that is called by the render thread
    void        textureShaders(bool state);                             // Turn textures on or off
protected:
    void        drawShape(Shape* s);                                    // Draw a shape type
public:
    /*!
     * \brief Constructs a new Canvas.
     * \details This is the default constructor for the Canvas class
     *      \param b The size of the Canvas's internal vertex buffer.
     * \return A new 800x600 Canvas in the middle of the screen with no title.
     */
    Canvas(unsigned int b);

    /*!
     * \brief Explicitly constructs a new Canvas.
     * \details This is an explicit constructor for the Canvas class.
     *      \param xx The x position of the Canvas window.
     *      \param yy The y position of the Canvas window.
     *      \param w The x dimension of the Canvas window.
     *      \param h The y dimension of the Canvas window.
     *      \param b The size of the Canvas's internal vertex buffer.
     *      \param title The title of the window.
     * \return A new Canvas with the specified positional data and title.
     */
    Canvas(int xx, int yy, int w, int h, unsigned int b, std::string title = "");

    /*!
     * \brief Destructor for the Canvas class.
     */
    virtual ~Canvas();

    /*!
     * \brief Binds a key or button to a function.
     * \details This function binds a key or mouse button to a function pointer.
     *  Upon pressing or releasing the given key, Canvas will call the specified function.
     *      \param button The key or button to bind, as specified in the Key enum.
     *      \param a The action to look out for (PG_PRESS or PG_RELEASE).
     *      \param f The function to call upon action a on button.
     * \warning PG_KEY_ESCAPE is automatically bound to closing the window. Overriding PG_KEY_ESCAPE will
     *  likely make you unable to close the window.
     */
    void bindToButton(Key button, Action a, voidFunction f);

    /*!
     * \brief Binds the mouse wheel to a function.
     * \details This function binds the mouse wheel to a function pointer.
     *  Upon scrolling, Canvas will call the specified function.
     *      \param f A function taking x and y parameters to be called when the mouse is scrolled.
     */
    void bindToScroll(std::function<void(double, double)> f);

    /*!
     * \brief Clears the Canvas.
     * \details This function removes all shapes and sets the background to the color specified in
     *  setBackgroundColor().
     */
    void clear();

    /*!
     * \brief Waits for the user to close the Canvas.
     * \details This function blocks until the user closes the Canvas. This function has no effect
     *  and will not request for close. Use end() before this to make that request.
     * \return 0 if exit is successful, -1 if the canvas has not started yet.
     * \see start(), end()
     */
    int close();

    /*!
     * \brief Draw a circle.
     * \details This function draws a circle with the given origin coordinates, radius, resolution
     *  (number of sides), and color.
     *      \param x The x coordinate of the circle's origin.
     *      \param y The y coordinate of the circle's origin.
     *      \param radius The radius of the circle in pixels.
     *      \param res The number of sides to use in the circle.
     *      \param color The color of the circle.
     *      \param filled Whether the circle should be filled.
     */
    virtual void drawCircle(int x, int y, int radius, int res, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draw an arbitrary polygon with colored vertices.
     * \details This function draws a ColoredPolygon with the given vertex data
     *      \param size the number of vertices in the polygon
     *      \param x an array of x positions of the vertices
     *      \param y an array of y positions of the vertices
     *      \param color an array of colors for the vertices
     *      \param filled whether the colored polygon should be filled (true) or not (false)
     */
    virtual void drawColoredPolygon(int size, int x[], int y[], ColorFloat color[], bool filled = true);

    /*!
     * \brief Draw an image.
     * \details This function draws an Image with the given coordinates and dimensions.
     *      \param fname The name of the file to load the image from.
     *      \param x The x coordinate of the Image's left edge.
     *      \param y The y coordinate of the Image's top edge.
     *      \param w The width of the Image.
     *      \param h The height of the Image.
     *      \param a The alpha with which to draw the Image.
     */
    virtual void drawImage(std::string fname, int x, int y, int w, int h, float a = 1.0f);

    /*!
     * \brief Draw a line.
     * \details This function draws a Line at the given coordinates with the given color.
     *      \param x1 The x position of the start of the line.
     *      \param y1 The y position of the start of the line.
     *      \param x2 The x position of the end of the line.
     *      \param y2 The y position of the end of the line.
     *      \param color The color of the line.
     */
    virtual void drawLine(int x1, int y1, int x2, int y2, ColorFloat color = BLACK);

    /*!
     * \brief Draw a single pixel.
     * \details This function draws a Point at the given coordinates with the given color.
     *      \param x The x position of the point.
     *      \param y The y position of the point.
     *      \param color The color of the point.
     */
    virtual void drawPoint(int x, int y, ColorFloat color = BLACK);

    /*!
     * \brief Draw a rectangle.
     * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
     *      \param x The x coordinate of the Rectangle's left edge.
     *      \param y The y coordinate of the Rectangle's top edge.
     *      \param w The width of the Rectangle.
     *      \param h The height of the Rectangle.
     *      \param color The color of the rectangle.
     *      \param filled Whether the rectangle should be filled.
     */
    virtual void drawRectangle(int x, int y, int w, int h, ColorFloat color = BLACK, bool filled = true);

    /*!
     * \brief Draw a string of text.
     * \details This function draws a given string of Text at the given coordinates with the given color.
     *      \param s The string to draw.
     *      \param x The x coordinate of the text's left bound.
     *      \param y The y coordinate of the text's left bound.
     *      \param size The size of the text in pixels.
     *      \param color The color of the Text.
     */
    virtual void drawText(std::wstring s, int x, int y, unsigned int size, ColorFloat color = BLACK);

    /*!
     * \brief Draw a triangle.
     * \details This function draws a Triangle with the given vertices.
     *      \param x1 the x coordinate of the first vertex of the Triangle.
     *      \param y1 the y coordinate of the first vertex of the Triangle.
     *      \param x2 the x coordinate of the second vertex of the Triangle.
     *      \param y2 the y coordinate of the second vertex of the Triangle.
     *      \param x3 the x coordinate of the third vertex of the Triangle.
     *      \param y3 the y coordinate of the third vertex of the Triangle.
     *      \param color the color of the Triangle.
     *      \param filled Whether the Triangle should be filled.
     */
    virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color = BLACK,
                              bool filled = true);

    /*!
     * \brief Stops the Canvas.
     * \details This function stops rendering the Canvas. This does not close the Canvas; it will stay open.
     *  To rejoin the threads and complete the close, close() must be called. end() is not required to close
     *  the Canvas, closing the window by hitting escape or with the window manager does the same thing.
     * \details Once end() has been called, either manually, by the window manager, or by pressing escape,
     *  getIsOpen() will return false, and your computation should exit.
     * \see start(), close()
     */
    void end();

    /*!
     * \brief Accessor for the current frame number.
     * \return The number of frames rendered so far.
     */
    int getFrameNumber();

    /*!
     * \brief Accessor for the current FPS.
     * \return The current actual FPS.
     */
    float getFPS();

    /*!
     * \brief Accessor for window's closed status.
     * \return Whether the window is still open (that is, the user has not closed it).
     */
    bool getIsOpen();

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
     * \brief Accessor for the Canvas's currently drawn image.
     * \note This array starts in the bottom left corner of the image.
     * \return A pointer to the RGB pixel buffer for the current Canvas.
     */
    // TODO: Make this return NULL if record isn't on
    uint8_t* getScreenBuffer();

    /*!
     * \brief Accessor for the time since the Canvas was initialized.
     * \return The time elapsed since the Canvas has started drawing (in microseconds).
     */
    double getTime();

    /*!
     * \brief Accessor for the Canvas's width.
     * \return The width in pixels of the Canvas.
     */
    int getWindowWidth();

    /*!
     * \brief Accessor for the Canvas's height.
     * \return The height in pixels of the Canvas.
     */
    int getWindowHeight();

    /*!
     * \brief Accessor for the Canvas's x=position.
     * \return The x coordinate in pixels of the Canvas (0 = left of monitor).
     */
    int getWindowX();

    /*!
     * \brief Accessor for the Canvas's y-position.
     * \return The y coordinate in pixels of the Canvas (0 = top of monitor).
     */
    int getWindowY();

    /*!
     * \brief Records the Canvas for a specified number of frames.
     * \details This function starts dumping screenshots of the Canvas to the file system every draw cycle.
     *  The function automatically terminates after num_frames cycles have completed.
     */
    void recordForNumFrames(unsigned int num_frames);

    /*!
     * \brief Mutator for the background color.
     * \details This function sets the color to clear to when Canvas::clear() is called.
     *      \param color The color to clear to.
     * \note The alpha channel of the color is ignored
     */
    void setBackgroundColor(ColorFloat color);


    /*!
     * \brief Mutator for the currently loaded font
     * \details This function sets the loaded font, loading from the file or from disk if the file was
     *  loaded previously. Subsequent calls to drawText() will use this font to print.
     *      \param filename The filename of the font to load.
     * \note Supports all font types that FreeType supports, which is almost anything.
     */
    void setFont(std::string filename);

    /*!
     * \brief Mutator for showing the FPS.
     *      \param b Whether to print the FPS to stdout every draw cycle (for debugging purposes).
     */
    void setShowFPS(bool b);

    /*!
     * \brief Mutator for keeping track of the screen's drawn buffer.
     *      \param b Whether to copy the screen's contents to an RGB color buffer every draw cycle.
     * \note The buffer in question can be accessed with getScreenBuffer().
     * \note takeScreenShot() and recordForNumFrames() automatically sets this to true.
     */
    void setUpdateScreenCopy(bool b);

    /*!
     * \brief Stops recording the Canvas.
     * \details This function stops dumping images to the file system.
     */
    void stopRecording();

    /*!
     * \brief Opens the Canvas.
     * \details This function starts rendering the Canvas.
     * \return 0 if start is successful, -1 if the canvas has already started.
     * \see close(), end()
     */
    int start();

    /*!
     * \brief Takes a screenshot.
     * \details This function saves a screenshot of the current Canvas to the working directory.
     * \bug Multiple calls to this function in rapid succession seem to make the FPS counter inaccurate and
     *  make rendering slowdown.
     */
    void takeScreenShot();
};

#endif /* CANVAS_H_ */
