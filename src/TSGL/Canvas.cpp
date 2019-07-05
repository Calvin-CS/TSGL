#include "Canvas.h"

namespace tsgl {

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
    "    outColor = vec4(Color);"
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
    "   Texcoord = texcoord;"
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
    "    outColor = texture(tex, Texcoord) * vec4(Color);"
    "}";

int Canvas::drawBuffer = GL_FRONT_LEFT;
bool Canvas::glfwIsReady = false;
std::mutex Canvas::glfwMutex;
GLFWvidmode const* Canvas::monInfo;
unsigned Canvas::openCanvases = 0;

 /*!
  * \brief Default Canvas constructor method.
  * \details This is the default constructor for the Canvas class.
  *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
  *     A value less than or equal to 0 sets it to automatic.
  * \return A new Canvas in the middle of the screen with no title.
  *   The created Canvas will take up approximately 90% of the monitor's height, and will
  *   have a 4:3 aspect ratio.
  */
Canvas::Canvas(double timerLength) {
    init(-1, -1, -1, -1, -1, "", timerLength);
}

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
Canvas::Canvas(int x, int y, int width, int height, std::string title, double timerLength) {
    init(x, y, width, height, width*height*2, title, timerLength);
}

 /*!
  * \brief Canvas destructor method.
  * \details This is the destructor for the Canvas class.
  * \details Frees up memory that was allocated to a Canvas instance.
  */
Canvas::~Canvas() {
    // Free our pointer memory
    delete myDrawables;
    delete drawableBuffer;
    delete[] proceduralBuffer;
    delete drawTimer;
    delete[] vertexData;
    delete [] screenBuffer;
    if (--openCanvases == 0) {
        glfwIsReady = false;
        glfwTerminate();  // Terminate GLFW
    }
}

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
void Canvas::bindToButton(Key button, Action action, voidFunction function) {
    boundKeys[button + action * (GLFW_KEY_LAST + 1)] = function;
}

 /*!
  * \brief Binds the mouse wheel to a function.
  * \details This function binds the mouse wheel to a function pointer.
  * \details Upon scrolling, Canvas will call the specified function.
  *   \param function A function taking x and y parameters to be called when the mouse is scrolled.
  */
void Canvas::bindToScroll(std::function<void(double, double)> function) {
    scrollFunction = function;
}

void Canvas::buttonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_REPEAT) return;
    Canvas* can = reinterpret_cast<Canvas*>(glfwGetWindowUserPointer(window));
    int index = button + action * (GLFW_KEY_LAST + 1);
    if (&(can->boundKeys[index]) != nullptr) if (can->boundKeys[index]) can->boundKeys[index]();
}

 /*!
  * \brief Clears the Canvas.
  * \details This function clears the screen to the color specified in setBackgroundColor().
  */
void Canvas::clear() {
    toClear = true;
}

 /*!
  * \brief Closes the Canvas window.
  * \details This function tells the Canvas to stop rendering and to close its rendering window.
  * \details Any threads that have called wait() will continue.
  * \see start(), stop(), wait()
  */
void Canvas::close() {
    glfwSetWindowShouldClose(window, GL_TRUE);
    TsglDebug("Window closed successfully.");
}

///////////////////////////////////////////////
// Object Interface
///////////////////////////////////////////////

/**
 * \brief Adds a Drawable to the Canvas.
 * \details If the Drawable's layer has not been set, it will default to <code>currentNewShapeLayerDefault</code>,
 * which can be set through <code>setDefaultLayer()</code>.
 *    \param shapePtr Pointer to the Drawable to add to this Canvas.
 */
void Canvas::add(Drawable * shapePtr) {

  //TODO: make this check for duplicates
  //TODO: check that this is properly thread safe now
  //TODO: check that the shapes will change layer if layer is changed after addition.

  // Set the default current layer if layer not explicitly set
  // if (shapePtr->getLayer() < 0) shapePtr->setLayer(currentNewShapeLayerDefault);

  objectMutex.lock();
  objectBuffer.push_back(shapePtr);
  objectBufferEmpty = false;
  // std::stable_sort(objectBuffer.begin(), objectBuffer.end(), [](Drawable * a, Drawable * b)->bool {
  //   return (a->getLayer() < b->getLayer());  // true if A's layer is higher than B's layer
  // });
  objectMutex.unlock();

}

/**
 * \brief Removes a Drawable from the Canvas.
 * \details Removes shapePtr from the Canvas's drawing buffer.
 *    \param shapePtr Pointer to the Drawable to remove from this Canvas.
 * \warning The Drawable being deleted or going out of scope before remove() is called will cause a segmentation fault.
 * \warning If shapePtr is not in the drawing buffer, behavior is undefined.
 */
void Canvas::remove(Drawable * shapePtr) {

  //TODO: make this thread safe! (check that it is now)

  objectMutex.lock();
  objectBuffer.erase(std::remove(objectBuffer.begin(), objectBuffer.end(), shapePtr), objectBuffer.end());
  objectMutex.unlock();

}

/**
 * \brief Removes all Drawables from the Canvas.
 * \details Clears all Drawables from the drawing buffer.
 *    \param shouldFreeMemory Whether the pointers will be deleted as well as removed and free their memory. (Defaults to false.)
 * \warning Setting shouldFreeMemory to true will cause a segmentation fault if the user continues to access the pointer to a
 *  Drawable that has been added to the Canvas.
 * \warning Setting shouldFreeMemory to false will leak memory from any objects created in Canvas draw methods.
 */
void Canvas::clearObjectBuffer(bool shouldFreeMemory) {
  if( shouldFreeMemory ) {
    for(unsigned i = 0; i < objectBuffer.size(); i++) {
      delete objectBuffer[i]; //TODO fix this, causes to crash
    }
  }
  objectBuffer.clear();
}

