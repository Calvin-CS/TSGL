#include "Canvas.h"

namespace tsgl {

  inline void die_on_gl_error(const char* location) {
    GLenum error = GL_NO_ERROR;
    error = glGetError();
    if (GL_NO_ERROR != error) {
      printf("GL Error %x encountered in %s.\n", error, location);
      exit(1);
    }
  }


  // Shader Source Text TODO: make these into separate files

  // const char *vertexShaderSource =
  // "#version 120\n"
  // "attribute vec2 position;"
  // "void main(void) {"
  // "  gl_Position = vec4(position, 0.0, 1.0);"
  // "}";
  //
  // const char *fragmentShaderSource =
  // "#version 120\n"
  // "void main(void) {"
  // "  gl_FragColor[0] = .5;"
  // "  gl_FragColor[1] = 0.0;"
  // "  gl_FragColor[2] = 1.0;"
  // "}";


  //TODO move this somewhere better
  char* filetobuf(char *file)
  {
    FILE *fptr;
    long length;
    char *buf;

    fptr = fopen(file, "r"); /* Open file for reading */
    if (!fptr) { /* Return NULL on failure */
      fprintf(stderr, "%s\n", "Unable to open file for reading!");
      return NULL;
    }
    fseek(fptr, 0, SEEK_END); /* Seek to the end of the file */
    length = ftell(fptr); /* Find out how many bytes into the file we are */
    buf = (char*)malloc(length + 1); /* Allocate a buffer for the entire length of the file plus a null terminator */
    fseek(fptr, 0, SEEK_SET); /* Go back to the beginning of the file */
    fread(buf, length, 1, fptr); /* Read the contents of the file in to the buffer */
    fclose(fptr); /* Close the file */
    buf[length] = 0; /* Null terminator */

    return buf; /* Return the buffer */
  }


  int Canvas::drawBuffer = GL_FRONT_LEFT;
  bool Canvas::glfwIsReady = false;
  std::mutex Canvas::glfwMutex;
  GLFWvidmode const* Canvas::monInfo;
  unsigned Canvas::openCanvases = 0;

  Canvas::Canvas(double timerLength) {
    init(-1, -1, -1, -1, -1, "", timerLength);
  }

  Canvas::Canvas(int x, int y, int width, int height, std::string title, double timerLength) {
    init(x, y, width, height, width*height*2, title, timerLength);
  }

  Canvas::~Canvas() {
    // Free our pointer memory
    delete myShapes;
    delete myBuffer;
    delete drawTimer;
    delete[] vertexData;
    delete [] screenBuffer;
    //TODO: make this also delete the object buffer?
    if (--openCanvases == 0) {
      glfwIsReady = false;
      glfwTerminate();  // Terminate GLFW
    }
  }

  void Canvas::bindToButton(Key button, Action action, voidFunction function) {
    boundKeys[button + action * (GLFW_KEY_LAST + 1)] = function;
  }

  void Canvas::bindToScroll(std::function<void(double, double)> function) {
    scrollFunction = function;
  }

