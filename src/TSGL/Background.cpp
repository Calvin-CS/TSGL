#include "Background.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Background.
  * \details Explicit constructor for a Background object.
  *   \param x The x coordinate of the center of the Background.
  *   \param y The y coordinate of the center of the Background.
  *   \param z The z coordinate of the center of the Background.
  *   \param width Background's width in pixels.
  *   \param yaw The Background's yaw, in degrees.
  *   \param pitch The Background's pitch, in degrees.
  *   \param roll The Background's roll, in degrees.
  *   \param c A ColorFloat for the Background's original color.
  * \warning An invariant is held where if width or height isn't positive then an error message is given.
  * \return A new Background with a buffer for storing the specified numbered of vertices.
  */
Background::Background(GLint width, GLint height, const ColorFloat &clearColor)  {
    if (width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Background with non-positive width or height.");
    }
    attribMutex.lock();
    myWidth = width;
    myHeight = height;
    myDrawables = new Array<Drawable*>(myWidth * myHeight * 2);
    baseColor = clearColor;
    toClear = false;
    complete = false;
    newPixelsDrawn = false;
    readPixelBuffer = new uint8_t[myWidth * myHeight * 3];
    for (int i = 0; i < myWidth * myHeight * 3; ++i) {
      readPixelBuffer[i] = 0;
    }
    pixelTextureBuffer = new uint8_t[myWidth * myHeight * 4];
    for (int i = 0; i < myWidth * myHeight * 4; ++i) {
      pixelTextureBuffer[i] = 0;
    }

    vertices = new GLfloat[30];
    vertices[0]  = vertices[1] = vertices[6] = vertices[10] = vertices[16]  = vertices[20] = -0.5; // x + y
    vertices[5] = vertices[11] = vertices[15] = vertices[21] = vertices[25] = vertices[26] = 0.5; // x + y
    vertices[2] = vertices[7] = vertices[12] = vertices[17] = vertices[22] = vertices[27] = 0; // z
    vertices[3]  = vertices[13]  = vertices[14] = vertices[23] = vertices[24] = vertices[29] = 0.0; // texture coord x + y
    vertices[4] = vertices[8] = vertices[9] = vertices[18] = vertices[19] = vertices[28] = 1.0; // texture coord x + y
    attribMutex.unlock();
}

 /*!
  * \brief Draw the Background.
  * \details This function actually draws the Background to the Canvas.
  * \note On each draw cycle, first any Drawables that have been newly drawn to the Background will be rendered, and then any new calls to drawPixel will be processed.
  */
void Background::draw() {
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

    // setting up texture shaders for both pixel drawing and post-blit render
    selectShaders(TEXTURE_SHADER_TYPE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(myWidth, myHeight, 1));

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

        newPixelsDrawn = false;
        for (int i = 0; i < myWidth * myHeight * 4; ++i) {
            pixelTextureBuffer[i] = 0;
        }
    }
    pixelBufferMutex.unlock();
    
    // blit MSAA framebuffer to non-MSAA framebuffer's texture
    glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, intermediateFBO);
    glBlitFramebuffer(0, 0, myWidth, myHeight, 0, 0, myWidth, myHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    glDisable(GL_DEPTH_TEST);

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

    glViewport(0,0,myWidth,myHeight);
    glReadPixels(0, 0, myWidth, myHeight, GL_RGB, GL_UNSIGNED_BYTE, readPixelBuffer);

    myDrawables->clear();
}

 /*!
  * \brief Gets the color of the pixel drawn on the current Background at the given row and column.
  * \note (0,0) signifies the <b>top-left</b> of the Background's texture.
  *      \param row The row (y-position) of the pixel to grab.
  *      \param col The column (x-position) of the pixel to grab.
  * \return A ColorInt containing the color of the pixel at (col,row).
  */
ColorInt Background::getPixel(int row, int col) {
    int yy = (myHeight-1) - row;
    int off = 3 * (yy * myWidth + col);
    return ColorInt(readPixelBuffer[off], readPixelBuffer[off + 1], readPixelBuffer[off + 2], 255);
}

 /*!
  * \brief Draws a single pixel, specified in row,column format.
  * \details This function alters the value at the specified row, column offset within the Background's buffer variable.
  * \note (0,0) signifies the <b>top-left</b> of the Background.
  *   \param row The row (y-position) of the pixel.
  *   \param col The column (x-position) of the pixel.
  *   \param color The color of the point.
  */
