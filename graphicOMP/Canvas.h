/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Mark Vander Stel, 7/3/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW
//#define GLEW_STATIC

#include "Array.h"          // Our own array for buffering drawing operations.
#include "color.h"          // Our own interface for converting color types
#include "keynums.h"        // Our enums for key presses
#include "Timer.h"          // Our own timer for steady FPS

#include "Image.h"          // Our own class for drawing images / textured quads
#include "Line.h"           // Our own class for drawing straight lines.
#include "Point.h"          // Our own class for drawing single points.
#include "Polyline.h"       // Our own class for drawing polylines.
#include "Rectangle.h"      // Our own class for drawing rectangles.
#include "ShinyPolygon.h"   // Our own class for drawing polygons with colored vertices.
#include "Text.h"           // Our own class for drawing text
#include "Triangle.h"       // Our own class for drawing triangles.

#include <chrono>           // For timing drawing and FPS
#include <functional>       // For callback upon key presses
#include <iostream>         // DEBUGGING
#include <mutex>            // Needed for locking the Canvas for thread-safety
#include <omp.h>            // For OpenMP support
#include <string>           // For window titles
#include <thread>           // For spawning rendering in a different thread

// GL libraries
#include <GL/gl.h>          // For GL functions
#include <GLFW/glfw3.h>     // For window creation and management

#define FPS 60              // Frames per second
#define FRAME 1.0f/FPS      // Number of seconds between frames

class Canvas {
private:
    typedef std::chrono::high_resolution_clock      highResClock;
    typedef highResClock::time_point                timePoint;
    typedef std::function<void()>                   voidFunction;
    typedef std::function<void(double, double)>     doubleFunction;
    typedef std::unique_lock<std::mutex>            mutexLock;

    bool            allPoints;
    float           aspect;                                             // Aspect ratio used for setting up the window
    voidFunction    boundKeys    [(GLFW_KEY_LAST+1)*2];                 // Array of function objects for key binding
    std::mutex      buffer;                                             // Mutex for locking the render buffer so that only one thread can read/write at a time
    Rectangle*      clearRectangle;                                     // Rectangle for clearing to the background color
    timePoint       cycleTime;                                          // Time when the last frame started
    int             framecounter;                                       // Counter for the number of frames that have elapsed in the current session (for animations)
    bool            isFinished;                                         // If the rendering is done, which will signal the window to close
    bool            keyDown;                                            // If a key is being pressed. Prevents an action from happening twice
    ImageLoader     loader;                                             // The ImageLoader that holds all our already loaded textures
    bool            loopAround;                                         // Whether our point buffer has looped back to the beginning this
    int             monitorX, monitorY;                                 // Monitor position for upper left corner
    double          mouseX, mouseY;                                     // Location of the mouse once HandleIO() has been called
    Array<Shape*> * myBuffer;                                           // Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
    Array<Shape*> * myShapes;                                           // Our buffer of shapes to draw
    std::mutex      pointArray;                                         // Mutex for the allPoints array
    unsigned int    pointBufferPosition, pointLastPosition;             // Holds the position of the allPoints array
    float           realFPS;                                            // Actual FPS of drawing
    std::thread     renderThread;                                       // Thread dedicated to rendering the Canvas
    uint8_t*        screenBuffer;                                       // Array that is a copy of the screen
    doubleFunction  scrollFunction;                                     // Single function object for scrolling
    GLuint          shaderFragment,                                     // Address of the fragment shader
                    shaderProgram,                                      // Addres of the shader program to send to the GPU
                    shaderVertex;                                       // Address of the vertex shader
    std::mutex      shapes;                                             // Mutex for locking the render array so that only one thread can read/write at a time
    bool            showFPS;                                            // Flag to show DEBUGGING FPS
    bool            started;                                            // Whether our canvas is running and the frame counter is counting
    timePoint       startTime;                                          // Start time, to show how much time has elapsed
    GLuint          tex,                                                // Texture
                    tex2,                                               // Texture 2
                    textureShaderFragment,                              // Address of the textured fragment shader
                    textureShaderProgram,                               // Addres of the textured shader program to send to the GPU
                    textureShaderVertex;                                // Address of the textured vertex shader
    Timer*          timer;                                              // Timer for steady FPS
    std::string     title_;                                             // Title of the window
    bool            toClear;                                            // Flag for clearing the canvas
    GLint           uniModel,                                           // Model perspective of the camera
                    uniView,                                            // View perspective of the camera
                    uniProj;                                            // Projection of the camera
    GLuint          vertexArray,                                        // Address of GL's array buffer object
                    vertexBuffer;                                       // Address of GL's vertex buffer object
    float*          vertexData;                                         // The allPoints array
    GLFWwindow*     window;                                             // GLFW window that we will draw to
    int             winWidth, winHeight;                                // Window sizes used for setting up the window

