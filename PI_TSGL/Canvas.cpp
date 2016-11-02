#include "Canvas.h"

static const GLchar* vertexSource =
  "#version 130\n"
  "in vec2 position;"
  "in vec4 color;"
  "out vec4 Color;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 proj;"
  "void main() {"
  "gl_Position = vec4(position, 0.0, 1.0);"
  "Color = color;"
  "}";
static const GLchar* fragmentSource =
  "#version 130\n"
  "in vec4 Color;"
  "out vec4 outColor;"
  "void main() {"
  "outColor = vec4(Color);"
  "}";
static const GLchar* textureVertexSource =
  "#version 130\n"
  "in vec2 position;"
  "in vec4 color;"
  "in vec2 texcoord;"
  "out vec4 Color;"
  "out vec2 Texcoord;"
  "uniform mat4 model;"
  "uniform mat4 view;"
  "uniform mat4 proj;"
  "void main() {"
  "Texcoord = texcoord;"
  "Color = color;"
  "gl_position = proj * view * model * vec4(position, 0.0, 1.0);"
  "}";
static const GLchar* textureFragmentSource =
  "#version 130\n"
  "in vec4 Color;"
  "in vec2 Texcoord;"
  "out vec4 outColor;"
  "uniform sampler2D tex;"
  "void main() {"
  "outColor = texture(tex, Texcoord) * vec4(Color);"
  "}";

int Canvas::drawBuffer = GL_FRONT_LEFT;
bool Canvas::glfwIsReady = false;
std::mutex Canvas::glfwMutex;
GLFWvidmode const* Canvas::monInfo;
unsigned Canvas::openCanvases = 0;

Canvas::Canvas(int x, int y, int width, int height, std::string title,
	       double timerLength){
  init(x, y, width, height, width*height*2, title, timerLength);
}

void Canvas::errorCallback(int error, const char* string){
  fprintf(stderr, "%i: %s\n", error,string);
}

void Canvas::bindToButton(Key button, Action action, voidFunction function){
  boundKeys[button + action * (GLFW_KEY_LAST + 1)] = function;
}

void Canvas::clear(){
  toClear = true;
}

void Canvas::buttonCallback(GLFWwindow* window, int button, int action,
			    int mods){
  if (action == GLFW_REPEAT) return;
  Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
  int index = button + action * (GLFW_KEY_LAST +1);
  if(&(can->boundKeys[index]) != nullptr) if (can->boundKeys[index])
					    can->boundKeys[index]();
}