  void Canvas::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_REPEAT) return;
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    int index = button + action * (GLFW_KEY_LAST + 1);
    if (&(can->boundKeys[index]) != nullptr) if (can->boundKeys[index]) can->boundKeys[index]();
  }

  void Canvas::clear() {
    //TODO this works with the new version now, but it could probably be cleaned up a bit
    //TODO move this to the section for backwards compatibility?
    this->clearObjectBuffer(true); //Clears the items from the buffer and deletes all pointers
    //TODO: decide whether this should also delete all items
  }

  void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    TsglDebug("Window closed successfully.");
  }




  void Canvas::add(Drawable * shapePtr) {

    //TODO: make this check for duplicates
    //TODO: check that this is properly thread safe now
    //TODO: check that the shapes will change layer if layer is changed after addition.

    // Set the default current layer if layer not explicitly set
    if (shapePtr->getLayer() < 0) shapePtr->setLayer(currentNewShapeLayerDefault);

    objectMutex.lock();
    objectBuffer.push_back(shapePtr);
    std::stable_sort(objectBuffer.begin(), objectBuffer.end(), [](Drawable * a, Drawable * b)->bool {
      return (a->getLayer() < b->getLayer());  // true if A's layer is higher than B's layer
    });
    objectMutex.unlock();

  }

  void Canvas::remove(Drawable * shapePtr) {

    //TODO: make this thread safe! (check that it is now)

    objectMutex.lock();
    objectBuffer.erase(std::remove(objectBuffer.begin(), objectBuffer.end(), shapePtr), objectBuffer.end());
    objectMutex.unlock();

  }

  void Canvas::clearObjectBuffer(bool shouldFreeMemory = false) {
    //TODO: check that this frees memory when the user requests it
    if( shouldFreeMemory ) {
      for(unsigned i = 0; i < objectBuffer.size(); i++) {
        delete objectBuffer[i]; //TODO fix this, causes to crash
      }
    }
    objectBuffer.clear();
  }

  void Canvas::printBuffer() {

    // std::cout << "Printing array:" << std::endl << std::endl;
    printf("Printing %ld elements in buffer:\n\n", objectBuffer.size());

    for(std::vector<Drawable *>::iterator it = objectBuffer.begin(); it != objectBuffer.end(); ++it) {
      std::cout << *it << std::endl;
    }
  }

  float data[] = {1.0, 0.0, 0.0, 0.0, 1.0, 0.0, -1.0, 0.0, 0.0};

  static const GLfloat g_vertex_buffer_data[] = {
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
    0.0f,  1.0f, 0.0f,
  };

  void Canvas::newInit() {
    printf("%s\n", "Initting stuff.");

    initShaders();
    //TODO: add shader support here.  Is the Raspi even capable of this???

  }

  GLfloat gQuadVertices[] = {
    100.0f,100.0f,
    0.0f,100.0f,
    0.0f,0.0f,
    100.0f, 0.0f
  };


  void Canvas::newDraw() {
    printf("%s\n", "Drawing stuff.");

    glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created

    // Reset the window close flag, so that the window stays open for this frame
    glfwSetWindowShouldClose(window, GL_FALSE);

    setupCamera();  //Camera transformations

    // Count number of frames
    int counter = 0;
    float lastTime = 0;
    while (!glfwWindowShouldClose(window)) {

      // glDrawBuffer(GL_BACK);
      // glUseProgram(shaderProgram);   // TODO enable this when we've got shader support

      // Clear the canvas
      glClear(GL_COLOR_BUFFER_BIT);

      // Enable vertex arrays
      glEnableClientState( GL_VERTEX_ARRAY );

      // Iterate through objects, render them
      objectMutex.lock();

      // printf("%s\n", "WAZZUP?????");

      for(std::vector<Drawable *>::iterator it = objectBuffer.begin(); it != objectBuffer.end(); ++it) {
        try {
          if ((*it)->getIsDiscreteRendered()) {
            Text* rc = *it; //TODO too hackey?
            rc->render();
          } else {
            Shape* rc = *it; //TODO too hackey?
            glVertexPointer(
              2,  // how many points per vertex (for us, that's x and y)
              GL_FLOAT, // the type of data being passed
              0, // byte offset between vertices
              rc->getPointerToVerticesArray()  // pointer to the array of vertices
            );
            glColor4f(
              rc->getObjectColor()->R,
              rc->getObjectColor()->G,
              rc->getObjectColor()->B,
              rc->getObjectColor()->A
            );
            glDrawArrays(
              rc->getGeometryType(), // The type of geometry from the object (eg. GL_TRIANGLES)
              0, // The starting index of the array
              rc->getNumberOfVertices() // The number of vertices from the object
            );
            if( (*it)->getHasOutline() ) {
              Polygon* poly = *it; //TODO too hackey?
              glVertexPointer(
                2,  // how many points per vertex (for us, that's x and y)
                GL_FLOAT, // the type of data being passed
                0, // byte offset between vertices
                poly->getPointerToOutlineVerticesArray()  // pointer to the array of vertices
              );
              glColor4f(
                poly->getOutlineColor()->R,
                poly->getOutlineColor()->G,
                poly->getOutlineColor()->B,
                poly->getOutlineColor()->A
              );
              glDrawArrays(
                poly->getOutlineGeometryType(), // The type of geometry from the object (eg. GL_TRIANGLES)
                0, // The starting index of the array
                poly->getOutlineNumberOfVertices() // The number of vertices from the object
              );
            }
          }

        }
        catch (std::exception& e) {
          std::cerr << "Caught an exception!!!" << e.what() << std::endl;
        }
      }


      objectMutex.unlock();





      // Disable vertex arrays
      glDisableClientState( GL_VERTEX_ARRAY );


      // Swap the buffer and handle IO
      // glFinish();
      glfwSwapBuffers(window);
      glfwPollEvents();


      // Framerate debug stuff
      frameCounter++;
      counter++;
      // printf("Frame %d finished.\n", counter);
      if (counter==60) {
        printf("Did 60 frames in %f seconds: %f FPS.\n", (glfwGetTime()-lastTime), 60/(glfwGetTime()-lastTime));
        counter = 0;
        lastTime = glfwGetTime();
      }
    }

    // Print any OpenGL errors, if there are any
    printf("OpenGL Error code: %d\n", glGetError());

  }












  //TODO: this will be rewritten to redo the renderer when that become a thing
  void Canvas::draw() {

    // Reset the window close flag, so that the window stays open for this frame
    glfwSetWindowShouldClose(window, GL_FALSE);


    // Get actual framebuffer size and adjust scaling accordingly
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    int scaling = round((1.0f*fbw)/winWidth);

    // // Stereoscopic rendering
    // if (hasStereo)
    // Canvas::setDrawBuffer(hasBackbuffer ? GL_FRONT_AND_BACK : GL_FRONT);
    // else
    // Canvas::setDrawBuffer(hasBackbuffer ? GL_LEFT : GL_FRONT_LEFT);


    // Clear the screen and set the background color for the window
    glClear(GL_COLOR_BUFFER_BIT);

    // Swap the screen buffer
    // glfwSwapBuffers(window);

    // We're ready to draw to the screen now!
    readyToDraw = true;
    bool nothingDrawn = false;  //Always draw the first frame

    glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created
    // glViewport(0,0,1,1);
    // glOrtho(-10,10,-10,10,-10,10);
    // glFrontFace(GL_CW);

    die_on_gl_error("Draw code");

    setupCamera();

    printf("%s\n", "Wazzup dawg?");











    GLuint attribute_coord2d;
    // glUseProgram(shaderProgram);

    GLfloat triangle_vertices[] = {
      0.0,  0.8,
      -0.8, -0.8,
      0.8, -0.8,
    };
    /* Describe our vertices array to OpenGL (it can't guess its format automatically) */
    glVertexAttribPointer(
      attribute_coord2d, // attribute
      2,                 // number of elements per vertex, here (x,y)
      GL_FLOAT,          // the type of each element
      GL_FALSE,          // take our values as-is
      0,                 // no extra data between each position
      triangle_vertices  // pointer to the C array
    );
    glfwSwapInterval(1);
    // glClearColor(1.0, 1.0, 1.0, 1.0);
    // glEnableVertexAttribArray(attribute_coord2d);

    int nbFrames;
    double lastTime;

    float f = 0.0;

    while (!glfwWindowShouldClose(window))
    {

      // Measure speed
      double currentTime = glfwGetTime();
      nbFrames++;
      if ( currentTime - lastTime >= 1.0 ){ // If last prinf() was more than 1 sec ago
        // printf and reset timer
        printf("%f ms/frame with %d objects.\n", 1000.0/double(nbFrames), objectBuffer.size());
        //  printf("%f ms/frame with objects.\n", 1000.0/double(nbFrames));
        nbFrames = 0;
        lastTime += 1.0;
      }


      glClear(GL_COLOR_BUFFER_BIT);

      /* Push each element in buffer_vertices to the vertex shader */
      // glDrawArrays(GL_TRIANGLES, 0, 3);

      // glDisableVertexAttribArray(attribute_coord2d);

      // // Render quad
      // glColor4f( 1.f, 1.f, 0.f, 1.0f );
      // // glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
      // // glClear(GL_COLOR_BUFFER_BIT);
      //  glBegin( GL_QUADS );
      //      glVertex2f( 0.0f+f, 0.0f+f );
      //      glVertex2f(  100.0f+f, 0.0f+f );
      //      glVertex2f(  100.0f+f,  100.0f+f );
      //      glVertex2f( 0.0f+f,  100.0f+f );
      //  glEnd();
      // //  glFinish();
      //
      //  f = f+.01;

      //pushObjectsToVertexBuffer();




      glfwSwapBuffers(window);
      glfwPollEvents();


      // printf("%s\n", "YO!");


      // Rectangle *testA = new Rectangle(10, 10, 50, 50, WHITE);
      // testA->render();





      // glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
      // glClear(1);
      // usleep(1000);


      // New method for dumping vertices from the objectBuffer into the GL buffer
      // glClear(GL_COLOR_BUFFER_BIT);
      // die_on_gl_error("before push");
      // pushObjectsToVertexBuffer();
      // die_on_gl_error("after push");
    }



    // This loop continues in the render thread for the life of the program,
    // and continually redraws the screen based on the objects that are
    // in the objectBuffer.
    // for (frameCounter = 0; !glfwWindowShouldClose(window); frameCounter++) {
    //
    //   drawTimer->sleep(true);
    //
    //   syncMutex.lock();
    //
    //   #ifdef __APPLE__
    //   windowMutex.lock();
    //   #endif
    //   glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created
    //
    //   // FPS indicator
    //   realFPS = round(1 / drawTimer->getTimeBetweenSleeps());
    //   if (showFPS) std::cout << realFPS << "/" << FPS << std::endl;
    //   std::cout.flush();
    //
    //
    //   //TODO what does this even do?
    //   int pos = pointBufferPosition;
    //   int posLast = pointLastPosition;
    //   pointLastPosition = pos;
    //
    //   //TODO what is this for?
    //   if (loopAround || pos != posLast)
    //   nothingDrawn = false;
    //
    //
    //   // NOTE: used to be the Draw loop
    //
    //   if (hasEXTFramebuffer)
    //   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
    //   else
    //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
    //   glDrawBuffer(GL_COLOR_ATTACHMENT0);
    //
    //   glViewport(0,0,winWidth,winHeight);
    //
    //
    //   // New method for dumping vertices from the objectBuffer into the GL buffer
    //   glClear(GL_COLOR_BUFFER_BIT);
    //   pushObjectsToVertexBuffer();
    //
    //   unsigned int size = myShapes->size();
    //   for (unsigned int i = 0; i < size; i++) {
    //     Shape* s = (*myShapes)[i];
    //     if (!s->getIsTextured()) {
    //       s->draw();  // Iterate through our queue until we've made it to the end
    //     } else {
    //       textureShaders(true);
    //       s->draw();
    //       textureShaders(false);
    //     }
    //   }
    //
    //   if (loopAround) {
    //     nothingDrawn = false;
    //     int toend = myShapes->capacity() - posLast;
    //     glBufferData(GL_ARRAY_BUFFER, toend * 6 * sizeof(float),
    //     &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
    //     glDrawArrays(GL_POINTS, 0, toend);
    //     posLast = 0;
    //     loopAround = false;
    //   }
    //   int pbsize = pos - posLast;
    //   if (pbsize > 0) {
    //     nothingDrawn = false;
    //     glBufferData(GL_ARRAY_BUFFER, pbsize * 6 * sizeof(float), &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
    //     glDrawArrays(GL_POINTS, 0, pbsize);
    //   }
    //
    //   // Reset drawn status for the next frame
    //   nothingDrawn = true;
    //
    //   // Update our screenBuffer copy with the screen
    //   glViewport(0,0,winWidth*scaling,winHeight*scaling);
    //   myShapes->clear();                           // Clear our buffer of shapes to be drawn
    //
    //   if (hasEXTFramebuffer)
    //   glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
    //   else
    //   glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
    //   glReadBuffer(GL_COLOR_ATTACHMENT0);
    //
    //   glReadPixels(0, 0, winWidthPadded, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
    //   if (toRecord > 0) {
    //     screenShot();
    //     --toRecord;
    //   }
    //
    //   glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
    //   glDrawBuffer(drawBuffer);
    //
    //   textureShaders(true);
    //   const float vertices[32] = {
    //     0,       0,        1,1,1,1,0,1,
    //     winWidth,0,        1,1,1,1,1,1,
    //     0,       winHeight,1,1,1,1,0,0,
    //     winWidth,winHeight,1,1,1,1,1,0
    //   };
    //   glBindTexture(GL_TEXTURE_2D,renderedTexture);
    //   glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    //   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    //   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    //   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    //   glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    //   glBufferData(GL_ARRAY_BUFFER,32*sizeof(float),vertices,GL_DYNAMIC_DRAW);
    //   glDrawArrays(GL_TRIANGLE_STRIP,0,4);
    //   glFlush();                                   // Flush buffer data to the actual draw buffer
    //   glfwSwapBuffers(window);                     // Swap out GL's back buffer and actually draw to the window
    //
    //   textureShaders(false);
    //
    //   #ifndef __APPLE__
    //   glfwPollEvents();                            // Handle any I/O
    //   #endif
    //   glfwGetCursorPos(window, &mouseX, &mouseY);
    //   glfwMakeContextCurrent(NULL);                // We're drawing to window as soon as it's created
    //   #ifdef __APPLE__
    //   windowMutex.unlock();
    //   #endif
    //
    //   syncMutex.unlock();
    //
    //   if (toClose) glfwSetWindowShouldClose(window, GL_TRUE);
    // }
  }


















  void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
  }

  ColorFloat Canvas::getBackgroundColor() {
    return bgcolor;
  }

  int Canvas::getDisplayHeight() {
    initGlfw();
    return monInfo->height;
  }

  int Canvas::getDisplayWidth() {
    initGlfw();
    return monInfo->width;
  }

  float Canvas::getFPS() {
    return realFPS;
  }

  int Canvas::getFrameNumber() {
    return frameCounter;
  }

  bool Canvas::isOpen() {
    return !isFinished;
  }

  int Canvas::getMouseX() {
    //TODO: add setting the mouseX and mouseY back in
    return mouseX;
  }

  int Canvas::getMouseY() {
    return mouseY;
  }

  ColorInt Canvas::getPixel(int row, int col) {
    return getPoint(col,row);
  }

  ColorInt Canvas::getPoint(int x, int y) {
    int yy;
    //if (atiCard)
    //  yy = (winHeight) - y; //glReadPixels starts from the bottom left, and we have no way to change that...
    //else
    yy = (winHeight-1) - y;
    int off = 3 * (yy * winWidthPadded + x);
    return ColorInt(screenBuffer[off], screenBuffer[off + 1], screenBuffer[off + 2], 255);
  }

  unsigned int Canvas::getReps() const {
    return drawTimer->getReps();
  }

  uint8_t* Canvas::getScreenBuffer() {
    return screenBuffer;
  }

  double Canvas::getTime() {
    return drawTimer->getTime();
  }

  double Canvas::getTimeBetweenSleeps() const {
    return drawTimer->getTimeBetweenSleeps();
  }

  int Canvas::getWindowHeight() {
    return winHeight;
  }

  int Canvas::getWindowWidth() {
    return winWidth;
  }

  int Canvas::getWindowX() {
    return monitorX;
  }

  int Canvas::getWindowY() {
    return monitorY;
  }

  void Canvas::glDestroy() {
    // Free up our resources
    glDetachShader(shaderProgram, shaderFragment);
    glDetachShader(shaderProgram, shaderVertex);
    glDeleteShader(shaderFragment);
    glDeleteShader(shaderVertex);
    glDeleteProgram(shaderProgram);
    glDetachShader(textureShaderProgram, textureShaderFragment);
    glDetachShader(textureShaderProgram, textureShaderVertex);
    glDeleteShader(textureShaderFragment);
    glDeleteShader(textureShaderVertex);
    glDeleteProgram(textureShaderProgram);
    glDeleteBuffers(1, &vertexBuffer);
    glDeleteVertexArrays(1, &vertexArray);
  }

  //Workaround for OS X
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

  void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title, double timerLength) {
    ++openCanvases;

    objectBuffer.clear();   // Clears the object buffer, just for peace-of-mind

    if (ww == -1)
    ww = 1.2*Canvas::getDisplayHeight();
    if (hh == -1)
    hh = 0.75*ww;
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
    for (unsigned i = 0; i < bufferSize; ++i) {
      screenBuffer[i] = 0;
    }

    started = false;                  // We haven't started the window yet
    monitorX = xx;
    monitorY = yy;
    myShapes = new Array<Drawable*>(b);  // Initialize myShapes
    myBuffer = new Array<Drawable*>(b);
    vertexData = new float[6 * b];    // Buffer for vertexes for points
    showFPS = false;                  // Set debugging FPS to false
    isFinished = false;               // We're not done rendering
    pointBufferPosition = pointLastPosition = 0;
    loopAround = false;
    toRecord = 0;

    bgcolor = GRAY;
    window = nullptr;

    drawTimer = new Timer((timerLength > 0.0f) ? timerLength : FRAME);

    for (int i = 0; i <= GLFW_KEY_LAST * 2 + 1; i++)
    boundKeys[i++] = nullptr;

    initGlfw();
    #ifndef _WIN32
    initWindow();
    initGLAD();
    // initShaders();
    newInit();
    glfwMakeContextCurrent(NULL);   // Reset the context
    #endif
  }

  void Canvas::initGl() {
    #ifdef _WIN32
    initWindow();
    initGLAD();
    // initShaders();
    newInit();
    #else
    glfwMakeContextCurrent(window);         // We're drawing to window as soon as it's created
    glfwSetWindowUserPointer(window, this);
    #endif

    // Enable and disable necessary stuff
    glDisable(GL_DEPTH_TEST);                           // Disable depth testing because we're not drawing in 3d
    glDisable(GL_DITHER);                               // Disable dithering because pixels do not (generally) overlap
    glDisable(GL_CULL_FACE);                            // Disable culling because the camera is stationary
    glEnable(GL_BLEND);                                 // Enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // Set blending mode to standard alpha blending

    bindToButton(TSGL_KEY_ESCAPE, TSGL_PRESS, [this]() {
      glfwSetWindowShouldClose(window, GL_TRUE);
      toClose = true;
    });

    unsigned char stereo[1] = {5}, dbuff[1] = {5};
    int aux[1] = {5};
    glGetBooleanv(GL_STEREO,stereo);
    glGetBooleanv(GL_DOUBLEBUFFER,dbuff);
    glGetIntegerv(GL_AUX_BUFFERS,aux);
    hasStereo = ((int)stereo[0] > 0);
    hasBackbuffer = ((int)dbuff[0] > 0);

    glfwMakeContextCurrent(NULL);   // Reset the context
  }

  void Canvas::initShaders() {
    GLint Vsuccess = 0;
    GLint Fsuccess = 0;

    /* Read our shaders into the appropriate buffers */
    vertexSource = filetobuf("src/TSGL/Shaders/vertexSource.txt");
    fragmentSource = filetobuf("src/TSGL/Shaders/fragmentSource.txt");

    /* Assign our handles a "name" to new shader objects */
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    /* Associate the source code buffers with each handle */
    glShaderSource(vertexShader, 1, (const GLchar**)&vertexSource, 0);
    glShaderSource(fragmentShader, 1, (const GLchar**)&fragmentSource, 0);

    /* Free the temporary allocated memory */
    free(vertexSource);
    free(fragmentSource);

    /* Compile our shader objects */
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &Vsuccess);
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &Fsuccess);


    /* Assign our program handle a "name" */
    shaderProgram = glCreateProgram();

    /* Attach our shaders to our program */
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);

    /* Bind attribute index 0 (shaderAttribute) to in_Position*/
    /* "in_Position" will represent "data" array's contents in the vertex shader */
    glBindAttribLocation(shaderProgram, shaderAttribute, "in_Position");

    /* Link shader program*/
    glLinkProgram(shaderProgram);


    printf("Vertex shader compile status: %d\n", Vsuccess);
    if (Vsuccess == GL_FALSE) {
      GLint logSize = 10240;
      GLint logLength = 0;
      glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logSize);
      GLchar shaderlog[logSize];
      glGetShaderInfoLog(vertexShader, logSize, logLength, shaderlog);
      printf("Vertex shader log:\n\n%s\n\n", shaderlog);
    }
    printf("Fragment shader compile status: %d\n", Fsuccess);
    if (Fsuccess == GL_FALSE) {
      GLint logSize = 10240;
      GLint logLength = 0;
      glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logSize);
      GLchar shaderlog[logSize];
      glGetShaderInfoLog(fragmentShader, logSize, logLength, shaderlog);
      printf("Fragment shader log:\n\n%s\n\n", shaderlog);
    }


  }

  void Canvas::initGLAD() {
    // Initialize the GLAD GL function loader
    if(!gladLoadGL()) {
      printf("Something went wrong loading GLAD!\n");
      exit(-1);
    }
    // TODO: this is here for debug purposes, pull out eventually?
    printf("OpenGL version: %d.%d\n", GLVersion.major, GLVersion.minor);
  }

  void Canvas::initGlfw() {
    // Inititalize the GLFW library.
    if (!glfwIsReady) {
      glfwInit();
      monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
      glfwIsReady = true;
    }
  }

  void Canvas::initWindow() {

    // Set the error callback for GLFW, ie. when an error happens, call this function!
    glfwSetErrorCallback(errorCallback);

    // Ask for a specific version of OpenGL - 2.1.  Usually GLFW will just give
    // a newer one, and thus never functions will work.  But we're targeting 2.1,
    //so we need that as a min.  It's up to the programmer to not use anything
    // that is not supported on 2.1!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);                  // Set target GL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);                  // Set target GL minor version to 3.2

    // Pass some other options to GLFW
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);                    // Disable the back buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first
    glfwWindowHint(GLFW_SAMPLES,4);

    // GLFW must be done with making a window before we start making another one
    // This mutex ensures that only one window is created at a time
    // Note that it is fine to have multiple windows, just create them one after the other!
    glfwMutex.lock();
    window = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), NULL, NULL);

    // Check that the window was properly created
    if (!window) {
      fprintf(stderr, "GLFW window creation failed. Was the library correctly initialized?\n");
      exit(100);
    }

    // Release the window mutex so that we can create other windows
    glfwMutex.unlock();


    // Check that we were able to get information about the user's monitor
    if (!monInfo) {
      fprintf(stderr, "GLFW failed to return monitor information. Was the library correctly initialized?\n");
      exit(101);
    }

    // Store information about the monitor, set GLFW's window position
    if (monitorX == -1)
    monitorX = (monInfo->width - winWidth) / 2;
    if (monitorY == -1)
    monitorY = (monInfo->height - winHeight) / 2;
    glfwSetWindowPos(window, monitorX, monitorY);


    // Finish initting the GLFW window, make it current, set up cursor stuff
    glfwMakeContextCurrent(window);
    glfwShowWindow(window);                 // Show the window
    glfwSetWindowUserPointer(window, this);

    // Set up TSGL to handle user IO
    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
  }

  void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
  }

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

  void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
  }

  void Canvas::reset() {
    drawTimer->reset();
  }

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

  void Canvas::run(void (*myFunction)(Canvas&) ) {
    start(); myFunction(*this); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, int), int i) {
    start(); myFunction(*this, i); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, unsigned), unsigned u) {
    start(); myFunction(*this, u); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, int, int), int i1, int i2) {
    start(); myFunction(*this, i1, i2); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
    start(); myFunction(*this, u1, u2); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, std::string),std::string s) {
    start(); myFunction(*this, s); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s) {
    start(); myFunction(*this, i, s); wait();
  }
  void Canvas::run(void (*myFunction)(Canvas&, std::string, int), std::string s, int i) {
    start(); myFunction(*this, s, i); wait();
  }

  void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.png", frameCounter);  // TODO: Make this save somewhere not in root

    loader.saveImageToFile(filename, screenBuffer, winWidthPadded, winHeight);
  }

  void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    if (can->scrollFunction) can->scrollFunction(xpos, ypos);
  }

  void Canvas::setBackgroundColor(ColorFloat color) {
    bgcolor = color;
    if (window != nullptr) {
      float a = color.A;
      glfwMakeContextCurrent(window);
      glClearColor(color.R,color.G,color.B,color.A);
      glfwMakeContextCurrent(NULL);
    }
  }

  void Canvas::setDrawBuffer(int buffer) {
    Canvas::drawBuffer = buffer;
  }

  void Canvas::setFont(std::string filename) {
    loader.loadFont(filename);
  }

  void Canvas::setShowFPS(bool b) {
    showFPS = b;
  }

  void Canvas::setupCamera() {
    // // Set up camera positioning
    // // Note: (winWidth-1) is a dark voodoo magic fix for some camera issues
    // float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
    //   -(winWidth-1) / 2.0f, (winHeight) / 2.0f, -(winHeight) / 2.0f, 1 };
    //   //    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
    //   //      -(winWidth-1) / 2.0f, (winHeight+0.5f) / 2.0f, -(winHeight-0.5f) / 2.0f, 1 };
    //   glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);
    //
    //   // Set up camera zooming
    //   float projF[] = { 1.0f / aspect, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.0f, -1, 0, 0, -0.02f, 0 };
    //   glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);
    //
    //   // Set up camera transformation
    //   float modelF[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    //   glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
    //
    //   die_on_gl_error("Camera setup function");

    //Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, winWidth, winHeight, 0.0, 1.0, -1.0 );

    //Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

  }

  void Canvas::sleep() {
    #ifdef __APPLE__
    handleIO();
    #endif
    drawTimer->sleep(false);
  }

  void Canvas::sleepFor(float seconds) {
    #ifdef __APPLE__
    handleIO();
    #endif
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (seconds * 1000000000)));
  }

  int Canvas::start() {
    if (started) return -1;
    started = true;
    #ifdef __APPLE__
    pthread_create(&renderThread,NULL,startDrawing,(void*)this);
    #else
    renderThread = std::thread(Canvas::startDrawing, this);  // Spawn the rendering thread
    #endif
    setBackgroundColor(WHITE); //TODO: determine the best place for this
    return 0;
  }

  #ifdef __APPLE__
  void* Canvas::startDrawing(void* cPtr) {
    Canvas* c = (Canvas*)cPtr;
    c->initGl();
    // c->draw();
    c->newDraw();
    c->isFinished = true;
    pthread_exit(NULL);
  }
  #else
  void Canvas::startDrawing(Canvas *c) {
    c->initGl();
    // c->draw();
    c->newDraw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
  }
  #endif

  void Canvas::stop() {
    close();
    wait();
  }

  void Canvas::stopRecording() {
    toRecord = 0;
  }

  void Canvas::takeScreenShot() {
    if (toRecord == 0) toRecord = 1;
  }

  void Canvas::textureShaders(bool on) {
    GLint program;
    if (!on) {
      program = shaderProgram;

      // Relocate the shader attributes
      GLint posAttrib = glGetAttribLocation(shaderProgram, "position");
      glEnableVertexAttribArray(posAttrib);
      glVertexAttribPointer(posAttrib, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(float), 0);
      GLint colAttrib = glGetAttribLocation(shaderProgram, "color");
      glEnableVertexAttribArray(colAttrib);
      glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*) (2 * sizeof(float)));

    } else {
      program = textureShaderProgram;

      // Relocate the shader attributes
      GLint texturePosAttrib = glGetAttribLocation(textureShaderProgram, "position");
      glEnableVertexAttribArray(texturePosAttrib);
      glVertexAttribPointer(texturePosAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), 0);
      GLint textureColAttrib = glGetAttribLocation(textureShaderProgram, "color");
      glEnableVertexAttribArray(textureColAttrib);
      glVertexAttribPointer(textureColAttrib, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void*) (2 * sizeof(float)));
      GLint textureTexAttrib = glGetAttribLocation(textureShaderProgram, "texcoord");
      glEnableVertexAttribArray(textureTexAttrib);
      glVertexAttribPointer(textureTexAttrib, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float),
      (void*) (6 * sizeof(float)));
    }

    // Reallocate the shader program for use
    glUseProgram(program);


    //TODO remove this, no longer needed (get all instances)
    // // Recompute the camera matrices
    // uniModel = glGetUniformLocation(program, "model");
    // uniView = glGetUniformLocation(program, "view");
    // uniProj = glGetUniformLocation(program, "proj");

    // Update the camera
    // setupCamera();
  }

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


















































  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  //     TODO: These will need to be rewritten when we change to the OOP API
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////


  void Canvas::drawCircle(int xverts, int yverts, int radius, int sides, ColorFloat color, bool filled) {
    float delta = 2.0f / sides * PI;
    if (filled) {
      Circle *c = new Circle(xverts, yverts, radius, sides, color);
      c->setHasOutline(false);
      this->add(c);
    } else {
      UnfilledCircle *c = new UnfilledCircle(xverts, yverts, radius, sides, color);
      this->add(c);
    }
  }

  void Canvas::drawConcavePolygon(int size, int xverts[], int yverts[], ColorFloat color[], bool filled) {
    if (filled) {
        ConcavePolygon* p = new ConcavePolygon(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i]);
        }
        p->setHasOutline(false);
        this->add(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i]);
        }
        this->add(p);  // Push it onto our drawing buffer
    }
  }

  void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    if (filled) {
        ConvexPolygon* p = new ConvexPolygon(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(x[i], y[i]);
        }
        p->setHasOutline(false);
        this->add(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(x[i], y[i]);
        }
        this->add(p);  // Push it onto our drawing buffer
    }
  }

  void Canvas::drawImage(std::string filename, int x, int y, int width, int height, float alpha) {
    Image* im = new Image(filename, loader, x, y, width, height, alpha);  // Creates the Image with the specified coordinates
    this->add(im);                                        // Push it onto our drawing buffer
  }

  void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorFloat color) {
    Line* l = new Line(x1, y1, x2, y2, color);  // Creates the Line with the specified coordinates and color
    this->add(l);                               // Push it onto our drawing buffer
  }

  inline void Canvas::drawPixel(int row, int col, ColorFloat color) {
    drawPoint(col, row, color); //TODO: update with new api
  }

  void Canvas::drawPoint(int x, int y, ColorFloat color) {
    pointArrayMutex.lock();
    if (pointBufferPosition >= myShapes->capacity()) {
      loopAround = true;
      pointBufferPosition = 0;
    }
    int tempPos = pointBufferPosition * 6;
    pointBufferPosition++;

    float atioff = atiCard ? 0.5f : 0.0f;
    vertexData[tempPos] = x;
    vertexData[tempPos + 1] = y+atioff;
    vertexData[tempPos + 2] = color.R;
    vertexData[tempPos + 3] = color.G;
    vertexData[tempPos + 4] = color.B;
    vertexData[tempPos + 5] = color.A;
    pointArrayMutex.unlock();
  }

  //TODO: change to just add the ProgressBar as one item (rather than rect and border)
  void Canvas::drawProgress(ProgressBar* p) {
    for (int i = 0; i < p->getSegs(); ++i) {
      drawText(to_string(i),p->getSegX(i)+8,p->getSegY()-8,32,BLACK);
      this->add(p->getRect(i));
      this->add(p->getBorder(i));
    }
  }

