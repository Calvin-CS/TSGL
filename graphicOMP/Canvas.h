/*
 * Canvas.h provides a window / canvas for all of the drawing operations in the graphicOMP library
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/20/2014
 */

#ifndef CANVAS_H_
#define CANVAS_H_

// Link statically with GLEW
//#define GLEW_STATIC

#include "Point.h"					// Our own class for drawing single points.
#include "Line.h"					// Our own class for drawing straight lines.
#include "Rectangle.h"				// Our own class for drawing rectangles.
#include "Triangle.h"				// Our own class for drawing triangles.
#include "ShinyPolygon.h"			// Our own class for drawing polygons with colored vertices.
#include "Polyline.h"				// Our own class for drawing polylines.
#include "Array.h"					// Our own array for buffering drawing operations.
#include "color.h"					// Our own interface for converting color types
#include "Timer.h"
#include <omp.h>					// For OpenMP support
#include <cmath>					// For converting HSV to RGB and other math stuff
#include <chrono>					// For timing drawing and FPS
#include <thread>					// For spawning rendering in a different thread
#include <mutex>					// Needed for locking the Canvas for thread-safety
#include <iostream> 				// DEBUGGING

//// Disables some deprecation warnings, but messes zooming up
//#define GLM_FORCE_RADIANS

// GL libraries
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GL/gl.h>
#include <GL/glx.h>

// Standard libraries
#include <stdio.h>

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
	"	outColor = vec4(Color);"
	"}";

#define FPS 60
#define FRAME 1.0f/FPS				// Represents a single frame

typedef std::chrono::high_resolution_clock highResClock;
typedef std::unique_lock<std::mutex> mutexLock;

class Canvas {
protected:
	char* title_;													// Title of the window
	Array<Shape*> * myShapes;										// Our buffer of shapes to draw
	Array<Shape*> * myBuffer;										// Our buffer of shapes that the can be pushed to, and will later be flushed to the shapes array
	Timer* t;														// Timer for steady FPS
	int framecounter;												// Counter for the number of frames that have elapsed in the current session (for animations)
	int monitorX,monitorY,monitorWidth,monitorHeight;  				// Positioning and sizing data for the Canvas
	bool toClear;													// Flag for clearing the canvas
	bool started;													// Whether our canvas is running and the frame counter is counting
	std::thread renderThread;										// Thread dedicated to rendering the Canvas
	std::mutex shapes, buffer;										// Mutexes for locking the Canvas so that only one thread can read/write at a time
	float realFPS;													// Actual FPS of drawing
	bool showFPS_;													// Flag to show DEBUGGING FPS
	highResClock::time_point cycleTime, startTime;					// Times to show FPS and render time
	float *vertexData;												// Buffer for vertexes to render with GL
	int winWidth, winHeight;										// Window sizes used for setting up the window
	float aspect;													// Aspect ratio used for setting up the window
	GLFWwindow* window;												// GLFW window that we will draw to
	GLuint shaderProgram, vertexShader, fragmentShader, vao, vbo, ebo;
	GLint uniModel, uniView, uniProj;
	bool keyDown, isFinished, allPoints;
	double mouseX, mouseY;											// Location of the mouse once HandleIO() has been called
	Rectangle* clearRectangle;										// Rectangle for clearing to the background color

	void init(int xx,int yy,int ww,int hh,unsigned int b,char* title);	// Method for initializing the canvas
	void glInit();													// Initializes the GL and GLFW things that are specific for this canvas
	static void drawFunction(Canvas *c);							// Static method that is called by the render thread
	void draw();													// Method for drawing the canvas and the shapes within
	virtual void HandleIO();
	void SetupCamera();
public:
	Canvas(unsigned int b);											// Default constructor for our Canvas
	Canvas(int xx, int yy, int w, int h, unsigned int b, char* title = (char*)"");	// Explicit constructor for our Canvas
	virtual ~Canvas();
	static void glStaticInit();
	int start();													// Function to start rendering our Canvas
	int end();														// Function to end rendering our Canvas
	virtual void drawPoint(int x, int y, RGBfloatType color = BLACK);					// Draws a point at the given coordinates with the given color
	virtual void drawLine(int x1, int y1, int x2, int y2, RGBfloatType color = BLACK);	// Draws a line at the given coordinates with the given color
	virtual void drawRectangle(int x, int y, int w, int h, RGBfloatType color = BLACK);	// Draws a rectangle at the given coordinates with the given dimensions and color
	virtual void drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color = BLACK);	// Draws a triangle with the given vertices and color
	virtual void drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]);	// Draws a polygon of with given number of vertices with shading across it
//	virtual void drawText(const char * s, int x, int y, RGBfloatType color = BLACK);	// Draws a string of text at the given position, with the given color
	void setBackgroundColor(RGBfloatType color);					// Changes the background color
	void clear();													// Clears the canvas
	int getWindowX() 		{ return monitorX; }					// Accessor for the window width
	int getWindowY() 		{ return monitorY; }					// Accessor for the window height
	int getWindowWidth() 	{ return monitorWidth; }				// Accessor for the window width
	int getWindowHeight() 	{ return monitorHeight; }				// Accessor for the window height
	int getFrameNumber() 	{ return framecounter; }				// Accessor for the number of frames rendered so far
	float getFPS() 			{ return realFPS; }						// Accessor for true FPS
	bool isOpen() 			{ return !isFinished; }					// Returns if the window is closed
	void showFPS(bool b) 	{ showFPS_ = b; }						// Mutator to show debugging FPS
	void onlyPoints(bool b) { allPoints = b; }						// Whether we're only drawing points
	double getTime();												// Returns the time since initialization
};

#endif /* CANVAS_H_ */