void Canvas::draw(){
  glfwSetWindowShouldClose(window, GL_FALSE);
  
  int fbw, fbh;
  glfwGetFramebufferSize(window, &fbw, &fbh);
  int scaling = round((1.0f*fbw)/winWidth);
  
  if (hasStereo)
    Canvas::setDrawBuffer(hasBackbuffer ? GL_FRONT_AND_BACK : GL_FRONT);
  else
    Canvas::setDrawBuffer(hasBackbuffer ? GL_LEFT : GL_FRONT_LEFT);
  
  
  setBackgroundColor(bgcolor);
  glClear(GL_COLOR_BUFFER_BIT);
  glFlush();
  glfwSwapBuffers(window);
  readyToDraw = true;
  bool nothingDrawn = false;
  
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
    if (myBuffer->size() > 0) {     // But only if there is anything to flush
      nothingDrawn = false;
      for (unsigned int i = 0; i < myBuffer->size(); i++)
	myShapes->push((*myBuffer)[i]);
      myBuffer->shallowClear();  // We want to clear the buffer but not delete those objects as we still need to draw them
    }
    bufferMutex.unlock();
    
    int pos = pointBufferPosition;
    int posLast = pointLastPosition;
    pointLastPosition = pos;
    
    if (loopAround || pos != posLast)
      nothingDrawn = false;
    
    if (!nothingDrawn) {
      
      // if (hasEXTFramebuffer)
      //   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
      // else
      //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
      // glDrawBuffer(GL_COLOR_ATTACHMENT0);
      
      glViewport(0,0,winWidth,winHeight);
      
      unsigned int size = myShapes->size();
      for (unsigned int i = 0; i < size; i++) {
	Shape* s = (*myShapes)[i];
	if (!s->getIsTextured()) {
	  s->draw(vertexArray, shaderProgram);  // Iterate through our queue until we've made it to the end
	} else {
	  textureShaders(true);
	  s->draw(vertexArray, shaderProgram);
	  textureShaders(false);
	}
	if (toClear){
	glClear(GL_COLOR_BUFFER_BIT);
	}
	toClear = false;
      }
      
      if (loopAround) {
	nothingDrawn = false;
	int toend = myShapes->capacity() - posLast;
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
    myShapes->clear();                           // Clear our buffer of shapes to be drawn
    
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
    glFlush();
    glDrawArrays(GL_TRIANGLE_STRIP,0,4); //this is making a
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

void Canvas::drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color,
			   bool filled){
  if(filled){
    if(x2 < x1){ int t = x1; x1 = x2; x2 = t; }
    if(y2 < y1){ int t = y1; y1 = y2; y2 = t; }
    Rectangle* rec = new Rectangle(x1, y1, x2-x1, y2-y1, winWidth, winHeight,
				   color, positionLoc, colorLoc);
    drawShape(rec);
  }
  else {
    Polyline* p = new Polyline(5, winWidth, winHeight, positionLoc, colorLoc);
    p->addNextVertex(x1, y1, color);
    p->addNextVertex(x1, y2, color);
    p->addNextVertex(x2, y2, color);
    p->addNextVertex(x2, y1, color);
    p->addNextVertex(x1, y1, color);
    drawShape(p);
  }
}

void Canvas::drawShape(Shape* s){
  if(!started){
    TsglDebug("No drawing before Canvas is started! Ignoring draw request.");
    return;
  }
  while(!readyToDraw)
    sleep();
  bufferMutex.lock();
  myBuffer->push(s);
  bufferMutex.unlock();
}

bool Canvas::isOpen(){
  return !isFinished;
}

int Canvas::getDisplayHeight(){
  initGlfw();
  return monInfo->height;
}

int Canvas::getDisplayWidth(){
  initGlfw();
  return monInfo->width;
}

void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b,
		  std::string title, double timerLength){
  openCanvases++;

  if(ww == -1)
    ww = 1.2*Canvas::getDisplayHeight();
  if(hh == -1)
    hh = -.75*ww;
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
  for(unsigned i = 0; i < bufferSize; ++i){
    screenBuffer[i] = 0;
  }

  toClear = true;
  started = false;
  monitorX = xx;
  monitorY = yy;
  myShapes = new Array<Shape*>(b);
  myBuffer = new Array<Shape*>(b);
  vertexData = new float[6 *b];
  showFPS = false;
  isFinished = false;
  pointBufferPosition = pointLastPosition = 0;
  loopAround = false;
  toRecord = 0;

  bgcolor = GRAY;
  window = nullptr;

  drawTimer = new Timer((timerLength > 0.0f) ? timerLength : FRAME);

  for(int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
    boundKeys[i++] = nullptr;

  initGlfw();
  initWindow();
  initGlew();
  glfwMakeContextCurrent(NULL);
}

void Canvas::initGl(){
  glfwMakeContextCurrent(window);
  glfwSetWindowUserPointer(window, this);

  glDisable(GL_DEPTH_TEST);
  glDisable(GL_DITHER);
  glDisable(GL_CULL_FACE);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this](){
      glfwSetWindowShouldClose(window, GL_TRUE);
      toClose = true;
    });

  unsigned char stereo[1] = {5}, dbuff[1] = {5};
  int aux[1] = {5};
  glGetBooleanv(GL_STEREO, stereo);
  glGetBooleanv(GL_DOUBLEBUFFER, dbuff);
  glGetIntegerv(GL_AUX_BUFFERS, aux);
  hasStereo = ((int)stereo[0] > 0);
  hasBackbuffer = ((int)dbuff[0] > 0);

  glfwMakeContextCurrent(NULL);
}

