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
	"	Texcoord = texcoord;"
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
	"	outColor = texture(tex, Texcoord) * vec4(Color);"
	"}";

/*
 * Default constructor for the canvas class
 * Parameter:
 * 		b, the buffer size for the Shapes
 * Returns: a new 800x600 Canvas on the top left of the screen with no title
 */
Canvas::Canvas(unsigned int b) {
	init(0,0,800,600,b,"");
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
Canvas::Canvas(int xx, int yy, int w, int h, unsigned int b, std::string title) {
	init(xx,yy,w,h,b,title);
}

void Canvas::cleanup() {
	std::cout << "tearing down..." << std::endl;
	// Free our pointer memory
	delete clearRectangle;
	delete myShapes;
	delete myBuffer;
	delete timer;
	delete[] vertexData;

	// Free up our resources
	glDetachShader(shaderProgram,shaderFragment);
	glDetachShader(shaderProgram,shaderVertex);
	glDeleteShader(shaderFragment);
	glDeleteShader(shaderVertex);
	glDeleteProgram(shaderProgram);
	glDetachShader(textureShaderProgram,textureShaderFragment);
	glDetachShader(textureShaderProgram,textureShaderVertex);
	glDeleteShader(textureShaderFragment);
	glDeleteShader(textureShaderVertex);
	glDeleteProgram(textureShaderProgram);
	glDeleteBuffers(1, &vertexBuffer);
	glDeleteVertexArrays(1, &vertexArray);
	glDeleteTextures(1,&tex);
	std::cout << "torn down..." << std::endl;
}

/*
 * bindToButton binds a button or a key to a function pointer, an on that push, Canvas will call that method
 */
void Canvas::bindToButton(key button, action a, voidFunction f) {
	boundKeys[button+a*(GLFW_KEY_LAST+1)] = f;
}

void Canvas::bindToScroll(std::function<void(double, double)> f) {
	scrollFunction = f;
}

void Canvas::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_REPEAT) return;
	Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
	int index = button+action*(GLFW_KEY_LAST+1);
	if (&(can->boundKeys[index]) != nullptr)
		if (can->boundKeys[index])
			can->boundKeys[index]();
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
			toggleTextures(true);
			clearRectangle->draw();
		}

		// Calculate CycleTime since draw() was last called
		highResClock::time_point end = highResClock::now();
		realFPS = round(1.0 / std::chrono::duration_cast<std::chrono::nanoseconds>(end - cycleTime).count() * 1000000000.0);
		cycleTime = end;

		#pragma omp critical (fps)
		if (showFPS) std::cout << realFPS << '/' << FPS << std::endl;

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
			toggleTextures(false);
			toClear = false;
		}

		unsigned int size = myShapes->size();
		if (size == myShapes->capacity())
			std::cerr << "BUFFER OVERFLOW" << std::endl;
		if (allPoints) {
			Point* p;
			unsigned int max = size*6;
			for (unsigned int i = 0, x = 0; i < max; i+= 6, x++) {
				p = (Point*)myShapes->operator[](x);
				for (unsigned j = 0; j < 6; j++)
					vertexData[i+j] = p->vertices[j];
			}
			glBufferData(GL_ARRAY_BUFFER, size*6*sizeof(float), vertexData, GL_DYNAMIC_DRAW);
			glDrawArrays(GL_POINTS, 0, size);
		} else for (unsigned int i = 0; i < size; i++){
			if (!myShapes->operator[](i)->getIsTextured())
				myShapes->operator[](i)->draw(); // Iterate through our queue until we've made it to the end
			else {
				toggleTextures(true);
//				std::cout << "fancy" << std::endl;
				myShapes->operator[](i)->draw();
				toggleTextures(false);
			}
		}

		myShapes->clear();					// Clear our buffer of shapes to be drawn
		glFlush();
		glDrawBuffer(GL_BACK_LEFT);
		glfwSwapBuffers(window);			// Swap out GL's back buffer and actually draw to the window

		glfwPollEvents();					// Handle any I/O
		glfwGetCursorPos(window,&mouseX,&mouseY);
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
 * Returns: 0 if exit is successful, -1 if the canvas has not started yet
 */
int Canvas::end() {
	if (!started) return -1;						// If we haven't even started yet, return error code -1
	renderThread.join();							// Blocks until ready to join, which will be when the window is closed
	cleanup();
	return 0;
}

/*
 * getTime returns the time elapsed since the Canvas has started drawing (in microseconds)
 */
double Canvas::getTime() {
	return std::chrono::duration_cast<std::chrono::microseconds>(highResClock::now() - startTime).count() / 1000000.0;
}