void Canvas::draw() {
    // Reset the window
    glfwSetWindowShouldClose(window, GL_FALSE);

    // Get actual framebuffer size and adjust scaling accordingly
    int fbw, fbh;
    glfwGetFramebufferSize(window, &fbw, &fbh);
    int scaling = round((1.0f*fbw)/winWidth);

    if (hasStereo)
      Canvas::setDrawBuffer(hasBackbuffer ? GL_FRONT_AND_BACK : GL_FRONT);
    else
      Canvas::setDrawBuffer(hasBackbuffer ? GL_LEFT : GL_FRONT_LEFT);


    setBackgroundColor(bgcolor); //Set our initial clear / background color
    // glClear(GL_COLOR_BUFFER_BIT);
    glfwSwapBuffers(window);
    readyToDraw = true;
    bool newThingDrawn = true;  //Always draw the first frame
    int frame = 0;

    // Start the drawing loop
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
        if (drawableBuffer->size() > 0) {     // But only if there is anything to flush
          newThingDrawn = true;
          for (unsigned int i = 0; i < drawableBuffer->size(); i++)
            myDrawables->push((*drawableBuffer)[i]);
          drawableBuffer->shallowClear();  // We want to clear the buffer but not delete those objects as we still need to draw them
        }
        bufferMutex.unlock();

        int pos = pointBufferPosition;
        int posLast = pointLastPosition;
        pointLastPosition = pos;

        if (loopAround || pos != posLast)
          newThingDrawn = true;

        if (newThingDrawn || !objectBufferEmpty) {

          if (hasEXTFramebuffer)
            glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
          else
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER_EXT, frameBuffer);
          glDrawBuffer(GL_COLOR_ATTACHMENT0);

          glViewport(0,0,winWidth,winHeight);
          
          if (toClear) {
            glClear(GL_COLOR_BUFFER_BIT);
            if(frame > 1) {
              textureShaders(true);
              loader.drawGLtextureFromBuffer(proceduralBuffer, -1, 0, winWidth, winHeight, GL_RGBA);
              textureShaders(false);
            }
          }
          toClear = false;


          unsigned int size = myDrawables->size();
          for (unsigned int i = 0; i < size; i++) {
            Drawable* d = (*myDrawables)[i];
            if(d->isProcessed()) {
              if (!d->getIsTextured()) {
                d->draw();
              } else {
                textureShaders(true);
                d->draw();
                textureShaders(false);
              }
            }
          }

          if (loopAround) {
            newThingDrawn = true;
            int toend = myDrawables->capacity() - posLast;
            glBufferData(GL_ARRAY_BUFFER, toend * 6 * sizeof(float),
                   &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, toend);
            posLast = 0;
            loopAround = false;
          }
          int pbsize = pos - posLast;
          if (pbsize > 0) {
            newThingDrawn = true;
            glBufferData(GL_ARRAY_BUFFER, pbsize * 6 * sizeof(float), &vertexData[posLast * 6], GL_DYNAMIC_DRAW);
            glDrawArrays(GL_POINTS, 0, pbsize);
          }

          if(frame > 0) {
            if(newThingDrawn) {
              glReadPixels(0, 0, winWidth, winHeight, GL_RGBA, GL_UNSIGNED_BYTE, proceduralBuffer);
            }
            // Reset drawn status for the next frame
            newThingDrawn = false;
            frame = 2;
          } else {
            frame = 1;
          }

          if (objectBuffer.size() > 0) {
            for (unsigned int i = 0; i < objectBuffer.size(); i++) {
              Drawable* d = objectBuffer[i];
              if(d->isProcessed()) {
                if (!d->getIsTextured()) {
                  d->draw();
                } else {
                  textureShaders(true);
                  d->draw();
                  textureShaders(false);
                }
              }
            }
          } else {
            objectBufferEmpty = true;
          }
        }

        // Update our screenBuffer copy with the screen
        glViewport(0,0,winWidth*scaling,winHeight*scaling);
        if(frame > 1) {
          myDrawables->clear();                           // Clear our buffer of shapes to be drawn
        }

        /* not sure what the point of this chunk is. */
        if (hasEXTFramebuffer)
          glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
        else
          glBindFramebuffer(GL_READ_FRAMEBUFFER_EXT, frameBuffer);
        glReadBuffer(GL_COLOR_ATTACHMENT0);

        // screenshots and testing
        glReadPixels(0, 0, winWidthPadded, winHeight, GL_RGB, GL_UNSIGNED_BYTE, screenBuffer);
        if (toRecord > 0) {
          screenShot();
          --toRecord;
        }

        /* very vital */
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
        /* apparently not very vital at all */
        glDrawBuffer(drawBuffer);

        textureShaders(true);
        const float vertices[32] = {
          0,       0,        1,1,1,1,0,1,
          winWidth,0,        1,1,1,1,1,1,
          0,       winHeight,1,1,1,1,0,0,
          winWidth,winHeight,1,1,1,1,1,0
        };
        glBindTexture(GL_TEXTURE_2D,renderedTexture);
        /* these 5 lines don't seem to do anything */
        glPixelStorei(GL_UNPACK_ALIGNMENT,4);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
        /* next two lines are very essential */
        glBufferData(GL_ARRAY_BUFFER,32*sizeof(float),vertices,GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLE_STRIP,0,4);
        glFlush();                                   // Flush buffer data to the actual draw buffer
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

 /*!
  * \brief Draws a monocolored arrow.
  * \details This function draws an arrow with the given endpoints, color, and doubleheaded status
  *   \param x1 The x coordinate of the first endpoint.
  *   \param y1 The y coordinate of the first endpoint.
  *   \param x2 The x coordinate of the second endpoint.
  *   \param y2 The y coordinate of the second endpoint.
  *   \param color A single color for the arrow.
  *   \param doubleArrow Boolean value that determines if the first endpoint is also an arrowhead.
  */
void Canvas::drawArrow(float x1, float y1, float x2, float y2, const ColorFloat color, bool doubleArrow) {
  Arrow * arrow = new Arrow(x1, y1, x2, y2, color, doubleArrow);
  drawDrawable(arrow);
}

 /*!
  * \brief Draws a multicolored arrow.
  * \details This function draws an arrow with the given endpoints, color, and doubleheaded status
  *   \param x1 The x coordinate of the first endpoint.
  *   \param y1 The y coordinate of the first endpoint.
  *   \param x2 The x coordinate of the second endpoint.
  *   \param y2 The y coordinate of the second endpoint.
  *   \param color An array of colors for the circle.
  *   \param doubleArrow Boolean value that determines if the first endpoint is also an arrowhead.
  */
void Canvas::drawArrow(float x1, float y1, float x2, float y2, const ColorFloat color[], bool doubleArrow) {
  Arrow * arrow = new Arrow(x1, y1, x2, y2, color, doubleArrow);
  drawDrawable(arrow);
}

 /*!
  * \brief Draws a monocolored filled or outlined circle.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param color A single color for the circle.
  *   \param filled Whether the circle should be filled
  *     (set to true by default).
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat color, bool filled) {
    // version 1
    Circle* c = new Circle(x, y, radius, color, filled);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, filled, !filled);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a multicolored filled or outlined circle.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param color An array of colors for the circle.
  *   \param filled Whether the circle should be filled
  *     (set to true by default).
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat color[], bool filled) {
    // version 1
    Circle* c = new Circle(x, y, radius, color, filled);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, filled, !filled);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),color[i]);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a filled and outlined circle with different monocolored fill and outline.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor A single color for circle's fill vertices.
  *   \param outlineColor A single color for circle's outline vertices.
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat fillColor, ColorFloat outlineColor) {
    // version 1
    Circle* c = new Circle(x, y, radius, fillColor, outlineColor);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, true, true);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),fillColor, outlineColor);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a filled and outlined circle with multicolored fill and monocolored outline.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor An array of colors for circle's fill vertices.
  *   \param outlineColor A single color for circle's outline vertices.
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat fillColor[], ColorFloat outlineColor) {
    // version 1
    Circle* c = new Circle(x, y, radius, fillColor, outlineColor);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, true, true);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),fillColor[i], outlineColor);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a filled and outlined circle with monocolored fill and multicolored outline.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor A single color for circle's fill vertices.
  *   \param outlineColor An array of colors for circle's outline vertices.
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat fillColor, ColorFloat outlineColor[]) {
    // version 1
    Circle* c = new Circle(x, y, radius, fillColor, outlineColor);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, true, true);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),fillColor, outlineColor[i]);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a filled and outlined circle with different multicolored fill and outline.
  * \details This function draws a circle with the given center, radius, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the circle's center.
  *   \param y The y coordinate of the circle's center.
  *   \param radius The radius of the circle in pixels.
  *   \param fillColor An array of colors for circle's fill vertices.
  *   \param outlineColor An array of colors for circle's outline vertices.
  */
