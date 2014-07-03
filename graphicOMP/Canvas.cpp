#include "Canvas.h"

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


/*
 * Default constructor for the canvas class
 * Parameter:
 * 		b, the buffer size for the Shapes
 * Returns: a new 800x600 Canvas on the top left of the screen with no title
 */
Canvas::Canvas(unsigned int b) {
	init(0,0,800,600,b,(char*)"Canvas");
}

/*
 * Explicit constructor for the canvas class
 * Parameters:
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		w, the x dimension of the Canvas window
 * 		h, the y dimension of the Canvas window
 * 		b, the buffer size for the Shapes
 * 		title, the title of the window
 * Returns: a new Canvas with the specified positional data and title
 */
Canvas::Canvas(int xx, int yy, int w, int h, unsigned int b, char* title) {
	init(xx,yy,w,h,b,title);
}

Canvas::~Canvas() {
	std::cout << "Tearing Down" << std::endl;

	// Free our clear rectangle's memory
	delete clearRectangle;

	// Free up our resources
	glDeleteProgram(shaderProgram);
	glDeleteShader(shaderFragment);
	glDeleteShader(shaderVertex);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);

	// Release GLFW
	glfwTerminate();
	std::cout << "Torn Down" << std::endl;
}

/*
 * clear removes all shapes and sets the background to the set color
 */
void Canvas::clear() {
	toClear = true;
}

/*
 * draw actually draws our Canvas and its contents to the display
 * Note: this function is called automatically by the spawned thread, which is why it's private
 */
void Canvas::draw() {
	//Reset the window
	glfwSetWindowShouldClose(window, GL_FALSE);

	//Start the drawing loop
	for(framecounter = 0; !glfwWindowShouldClose(window); framecounter++)
	{
		timer->sleep();

		if (toClear) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			clearRectangle->draw();
		}

		// Calculate CycleTime since draw() was last called
		highResClock::time_point end = highResClock::now();
		realFPS = round(1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - cycleTime).count() * 1000000000.0);
		cycleTime = end;

		#pragma omp critical (fps)
		if (showFPS) std::cout << realFPS << '/' << FPS << std::endl;

		Shape* s;				// Pointer to the next Shape in the queue

		mutexLock mBufferLock(buffer);						// Time to flush our buffer
		if (myBuffer->size() > 0) {							// But only if there is anything to flush
			for (unsigned int i = 0; i < myBuffer->size(); i++) {
				myShapes->push(myBuffer->operator[](i));
			}
			myBuffer->shallowClear();						// We want to clear the buffer but not delete those objects as we still need to draw them
		}
		mBufferLock.unlock();

		glDrawBuffer(GL_LEFT);								// See: http://www.opengl.org/wiki/Default_Framebuffer#Color_buffers
		if (toClear) {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_ACCUM_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
			clearRectangle->draw();
			toClear = false;
		}

		unsigned size = myShapes->size();
		if (size == myShapes->capacity())
			std::cerr << "BUFFER OVERFLOW" << std::endl;
		if (allPoints) {
			Point* p;
			unsigned max = size*6;
			for (unsigned int i = 0, x = 0; i < max; i+= 6, x++) {
				p = (Point*)myShapes->operator[](x);
				for (unsigned j = 0; j < 6; j++)
					vertexData[i+j] = p->vertices[j];
			}
			glBufferData(GL_ARRAY_BUFFER, size*sizeof(ColoredVertex), vertexData, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_POINTS, 0, size);
		} else { // Iterate through our queue until we've made it to the end
			for (unsigned int i = 0; i < size; i++) {
				s = myShapes->operator[](i);
				s->draw();
			}
		}

		myShapes->clear();				// Clear our buffer of shapes to be drawn
		glFlush();
		glDrawBuffer(GL_BACK_LEFT);
		glfwSwapBuffers(window);		// Swap out GL's back buffer and actually draw to the window
		HandleIO();						// Handle any I/O
	}
}

/*
 * drawLineColor draws a line at the given coordinates with the given color
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 		color, the RGB color (optional)
 */
