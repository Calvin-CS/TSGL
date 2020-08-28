#include "CartesianBackground.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new CartesianBackground.
  * \details Creates a Background whose world dimensions don't have to match its pixel dimensions.
  *   \param width CartesianBackground's width in pixels.
  *   \param height CartesianBackground's height in pixels.
  *   \param xMin x value of the left side of the CartesianBackground in world coordinates.
  *   \param xMax x value of the right side of the CartesianBackground in world coordinates.
  *   \param yMin y value of the bottom side of the CartesianBackground in world coordinates.
  *   \param yMax y value of the top side of the CartesianBackground in world coordinates.
  *   \param c A ColorFloat for the CartesianBackground's original color.
  * \warning An invariant is held where if xMax or xMin isn't greater than its respective min then an error message is given.
  * \return A new CartesianBackground to which Drawables and Pixels can be drawn procedurally.
  */
CartesianBackground::CartesianBackground(GLint width, GLint height, Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax, const ColorFloat &clearColor) : Background(width, height, clearColor) 
{
    if (xMax < xMin || yMax < yMin) {
        TsglErr("Maximum values must be greater than minimum values.");
        return;
    }
    attribMutex.lock();
    myXMin = xMin;
    myXMax = xMax;
    myYMin = yMin;
    myYMax = yMax;
    myCartWidth = xMax - xMin;
    myCartHeight = yMax - yMin;
    pixelWidth = myCartWidth / (myWidth - 1);
    pixelHeight = myCartHeight / (myHeight - 1);  //Minor hacky fix
    attribMutex.unlock();
}

 /*!
  * \brief Draw the CartesianBackground.
  * \details This function actually draws the CartesianBackground to the Canvas.
  * \note On each draw cycle, first any Drawables that have been newly added to the CartesianBackground will be rendered, and then any new calls to drawPixel will be processed.
  */
void CartesianBackground::draw() {
    if (!complete) {
        TsglDebug("Shaders have not been defined for this background.");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);
    glEnable(GL_DEPTH_TEST);

    if (toClear) {
        attribMutex.lock();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        toClear = false;
        attribMutex.unlock();
    }

    drawableMutex.lock();
    for (unsigned int i = 0; i < myDrawables->size(); i++)
    {
        Drawable* d = (*myDrawables)[i];
        if(d->isProcessed()) {
            selectShaders(d->getShaderType());
            if (d->getShaderType() == SHAPE_SHADER_TYPE) {
                d->draw(shapeShader);
            } else if (d->getShaderType() == TEXTURE_SHADER_TYPE) {
                d->draw(textureShader);
            } else if (d->getShaderType() == TEXT_SHADER_TYPE) {
                d->draw(textShader);
            }
        }
    }
    myDrawables->clear();
    drawableMutex.unlock();

    // setting up texture shaders for both pixel drawing and post-blit render
    selectShaders(TEXTURE_SHADER_TYPE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3((float) -(myXMax + myXMin) / -2, (float) -(myYMax + myYMin) / -2, 0));
    model = glm::scale(model, glm::vec3((float)myCartWidth, (float)myCartHeight, 1));

    unsigned int modelLoc = glGetUniformLocation(textureShader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int alphaLoc = glGetUniformLocation(textureShader->ID, "alpha");
    glUniform1f(alphaLoc, 1.0f);

    // check for new pixels being drawn
    pixelBufferMutex.lock();
    if (newPixelsDrawn) {
        glBindTexture(GL_TEXTURE_2D, pixelTexture);

        // actually generate the texture + mipmaps
        glPixelStorei(GL_UNPACK_ALIGNMENT, 4);	
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, myWidth, myHeight, 0,
                    GL_RGBA, GL_UNSIGNED_BYTE, pixelTextureBuffer);
        glGenerateMipmap(GL_TEXTURE_2D);

        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, vertices, GL_DYNAMIC_DRAW);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        for (int i = 0; i < myWidth * myHeight * 4; ++i) {
            pixelTextureBuffer[i] = 0;
        }
        newPixelsDrawn = false;
    }
    pixelBufferMutex.unlock();
    
    // blit MSAA framebuffer to non-MSAA framebuffer's texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, myWidth, myHeight, 0, 0, myWidth, myHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

    model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3((float)myCartWidth, (float)myCartHeight, 1));

    modelLoc = glGetUniformLocation(textureShader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glm::mat4 view          = glm::mat4(1.0f);
    view  = glm::translate(view, glm::vec3(0, 0, -(((float)myCartHeight / 2) / tan(glm::pi<float>()/6))));

    glUniformMatrix4fv(glGetUniformLocation(textureShader->ID, "view"), 1, GL_FALSE, &view[0][0]);

    // render non-MSAA framebuffer's texture to default framebuffer
    glBindTexture(GL_TEXTURE_2D,intermediateTexture);
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    /* next two lines are very essential */
    glBufferData(GL_ARRAY_BUFFER,30*sizeof(float),vertices,GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES,0,6);
    glEnable(GL_DEPTH_TEST);

    glViewport(myXMin,myYMin,myCartWidth,myCartHeight);
    readPixelMutex.lock();
    glReadPixels(0, 0, myWidthPadded, myHeight, GL_RGB, GL_UNSIGNED_BYTE, readPixelBuffer);
    readPixelMutex.unlock();
}

 /*!
  * \brief Draws axes on the Cartesian Background.
  * \details This function draws axes (with tick marks) on the CartesianBackground, centered at the
  *  given (Cartesian) coordinates
  *    \param originX The horizontal location of the y-axis line.
  *    \param originY The vertical location of the x-axis line.
  *    \param spacingX The distance between marks on the x-axis.
  *    \param spacingY The distance between marks on the y-axis.
  */