void Canvas::drawCircle(int x, int y, int radius, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    // version 1
    Circle* c = new Circle(x, y, radius, fillColor, outlineColor);  // Creates the Line with the specified coordinates and color
    drawDrawable(c);                               // Push it onto our drawing buffer

    //version 2 requires sides parameter
    // float delta = 2.0f / sides * PI;
    // ConvexPolygon *s = new ConvexPolygon(sides, true, true);
    // for (int i = 0; i < sides; ++i)
    //     s->addVertex(x+radius*cos(i*delta), y+radius*sin(i*delta),fillColor[i],outlineColor[i]);
    // drawDrawable(s);
}

 /*!
  * \brief Draws a monocolored filled or outlined concave polygon.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param color A single color for the said vertices.
  *   \param filled Whether the ConcavePolygon should be filled in or not
  *     (set to true by default).
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat color, bool filled) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored filled or outlined concave polygon.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param color An array of colors for the said vertices.
  *   \param filled Whether the ConcavePolygon should be filled in or not
  *     (set to true by default).
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined concave polygon with different monocolored fill and outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor A single color for the outline vertices.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined concave polygon with multicolored fill and monocolored outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor A single color for the outline vertices.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined concave polygon with monocolored fill and multicolored outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined concave polygon with different multicolored fill and outline.
  * \details This function draws a ConcavePolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of said vertices.
  *   \param y An array of y positions of said vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  * \warning <b>This function is significantly slower than drawConvexPolygon(). It is not recommended
  *   that you draw convex polygons with this function.
  * \see drawConvexPolygon().
  */