void Background::drawPixel(int x, int y, ColorInt c) {
    if (abs(x) > myWidth / 2 || abs(y) > myHeight / 2) {
        TsglErr("Pixel x and y coordinates must be within Background dimensions.");
        return;
    }
    pixelBufferMutex.lock();
    x += myWidth / 2;
    y += myHeight / 2;
    pixelTextureBuffer[(y * myWidth + x) * 4] = c.R;
    pixelTextureBuffer[(y * myWidth + x) * 4 + 1] = c.G;
    pixelTextureBuffer[(y * myWidth + x) * 4 + 2] = c.B;
    pixelTextureBuffer[(y * myWidth + x) * 4 + 3] = c.A;
    newPixelsDrawn = true;
    pixelBufferMutex.unlock();
}

void Background::selectShaders(unsigned int sType) {
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
    } else if (sType == TEXTURE_SHADER_TYPE) {
        program = textureShader;
        GLint posAttrib = glGetAttribLocation(textureShader->ID, "aPos");
        glEnableVertexAttribArray(posAttrib);
        glVertexAttribPointer(posAttrib, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        // texture coord attribute
        GLint texAttrib = glGetAttribLocation(textureShader->ID, "aTexCoord");
        glEnableVertexAttribArray(texAttrib);
        glVertexAttribPointer(texAttrib, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }
 
    program->use();
    
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)myWidth/(float)myHeight, 0.1f, 1000.0f);
    glm::mat4 view          = glm::mat4(1.0f);
    view  = glm::translate(view, glm::vec3(0.0f, 0.0f, -((myHeight / 2) / tan(glm::pi<float>()/6))));
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(program->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

void Background::init(Shader * shapeS, Shader * textS, Shader * textureS, GLFWwindow * window) {
    glfwMakeContextCurrent(window);
    // configure MSAA framebuffer
    // --------------------------
    glGenFramebuffers(1, &multisampledFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);
    // create a multisampled color attachment texture
    glGenTextures(1, &multisampledTexture);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture);
    glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, 4, GL_RGB, myWidth, myHeight, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, multisampledTexture, 0);
    // create multisampled renderbuffer object
    glGenRenderbuffers(1, &RBO);
    glBindRenderbuffer(GL_RENDERBUFFER, RBO);
    glRenderbufferStorageMultisample(GL_RENDERBUFFER, 4, GL_DEPTH24_STENCIL8, myWidth, myHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RBO); // now actually attach it

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      TsglErr("FRAMEBUFFER CREATION FAILED");

    glClearColor(baseColor.R, baseColor.G, baseColor.B, baseColor.A);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Create a second framebuffer, non-MSAA
    // --------------------------
    intermediateFBO = 0;
    glGenFramebuffers(1, &intermediateFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, intermediateFBO);
    glGenTextures(1, &intermediateTexture);
    glBindTexture(GL_TEXTURE_2D, intermediateTexture);
    glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, myWidth, myHeight, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D,intermediateTexture, 0);

    // Always check that our framebuffer is ok
    if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
      TsglErr("FRAMEBUFFER CREATION FAILED");

    glClear(GL_COLOR_BUFFER_BIT);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // generate a texture for the pixels
    // --------------------------
    glGenTextures(1, &pixelTexture);
    glBindTexture(GL_TEXTURE_2D, pixelTexture);
    // Set texture parameters for wrapping.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Set texture parameters for filtering.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);

    attribMutex.lock();
    shapeShader = shapeS;
    textShader = textS;
    textureShader = textureS;    
    complete = true;
    attribMutex.unlock();
    glfwMakeContextCurrent(0);
}

void Background::drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow) {
    Arrow * a = new Arrow(x,y,z,length,width,yaw,pitch,roll,color,doubleArrow);
    attribMutex.lock();
    myDrawables->push(a);
    attribMutex.unlock();
}

void Background::drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow) {
    Arrow * a = new Arrow(x,y,z,length,width,yaw,pitch,roll,color,doubleArrow);
    attribMutex.lock();
    myDrawables->push(a);
    attribMutex.unlock();
}