//TODO: maye works with the new backend
void Canvas::drawRectangle(int x1, int y1, int x2, int y2, ColorFloat color, bool filled) {
  if (x2 < x1) { int t = x1; x1 = x2; x2 = t; }
  if (y2 < y1) { int t = y1; y1 = y2; y2 = t; }
    if (filled) {
        Rectangle* rec = new Rectangle(x1, y1, x2-x1, y2-y1, color);  // Creates the Rectangle with the specified coordinates and color
        rec->setHasOutline(false);
        this->add(rec);                                     // Push it onto our drawing buffer
    }
    else {
        UnfilledRectangle* rec = new UnfilledRectangle(x1, y1, x2-x1, y2-y1, color);  // Creates the Rectangle with the specified coordinates and color
        this->add(rec);                                     // Push it onto our drawing buffer
    }
  }

  void Canvas::drawShape(Drawable* s) {
    if (!started) {
      TsglDebug("No drawing before Canvas is started! Ignoring draw request.");
      return;
    }
    while (!readyToDraw)
    sleep();
    bufferMutex.lock();
    myBuffer->push(s);  // Push it onto our drawing buffer
    bufferMutex.unlock();
  }

  void Canvas::drawText(std::string text, int x, int y, unsigned size, ColorFloat color) {
    // std::wstring wsTmp(text.begin(), text.end());
    // std::wstring ws = wsTmp;
    // drawText(ws, x, y, size, color);
  }

  void Canvas::drawText(std::wstring text, int x, int y, unsigned size, ColorFloat color) {
    // Text* t = new Text(text, loader, x, y, size, color);  // Creates the Point with the specified coordinates and color
    // this->add(t);                                // Push it onto our drawing buffer
  }

  void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled) {
    if (filled) {
        Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color);  // Creates the Triangle with the specified vertices and color
        t->setHasOutline(false);
        this->add(t);                                               // Push it onto our drawing buffer
    }
    else {
      UnfilledTriangle* t = new UnfilledTriangle(x1, y1, x2, y2, x3, y3, color);  // Creates the Triangle with the specified vertices and color
      this->add(t);                                               // Push it onto our drawing buffer
    }
}

  void Canvas::drawTriangleStrip(int size, int xverts[], int yverts[], ColorFloat color[], bool filled) {
    if (filled) {
        TriangleStrip* p = new TriangleStrip(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i]);
        }
        this->add(p);  // Push it onto our drawing buffer
    }
    else {
        Polyline* p = new Polyline(size, color[0]);
        for (int i = 0; i < size; i++) {
            p->addVertex(xverts[i], yverts[i]);
        }
        this->add(p);  // Push it onto our drawing buffer
    }
  }



  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////
  //       End the old API stuff
  ////////////////////////////////////////////////////////////////////////////////
  ////////////////////////////////////////////////////////////////////////////////















































  //-----------------Unit testing-------------------------------------------------------
  void Canvas::runTests() {
    TsglDebug("Testing Canvas class...");
    Canvas c1(0, 0, 500, 500, "", FRAME);
    c1.setBackgroundColor(WHITE);
    c1.start();
    tsglAssert(testFilledDraw(c1), "Unit test for filled draw failed!");
    tsglAssert(testLine(c1), "Unit test for line failed!");
    tsglAssert(testAccessors(c1), "Unit test for accessors failed!");
    tsglAssert(testDrawImage(c1), "Unit test for drawing images failed!");
    c1.stop();
    TsglDebug("Unit tests for Canvas complete.");
    std::cout << std::endl;
  }

  //Similar format is used for the remaining unit tests
  bool Canvas::testFilledDraw(Canvas& can) {
    int passed = 0;   //Passed tests
    int failed = 0;   //Failed tests
    ColorInt red(255, 0, 0);   //Fill color
    can.drawCircle(250, 250, 50, 32, red, true);  //Draw filled shape
    can.sleepFor(1);

    //Test 1: Get middle pixel and see if its red.
    if(can.getPixel(250, 250) == red) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, middle pixel for testFilledDraw() failed!");
    }

    //Test 2: Get leftmost and rightmost pixel of the circle
    //Have to add or subtract 1 from the y so that you can get the correct pixel (center radius is 1. No 0 radius).
    if(can.getPixel(250, 201) == red && can.getPixel(250, 299) == red) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, leftmost and rightmost pixel for testFilledDraw() failed!");
    }

    //Test 3: Outside pixels shouldn't equal inside pixels
    int test = 0;
    //Single pixel....
    if(can.getPixel(1, 1) != red) {
      //Multiple pixels....
      for(int i = 201; i <= 299; i++) {
        if(can.getPixel(1, i) != red) {
          test++;
        }
      }
      //Results of multiple pixels...
      if(test == 99) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 3, outside != inside, Multiple pixels for testFilledDraw() failed!");
      }
    } else {
      failed++;
      TsglErr("Test 3, outside != inside, Single pixel for testFilledDraw() failed!");
    }

    //Test 4: A LOT of the pixels on the inside should be red
    int count = 0;
    for(int i = 201; i <= 299; i++) {
      if(can.getPixel(250, i) == red) {
        count++;
      }
    }

    //Now check the count, should be 99
    if(count == 99) {
      passed++;
    } else {
      failed++;
      std::cout << "Count: " << count << std::endl;
      TsglErr("Test 4, multiple pixels for testFilledDraw() failed!");
    }

    //Determine if we passed all four tests or not, Results:
    if(passed == 4 && failed == 0) {
      can.clear();
      TsglDebug("Unit test for drawing filled shapes passed!");
      return true;
    } else {
      can.clear();
      TsglErr("This many passed for testFilledDraw(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testFilledDraw(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
  }

  bool Canvas::testLine(Canvas & can) {
    int passed = 0;
    int failed = 0;
    can.drawLine(0, 0, 250, 250, BLACK);  //Diagonal line
    can.drawLine(253, 253, 400, 253);  //Straight line
    can.sleepFor(1);
    ColorInt black(0, 0, 0);
    //Test 1: Near the ending endpoint? (Diagonal)
    if(can.getPoint(249, 249) == black) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Near the ending endpoint? for testLine() failed!");
    }

    //Test 2: Somewhere in the middle? (Diagonal)
    if(can.getPoint(155, 155) == black) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Somewhere in the middle? for testLine() failed!");
    }

    //Test 3: Near the starting endpoint? (Diagonal)
    if(can.getPoint(15, 15) == black) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Near the starting endpoint? for testLine() failed!");
    }

    //Test 4: An entire line? (Straight)
    int count = 0;
    for(int i = 253; i <= 399; i++) {
      if(can.getPoint(i, 253) == black) {
        count++;
      }
    }

    //Check the results of the Straight line test
    if(count == 147) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 4, An entire line? (Straight) for testLine() failed!");
    }

    //Results:
    if(passed == 4 && failed == 0) {
      can.clear();
      TsglDebug("Unit test for line passed!");
      return true;
    } else {
      can.clear();
      TsglErr("This many passed testLine(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testLine(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
  }

  bool Canvas::testAccessors(Canvas& can) {
    int passed = 0;
    int failed = 0;
    ColorFloat white = WHITE;  //Have to set these to new variables so that I can compare them
    ColorFloat black = BLACK;

    //Test 1: Background color
    if(can.getBackgroundColor() == white) {
      can.setBackgroundColor(BLACK);
      if(can.getBackgroundColor() == black) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 1, Background color for testAccessors() failed!");
      }
    }

    //Test 2: Window width/height
    //width
    if(can.getWindowWidth() == 500) {
      //height
      if(can.getWindowHeight() == 500) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 2 for testAccessors() failed! (height)");
      }
    } else {
      failed++;
      TsglErr("Test 2 for testAccessors() failed! (width)");
    }

    //Test 3: Window x/y
    //x
    if(can.getWindowX() == 0) {
      //y
      if(can.getWindowY() == 0) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 3 for testAccessors() failed! (y)");
      }
    } else {
      failed++;
      TsglErr("Test 3 for testAccessors() failed! (x)");
    }

    //Test 4: Window open?
    if(can.isOpen() == true) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 4, Window open? for testAccessors() failed!");
    }

    //Results:
    if(passed == 4 && failed == 0) {
      can.clear();
      TsglDebug("Unit test for accessors/mutators passed!");
      return true;
    } else {
      can.clear();
      TsglErr("This many passed for testAccessors(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testAccessors(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
  }

  bool Canvas::testDrawImage(Canvas& can) {
    can.drawImage("../assets/pics/ff0000.png", 0, 0, 200, 200);
    can.sleepFor(1);
    int passed = 0;
    int failed = 0;
    ColorInt red(255, 0, 0);
    //Test 1: Single pixel
    if(can.getPoint(1, 1) == red) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 1, Single pixel for testDrawImage() failed!");
    }

    //Test 2: Multiple pixels
    int count = 0;
    for(int i = 0; i < 200; i++) {
      if(can.getPoint(1, i) == red) {
        count++;
      }
    }

    //Results of Test 2:
    if(count == 200) {
      passed++;
    } else {
      failed++;
      std::cout << "Count: " << count << std::endl;
      TsglErr("Test 2, Multiple pixels for testDrawImage() failed!");
    }

    //Results of entire Unit test:s
    if(passed == 2 && failed == 0) {
      TsglDebug("Unit test for drawing images passed!");
      return true;
    } else {
      TsglErr("This many passed for testDrawImage(): ");
      std::cerr << " " << passed << std::endl;
      TsglErr("This many failed for testDrawImage(): ");
      std::cerr << " " << failed << std::endl;
      return false;
    }
  }
  //------------End Unit testing--------------------------------------------------------
}