void Canvas::drawConcavePolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    ConcavePolygon* p = new ConcavePolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a monocolored filled or outlined convex polygon.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of the x positions of said vertices.
  *   \param y An array of the y positions of said vertices.
  *   \param color A single color for the said vertices.
  *   \param filled Whether the ConvexPolygon should be filled in or not
  *     (set to true by default).
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat color, bool filled) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored filled or outlined convex polygon.
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
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat color[], bool filled) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined convex polygon with different monocolored fill and outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of the x positions of said vertices.
  *   \param y An array of the y positions of said vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined convex polygon with multicolored fill and monocolored outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of the x positions of said vertices.
  *   \param y An array of the y positions of said vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor A single color for the outline vertices.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined convex polygon with monocolored fill and multicolored outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of the x positions of said vertices.
  *   \param y An array of the y positions of said vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined convex polygon with different multicolored fill and outline.
  * \details This function draws a ConvexPolygon with the given vertex data, specified as the
  *   outer perimeter of the polygon.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of the x positions of said vertices.
  *   \param y An array of the y positions of said vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  * \note The difference between a convex polygon and a concave polygon
  *   is that a convex polygon has all interior angles less than
  *   180 degrees ( see http://www.mathopenref.com/polygonconvex.html ).
  */
void Canvas::drawConvexPolygon(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    ConvexPolygon* p = new ConvexPolygon(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

/*!
 * \brief Draws a Drawable object
 * \details This function pushes any Drawable object onto the drawable buffer
 *    \param d The Drawable object to be drawn
 * \note protected method
 */
void Canvas::drawDrawable(Drawable* d) {
	if (!started) {
	  TsglDebug("No drawing before Canvas is started! Ignoring draw request.");
	  return;
	}
	while (!readyToDraw)
	  sleep();
    bufferMutex.lock();
    drawableBuffer->push(d);  // Push it onto our drawing buffer
    bufferMutex.unlock();
}

 /*!
  * \brief Draws a monocolored filled or outlined ellipse.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param color A single color for ellipse.
  *   \param filled Whether the ellipse should be filled
  *     (set to true by default).
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat color, bool filled) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, color, filled);
  drawDrawable(e);
}

 /*!
  * \brief Draws a multicolored filled or outlined ellipse.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), color, and fill status.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param color An array of colors for ellipse.
  *   \param filled Whether the ellipse should be filled
  *     (set to true by default).
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat color[], bool filled) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, color, filled);
  drawDrawable(e);
}

 /*!
  * \brief Draws a filled and outlined ellipse with different monocolored fill and outline.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), colors.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param fillColor A single color for ellipse's fill vertices.
  *   \param outlineColor A single color for ellipse's outline vertices.
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor, ColorFloat outlineColor) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, fillColor, outlineColor);
  drawDrawable(e);
}

 /*!
  * \brief Draws a filled and outlined ellipse with multicolored fill and monocolored outline.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), colors.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param fillColor An array of colors for ellipse's fill vertices.
  *   \param outlineColor A single color for ellipse's outline vertices.
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor[], ColorFloat outlineColor) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, fillColor, outlineColor);
  drawDrawable(e);
}

 /*!
  * \brief Draws a filled and outlined ellipse with monocolored fill and multicolored outline.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), colors.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param fillColor A single color for ellipse's fill vertices.
  *   \param outlineColor An array of colors for ellipse's outline vertices.
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor, ColorFloat outlineColor[]) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, fillColor, outlineColor);
  drawDrawable(e);
}

 /*!
  * \brief Draws a filled and outlined ellipse with different multicolored fill and outline.
  * \details This function draws an ellipse with the given center, radii, resolution
  *   (number of sides), colors.
  *   \param x The x coordinate of the ellipse's center.
  *   \param y The y coordinate of the ellipse's center.
  *   \param xRadius The x radius of the ellipse in pixels.
  *   \param yRadius The x radius of the ellipse in pixels.
  *   \param sides The number of sides to use in the ellipse.
  *   \param fillColor An array of colors for ellipse's fill vertices.
  *   \param outlineColor An array of colors for ellipse's outline vertices.
  */
void Canvas::drawEllipse(int x, int y, int xRadius, int yRadius, ColorFloat fillColor[], ColorFloat outlineColor[]) {
  Ellipse * e = new Ellipse(x, y, xRadius, yRadius, fillColor, outlineColor);
  drawDrawable(e);
}

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
void Canvas::drawImage(std::string filename, int x, int y, int width, int height, float alpha) {
    Image* im = new Image(filename, loader, x, y, width, height, alpha);  // Creates the Image with the specified coordinates
    drawDrawable(im);                                       // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a monocolored line.
  * \details This function draws a Line at the given coordinates with the given color.
  *   \param x1 The x position of the start of the line.
  *   \param y1 The y position of the start of the line.
  *   \param x2 The x position of the end of the line.
  *   \param y2 The y position of the end of the line.
  *   \param color The color of the line
  *     (set to BLACK by default).
  */
void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorFloat color) {
    Line* l = new Line(x1, y1, x2, y2, color);  // Creates the Line with the specified coordinates and color
    drawDrawable(l);                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored line.
  * \details This function draws a Line at the given coordinates with the given color.
  *   \param x1 The x position of the start of the line.
  *   \param y1 The y position of the start of the line.
  *   \param x2 The x position of the end of the line.
  *   \param y2 The y position of the end of the line.
  *   \param color A color array for the line.
  */
void Canvas::drawLine(int x1, int y1, int x2, int y2, ColorFloat color[]) {
    Line* l = new Line(x1, y1, x2, y2, color);  // Creates the Line with the specified coordinates and color
    drawDrawable(l);                               // Push it onto our drawing buffer
}

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
inline void Canvas::drawPixel(int row, int col, ColorFloat color) {
    drawPoint(col, row, color);
}

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
void Canvas::drawPoint(int x, int y, ColorFloat color) {
    pointArrayMutex.lock();
    if (pointBufferPosition >= myDrawables->capacity()) {
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

 /*!
  * \brief Draws a monocolored series of connected lines.
  * \details This function draws Polyline at the given coordinates with the given color.
  *   \param size The number of vertices of the polyline.
  *   \param x An array of the x positions of the polyline's vertices.
  *   \param y An array of the y positions of the polyline's vertices.
  *   \param color A color for the Polyline.
  */
void Canvas::drawPolyline(int size, int x[], int y[], ColorFloat color) {
    Polyline* p = new Polyline(size, x, y, color);  // Creates the Line with the specified coordinates and color
    drawDrawable(p);                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored series of connected lines.
  * \details This function draws Polyline at the given coordinates with the given color.
  *   \param size The number of vertices of the polyline.
  *   \param x An array of the x positions of the polyline's vertices.
  *   \param y An array of the y positions of the polyline's vertices.
  *   \param color A color array for the Polyline.
  */
void Canvas::drawPolyline(int size, int x[], int y[], ColorFloat color[]) {
    Polyline* p = new Polyline(size, x, y, color);  // Creates the Line with the specified coordinates and color
    drawDrawable(p);                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a progress bar.
  * \details This function draws a previously created ProgressBar to the Canvas, as
  *   specified in that ProgressBar's constructor.
  *   \param p A pointer to a ProgressBar.
  * \note There is no equivalent function for CartesianCanvas. If you'd like to draw
  *   a ProgressBar on a CartesianCanvas, you can still use this function, but you must
  *   use absolute Canvas coordinates rather than the scaled CartesianCanvas coordinates.
  */
void Canvas::drawProgress(ProgressBar* p) {
    for (int i = 0; i < p->getSegs(); ++i) {
      drawText(to_string(i),p->getSegX(i)+8,p->getSegY()-8,32,BLACK);
      drawDrawable(p->getRect(i));
      drawDrawable(p->getBorder(i));
    }
}

 /*!
  * \brief Draws a monocolored filled or outlined rectangle.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param color A single color for Rectangle.
  *   \param filled Whether the Rectangle should be filled
  *     (set to true by default).
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat color, bool filled) {
    Rectangle* rec = new Rectangle(x, y, w, h, color, filled);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored filled or outlined rectangle.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param color An array of colors for Rectangle.
  *   \param filled Whether the Rectangle should be filled
  *     (set to true by default).
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat color[], bool filled) {
    Rectangle* rec = new Rectangle(x, y, w, h, color, filled);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined rectangle with different monocolored fill and outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor A single color for Rectangle's fill vertices.
  *   \param outlineColor A single color for Rectangle's outline vertices.
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat fillColor, ColorFloat outlineColor) {
    Rectangle* rec = new Rectangle(x, y, w, h, fillColor, outlineColor);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief  Draws a filled and outlined rectangle with multicolored fill and monocolored outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor An array of colors for Rectangle's fill vertices.
  *   \param outlineColor A single color for Rectangle's outline vertices.
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat fillColor[], ColorFloat outlineColor) {
    Rectangle* rec = new Rectangle(x, y, w, h, fillColor, outlineColor);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined rectangle with monocolored fill and multicolored outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor A single color for Rectangle's fill vertices.
  *   \param outlineColor An array of colors for Rectangle's outline vertices.
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat fillColor, ColorFloat outlineColor[]) {
    Rectangle* rec = new Rectangle(x, y, w, h, fillColor, outlineColor);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined rectangle with different multicolored fill and outline.
  * \details This function draws a Rectangle with the given coordinates, dimensions, and color.
  *   \param x The x coordinate of the Rectangle's left edge.
  *   \param y The y coordinate of the Rectangle's top edge.
  *   \param w The Rectangle's width.
  *   \param h The Rectangle's height.
  *   \param fillColor An array of colors for Rectangle's fill vertices.
  *   \param outlineColor An array of colors for Rectangle's outline vertices.
  * \bug The bottom-right pixel of a non-filled Rectangle may not get drawn on some machines.
  */
void Canvas::drawRectangle(float x, float y, float w, float h, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    Rectangle* rec = new Rectangle(x, y, w, h, fillColor, outlineColor);  // Creates the Rectangle with the specified coordinates and color
    drawDrawable(rec);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief  Draws a monocolored filled or outlined regular polygon.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and colors
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param color A single color for RegularPolygon.
  *   \param filled Whether the regular polygon should be filled
  *     (set to true by default).
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat color, bool filled) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, color, filled);
    drawDrawable(c);
}

 /*!
  * \brief Draws a multicolored filled or outlined regular polygon.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and color
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param color An array of colors for RegularPolygon.
  *   \param filled Whether the regular polygon should be filled
  *     (set to true by default).
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat color[], bool filled) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, color, filled);
    drawDrawable(c);
}

 /*!
  * \brief Draws a filled and outlined regular polygon with different monocolored fill and outline.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and colors
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param fillColor A single color for RegularPolygon's fill vertices.
  *   \param outlineColor A single color for RegularPolygon's outline vertices.
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor, ColorFloat outlineColor) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, fillColor, outlineColor);
    drawDrawable(c);
}

 /*!
  * \brief Draws a filled and outlined regular polygon with multicolored fill and monocolored outline.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and colors
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param fillColor An array of colors for RegularPolygon's fill vertices.
  *   \param outlineColor A single color for RegularPolygon's outline vertices.
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor[], ColorFloat outlineColor) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, fillColor, outlineColor);
    drawDrawable(c);
}

 /*!
  * \brief Draws a filled and outlined regular polygon with monocolored fill and multicolored outline.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and colors
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param fillColor A single color for RegularPolygon's fill vertices.
  *   \param outlineColor An array of colors for RegularPolygon's outline vertices.
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor, ColorFloat outlineColor[]) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, fillColor, outlineColor);
    drawDrawable(c);
}

 /*!
  * \brief Draws a filled and outlined regular polygon with different multicolored fill and outline.
  * \details This function draws a RegularPolygon with the given coordinates, radius, sides, and colors
  *   \param x The x coordinate of the RegularPolygon's center
  *   \param y The y coordinate of the RegularPolygon's center
  *   \param radius The distance from the center to each vertex
  *   \param sides The number of sides for the RegularPolygon
  *   \param fillColor An array of colors for RegularPolygon's fill vertices.
  *   \param outlineColor An array of colors for RegularPolygon's outline vertices.
  */
void Canvas::drawRegularPolygon(int x, int y, int radius, int sides, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    RegularPolygon *c = new RegularPolygon(x, y, radius, sides, fillColor, outlineColor);
    drawDrawable(c);
}

 /*!
  * \brief Draws a monocolored filled or outlined square.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param color A single color for Square.
  *   \param filled Whether the Square should be filled
  *     (set to true by default).
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat color, bool filled) {
    Square* s = new Square(x1, y1, sideLength, color, filled);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a multicolored filled or outlined Square.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param color An array of colors for Square.
  *   \param filled Whether the Square should be filled
  *     (set to true by default).
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat color[], bool filled) {
    Square* s = new Square(x1, y1, sideLength, color, filled);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined Square with different monocolored fill and outline.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param fillColor A single color for Square's fill vertices.
  *   \param outlineColor A single color for Square's outline vertices.
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor, ColorFloat outlineColor) {
    Square* s = new Square(x1, y1, sideLength, fillColor, outlineColor);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief  Draws a filled and outlined Square with multicolored fill and monocolored outline.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param fillColor An array of colors for Square's fill vertices.
  *   \param outlineColor A single color for Square's outline vertices.
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor[], ColorFloat outlineColor) {
    Square* s = new Square(x1, y1, sideLength, fillColor, outlineColor);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined Square with monocolored fill and multicolored outline.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param fillColor A single color for Square's fill vertices.
  *   \param outlineColor An array of colors for Square's outline vertices.
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor, ColorFloat outlineColor[]) {
    Square* s = new Square(x1, y1, sideLength, fillColor, outlineColor);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined Square with different multicolored fill and outline.
  * \details This function draws a Square with the given coordinates, dimensions, and color.
  *   \param x1 The x coordinate of the Square's left edge.
  *   \param y1 The y coordinate of the Square's top edge.
  *   \param x2 The x coordinate of the Square's right edge.
  *   \param y2 The y coordinate of the Square's bottom edge.
  *   \param fillColor An array of colors for Square's fill vertices.
  *   \param outlineColor An array of colors for Square's outline vertices.
  * \bug The bottom-right pixel of a non-filled Square may not get drawn on some machines.
  */
void Canvas::drawSquare(int x1, int y1, int sideLength, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    Square* s = new Square(x1, y1, sideLength, fillColor, outlineColor);  // Creates the Square with the specified coordinates and color
    drawDrawable(s);                                     // Push it onto our drawing buffer
}


 /*!
  * \brief Draws a monocolored filled or outlined star.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param color A single color or array of colors for the star vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  *   \param filled Whether the star should be filled
  *     (set to true by default).
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat color, bool ninja, float rotation, bool filled) {
  Star * star = new Star(x, y, radius, points, color, ninja, rotation, filled);
  drawDrawable(star);
}

 /*!
  * \brief Draws a multicolored filled or outlined star.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param color A single color or array of colors for the star vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  *   \param filled Whether the star should be filled
  *     (set to true by default).
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat color[], bool ninja, float rotation, bool filled) {
  Star * star = new Star(x, y, radius, points, color, ninja, rotation, filled);
  drawDrawable(star);
}

 /*!
  * \brief Draws a filled and outlined star with different monocolored fill and outline.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param fillColor A single color or array of colors for the star's fill vertices.
  *   \param outlineColor A single color or array of colors for the star's outline vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat fillColor, ColorFloat outlineColor, bool ninja, float rotation) {
  Star * star = new Star(x, y, radius, points, fillColor, outlineColor, ninja, rotation);
  drawDrawable(star);
}

 /*!
  * \brief Draws a filled and outlined star with multicolored fill and monocolored outline.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param fillColor A single color or array of colors for the star's fill vertices.
  *   \param outlineColor A single color or array of colors for the star's outline vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat fillColor[], ColorFloat outlineColor, bool ninja, float rotation) {
  Star * star = new Star(x, y, radius, points, fillColor, outlineColor, ninja, rotation);
  drawDrawable(star);
}

 /*!
  * \brief Draws a filled and outlined star with monocolored fill and multicolored outline.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param fillColor A single color or array of colors for the star's fill vertices.
  *   \param outlineColor A single color or array of colors for the star's outline vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat fillColor, ColorFloat outlineColor[], bool ninja, float rotation) {
  Star * star = new Star(x, y, radius, points, fillColor, outlineColor, ninja, rotation);
  drawDrawable(star);
}

 /*!
  * \brief Draws a filled and outlined star with different multicolored fill and outline.
  * \details This function draws a Star with the given coordinates, radius, points, and color.
  *   \param x1 The x coordinate of the star's center
  *   \param y1 The y coordinate of the star's center
  *   \param radius Radius of the outer points of the star
  *   \param points The number of points on the star
  *   \param fillColor A single color or array of colors for the star's fill vertices.
  *   \param outlineColor A single color or array of colors for the star's outline vertices.
  *   \param ninja makes it look conventional or like a shuriken
  *   \param rotation how many radians the star should be rotated
  */
void Canvas::drawStar(int x, int y, int radius, int points, ColorFloat fillColor[], ColorFloat outlineColor[], bool ninja, float rotation) {
  Star * star = new Star(x, y, radius, points, fillColor, outlineColor, ninja, rotation);
  drawDrawable(star);
}

 /*!
  * \brief Draw a string of text.
  * \details This function draws a given string of Text at the given coordinates with the given color.
  *   \param text The string to draw.
  *   \param x The x coordinate of the text's left bound.
  *   \param y The y coordinate of the text's left bound.
  *   \param size The size of the text in pixels.
  *   \param color The color of the Text (set to BLACK by default).
  */
void Canvas::drawText(std::string text, int x, int y, unsigned size, ColorFloat color) {
    std::wstring wsTmp(text.begin(), text.end());
    std::wstring ws = wsTmp;
    drawText(ws, x, y, size, color);
}

 /*!
  * \brief Draws a UTF8-encoded string of text.
  * \details This function draws a given string of UTF-8 encoded Text at the given coordinates with the given color.
  *   \param text The UTF8-encoded string to draw.
  *   \param x The x coordinate of the text's left bound.
  *   \param y The y coordinate of the text's left bound.
  *   \param size The size of the text in pixels.
  *   \param color The color of the Text (set to BLACK by default).
  * \note Identical to the drawText(std::string, ...) aside from the first parameter.
  * \see drawText(std::string s, int x, int y, unsigned size, ColorFloat color = BLACK).
  */
void Canvas::drawText(std::wstring text, int x, int y, unsigned int size, ColorFloat color) {
    Text* t = new Text(text, loader, x, y, size, color);  // Creates the Point with the specified coordinates and color
    drawDrawable(t);                                // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a monocolored filled or outlined triangle.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param color A single color for the Triangle vertices.
  *   \param filled Whether the Triangle should be filled (set to true by default).
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color, bool filled) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color, filled);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief  Draws a multicolored filled or outlined triangle.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param color An array of colors for the Triangle vertices.
  *   \param filled Whether the Triangle should be filled (set to true by default).
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat color[], bool filled) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, color, filled);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief  Draws a filled and outlined triangle with different monocolored fill and outline.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param fillColor A single color for the Triangle's fill vertices.
  *   \param outlineColor A single color for the Triangle's outline vertices.
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor, ColorFloat outlineColor) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, fillColor, outlineColor);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined triangle with multicolored fill and monocolored outline.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param fillColor An array of colors for the Triangle's fill vertices.
  *   \param outlineColor A single color for the Triangle's outline vertices.
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor[], ColorFloat outlineColor) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, fillColor, outlineColor);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined triangle with monocolored fill and multicolored outline.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param fillColor A single color for the Triangle's fill vertices.
  *   \param outlineColor An array of colors for the Triangle's outline vertices.
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor, ColorFloat outlineColor[]) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, fillColor, outlineColor);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws a filled and outlined triangle with different multicolored fill and outline.
  * \details This function draws a Triangle with the given vertices.
  *   \param x1 The x coordinate of the first vertex of the Triangle.
  *   \param y1 The y coordinate of the first vertex of the Triangle.
  *   \param x2 The x coordinate of the second vertex of the Triangle.
  *   \param y2 The y coordinate of the second vertex of the Triangle.
  *   \param x3 The x coordinate of the third vertex of the Triangle.
  *   \param y3 The y coordinate of the third vertex of the Triangle.
  *   \param fillColor An array of colors for the Triangle's fill vertices.
  *   \param outlineColor An array of colors for the Triangle's outline vertices.
  */
