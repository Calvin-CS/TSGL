#include "CartesianCanvas.h"

namespace tsgl {

 /*!
  * \brief Default CartesianCanvas constructor method.
  * \details This is the default constructor for the CartesianCanvas class.
  *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
  *     A value less than or equal to 0 sets it to automatic.
  * \return A new CartesianCanvas, stretching from -400 to +400 on the x axis and
  *   -300 to +300 on the y axis, in the middle of the screen with no title.
  *   The created CartesianCanvas will take up approximately 90% of the monitor's height, and will
  *   have a 4:3 aspect ratio.
  */
CartesianCanvas::CartesianCanvas(double timerLength)
    : Canvas(timerLength, new CartesianBackground(1.2*Canvas::getDisplayHeight(), 0.7*1.2*Canvas::getDisplayHeight(), -400, -300, 400, 300)) {
    recomputeDimensions(-400, -300, 400, 300);
}

 /*!
  * \brief Explicit CartesianCanvas constructor method.
  * \details This is an explicit constructor for the CartesianCanvas class.
  *   \param x The x position of the CartesianCanvas window.
  *   \param y The y position of the CartesianCanvas window.
  *   \param width The x dimension of the CartesianCanvas window.
  *   \param height The y dimension of the CartesianCanvas window.
  *   \param xMin The Cartesian coordinates of the CartesianCanvas's left bound.
  *   \param yMin The Cartesian coordinates of the CartesianCanvas's bottom bound.
  *   \param xMax The Cartesian coordinates of the CartesianCanvas's right bound.
  *   \param yMax The Cartesian coordinates of the CartesianCanvas's top bound.
  *   \param title The title of the window.
  *   \param timerLength The minimum number of seconds between draw cycles for the Canvas.
  *     A value less than or equal to 0 sets it to automatic.
  * \return A new CartesianCanvas with the specified position, dimensions, scaling, title,
  *   and timer length.
  */
CartesianCanvas::CartesianCanvas(int x, int y, int width, int height, Decimal xMin, Decimal yMin, Decimal xMax,
                                 Decimal yMax, std::string t, ColorFloat backgroundColor, double timerLength)
    : Canvas(x, y, width, height, t, backgroundColor, new CartesianBackground(width, height, xMin, yMin, xMax, yMax, backgroundColor), timerLength) {
    recomputeDimensions(xMin, yMin, xMax, yMax);
}

 /*!
  * \brief Translates Cartesian coordinates into window coordinates.
  * \details getCartesianCoordinates() takes a pair of on-screen coordinates and translates them to Cartesian
  *  coordinates.
  *    \param screenX The window's x coordinate.
  *    \param screenY The window's y coordinate.
  *    \param cartX A reference variable to be filled with screenX's Cartesian position.
  *    \param cartY A reference variable to be filled with screenY's Cartesian position.
  */
void CartesianCanvas::getCartesianCoordinates(int screenX, int screenY, Decimal &cartX, Decimal &cartY) {
    cartX = (screenX * cartWidth) / getWindowWidth() + minX;
    cartY = minY - (screenY - getWindowHeight()) * cartHeight / getWindowHeight();
}

 /*!
  * \brief Translates window coordinates into Cartesian coordinates.
  * \details getScreenCoordinates() takes a pair of Cartesian coordinates and translates them to on-screen
  *   coordinates.
  *   \param cartX The Cartesian x coordinate.
  *   \param cartY The Cartesian y coordinate.
  *   \param screenX A reference variable to be filled with cartX's window position.
  *   \param screenY A reference variable to be filled with cartY's window position.
  */
void CartesianCanvas::getScreenCoordinates(Decimal cartX, Decimal cartY, int &screenX, int &screenY) {
    screenX = round((cartX - minX) / pixelWidth);
    if (atiCard)
      screenY = getWindowHeight() - round((cartY - minY) / pixelHeight + pixelHeight*0.5f);
    else
      screenY = getWindowHeight() - 1 - round((cartY - minY) / pixelHeight + pixelHeight*0.5f);
}

 /*!
  * \brief Accessor for the mouse's x-position in Cartesian coordinates.
  * \return The Cartesian x coordinates of the mouse on the CartesianCanvas.
  */
float CartesianCanvas::getMouseX() {
    float mx = (mouseX * cartWidth) / winWidth + minX;
    return mx;
}

 /*!
  * \brief Accessor for the mouse's y-position in Cartesian coordinates.
  * \return The Cartesian y coordinates of the mouse on the CartesianCanvas.
  */
float CartesianCanvas::getMouseY() {
    float my = -((mouseY * cartHeight) / winHeight + minY);
    return my;
}