void Canvas::drawLine(int x1, int y1, int x2, int y2, RGBfloatType color) {
	Line* l = new Line(x1,y1,x2,y2,color);			// Creates the Line with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(l);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		color, the RGB color (optional)
 */
void Canvas::drawPoint(int x, int y, RGBfloatType color) {
	Point* p = new Point(x,y,color);				// Creates the Point with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(p);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawRectangleColor draws a rectangle with the given coordinates, dimensions, and color
 * Parameters:
 * 		x, the x coordinate of the Rectangle's left edge
 *		y, the y coordinate of the Rectangle's top edge
 * 		w, the width of the Rectangle
 *		h, the height of the Rectangle
 * 		color, the RGB color (optional)
 */
void Canvas::drawRectangle(int x, int y, int w, int h, RGBfloatType color) {
	Rectangle* rec = new Rectangle(x,y,w,h,color);	// Creates the Rectangle with the specified coordinates and color
	mutexLock mlock(buffer);
	myBuffer->push(rec);							// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawShinyPolygon draws a ShinyPolygon with the given vertex data
 * Parameters:
 * 		size, the number of vertices in the polygon
 * 		x, an array of x positions of the vertices
 * 		y, an array of y positions of the vertices
 * 		color, an array of colors for the vertices
 */
void Canvas::drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]) {
	ShinyPolygon* p = new ShinyPolygon(size);
	for (int i = 0; i < size; i++) {
		p->addVertex(x[i],y[i],color[i]);
	}
	mutexLock mlock(buffer);
	myBuffer->push(p);								// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * drawTextColor prints text at the given coordinates with the given color
 * Parameters:
 * 		s, the string to print
 * 		x, the x coordinate of the text's left edge
 * 		y, the y coordinate of the text's top edge
 * 		color, the RGB color (optional)
 */
//void Canvas::drawText(const char * s, int x, int y, RGBfloatType color) {
//	Text* t = new Text(s,x,y,color);				// Creates the Text with the specified string and coordinates
//	mutexLock mlock(buffer);
//	myBuffer->push(t);								// Push it onto our drawing buffer
//	mlock.unlock();
//}

/*
 * drawTriangle draws a Triangle with the given vertices
 * Parameters:
 * 		x1, the x position of the first vertex of the triangle
 * 		y1, the y position of the first vertex of the triangle
 *		x2, the x position of the second vertex of the triangle
 * 		y2, the y position of the second vertex of the triangle
 * 		x3, the x position of the third vertex of the triangle
 * 		y3, the y position of the third vertex of the triangle
 * 		color, the RGB color (optional)
 */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, RGBfloatType color) {
	Triangle* t = new Triangle(x1,y1,x2,y2,x3,y3,color);	// Creates the Triangle with the specified vertices and color
	mutexLock mlock(buffer);
	myBuffer->push(t);										// Push it onto our drawing buffer
	mlock.unlock();
}

/*
 * end closes the Canvas
 * Returns: 0 if exit is successful, -1 otherwise
 */
int Canvas::end() {
	if (!started) return -1;						// If we haven't even started yet, return error code -1
	renderThread.join();							// Blocks until ready to join, which will be when the window is closed
	return 0;
}

/*
 * getTime returns the time elapsed since the Canvas has started drawing (in microseconds)
 */
double Canvas::getTime() {
	return std::chrono::duration_cast<std::chrono::microseconds>(highResClock::now() - startTime).count() / 1000000.0;
}

void Canvas::glInit() {
	glfwInit();										// Initialize GLFW

	// Create a Window and the Context
	//TODO: Add comments here because this is very confusing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_STEREO, GL_FALSE);
	glfwWindowHint(GLFW_VISIBLE,GL_FALSE);
	window = glfwCreateWindow(winWidth, winHeight, title_, nullptr, nullptr); // Windowed
	glfwMakeContextCurrent(window);
	glfwShowWindow(window);

	// Enable and disable necessary stuff
	//TODO: Again, more comments
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_DITHER);
	glDisable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Enable Experimental GLEW to Render Properly
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	// Create / compile vertex shader
	shaderVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shaderVertex, 1, &vertexSource, NULL);
	glCompileShader(shaderVertex);
	GLint status;
	glGetShaderiv(shaderVertex, GL_COMPILE_STATUS, &status);

	// Create / compile fragment shader
	shaderFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shaderFragment, 1, &fragmentSource, NULL);
	glCompileShader(shaderFragment);
	glGetShaderiv(shaderFragment, GL_COMPILE_STATUS, &status);

	// Attach both shaders to a shader program, link the program, and use it
	shaderProgram = glCreateProgram();
	glAttachShader(shaderProgram, shaderVertex);
	glAttachShader(shaderProgram, shaderFragment);
	glBindFragDataLocation(shaderProgram, 0, "outColor");
	glLinkProgram(shaderProgram);
	glUseProgram(shaderProgram);

	// Create and bind our Vertex Array Object
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Create and bind our Vertex Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// Specify the layout of the vertex data
	GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
	GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

	// Get uniform location for camera matrices
	uniModel = glGetUniformLocation(shaderProgram, "model");
	uniView = glGetUniformLocation(shaderProgram, "view");
	uniProj = glGetUniformLocation(shaderProgram, "proj");

	SetupCamera();									// Update the camera with magic numbers
}