void Background::drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color) {
    Circle * c = new Circle(x,y,z,radius,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color[]) {
    Circle * c = new Circle(x,y,z,radius,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color) {
    ConcavePolygon * c = new ConcavePolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]) {
    ConcavePolygon * c = new ConcavePolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color) {
    ConvexPolygon * c = new ConvexPolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]) {
    ConvexPolygon * c = new ConvexPolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(c);
    attribMutex.unlock();
}

void Background::drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color) {
    Ellipse * e = new Ellipse(x,y,z,xRadius,yRadius,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(e);
    attribMutex.unlock();
}

void Background::drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color[]) {
    Ellipse * e = new Ellipse(x,y,z,xRadius,yRadius,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(e);
    attribMutex.unlock();
}

void Background::drawImage(float x, float y, float z, std::string filename, float width, float height, float yaw, float pitch, float roll, float alpha) {
    Image * i = new Image(x,y,z,filename,width,height,yaw,pitch,roll,alpha);
    attribMutex.lock();
    myDrawables->push(i);
    attribMutex.unlock();
}

void Background::drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color) {
    Line * l = new Line(x,y,z,length,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(l);
    attribMutex.unlock();
}

void Background::drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color[]) {
    Line * l = new Line(x,y,z,length,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(l);
    attribMutex.unlock();
}

void Background::drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color) {
    Polyline * p = new Polyline(x,y,z,numVertices,lineVertices,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(p);
    attribMutex.unlock();
}

void Background::drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]) {
    Polyline * p = new Polyline(x,y,z,numVertices,lineVertices,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(p);
    attribMutex.unlock();
}

void Background::drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color) {
    Rectangle * r = new Rectangle(x,y,z,width,height,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(r);
    attribMutex.unlock();
}

void Background::drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color[]) {
    Rectangle * r = new Rectangle(x,y,z,width,height,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(r);
    attribMutex.unlock();
}

void Background::drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color) {
    RegularPolygon * r = new RegularPolygon(x,y,z,radius,sides,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(r);
    attribMutex.unlock();
}

void Background::drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color[]) {
    RegularPolygon * r = new RegularPolygon(x,y,z,radius,sides,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(r);
    attribMutex.unlock();
}

void Background::drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color) {
    Square * s = new Square(x,y,z,sidelength,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color[]) {
    Square * s = new Square(x,y,z,sidelength,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color, bool ninja) {
    Star * s = new Star(x,y,z,radius,points,yaw,pitch,roll,color,ninja);
    attribMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    attribMutex.unlock();   
}

void Background::drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color[], bool ninja) {
    Star * s = new Star(x,y,z,radius,points,yaw,pitch,roll,color,ninja);
    attribMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    attribMutex.unlock();   
}

void Background::drawText(float x, float y, float z, std::wstring text, std::string fontFilename, unsigned int fontsize, float yaw, float pitch, float roll, const ColorFloat &color) {
    Text * t = new Text(x,y,z,text,fontFilename,fontsize,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color) {
    Triangle * t = new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color[]) {
    Triangle * t = new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color) {
    TriangleStrip * t = new TriangleStrip(centerX,centerY,centerZ,numVertices,x,y,z,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color[]) {
    TriangleStrip * t = new TriangleStrip(centerX,centerY,centerZ,numVertices,x,y,z,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    attribMutex.unlock();
}

void Background::setClearColor(ColorFloat c) {
    attribMutex.lock();
    baseColor = c;
    glClearColor(baseColor.R, baseColor.G, baseColor.B, baseColor.A);
    attribMutex.unlock();
}

/*!
* \brief Destructor for the Background.
*/
Background::~Background() {
    myDrawables->clear();
    delete [] readPixelBuffer;
    delete [] pixelTextureBuffer;
    delete [] vertices;
    delete myDrawables;
    glDeleteTextures(1, &pixelTexture);
    glDeleteTextures(1, &intermediateFBO);
    glDeleteFramebuffers(1, &intermediateTexture);
    glDeleteTextures(1, &multisampledTexture);
    glDeleteFramebuffers(1, &multisampledFBO);
}

}