    static void buttonCallback(GLFWwindow* window, int key,
                               int action, int mods);                   // GLFW callback for mouse buttons
    void        draw();                                                 // Method for drawing the canvas and the shapes within
    void        init(int xx,int yy,int ww,int hh,
                     unsigned int b,std::string title);                 // Method for initializing the canvas
    void        glInit();                                               // Initializes the GL and GLFW things that are specific for this canvas
    static void keyCallback(GLFWwindow* window, int key,
                            int scancode, int action, int mods);        // GLFW callback for keys
    static void scrollCallback(GLFWwindow* window, double xpos,
                               double ypos);                            // GLFW callback for scrolling
    void        SetupCamera();                                          // Setup the 2D camera for smooth rendering
    static void startDrawing(Canvas *c);                                // Static method that is called by the render thread
    void        textureShaders(bool state);                             // Turn textures on or off
protected:
    void     drawShape(Shape* s);                                       // Draw a shape type
public:
    Canvas(unsigned int b);                                             // Default constructor for our Canvas
    Canvas(int xx, int yy, int w, int h,
           unsigned int b, std::string title = "");                     // Explicit constructor for our Canvas
    virtual ~Canvas();

    void     bindToButton(Key button, Action a, voidFunction f);        // Bind a method to a mouse button or key
    void     bindToScroll(std::function<void(double, double)> f);       // Bind a method to scrolling
    void     clear();                                                   // Clears the canvas

    virtual void drawImage(std::string fname, int x, int y,
            int w, int h, float a = 1.0f);                              // Draws an image at the given coordinates with the given dimensions
    virtual void drawLine(int x1, int y1, int x2, int y2,
            RGBfloatType color = BLACK);                                // Draws a line at the given coordinates with the given color
    virtual void drawPoint(int x, int y,
            RGBfloatType color = BLACK);                                // Draws a point at the given coordinates with the given color
    virtual void drawRectangle(int x, int y, int w, int h,
            RGBfloatType color = BLACK);                                // Draws a rectangle at the given coordinates with the given dimensions and color
    virtual void drawShinyPolygon(int size, int x[], int y[],
            RGBfloatType color[]);                                      // Draws a polygon of with given number of vertices with shading across it
    virtual void drawText(std::string s, int x, int y,
            RGBfloatType color = BLACK);                                // Draws a string of text at the given coordinates with the given color
    virtual void drawTriangle(int x1, int y1, int x2, int y2,
            int x3, int y3, RGBfloatType color = BLACK);                // Draws a triangle with the given vertices and color

    int      end();                                                     // Function to end rendering our Canvas

    int      getFrameNumber()      { return framecounter; }             // Accessor for the number of frames rendered so far
    float    getFPS()              { return realFPS; }                  // Accessor for actual FPS
    bool     getIsOpen()           { return !isFinished; }              // Returns if the window is closed
    int      getMouseX()           { return mouseX; }                   // Returns the screen X coordinate of the mouse
    int      getMouseY()           { return mouseY; }                   // Returns the screen Y coordinate of the mouse
    uint8_t* getScreenBuffer()     { return screenBuffer; }             // Returns a pointer to a copy of the screen
    double   getTime();                                                 // Returns the time since initialization
    int      getWindowWidth()      { return winWidth; }                 // Accessor for the window width
    int      getWindowHeight()     { return winHeight; }                // Accessor for the window height
    int      getWindowX()          { return monitorX; }                 // Accessor for the monitor x coord
    int      getWindowY()          { return monitorY; }                 // Accessor for the monitor y coord

    void     setOnlyPoints(bool b) { allPoints = b; }                   // Whether we're only drawing points
    void     setShowFPS(bool b)    { showFPS = b; }                     // Mutator to show debugging FPS
    void     setBackgroundColor(RGBfloatType color);                    // Changes the background color

    int      start();                                                   // Function to start rendering our Canvas
};

#endif /* CANVAS_H_ */
