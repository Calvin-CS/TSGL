#include "Canvas.h"

namespace tsgl {

  // Setup vars
  int Canvas::drawBuffer = GL_FRONT_LEFT;
  bool Canvas::glfwIsReady = false;
  std::mutex Canvas::glfwMutex;
  GLFWvidmode const* Canvas::monInfo;
  unsigned Canvas::openCanvases = 0;


  ///////////////////////////////////////////////
  // Initialization and Destructor
  ///////////////////////////////////////////////

  Canvas::Canvas(double timerLength) {
    init(-1, -1, -1, -1, -1, "", timerLength);
  }

  Canvas::Canvas(int x, int y, int width, int height, std::string title, double timerLength) {
    init(x, y, width, height, width*height*2, title, timerLength);
  }

  void Canvas::init(int xx, int yy, int ww, int hh, unsigned int b, std::string title, double timerLength) {
    ++openCanvases;

    // printf("Canvas height, width %d, %d\n", ww, hh);

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
    glfwMakeContextCurrent(NULL);   // Reset the context
    #endif
  }

  void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    TsglDebug("Window closed successfully.");
  }

  void Canvas::stop() {
    close();
    wait();
  }

  Canvas::~Canvas() {
    // Free our pointer memory
    delete drawTimer;
    // delete[] vertexData;  // TODO does this cause E's segfaults? Also not necessarily.
    delete [] screenBuffer;
    //TODO: make this also delete the object buffer?
    if (--openCanvases == 0) {
      glfwIsReady = false;
      glfwTerminate();  // Terminate GLFW
    }
  }

  void Canvas::initGl() {
    #ifdef _WIN32
    initWindow();
    initGLAD();
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

  void Canvas::initGLAD() {
    // Initialize the GLAD GL function loader
    if(!gladLoadGL()) {
      printf("Something went wrong loading GLAD!\n");
      exit(-1);
    }
    #ifdef __DEBUG__
    printf("OpenGL version: %d.%d\n", GLVersion.major, GLVersion.minor);
    #endif
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
    // if( isRaster ) { glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE); }
    // else { glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE); }
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);
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

  void Canvas::setupCamera() {
    // Set up camera positioning
    // Initialize Projection Matrix
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, winWidth, winHeight, 0.0, 1.0, -1.0 );

    // Initialize Modelview Matrix
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

  }


  ///////////////////////////////////////////////
  // I/O Handling
  ///////////////////////////////////////////////

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

  int Canvas::getMouseX() {
    //TODO: add setting the mouseX and mouseY back in
    return mouseX;
  }

  int Canvas::getMouseY() {
    return mouseY;
  }

  void Canvas::scrollCallback(GLFWwindow* window, double xpos, double ypos) {
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    if (can->scrollFunction) can->scrollFunction(xpos, ypos);
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
    }
    #endif
  }

  void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
  }

  ///////////////////////////////////////////////
  // Object Interface
  ///////////////////////////////////////////////

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

  void Canvas::clear() {
    //TODO this works with the new version now, but it could probably be cleaned up a bit
    //TODO move this to the section for backwards compatibility?
    this->clearObjectBuffer(true); //Clears the items from the buffer and deletes all pointers
    //TODO: decide whether this should also delete all items
  }

  void Canvas::printBuffer() {
    // std::cout << "Printing array:" << std::endl << std::endl;
    printf("Printing %ld elements in buffer:\n\n", objectBuffer.size());

    for(std::vector<Drawable *>::iterator it = objectBuffer.begin(); it != objectBuffer.end(); ++it) {
      std::cout << *it << std::endl;
    }
  }

  int Canvas::getDefaultLayer() {
    return currentNewShapeLayerDefault;
  }

  void Canvas::setDefaultLayer(int n) {
    if (n >= 0) currentNewShapeLayerDefault = n;
    else return;
    //TODO: make this throw an error if layer is invalid (< 0)
  }


  ///////////////////////////////////////////////
  // Canvas Attribute Getters/Setters
  ///////////////////////////////////////////////
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

  ColorInt Canvas::getPixel(int row, int col) {
    return getPoint(col,row);
  }

  ColorInt Canvas::getPoint(int x, int y) {
    int yy;
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
  void Canvas::setBackgroundColor(ColorFloat color) {
    bgcolor = color;
    if (window != nullptr) {
      float a = color.A;
      glfwMakeContextCurrent(window);
      glClearColor(color.R,color.G,color.B,color.A);
      glfwMakeContextCurrent(NULL);
    }
  }
  void Canvas::setFont(std::string filename) {
    loader.loadFont(filename);
  }
  void Canvas::setShowFPS(bool b) {
    showFPS = b;
  }
  void Canvas::setDrawBuffer(int buffer) {
    Canvas::drawBuffer = buffer;
  }
  void Canvas::reset() {
    drawTimer->reset();
  }


  ///////////////////////////////////////////////
  // Drawing Loop
  ///////////////////////////////////////////////

  void Canvas::draw() {

    // bool debugRectIsRed = false;
    // printf("Note: Canvas is drawing a flashing square in the corner to test whether it remains active during a crash.\n");
    // Rectangle* debugRect = new Rectangle(0, 0, 10, 10, BLACK, BLACK);
    // debugRect->setLayer(100);
    // add(debugRect);

    // printf("%s\n", "Drawing stuff.");

    glfwMakeContextCurrent(window);  // We're drawing to window as soon as it's created

    glfwSwapInterval(1);  // Enable VSYNC

    // Reset the window close flag, so that the window stays open for this frame
    glfwSetWindowShouldClose(window, GL_FALSE);

    setupCamera();  //Camera transformations

    // Count number of frames
    int counter = 0;
    float lastTime = 0;
    while (!glfwWindowShouldClose(window) && !isFinished) {

      #ifdef __APPLE__
      windowMutex.lock();
      #endif

      if( !isRaster ) { glDrawBuffer(GL_BACK); }
      else { glDrawBuffer(GL_FRONT_AND_BACK); }

      // Clear the canvas
      if( !isRaster ) glClear(GL_COLOR_BUFFER_BIT);

      // Enable vertex arrays
      glEnableClientState( GL_VERTEX_ARRAY );

      // Iterate through objects, render them
      objectMutex.lock();

      // printf("%s\n", "WAZZUP?????");
      glfwGetCursorPos(window, &mouseX, &mouseY); //TODO: decide if this is the right place. This does keep it within the lock, which is good.
      //TODO: also lock the accessors for this so we can't be reading them as they change here. Might want to use a less vital mutex though so we don't hold up drawing so much

      for(std::vector<Drawable *>::iterator it = objectBuffer.begin(); it != objectBuffer.end(); ++it) {
        // if( isRaster && (*it)->getIsRendered() ) continue;
        try {
          if ((*it)->getIsDiscreteRendered()) {
            DiscreteDrawable* rc = *it;
            rc->render();
          } else {
            Shape* rc = *it;
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

      if( isRaster ) {
        objectBuffer.clear();
      }

      objectMutex.unlock();


      // Disable vertex arrays
      glDisableClientState( GL_VERTEX_ARRAY );


      // Swap the buffer and handle IO
      // glFinish();
      glfwSwapBuffers(window);

      #ifndef __APPLE__
      glfwPollEvents();
      #endif


      // Framerate debug stuff
      frameCounter++;
      counter++;
      // printf("Frame %d finished.\n", counter);
      if (counter==60) {
        #ifdef __DEBUG__
        printf("Did 60 frames in %f seconds: %f FPS.\n", (glfwGetTime()-lastTime), 60/(glfwGetTime()-lastTime));
        #endif
        counter = 0;
        lastTime = glfwGetTime();
      }

      #ifdef __APPLE__
      windowMutex.unlock();
      #endif
    }


    // Print any OpenGL errors, if there are any
    int glError = glGetError();
    if (glError) printf("OpenGL Error code: %d\n", glError);


  }

  int Canvas::start() {
    if (started) return -1;
    started = true;

    renderThread = std::thread(Canvas::startDrawing, this);  // Spawn the rendering thread

    return 0;
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


  ///////////////////////////////////////////////
  // Utility Functions
  ///////////////////////////////////////////////

  void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
  }
  inline void die_on_gl_error(const char* location) {
    GLenum error = GL_NO_ERROR;
    error = glGetError();
    if (GL_NO_ERROR != error) {
      printf("GL Error %x encountered in %s.\n", error, location);
      exit(1);
    }
  }


  ///////////////////////////////////////////////
  // Canvas Recording Functions
  ///////////////////////////////////////////////

  void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
  }

  void Canvas::screenShot() {
    char filename[25];
    sprintf(filename, "Image%06d.png", frameCounter);  // TODO: Make this save somewhere not in root

    loader.saveImageToFile(filename, screenBuffer, winWidthPadded, winHeight);
  }

  void Canvas::takeScreenShot() {
    if (toRecord == 0) toRecord = 1;
  }

  void Canvas::stopRecording() {
    toRecord = 0;
  }



  ///////===================




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











  void Canvas::startDrawing(Canvas *c) {
    c->initGl();
    c->draw();
    c->isFinished = true;
    #ifndef __APPLE__
    glfwDestroyWindow(c->window);
    #endif
  }



  int Canvas::wait() {
    if (!started) return -1;  // If we haven't even started yet, return error code -1
    #ifdef __APPLE__
    while(!isFinished)
    sleepFor(0.1f);
    #endif
    renderThread.join();
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
      RegularPolygon *c = new RegularPolygon(xverts, yverts, radius, sides, color);
      c->setHasOutline(false);
      this->add(c);
    } else {
      UnfilledRegularPolygon *c = new UnfilledRegularPolygon(xverts, yverts, radius, sides, color);
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
    // int(col, row, color); //TODO: update with new api
  }

  void Canvas::drawPoint(int x, int y, ColorFloat color) {
    //TODO convert to point layer
    // Point* p = new Point(x, y, color);
    // this->add(p);  //TODO test thread safety
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

  void Canvas::drawText(std::string text, int x, int y, unsigned size, ColorFloat color) {
    Text* t = new Text(text, x, y, size, color);  // Creates the Point with the specified coordinates and color
    this->add(t);                                // Push it onto our drawing buffer
  }

  void Canvas::drawText(std::wstring text, int x, int y, unsigned size, ColorFloat color) {
    drawText( to_string("text"), x, y, size, color); //TODO: add conversion from std::wstring to std::string
  }

  void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled) {
    if (filled) {
      Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color);  // Creates the Triangle with the specified vertices and color
      t->setHasOutline(false);
      this->add(t);                                               // Push it onto our drawing buffer
    } else {
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
    } else {
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