void CartesianBackground::drawAxes(Decimal originX, Decimal originY, Decimal spacingX, Decimal spacingY) {
    drawLine(myXMax, originY, 0, myXMin, originY, 0, 0,0,0, BLACK);  // Make the two axes
    drawLine(originX, myYMax, 0, originX, myYMin, 0, 0,0,0, BLACK);

    if (spacingX != 0.0) {
        if (spacingX < 0.0) spacingX = -spacingX;

        for (Decimal x_ = originX + spacingX; x_ < myXMax; x_ += spacingX) {
            drawLine(x_, originY + 8 * pixelHeight, 0, x_, originY - 8 * pixelHeight, 0, 0,0,0, BLACK);
        }
        for (Decimal x_ = originX - spacingX; x_ > myXMin; x_ -= spacingX) {
            drawLine(x_, originY + 8 * pixelHeight, 0, x_, originY - 8 * pixelHeight, 0, 0,0,0, BLACK);
        }
    }
    if (spacingY != 0.0) {
        if (spacingY < 0.0) spacingY = -spacingY;

        for (Decimal y_ = originY + spacingY; y_ < myYMax; y_ += spacingY) {
            drawLine(originX + 8 * pixelWidth, y_, 0, originX - 8 * pixelWidth, y_, 0, 0,0,0, BLACK);
        }
        for (Decimal y_ = originY - spacingY; y_ > myYMin; y_ -= spacingY) {
            drawLine(originX + 8 * pixelWidth, y_, 0, originX - 8 * pixelWidth, y_, 0, 0,0,0, BLACK);
        }
    }
}

 /*!
  * \brief Plots a function on the screen.
  * \details This function receives a TSGL Function instance as a parameter and plots the function on the CartesianCanvas.
  *   \param function Reference to the Function to plot.
  *   \param color The color of the vertices of the plotted function (set to BLACK by default).
  */
void CartesianBackground::drawFunction(const Function &function, ColorFloat color) {
    int index = 0;
    int values = (int) ceil((myXMax - myXMin) / pixelWidth);
    float vertices[values*3];
    Decimal y;
    for (Decimal x = myXMin; x < myXMax; x += pixelWidth) {
        y = function.valueAt(x);
        vertices[index] = x;
        vertices[index+1] = y;
        vertices[index+2] = 0; 
        index += 3;
    }
    drawPolyline(0,0,0,values,vertices,0,0,0,color);
}

 /*!
  * \brief Plots a function on the screen.
  * \details This function receives a pointer to a function method as a parameter and plots the function on
  *   the CartesianCanvas.
  *   \param function Pointer to the function-drawing method to plot.
  *   \param color The color of the vertices of the plotted function (set to BLACK by default).
  * \note <code>function</code> must receive exactly one Decimal x parameter, and return a Decimal y parameter.
  */