void Canvas::drawTriangle(int x1, int y1, int x2, int y2, int x3, int y3, ColorFloat fillColor[], ColorFloat outlineColor[]) {
    Triangle* t = new Triangle(x1, y1, x2, y2, x3, y3, fillColor, outlineColor);  // Creates the Triangle with the specified vertices and color
    drawDrawable(t);                                               // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled or outlined monocolored triangle strip.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param color A single color for the vertices.
  *   \param filled Whether the triangle strip should be filled (true) or not (false).
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat color, bool filled) {
    TriangleStrip* p = new TriangleStrip(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled or outlined multicolored triangle strip.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param color An array of colors for the said vertices.
  *   \param filled Whether the triangle strip should be filled (true) or not (false).
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat color[], bool filled) {
    TriangleStrip* p = new TriangleStrip(size, x, y, color, filled);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled and outlined triangle strip with different monocolored fill and outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor A single color for the outline vertices.
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) {
    TriangleStrip* p = new TriangleStrip(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled and outlined triangle strip with multicolored fill and monocolored outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor A single color for the outline vertices.
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) {
    TriangleStrip* p = new TriangleStrip(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled and outlined triangle strip with monocolored fill and multicolored outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param fillColor A single color for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) {
    TriangleStrip* p = new TriangleStrip(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

 /*!
  * \brief Draws an arbitrary filled and outlined triangle strip with different multicolored fill and outline.
  * \details This function draws a TriangleStrip with the given vertex data, specified as
  *   a triangle strip.
  *   \param size The number of vertices in the polygon.
  *   \param x An array of x positions of the vertices.
  *   \param y An array of y positions of the vertices.
  *   \param fillColor An array of colors for the fill vertices.
  *   \param outlineColor An array of colors for the outline vertices.
  */
void Canvas::drawTriangleStrip(int size, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) {
    TriangleStrip* p = new TriangleStrip(size, x, y, fillColor, outlineColor);
    drawDrawable(p);  // Push it onto our drawing buffer
}

void Canvas::errorCallback(int error, const char* string) {
    fprintf(stderr, "%i: %s\n", error, string);
}

 /*!
  * \brief Accessor for the current background color.
  * \return The color that the Canvas clears to when clear() is called.
  */
ColorFloat Canvas::getBackgroundColor() {
  return bgcolor;
}

 /*!
  * \brief Accessor for the height of the user's primary monitor.
  * \return The height of the user's primary monitor.
  */
int Canvas::getDisplayHeight() {
  initGlfw();
  return monInfo->height;
}

 /*!
  * \brief Accessor for the width of the user's primary monitor.
  * \return The width of the user's primary monitor.
  */
int Canvas::getDisplayWidth() {
  initGlfw();
  return monInfo->width;
}

 /*!
  * \brief Accessor for the current frame number.
  * \return The number of actual draw cycles / frames the Canvas has rendered so far.
  * \see getReps()
  */
int Canvas::getFrameNumber() {
    return frameCounter;
}

 /*!
  * \brief Accessor for the current FPS.
  * \return The average number of frames being rendered per second.
  */
float Canvas::getFPS() {
    return realFPS;
}

 /*!
  * \brief Accessor for window's closed status.
  * \return Whether the window is still open (that is, the user has not closed it).
  */
bool Canvas::isOpen() {
    return !isFinished;
}

 /*!
  * \brief Accessor for the mouse's x-position.
  * \return The x coordinates of the mouse on the Canvas.
  */
int Canvas::getMouseX() {
    return mouseX;
}

 /*!
  * \brief Accessor for the mouse's y-position.
  * \return The y coordinates of the mouse on the Canvas.
  */
int Canvas::getMouseY() {
    return mouseY;
}

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
ColorInt Canvas::getPixel(int row, int col) {
    return getPoint(col,row);
}

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
ColorInt Canvas::getPoint(int x, int y) {
    int yy;
    //if (atiCard)
    //  yy = (winHeight) - y; //glReadPixels starts from the bottom left, and we have no way to change that...
    //else
      yy = (winHeight-1) - y;
    int off = 3 * (yy * winWidthPadded + x);
    return ColorInt(screenBuffer[off], screenBuffer[off + 1], screenBuffer[off + 2], 255);
}

 /*!
  * \brief Accessor for the number of theoretical draw cycles that have elapsed
  * \details This function returns the time elapsed since the Canvas has been opened divided
  *   by the drawTimer's period.
  * \return The number of times the drawTimer has expired since starting the Canvas.
  * \see getFrameNumber()
  */
unsigned int Canvas::getReps() const {
    return drawTimer->getReps();
}

 /*!
  * \brief Accessor for the Canvas's currently drawn image.
  * \return A pointer to the RGB pixel buffer for the current Canvas.
  * \note The array starts in the bottom left corner of the image, and is in row-major ordering.
  * \deprecated <b>This function returns a pointer directly to the Canvas' screen buffer. This
  *   function may be removed in future versions of TSGL. Please use getPixel() or getPoint()
  *   get individual pixels.
  */
uint8_t* Canvas::getScreenBuffer() {
    return screenBuffer;
}

 /*!
  * \brief Accessor for the time since the Canvas was initialized.
  * \return The elapsed time in microseconds since the Canvas has started drawing.
  */
double Canvas::getTime() {
    return drawTimer->getTime();
}

 /*!
  * \brief Accessor that gets the time between two sleep times of the internal drawing timer of a Canvas object.
  * \return The time between two sleep cycles of the internal drawing timer.
  */
double Canvas::getTimeBetweenSleeps() const {
    return drawTimer->getTimeBetweenSleeps();
}

 /*!
  * \brief Accessor for the Canvas's window height.
  * \return The height in pixels of the Canvas window.
  */
int Canvas::getWindowHeight() {
    return winHeight;
}

 /*!
  * \brief Accessor for the Canvas's window width.
  * \return The width in pixels of the Canvas window.
  */
int Canvas::getWindowWidth() {
    return winWidth;
}

 /*!
  * \brief Accessor for the Canvas's x-position.
  * \return The x coordinate in pixels of the left of the Canvas (0 = left of monitor).
  */
int Canvas::getWindowX() {
    return monitorX;
}

 /*!
  * \brief Accessor for the Canvas's y-position.
  * \return The y coordinate in pixels of the top of the Canvas (0 = top of monitor).
  */
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

 /*!
  * \brief Manually handles keyboard/mouse I/O.
  * \note This function will return immediately if not called from the main thread.
  * \note <b>OS X:</b> This function exists for OS X compatibility purposes.
  *   This function does nothing on Linux or Windows.
  */
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
    proceduralBufferSize = 4 * winWidth * winHeight;
    screenBuffer = new uint8_t[bufferSize];
    proceduralBuffer = new GLubyte[proceduralBufferSize];
    for (unsigned i = 0; i < bufferSize; ++i) {
      screenBuffer[i] = 0;
    }
    for (unsigned i = 0; i < proceduralBufferSize; i++) {
      proceduralBuffer[i] = 255;
    }

    toClear = true;                   // Don't need to clear at the start
    started = false;                  // We haven't started the window yet
    monitorX = xx;
    monitorY = yy;
    myDrawables = new Array<Drawable*>(b);  // Initialize myDrawables
    drawableBuffer = new Array<Drawable*>(b);
    // objectBuffer = new Array<Drawable*>(b);
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
    initGlew();
    glfwMakeContextCurrent(NULL);   // Reset the context
#endif
}

void Canvas::initGl() {
#ifdef _WIN32
    initWindow();
    initGlew();
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

void Canvas::initGlew() {
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

    // Create and bind our Vertex Array Object
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);

    // Create and bind our Vertex Buffer Object
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);

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

void Canvas::initGlfw() {
  if (!glfwIsReady) {
    glfwInit();  // Initialize GLFW
    monInfo = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwIsReady = true;
  }
}

void Canvas::initWindow() {
    glfwSetErrorCallback(errorCallback);

     // Create a Window and the Context
  #ifdef __arm__                                                    // Tentative fix for the Raspberry Pi
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);                  // Set target GL major version to 2
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);                  // Set target GL minor version to 2.0
    glfwWindowHint(GLFW_CLIENT_API,GLFW_OPENGL_ES_API);             // Pi uses OpenGL ES
  #else
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);                  // Set target GL major version to 3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);                  // Set target GL minor version to 3.2
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // We're using the standard GL Profile
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Don't use methods that are deprecated in the target version
  #endif
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);                       // Do not let the user resize the window
    glfwWindowHint(GLFW_STEREO, GL_FALSE);                          // Disable the right buffer
    glfwWindowHint(GLFW_DOUBLEBUFFER, GL_FALSE);                    // Disable the back buffer
    glfwWindowHint(GLFW_VISIBLE, GL_FALSE);                         // Don't show the window at first
    glfwWindowHint(GLFW_SAMPLES,4);

    glfwMutex.lock();                                  // GLFW crashes if you try to make more than once window at once
    window = glfwCreateWindow(winWidth, winHeight, winTitle.c_str(), NULL, NULL);  // Windowed
 //   window = glfwCreateWindow(monInfo->width, monInfo->height, title_.c_str(), glfwGetPrimaryMonitor(), NULL);  // Fullscreen
    if (!window) {
        fprintf(stderr, "GLFW window creation failed. Was the library correctly initialized?\n");
        exit(100);
    }
    glfwMutex.unlock();

    if (!monInfo) {
        fprintf(stderr, "GLFW failed to return monitor information. Was the library correctly initialized?\n");
        exit(101);
    }
    if (monitorX == -1)
      monitorX = (monInfo->width - winWidth) / 2;
    if (monitorY == -1)
      monitorY = (monInfo->height - winHeight) / 2;
    glfwSetWindowPos(window, monitorX, monitorY);

    glfwMakeContextCurrent(window);
    glfwShowWindow(window);                 // Show the window
    glfwSetWindowUserPointer(window, this);

    glfwSetMouseButtonCallback(window, buttonCallback);
    glfwSetKeyCallback(window, keyCallback);
    glfwSetScrollCallback(window, scrollCallback);
}