void Canvas::initGlew(){
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
    GLchar infoLog[512];

    // Create and bind our Vertex Array Object
    glGenVertexArrays(1, &vertexArray);

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
    // Specify the layout of the vertex data in our standard shader
    glLinkProgram(shaderProgram);

    positionLoc = glGetAttribLocation(shaderProgram, "position");
    colorLoc = glGetAttribLocation(shaderProgram, "color");

    glDeleteShader(shaderVertex);
    glDeleteShader(shaderFragment);

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

void Canvas::initGlfw(){
  if(!glfwIsReady){
    glfwInit();
    monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwIsReady = true;
  }
}

void Canvas::initWindow(){
  glfwSetErrorCallback(errorCallback);

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
  
  glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
  glfwWindowHint(GLFW_STEREO, GL_FALSE);
  glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  glfwWindowHint(GLFW_SAMPLES, 4);

  glfwMutex.lock();
  window = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(),
			    NULL, NULL);
  if(!window){
    fprintf(stderr, "GLFW window creation failed");
    exit(100);
  }
  glfwMutex.unlock();

  if(!monInfo){
    fprintf(stderr, "no window info");
    exit(101);
  }

  if(monitorX == -1)
    monitorX = (monInfo->width - winWidth)/2;
  if(monitorY == -1)
    monitorY = (monInfo->height - winHeight)/2;

  glfwSetWindowPos(window, monitorX, monitorY);

  glfwMakeContextCurrent(window);
  glfwShowWindow(window);
  glfwSetWindowUserPointer(window, this);

  glfwSetMouseButtonCallback(window, buttonCallback);
  glfwSetKeyCallback(window, keyCallback);
  glfwSetScrollCallback(window, scrollCallback);
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode,
			 int action, int mods){
  buttonCallback(window, key, action, mods);
}

void Canvas::sleep(){
  drawTimer->sleep(false);
}

void Canvas::screenShot(){
  char filename[25];
  sprintf(filename, "Image%06d.png", frameCounter);

  loader.saveImageToFile(filename, screenBuffer, winWidthPadded, winHeight);
}

void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos){
  Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
  if(can->scrollFunction)
    can->scrollFunction(xpos,ypos);
}

void Canvas::setBackgroundColor(ColorFloat color){
  bgcolor = color;
  if (window != nullptr){
    glfwMakeContextCurrent(window);
    glClearColor(color.R,color.G,color.B,color.A);
    glfwMakeContextCurrent(NULL);
  }
}

void Canvas::setDrawBuffer(int buffer){
  Canvas::drawBuffer = buffer;
}

void Canvas::setupCamera(){
  float viewF[] = {1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
		   -(winWidth-1)/2.0f, (winHeight)/2.0f, -(winHeight)/2.0f, 1};
  glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

  float projF[] = {1.0f/aspect,0,0,0,0,1,0,0,0,0,-1.0f,-1,0,0,-0.02f,0};
  glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

  float modelF[] = {1,0,0,0,0,1,0,0,0,0,1,0,0,0,0,1};
  glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
  
}

int Canvas::start(){
  if (started) return -1;
  started = true;

  renderThread = std::thread(Canvas::startDrawing, this);

  return 0;
}

void Canvas::startDrawing(Canvas* c){
  c->initGl();
  c->draw();
  c->isFinished = true;
  glfwDestroyWindow(c->window);
  //c->glDestroy();
}

void Canvas::textureShaders(bool on){
  GLint program;
  if(!on){
    program = shaderProgram;

    GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
    glEnableVertexAttribArray(posAttrib);
    glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE,
			6 * sizeof(float), 0);
    GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
    glEnableVertexAttribArray(colAttrib);
    glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE,
			  6 * sizeof(float),
			  (void*) (2 * sizeof(float)));
  }
  
  else {
    program = textureShaderProgram;

    GLint texturePosAttrib = glGetAttribLocation(textureShaderProgram,
						 "position");
    glEnableVertexAttribArray(texturePosAttrib);
    glVertexAttribPointer(texturePosAttrib, 2, GL_FLOAT, GL_FALSE,
			  8 * sizeof(float), 0);
    GLint textureColAttrib = glGetAttribLocation(textureShaderProgram,
						 "color");
    glEnableVertexAttribArray(textureColAttrib);
    glVertexAttribPointer(textureColAttrib, 4, GL_FLOAT, GL_FALSE,
			  8 * sizeof(float), (void*) (2 * sizeof(float)));
    GLint textureTexAttrib = glGetAttribLocation(textureShaderProgram,
						 "texcoord");
    glEnableVertexAttribArray(textureTexAttrib);
    glVertexAttribPointer(textureTexAttrib, 2, GL_FLOAT, GL_FALSE,
			  8 * sizeof(float), (void*) (6 * sizeof(float)));
  }

  glUseProgram(program);

  uniModel = glGetUniformLocation(program, "model");
  uniView = glGetUniformLocation(program, "view");
  uniProj = glGetUniformLocation(program, "proj");
  
  setupCamera();
}

int Canvas::wait(){
  if(!started)
    return -1;

  renderThread.join();

  return 0;
}