 /*!
  * \brief Recomputes the CartesianCanvas's bounds.
  * \details This function recomputes the size variables of CartesianCanvas according to new bounds.
  *   \param xMin A real number corresponding to the new left edge of the CartesianCanvas.
  *   \param YMin A real number corresponding to the new bottom edge of the CartesianCanvas.
  *   \param xMax A real number corresponding to the new right edge of the CartesianCanvas.
  *   \param xMax A real number corresponding to the new top edge of the CartesianCanvas.
  */
void CartesianCanvas::recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax) {
    minX = xMin;
    minY = yMin;
    maxX = xMax;
    maxY = yMax;
    cartWidth = maxX - minX;
    cartHeight = maxY - minY;
    pixelWidth = cartWidth / (getWindowWidth() - 1);
    pixelHeight = cartHeight / (getWindowHeight() - 1);  //Minor hacky fix
    backgroundMutex.lock();
    backgroundMutex.unlock();
}

/*! \brief Activates the corresponding Shader for a given Drawable.
 *  \param sType Unsigned int with a corresponding value for each type of Shader.
 */
void CartesianCanvas::selectShaders(unsigned int sType) {
    Shader * program = 0;
    if (sType == TEXT_SHADER_TYPE) {
        program = textShader;
        // position attribute
        GLint posAttrib = glGetAttribLocation(textShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint texAttrib = glGetAttribLocation(textShader->ID, "aTexCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
        unsigned int aspectLoc = glGetUniformLocation(program->ID, "aspect");
        glm::mat4 aspect = glm::mat4(1.0f);
        aspect = glm::scale(aspect, glm::vec3( (cartWidth/cartHeight) / ((float) winWidth/winHeight), 1.0f, 1.0f));
        glUniformMatrix4fv(aspectLoc, 1, GL_FALSE, glm::value_ptr(aspect));
    } else if (sType == SHAPE_SHADER_TYPE)  {
        program = shapeShader;
        // position attribute
        GLint posAttrib = glGetAttribLocation(shapeShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint colAttrib = glGetAttribLocation(shapeShader->ID, "aColor");
        glEnableVertexAttribArray(colAttrib);
        glVertexAttribPointer(colAttrib, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
    } else if (sType == TEXTURE_SHADER_TYPE) {
        program = textureShader;
        GLint posAttrib = glGetAttribLocation(textureShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint texAttrib = glGetAttribLocation(textureShader->ID, "aTexCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        program->use();
    }
    
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)cartWidth/(float)cartHeight, 0.1f, 1000.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    view  = glm::translate(view, glm::vec3((float) (maxX + minX) / -2, (float) (maxY + minY) / -2, -(((float)cartHeight / 2) / tan(glm::pi<float>()/6))));
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(program->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

 /*!
  * \brief Start the CartesianCanvas, run a function on it, and wait for the user to close it
  * \details This function binds another function to the current CartesianCanvas, waits until that function is
  *   complete, and waits for the user to close the CartesianCanvas.  This function effectively calls start(),
  *   <code>myFunction</code>(), and wait() in sequence.
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&) ) {
  start(); myFunction(*this); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int), int i) {
  start(); myFunction(*this, i); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param u An unsigned integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, unsigned), unsigned u) {
  start(); myFunction(*this, u); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i1 An integer argument to myFunction
  * \param i2 An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int, int), int i1, int i2) {
  start(); myFunction(*this, i1, i2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param u1 An unsigned integer argument to myFunction
  * \param u2 An unsigned integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, unsigned, unsigned), unsigned u1, unsigned u2) {
  start(); myFunction(*this, u1, u2); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param s A string argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, std::string),std::string s) {
  start(); myFunction(*this, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param i An integer argument to myFunction
  * \param s A string argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, int, std::string), int i, std::string s) {
  start(); myFunction(*this, i, s); wait();
}

 /*!
  * \brief Overload for run()
  * \param myFunction The function to run on the CartesianCanvas. Must take exactly one parameter of type
  *   CartesianCanvas&, which is a reference to the CartesianCanvas to render to.
  * \param s A string argument to myFunction
  * \param i An integer argument to myFunction
  */
void CartesianCanvas::run(void (*myFunction)(CartesianCanvas&, std::string, int), std::string s, int i) {
  start(); myFunction(*this, s, i); wait();
}

 /*!
  * \brief Zoom the CartesianCanvas with a given center.
  * \details This function will re-center the CartesianCanvas at the given coordinates, then zoom with
  *   respect to the given scale.
  *   \param x The coordinate to re-center the screen on.
  *   \param y The coordinate to re-center the screen on.
  *   \param scale The zoom scale compared to the original. Less than 1 zooms in, greater than 1 zooms out.
  * \note This function will automatically maintain the current aspect ratio.
  */
// void CartesianCanvas::zoom(Decimal x, Decimal y, Decimal scale) {
//     Decimal newWidth = cartWidth * scale;
//     Decimal newHeight = cartHeight * scale;
//     recomputeDimensions(x - .5 * newWidth, y - .5 * newHeight, x + .5 * newWidth, y + .5 * newHeight);
// }

 /*!
  * \brief Zoom the CartesianCanvas with the given bounding (Cartesian) coordinates.
  * \details This function will zoom the CartesianCanvas with respect to the given bounding coordinates.
  *   \param x1 The left Cartesian bound.
  *   \param y1 The bottom Cartesian bound.
  *   \param x2 The right Cartesian bound.
  *   \param y2 The top Cartesian bound.
  * \note Setting the right bound lower than the left bound or the top lower than the bottom will just
  *   swap the variables.
  * \warning This function will *NOT* automatically maintain the previous aspect ratio.
  * \warning Change the aspect ratio on-the-fly only with caution.
  */
// void CartesianCanvas::zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
//     Decimal scale = (std::abs(x2 - x1) / cartWidth + std::abs(y2 - y1) / cartHeight) / 2.0;
//     zoom((x2 + x1) / 2, (y2 + y1) / 2, scale);
// }

//-----------------------Unit testing-------------------------------------------------
 /*!
  * \brief Runs the Unit tests for CartesianCanvas.
  */
// void CartesianCanvas::runTests() {
//   TsglDebug("Testing CartesianCanvas class...");
//   CartesianCanvas c1(0.0f);
//   c1.setBackgroundColor(WHITE);
//   c1.start();

//   std::this_thread::sleep_for(std::chrono::seconds(1));
//   tsglAssert(testZoom(c1), "Unit test for zoom() functions failed!");
//   tsglAssert(testRecomputeDimensions(c1), "Unit test for recomputing dimensions failed!");
//   c1.stop();

//   CartesianCanvas c2(-1, -1, 800, 600, -1, -1, 3, 2,"");
//   c2.setBackgroundColor(WHITE);
//   c2.start();
//   tsglAssert(testDraw(c2), "Unit test for drawing functions failed!");
//   c2.wait();

//   TsglDebug("Unit tests for CartesianCanvas complete.");
// }

// bool CartesianCanvas::testDraw(CartesianCanvas& can) {
//   int passed = 0;
//   int failed = 0;

//   float pw = can.getPixelWidth();
//   float ph = can.getPixelHeight();

//   //Test 1: Physical to Cartesian point mapping
//   can.drawPoint(-1.0f,-1.0f,BLACK); //outer bottomleft
//   can.drawPoint(3.0f,-1.0f,BLACK);  //outer bottomright
//   can.drawPoint(-1.0f,2.0f,BLACK);  //outer topleft
//   can.drawPoint(3.0f,2.0f,BLACK);   //outer topright
//   can.drawPoint(0.0f,0.0f,BLACK);   //1/4 over, 1/3 down (origin)

//   can.drawPoint(-1.0f+pw,-1.0f+ph,BLACK); //inner bottomleft
//   can.drawPoint(3.0f-pw,-1.0f+ph,BLACK);  //inner bottomright
//   can.drawPoint(-1.0f+pw,2.0f-ph,BLACK);  //inner topleft
//   can.drawPoint(3.0f-pw,2.0f-ph,BLACK);   //inner topright
//   can.sleepFor(1.0f);

//   if(can.getPoint(200,399).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, origin pixel for testDraw() failed!");
//   }
//   if(can.getPoint(0,0).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, outer topleft pixel for testDraw() failed!");
//   }
//   if(can.getPoint(799,0).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, outer topright pixel for testDraw() failed!");
//   }
//   if(can.getPoint(0,599).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, outer bottomleft pixel for testDraw() failed!");
//   }
//   if(can.getPoint(799,599).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, outer bottomright pixel for testDraw() failed!");
//   }

//   if(can.getPoint(1,1).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, inner topleft pixel for testDraw() failed!");
//   }
//   if(can.getPoint(798,1).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, inner topright pixel for testDraw() failed!");
//   }
//   if(can.getPoint(1,598).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, inner bottomleft pixel for testDraw() failed!");
//   }
//   if(can.getPoint(798,598).R == 0) {
//     passed++;
//   } else {
//     failed++;
//     TsglErr("Test 1, inner bottomright pixel for testDraw() failed!");
//   }

//   //Results:
//   if(failed == 0) {
//     TsglDebug("Unit test for drawing passed!");
//     return true;
//   } else {
//     TsglErr("This many passed for testDraw(): ");
//     std::cout << " " << passed << std::endl;
//     TsglErr("This many failed for testDraw(): ");
//     std::cout << " " << failed << std::endl;
//     return false;
//   }
// }

// bool CartesianCanvas::testZoom(CartesianCanvas& can) {
//     int passed = 0;
//     int failed = 0;
//     //Test 1: Zooming out
//     //Had to use round() because there was a floating-point error that
//     //propagated to the rest of the tests after the first two
//     can.zoom(0, 0, 1.5);
//     if(round(can.getCartWidth()) == 1200 && round(can.getCartHeight()) == 900) {
//       passed++;
//     } else {
//       failed++;
//       TsglErr("Test 1, Zooming out for testZoom() failed!");
//     }

//     //Test 2: Zooming in
//     can.zoom(0, 0, .5);
//     if(round(can.getCartWidth()) == 600 && round(can.getCartHeight()) == 450) {
//       passed++;
//     } else {
//       failed++;
//       TsglErr("Test 2, Zooming in for testZoom() failed!");
//     }

//     //Test 3: Zooming out/in on a different point

//     //Zooming out....
//     can.zoom(10, 10, 1.2);
//     if(round(can.getCartWidth()) == 720 && round(can.getCartHeight()) == 540) {
//       passed++;
//     } else {
//       failed++;
//       TsglErr("Test 3, Zooming out on a different point for testZoom() failed!");
//     }

//     //Zooming in....
//     can.zoom(15, 20, .9);
//     if(round(can.getCartWidth()) == 648 && round(can.getCartHeight()) == 486) {
//       passed++;
//     } else {
//       failed++;
//       TsglErr("Test 3, Zooming in on a different point for testZoom() failed!");
//     }

//     //Results:
//     if(passed == 4 && failed == 0) {
//       TsglDebug("Unit test for zooming in & out passed!");
//       return true;
//     } else {
//       TsglErr("This many passed for testZoom(): ");
//       std::cout << " " << passed << std::endl;
//       TsglErr("This many failed for testZoom(): ");
//       std::cout << " " << failed << std::endl;
//       return false;
//     }
// }

// bool CartesianCanvas::testRecomputeDimensions(CartesianCanvas& can) {
//    int passed = 0;
//    int failed = 0;
//    Decimal xMin, xMax;
//    Decimal yMin, yMax;
//    //Test 1: Positive values only (with 0.0)
//    xMin = 0.0;
//    xMax = 500.0;
//    yMin = 0.0;
//    yMax = 500.0;
//    can.recomputeDimensions(xMin, yMin, xMax, yMax);
//    if(can.getCartWidth() == 500.0 && can.getCartHeight() == 500.0) {
//      passed++;
//    } else {
//      failed++;
//      TsglErr("Test 1, Positive values only for testRecomputeDimensions() failed!");
//    }

//    //Test 2: Negative values included
//    xMin = xMax = yMin = yMax = 0.0;
//    xMin = -300.0;
//    xMax = 900.0;
//    yMin = -500.0;
//    yMax = 1000.0;
//    can.recomputeDimensions(xMin, yMin, xMax, yMax);

//    if(can.getCartWidth() == 1200.0 && can.getCartHeight() == 1500.0) {
//      passed++;
//    } else {
//      failed++;
//      TsglErr("Test 2, Negative values for testRecomputeDimensions() failed!");
//    }

//    //Test 3: Same as Test 2, but negative values are max
//    xMin = xMax = yMin = yMax = 0.0;
//    xMin = -900.0;
//    xMax = -100.0;
//    yMin = -800.0;
//    yMax = -50.0;
//    can.recomputeDimensions(xMin, yMin, xMax, yMax);

//    if(can.getCartWidth() == 800.0 && can.getCartHeight() == 750.0) {
//      passed++;
//    } else {
//      failed++;
//      TsglErr("Test 3, Max negative values for testRecomputeDimensions() failed!");
//    }

//    if(passed == 3 && failed == 0) {
//      TsglDebug("Unit test for recomputing dimensions passed!");
//      return true;
//    } else {
//      TsglErr("This many tests passed for testRecomputeDimensions(): ");
//      std::cout << " " << passed << std::endl;
//      TsglErr("This many tests failed for testRecomputeDimensions(): ");
//      std::cout << " " << failed << std::endl;
//      return false;
//    }
// }
//-----------------End Unit testing----------------------------------------------------
}