void Canvas::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    buttonCallback(window, key, action, mods);
}

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

 /*!
  * \brief Records the Canvas for a specified number of frames.
  * \details This function starts dumping screenshots of the Canvas to the working directory every draw
  *   cycle.
  * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
  * \details The function automatically terminates after num_frames cycles have completed.
  *   \param num_frames The number of frames to dump screenshots for.
  */
void Canvas::recordForNumFrames(unsigned int num_frames) {
    toRecord = num_frames;
}

 /*!
  * \brief Resets the internal drawing timer of a Canvas instance.
  * \details This function resets the starting time of the Canvas' draw timer
  *   to the current time.
  */
void Canvas::reset() {
    drawTimer->reset();
}

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

 /*!
  * \brief Start the Canvas, run a function on it, and wait for the user to close it
  * \details This function binds another function to the current Canvas, waits until that function is
  *   complete, and waits for the user to close the Canvas.  This function effectively calls start(),
  *   <code>myFunction</code>(), and wait() in sequence.
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  */
void Canvas::run(void (*myFunction)(Canvas&) ) {
  start(); myFunction(*this); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int), int i) {
  start(); myFunction(*this, i); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param u An unsigned integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, unsigned), unsigned u) {
  start(); myFunction(*this, u); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i1 An integer argument to myFunction
  * \param i2 An integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, int), int i1, int i2) {
  start(); myFunction(*this, i1, i2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param u1 An unsigned integer argument to myFunction
  * \param u2 An unsigned integer argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
  start(); myFunction(*this, u1, u2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param s A string argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, std::string),std::string s) {
  start(); myFunction(*this, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param i An integer argument to myFunction
  * \param s A string argument to myFunction
  */
void Canvas::run(void (*myFunction)(Canvas&, int, std::string), int i, std::string s) {
  start(); myFunction(*this, i, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the Canvas. Must take exactly one parameter of type Canvas&,
  *   which is a reference to the Canvas to render to.
  * \param s A string argument to myFunction
  * \param i An integer argument to myFunction
  */
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

 /*!
  * \brief Mutator for the background color.
  * \details This function sets the clear color for when Canvas::clear() is called.
  *   \param color The color to clear to.
  * \note The alpha channel of the color is ignored.
  */
void Canvas::setBackgroundColor(ColorFloat color) {
    bgcolor = color;
    if (window != nullptr) {
      glfwMakeContextCurrent(window);
      glClearColor(color.R,color.G,color.B,color.A);
      glfwMakeContextCurrent(NULL);
    }
}

void Canvas::setDrawBuffer(int buffer) {
    Canvas::drawBuffer = buffer;
}

 /*!
  * \brief Mutator for the currently loaded font.
  * \details This function sets the font with the specified filename into memory.
  *   Subsequent calls to drawText() will use this font to print.
  *   \param filename The filename of the font to load.
  * \note Supports all font types that FreeType supports.
  */
void Canvas::setFont(std::string filename) {
    loader.loadFont(filename);
}

 /*!
  * \brief Mutator for showing the FPS.
  *   \param b Whether to print the FPS to stdout every draw cycle (for debugging purposes).
  */
void Canvas::setShowFPS(bool b) {
    showFPS = b;
}

void Canvas::setupCamera() {
    // Set up camera positioning
    // Note: (winWidth-1) is a dark voodoo magic fix for some camera issues
    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
      -(winWidth-1) / 2.0f, (winHeight) / 2.0f, -(winHeight) / 2.0f, 1 };
//    float viewF[] = { 1, 0, 0, 0, 0, -1, 0, 0, 0, 0, -1, 0,
//      -(winWidth-1) / 2.0f, (winHeight+0.5f) / 2.0f, -(winHeight-0.5f) / 2.0f, 1 };
    glUniformMatrix4fv(uniView, 1, GL_FALSE, &viewF[0]);

    // Set up camera zooming
    float projF[] = { 1.0f / aspect, 0, 0, 0, 0, 1, 0, 0, 0, 0, -1.0f, -1, 0, 0, -0.02f, 0 };
    glUniformMatrix4fv(uniProj, 1, GL_FALSE, &projF[0]);

    // Set up camera transformation
    float modelF[] = { 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 };
    glUniformMatrix4fv(uniModel, 1, GL_FALSE, &modelF[0]);
}

 /*!
  * \brief Sleeps the calling thread to sync with the Canvas.
  * \details Tells the calling thread to sleep until the Canvas' drawTimer expires.
  * \note It is recommened that you call sleep() at least once before doing any drawing;
  *   otherwise, the Canvas may not render the first frame of drawing.
  * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
  */
void Canvas::sleep() {
  #ifdef __APPLE__
    handleIO();
  #endif
    drawTimer->sleep(false);
}

 /*!
  * \brief Sleeps the calling thread for a set amount of time
  * \details Tells the calling thread to sleep for <code>seconds</code> seconds.
  *   \param seconds Number of seconds to sleep for
  * \note <b>OS X:</b> This function automatically calls handleIO() on OS X.
  */
void Canvas::sleepFor(float seconds) {
#ifdef __APPLE__
    handleIO();
#endif
    std::this_thread::sleep_for(std::chrono::nanoseconds((long long) (seconds * 1000000000)));
}

 /*!
  * \brief Opens the Canvas.
  * \details This function opens a GL window and tells the Canvas to start rendering.
  * \return 0 if start is successful, -1 if the canvas has already started.
  * \see wait(), stop(), close()
  */
int Canvas::start() {
    if (started) return -1;
    started = true;
  #ifdef __APPLE__
    pthread_create(&renderThread,NULL,startDrawing,(void*)this);
  #else
    renderThread = std::thread(Canvas::startDrawing, this);  // Spawn the rendering thread
  #endif
    return 0;
}

#ifdef __APPLE__
void* Canvas::startDrawing(void* cPtr) {
    Canvas* c = (Canvas*)cPtr;
    c->initGl();
    c->draw();
    c->isFinished = true;
    pthread_exit(NULL);
}
#else
void Canvas::startDrawing(Canvas *c) {
    c->initGl();
    c->draw();
    c->isFinished = true;
    glfwDestroyWindow(c->window);
    c->glDestroy();
}
#endif

 /*!
  * \brief Begins the process of closing the Canvas.
  * \details This function calls close() followed by wait() to gracefully close the Canvas
  *   at the earliest available opportunity.
  * \see start(), wait(), close()
  */
void Canvas::stop() {
    close();
    wait();
}

 /*!
  * \brief Stops recording the Canvas.
  * \details This function tells the Canvas to stop dumping images to the file system.
  */
void Canvas::stopRecording() {
    toRecord = 0;
}

 /*!
  * \brief Takes a screenshot.
  * \details This function saves a screenshot of the current Canvas to the working directory.
  * \details Images are saved as ImageXXXXXX.png, where XXXXXX is the current frame number.
  * \bug Multiple calls to this function in rapid succession render the FPS counter inaccurate.
  */
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

    // Recompute the camera matrices
    uniModel = glGetUniformLocation(program, "model");
    uniView = glGetUniformLocation(program, "view");
    uniProj = glGetUniformLocation(program, "proj");

    // Update the camera
    setupCamera();
}

 /*!
  * \brief Waits for the user to close the Canvas.
  * \details This function blocks the calling thread until the user closes the Canvas or until
  *   close() is called. This function has no effect if the Canvas has not started.
  * \return 0 if exit is successful, -1 if the Canvas has not started yet.
  * \see start(), end(), close(), stop().
  */
int Canvas::wait() {
  if (!started) return -1;  // If we haven't even started yet, return error code -1
  #ifdef __APPLE__
    while(!isFinished)
      sleepFor(0.1f);
  #endif
  renderThread.join();
  return 0;
}

//-----------------Unit testing-------------------------------------------------------
 /*!
  * \brief Runs unit tests for the Canvas.
  */
void Canvas::runTests() {
  TsglDebug("Testing Canvas class...");
  Canvas c1(0, 0, 500, 500, "", FRAME);
  c1.setBackgroundColor(WHITE);
  c1.start();
  tsglAssert(testFilledDraw(c1), "Unit test for filled draw failed!");
  tsglAssert(testLine(c1), "Unit test for line failed!");
  // tsglAssert(testAccessors(c1), "Unit test for accessors failed!");
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
  can.drawCircle(250, 250, 50, red, true);  //Draw filled shape
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