void Canvas::glInit() {
	// Create a Window and the Context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);					// Set target GL major version to 3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);					// Set target GL minor version to 3.2
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);	// We're using the standard GL Profile
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);			// Don't use methods that are deprecated in the target version
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);						// Do not let the user resize the window
	glfwWindowHint(GLFW_STEREO, GL_FALSE);							// Disable the right buffer
	glfwWindowHint(GLFW_VISIBLE,GL_FALSE);							// Don't show the window at first
	window = glfwCreateWindow(winWidth, winHeight,
							title_.c_str(), nullptr, nullptr);		// Windowed
	glfwMakeContextCurrent(window);								// We're drawing to window as soon as it's created
	glfwShowWindow(window);										// Show the window
	glfwSetWindowUserPointer(window, this);

	// Enable and disable necessary stuff
	glDisable(GL_DEPTH_TEST);										// Disable depth testing because we're not drawing in 3d
	glDisable(GL_DITHER);											// Disable dithering because pixels do not (generally) overlap
	glDisable(GL_CULL_FACE);										// Disable culling because the camera is stationary
	glEnable(GL_BLEND);											// Enable blending
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);				// Set blending mode to standard alpha blending

	// Enable Experimental GLEW to Render Properly
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	GLint status;

	// Create and bind our Vertex Array Object
	glGenVertexArrays(1, &vertexArray);
	glBindVertexArray(vertexArray);

	// Create and bind our Vertex Buffer Object
	glGenBuffers(1, &vertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

	// Create and generate a test texture
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Solid White Texture
	float pixels[] = {
		1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f, 1.0f, 1.0f
	};
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, pixels);

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

	toggleTextures(false);

	glfwSetMouseButtonCallback(window, buttonCallback);
	glfwSetKeyCallback(window, keyCallback);
	glfwSetScrollCallback(window, scrollCallback);

	bindToButton(PG_KEY_ESCAPE, PG_PRESS, [this]() {
		glfwSetWindowShouldClose(window, GL_TRUE);
	});
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	buttonCallback(window, key, action, mods);
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
void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title) {
	title_ = title;
	winWidth = ww, winHeight = hh;
	aspect = (float)winWidth / winHeight;
	keyDown = false;
	framecounter = 0;

	toClear = true;															// Don't need to clear at the start
	started = false;  														// We haven't started the window yet
	monitorX = xx; monitorY = yy; winWidth = ww; winHeight = hh;			// Initialize translation
	myShapes = new Array<Shape*>(b);										// Initialize myShapes
	myBuffer = new Array<Shape*>(b);
	vertexData = new float[6*b];											// Buffer for vertexes for points
	showFPS = false;														// Set debugging FPS to false
	isFinished = false;														// We're not done rendering
	allPoints = false;

	clearRectangle = new Rectangle(0,0,winWidth,winHeight,GREY);

	timer = new Timer(FRAME);
	for (int i = 0; i <= GLFW_KEY_LAST*2+1; boundKeys[i++] = nullptr);
}

void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
	Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
	if (can->scrollFunction)
		can->scrollFunction(xpos, ypos);
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
 * Returns: 0 if start is successful, -1 if the canvas has already started
 */
int Canvas::start() {
	if (started) return -1;										// If we're already started, return error code -1
	started = true;												// We've now started
    renderThread = std::thread(Canvas::startDrawing,this);		// Spawn the rendering thread
    startTime = highResClock::now();							// Record the start time
    return 0;
}

void Canvas::startDrawing(Canvas *c) {
	c->glInit();
	c->draw();
	c->isFinished = true;
	glfwDestroyWindow(c->window);
}

void Canvas::toggleTextures(bool on) {
	GLint program;
	if (!on) {
		program = shaderProgram;

		// Relocate the shader attributes
		GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
		glEnableVertexAttribArray(posAttrib);
		glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6*sizeof(float), 0);
		GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
		glEnableVertexAttribArray(colAttrib);
		glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6*sizeof(float), (void*)(2*sizeof(float)));

	} else {
		program = textureShaderProgram;

		// Relocate the shader attributes
		GLint texturePosAttrib = glGetAttribLocation(textureShaderProgram, "position");
		glEnableVertexAttribArray(texturePosAttrib);
		glVertexAttribPointer(texturePosAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), 0);
		GLint textureColAttrib = glGetAttribLocation(textureShaderProgram, "color");
		glEnableVertexAttribArray(textureColAttrib);
		glVertexAttribPointer(textureColAttrib, 4, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(2*sizeof(float)));
		GLint textureTexAttrib = glGetAttribLocation(textureShaderProgram, "texcoord");
		glEnableVertexAttribArray(textureTexAttrib);
		glVertexAttribPointer(textureTexAttrib, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), (void*)(6*sizeof(float)));
	}

	// Reallocate the shader program for use
	glUseProgram(program);

	// Recompute the camera matrices
	uniModel = glGetUniformLocation(program, "model");
	uniView  = glGetUniformLocation(program, "view");
	uniProj  = glGetUniformLocation(program, "proj");

	// Update the camera
	SetupCamera();
}