void CartesianBackground::drawFunction(functionPointer &function, ColorFloat color) {
  drawPartialFunction(function,myXMin,myXMax,color);
}

 /*!
  * \brief Plots part of a function on the screen.
  * \details This function receives a pointer to a function method as a parameter and plots the function on
  *   the CartesianCanvas between the specified minimum and maximum coordinates.
  *   \param function Pointer to the function-drawing method to plot.
  *   \param min Minimum x value to evaluate and plot
  *   \param max Maximum x value to evaluate and plot
  *   \param color The color of the vertices of the plotted function (set to BLACK by default).
  * \note <code>function</code> must receive exactly one Decimal x parameter, and return a Decimal y parameter.
  */
void CartesianBackground::drawPartialFunction(functionPointer &function, Decimal min, Decimal max, ColorFloat color) {
    int index = 0;
    int values = (int) ceil((max - min) / pixelWidth);
    float vertices[values*3];
    Decimal y;
    for (Decimal x = min; x < max; x += pixelWidth) {
        y = (function)(x);
        vertices[index] = x;
        vertices[index+1] = y;
        vertices[index+2] = 0; 
        index += 3;
    }
    drawPolyline(0,0,0,values,vertices,0,0,0,color);
}

 /*!
  * \brief Draws a single pixel, specified in x,y format, at the given Cartesian coordinates.
  * \details This function alters the value at the specified x, y offset within the Background's buffer variable.
  * \note x and y must be given in world (Cartesian coordinates).
  *   \param x The Cartesian x-position of the pixel.
  *   \param y The Cartesian y-position of the pixel.
  *   \param color The color of the point.
  */
void CartesianBackground::drawPixel(float x, float y, ColorInt c) {
    attribMutex.lock();
    float screenX = (x - myXMin - myCartWidth/2) * myWidth / myCartWidth;
    float screenY = (y - myYMin - myCartHeight/2) * myHeight / myCartHeight;
    attribMutex.unlock();
    Background::drawPixel(screenX, screenY, c);
}

 /*!
  * \brief Gets the color of the pixel drawn on the current Background at the given x and y Cartesian coordinates.
  * \note x and y must be given in world (Cartesian coordinates).
  *      \param x The Cartesian x-position of the pixel to grab.
  *      \param y The Cartesian y-position of the pixel to grab.
  * \return A ColorInt containing the color of the pixel at (x,y).
  */
ColorInt CartesianBackground::getPixel(float x, float y) {
    attribMutex.lock();
    float screenX = (x - myXMin - myCartWidth/2) * myWidth / myCartWidth;
    float screenY = (y - myYMin - myCartHeight/2) * myHeight / myCartHeight;
    attribMutex.unlock();
    return Background::getPixel(screenX, screenY);
}

/*! \brief Activates the corresponding Shader for a given Drawable.
 *  \param sType Unsigned int with a corresponding value for each type of Shader.
 */
void CartesianBackground::selectShaders(unsigned int sType) {
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
        aspect = glm::scale(aspect, glm::vec3((myCartWidth/myCartHeight) / ((float) myWidth/myHeight), 1.0, 1.0));
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
     
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)myCartWidth/(float)myCartHeight, 0.1f, 1000.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    view  = glm::translate(view, glm::vec3((float) (myXMax + myXMin) / -2, (float) (myYMax + myYMin) / -2, -(((float)myCartHeight / 2) / tan(glm::pi<float>()/6))));
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(program->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
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
void CartesianBackground::zoom(Decimal x, Decimal y, Decimal scale) {
    Decimal newWidth = myCartWidth * scale;
    Decimal newHeight = myCartHeight * scale;
    myXMin = x - .5 * newWidth;
    myXMax = x + .5 * newWidth;
    myYMin = y - .5 * newHeight;
    myYMax = y + .5 * newHeight;
    myCartWidth = myXMax - myXMin;
    myCartHeight = myYMax - myYMin;
    pixelWidth = myCartWidth / (myWidth - 1);
    pixelHeight = myCartHeight / (myHeight - 1);  //Minor hacky fix
}

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
void CartesianBackground::zoom(Decimal x1, Decimal y1, Decimal x2, Decimal y2) {
    Decimal scale = (std::abs(x2 - x1) / myCartWidth + std::abs(y2 - y1) / myCartHeight) / 2.0;
    zoom((x2 + x1) / 2, (y2 + y1) / 2, scale);
}

}