void Canvas::HandleIO() {
	// Check for keyboard / mouse interaction
	glfwPollEvents();

	glfwGetCursorPos(window,&mouseX,&mouseY);

	// ESCAPE (Exit)
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		if (!keyDown)									// GL will close multiple windows in a row if we don't check this
			glfwSetWindowShouldClose(window, GL_TRUE);	// Exit the program
		keyDown = true;
	}
	else keyDown = false;
}

/*
 * init initializes the Canvas with the values specified in the constructor
 * Parameters:
 * 		xx, the x position of the Canvas window
 * 		yy, the y position of the Canvas window
 * 		width, the x dimension of the Canvas window
 * 		height, the y dimension of the Canvas window
 * 		b, the buffer size for the Shapes
 * 		title, the title of the window to put on the top window bar
 */
void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, char* title) {
	title_ = title;
	winWidth = ww, winHeight = hh;
	aspect = (float)winWidth / winHeight;
	keyDown = false;
	framecounter = 0;

	//TODO: Remove or update toClear
	toClear = true;															// Don't need to clear at the start
	started = false;  														// We haven't started the window yet
	monitorX = xx; monitorY = yy; winWidth = ww; winHeight = hh;			// Initialize translation
	myShapes = new Array<Shape*>(b);										// Initialize myShapes
	myBuffer = new Array<Shape*>(b);
	vertexData = new float[6*b];											// Buffer for vertexes for points
	showFPS = false;														// Set debugging FPS to false
	//TODO: Redundant as we have a "started"?
	isFinished = false;														// We're not done rendering
	allPoints = false;

	clearRectangle = new Rectangle(0,0,winWidth,winHeight,GREY);

	timer = new Timer(FRAME);
}

/*
 * setBackgroundColor sets the background color
 * Parameters:
 * 		color, the RGBfloatType with the color. The alpha channel is ignored
 */
void Canvas::setBackgroundColor(RGBfloatType color) {
	delete clearRectangle;
	clearRectangle = new Rectangle(0,0,winWidth,winHeight,color);
}

void Canvas::SetupCamera() {
//	static float time = 0;
//	// Set up camera positioning
//	glm::mat4 view = glm::lookAt(
//		glm::vec3(cameraPanX, cameraPanY, -cameraDistance),// Camera position
//		glm::vec3(cameraPanX, cameraPanY, 0.0f),// On-screen center
//		glm::vec3(-0.0f, -1.0f, 0.0f)			// "Up" axis (y = 0.01 because undefined when same as camera position)
//	);
//	glUniformMatrix4fv(uniView, 1, GL_FALSE, glm::value_ptr(view));
//	// Set up camera zooming
//	glm::mat4 proj = glm::perspective(90.0f, 800.0f / 600.0f, 0.01f, 10000.0f);
//	glUniformMatrix4fv(uniProj, 1, GL_FALSE, glm::value_ptr(proj));
//	// Set up camera transformation
//	glm::mat4 model;				//Create a new (identity matrix)
//	model = glm::rotate(
//		model,						//Rotate the model (identity) matrix...
//		0.0f,						//...actually, don't rotate it at all
//		glm::vec3(0.0f, 0.0f, 1.0f)	//...along the Z-axis.
//	);
//	glUniformMatrix4fv(uniModel, 1, GL_FALSE, glm::value_ptr(model));

	// Set up camera positioning
	float viewF[] = { 1,0,0,0,0,-1,0,0,0,0,-1,0,-winWidth/2.0f,winHeight/2.0f,-winHeight/2.0f,1 };
	glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

	// Set up camera zooming
	float projF[] = { 1.0f/aspect,0,0,0,0,1,0,0,0,0,-1.00000191f,-1,0,0,-0.02000002f,0 };
	glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

	// Set up camera transformation
	float modelF[] = { 1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1 };
	glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
}

/*
 * start starts rendering the Canvas
 * Returns: the exit code of the FLTK render method
 */
int Canvas::start() {
	if (started) return -1;										// If we're already started, return error code -1
	started = true;												// We've now started
    renderThread = std::thread(Canvas::startDrawing,this);		// Spawn the rendering thread
    startTime = highResClock::now();							// Record the init time
    return 0;
}

void Canvas::startDrawing(Canvas *c) {
	c->glInit();
	c->draw();
	c->isFinished = true;
	glfwDestroyWindow(c->window);
}
