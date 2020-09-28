#include "Background.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Background.
  * \details Explicit constructor for a Background object.
  *   \param width Background's width in pixels.
  *   \param height Background's height in pixels.
  *   \param c A ColorFloat for the Background's original color.
  * \warning An invariant is held where if width or height isn't positive then an error message is given.
  * \return A new Background to which Drawables and Pixels can be drawn procedurally.
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
    newPixelsDrawn = true;

    pixelTextureBuffer = new uint8_t[myWidth * myHeight * 4];
    for (int i = 0; i < myWidth * myHeight * 4; ++i) {
        pixelTextureBuffer[i] = 0;
    }
    pixelBufferMutex.unlock();

    myWorldZ = 4000;
    vertices = new GLfloat[30];
    vertices[0]  = vertices[11] = vertices[21] = vertices[10] = vertices[26]  = vertices[20] = -0.5 * ((myHeight / 2) / tan(glm::pi<float>()/6) + myWorldZ) / ((myHeight / 2) / tan(glm::pi<float>()/6)); // x + y
    vertices[5] = vertices[1] = vertices[15] = vertices[6] = vertices[25] = vertices[16] = 0.5 * ((myHeight / 2) / tan(glm::pi<float>()/6) + myWorldZ) / ((myHeight / 2) / tan(glm::pi<float>()/6)); // x + y
    vertices[2] = vertices[7] = vertices[12] = vertices[17] = vertices[22] = vertices[27] = -myWorldZ; // z
    vertices[3]  = vertices[13]  = vertices[14] = vertices[23] = vertices[24] = vertices[29] = 0.0; // texture coord x + y
    vertices[4] = vertices[8] = vertices[9] = vertices[18] = vertices[19] = vertices[28] = 1.0; // texture coord x + y
    attribMutex.unlock();
}

/*! \brief Assigns Drawable shaders and initializes framebuffers within the parameter window's context.
 *  \details Assigns shapeShader, textShader, textureShader to parameters and initializes two framebuffers: MSAA and non-MSAA.
 *  \param shapeS Pointer to a shader for Shape Drawables.
 *  \param textS Pointer to a shader for Text Drawables.
 *  \param textureS Pointer to a shader for texture rendering.
 *  \param window GLFWwindow * within whose context the framebuffers will be initialized.
 */
void Background::init(Shader * shapeS, Shader * textS, Shader * textureS, Camera * camera, GLFWwindow * window) {
    attribMutex.lock();
    glfwMakeContextCurrent(window);
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    int padwidth = framebufferWidth % 4;
    if (padwidth > 0)
       padwidth = 4-padwidth;
    framebufferWidthPadded = framebufferWidth + padwidth;
    readPixelMutex.lock();
    readPixelBuffer = new uint8_t[framebufferWidthPadded * framebufferHeight * 3];
    for (int i = 0; i < framebufferWidthPadded * framebufferHeight * 3; ++i) {
      readPixelBuffer[i] = 0;
    }
    readPixelMutex.unlock();
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

    shapeShader = shapeS;
    textShader = textS;
    textureShader = textureS;  
    myCamera = camera;  
    complete = true;
    glfwMakeContextCurrent(0);
    attribMutex.unlock();
}

 /*!
  * \brief Draw the Background.
  * \details This function actually draws the Background to the Canvas.
  * \note On each draw cycle, first any Drawables that have been newly added to the Background will be rendered, and then any new calls to drawPixel will be processed.
  */
void Background::draw() {
    if (!complete) {
        TsglDebug("Shaders have not been defined for this background.");
        return;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, multisampledFBO);
    glEnable(GL_DEPTH_TEST);

    attribMutex.lock();
    if (toClear) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        toClear = false;
    }
    attribMutex.unlock();

    glViewport(0,0,myWidth,myHeight);

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
    model = glm::scale(model, glm::vec3(myWidth, myHeight, 1));

    unsigned int modelLoc = glGetUniformLocation(textureShader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    unsigned int alphaLoc = glGetUniformLocation(textureShader->ID, "alpha");
    glUniform1f(alphaLoc, 1.0f);

    glClear(GL_DEPTH_BUFFER_BIT);

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

    glViewport(0,0,framebufferWidth,framebufferHeight);

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

    // glViewport(0,0,myWidth,myHeight);
    readPixelMutex.lock();
    glReadPixels(0, 0, framebufferWidthPadded, framebufferHeight, GL_RGB, GL_UNSIGNED_BYTE, readPixelBuffer);
    readPixelMutex.unlock();
}

/*! \brief Activates the corresponding Shader for a given Drawable.
 *  \param sType Unsigned int with a corresponding value for each type of Shader.
 */
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
        program->use();
        unsigned int aspectLoc = glGetUniformLocation(program->ID, "aspect");
        glUniformMatrix4fv(aspectLoc, 1, GL_FALSE, glm::value_ptr(glm::mat4(1.0f)));
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
     
    glm::mat4 projection = glm::perspective(glm::radians(60.0f), (float)myWidth/(float)myHeight, 0.1f, 5000.0f);
    glm::mat4 view = myCamera->getViewMatrix();
    glm::mat4 model = glm::mat4(1.0f);

    glUniformMatrix4fv(glGetUniformLocation(program->ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(program->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
}

/*!\brief Procedurally draws an Arrow to the Background.
 * \details Initializes a new Arrow based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Arrow's center location.
 * \param y The y coordinate of the Arrow's center location.
 * \param z The z coordinate of the Arrow's center location.
 * \param length Length of the Arrow.
 * \param width Width of the Arrow.
 * \param yaw The Arrow's yaw rotation.
 * \param pitch The Arrow's pitch rotation.
 * \param roll The Arrow's roll rotation.
 * \param color ColorFloat for the Arrow's vertices.
 * \param outlined Boolean indicating if the Arrow should be outlined or not, defaulting to not.
 */
void Background::drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow, bool outlined) {
    Arrow * a = new Arrow(x,y,z,length,width,yaw,pitch,roll,color,doubleArrow);
    a->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(a);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws an Arrow to the Background.
 * \details Initializes a new Arrow based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Arrow's center location.
 * \param y The y coordinate of the Arrow's center location.
 * \param z The z coordinate of the Arrow's center location.
 * \param length Length of the Arrow.
 * \param width Width of the Arrow.
 * \param yaw The Arrow's yaw rotation.
 * \param pitch The Arrow's pitch rotation.
 * \param roll The Arrow's roll rotation.
 * \param color Array of ColorFloats for the Arrow's vertices.
 * \param outlined Boolean indicating if the Arrow should be outlined or not, defaulting to not.
 */
void Background::drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow, bool outlined) {
    Arrow * a = new Arrow(x,y,z,length,width,yaw,pitch,roll,color,doubleArrow);
    a->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(a);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Circle to the Background.
 * \details Initializes a new Circle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Circle's center location.
 * \param y The y coordinate of the Circle's center location.
 * \param z The z coordinate of the Circle's center location.
 * \param radius Radius of the Circle.
 * \param yaw The Circle's yaw rotation.
 * \param pitch The Circle's pitch rotation.
 * \param roll The Circle's roll rotation.
 * \param color Array of ColorFloats for the Circle's vertices.
 * \param outlined Boolean indicating if the Circle should be outlined or not, defaulting to not.
 */
void Background::drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    Circle * c = new Circle(x,y,z,radius,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Circle to the Background.
 * \details Initializes a new Circle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Circle's center location.
 * \param y The y coordinate of the Circle's center location.
 * \param z The z coordinate of the Circle's center location.
 * \param radius Radius of the Circle.
 * \param yaw The Circle's yaw rotation.
 * \param pitch The Circle's pitch rotation.
 * \param roll The Circle's roll rotation.
 * \param color ColorFloat for the Circle's vertices.
 * \param outlined Boolean indicating if the Circle should be outlined or not, defaulting to not.
 */
void Background::drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    Circle * c = new Circle(x,y,z,radius,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a ConcavePolygon to the Background.
 * \details Initializes a new ConcavePolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the ConcavePolygon's center location.
 * \param centerY The y coordinate of the ConcavePolygon's center location.
 * \param centerZ The z coordinate of the ConcavePolygon's center location.
 * \param numVertices The number of vertices to be drawn for the ConcavePolygon.
 * \param x Float array containing the ConcavePolygon's x vertices.
 * \param y Float array containing the ConcavePolygon's y vertices.
 * \param yaw The ConcavePolygon's yaw rotation.
 * \param pitch The ConcavePolygon's pitch rotation.
 * \param roll The ConcavePolygon's roll rotation.
 * \param color ColorFloat for the ConcavePolygon's vertices.
 * \param outlined Boolean indicating if the ConcavePolygon should be outlined or not, defaulting to not.
 * \warning Can sometimes incorrectly render; if this occurs, try shifting your last vertex to be your first vertex, or otherwise adjusting vertex order.
 */
void Background::drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    ConcavePolygon * c = new ConcavePolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a ConcavePolygon to the Background.
 * \details Initializes a new ConcavePolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the ConcavePolygon's center location.
 * \param centerY The y coordinate of the ConcavePolygon's center location.
 * \param centerZ The z coordinate of the ConcavePolygon's center location.
 * \param numVertices The number of vertices to be drawn for the ConcavePolygon.
 * \param x Float array containing the ConcavePolygon's x vertices.
 * \param y Float array containing the ConcavePolygon's y vertices.
 * \param yaw The ConcavePolygon's yaw rotation.
 * \param pitch The ConcavePolygon's pitch rotation.
 * \param roll The ConcavePolygon's roll rotation.
 * \param color Array of ColorFloats for the ConcavePolygon's vertices.
 * \param outlined Boolean indicating if the ConcavePolygon should be outlined or not, defaulting to not.
 * \warning Can sometimes incorrectly render; if this occurs, try shifting your last vertex to be your first vertex, or otherwise adjusting vertex order.
 */
void Background::drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    ConcavePolygon * c = new ConcavePolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a ConvexPolygon to the Background.
 * \details Initializes a new ConvexPolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the ConvexPolygon's center location.
 * \param centerY The y coordinate of the ConvexPolygon's center location.
 * \param centerZ The z coordinate of the ConvexPolygon's center location.
 * \param numVertices The number of vertices to be drawn for the ConvexPolygon.
 * \param x Float array containing the ConvexPolygon's x vertices.
 * \param y Float array containing the ConvexPolygon's y vertices.
 * \param yaw The ConvexPolygon's yaw rotation.
 * \param pitch The ConvexPolygon's pitch rotation.
 * \param roll The ConvexPolygon's roll rotation.
 * \param color ColorFloat for the ConvexPolygon's vertices.
 * \param outlined Boolean indicating if the ConvexPolygon should be outlined or not, defaulting to not.
 */
void Background::drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    ConvexPolygon * c = new ConvexPolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a ConvexPolygon to the Background.
 * \details Initializes a new ConvexPolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the ConvexPolygon's center location.
 * \param centerY The y coordinate of the ConvexPolygon's center location.
 * \param centerZ The z coordinate of the ConvexPolygon's center location.
 * \param numVertices The number of vertices to be drawn for the ConvexPolygon.
 * \param x Float array containing the ConvexPolygon's x vertices.
 * \param y Float array containing the ConvexPolygon's y vertices.
 * \param yaw The ConvexPolygon's yaw rotation.
 * \param pitch The ConvexPolygon's pitch rotation.
 * \param roll The ConvexPolygon's roll rotation.
 * \param color Array of ColorFloats for the ConvexPolygon's vertices.
 * \param outlined Boolean indicating if the ConvexPolygon should be outlined or not, defaulting to not.
 */
void Background::drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    ConvexPolygon * c = new ConvexPolygon(centerX,centerY,centerZ,numVertices,x,y,yaw,pitch,roll,color);
    c->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(c);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws an Ellipse to the Background.
 * \details Initializes a new Ellipse based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Ellipse's center location.
 * \param y The y coordinate of the Ellipse's center location.
 * \param z The z coordinate of the Ellipse's center location.
 * \param xRadius Horizontal radius of the Ellipse.
 * \param yRadius Vertical radius of the Ellipse.
 * \param yaw The Ellipse's yaw rotation.
 * \param pitch The Ellipse's pitch rotation.
 * \param roll The Ellipse's roll rotation.
 * \param color ColorFloat for the Ellipse's vertices.
 * \param outlined Boolean indicating if the Ellipse should be outlined or not, defaulting to not.
 */
void Background::drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    Ellipse * e = new Ellipse(x,y,z,xRadius,yRadius,yaw,pitch,roll,color);
    e->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(e);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws an Ellipse to the Background.
 * \details Initializes a new Ellipse based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Ellipse's center location.
 * \param y The y coordinate of the Ellipse's center location.
 * \param z The z coordinate of the Ellipse's center location.
 * \param xRadius Horizontal radius of the Ellipse.
 * \param yRadius Vertical radius of the Ellipse.
 * \param yaw The Ellipse's yaw rotation.
 * \param pitch The Ellipse's pitch rotation.
 * \param roll The Ellipse's roll rotation.
 * \param color Array of ColorFloats for the Ellipse's vertices.
 * \param outlined Boolean indicating if the Ellipse should be outlined or not, defaulting to not.
 */
void Background::drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    Ellipse * e = new Ellipse(x,y,z,xRadius,yRadius,yaw,pitch,roll,color);
    e->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(e);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws an Image to the Background.
 * \details Initializes a new Image based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Image's center location.
 * \param y The y coordinate of the Image's center location.
 * \param z The z coordinate of the Image's center location.
 * \param filename String containing the file location of the Image.
 * \param yaw The Image's yaw rotation.
 * \param pitch The Image's pitch rotation.
 * \param roll The Image's roll rotation.
 * \param alpha Alpha value for the Image's transparency.
 */
void Background::drawImage(float x, float y, float z, std::string filename, float width, float height, float yaw, float pitch, float roll, float alpha) {
    Image * i = new Image(x,y,z,filename,width,height,yaw,pitch,roll,alpha);
    drawableMutex.lock();
    myDrawables->push(i);
    drawableMutex.unlock();
}

/*!
 * \brief Procedurally draws a Line to the Background.
 * \details Initializes a new Line based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 *      \param x1 The x coordinate of the first endpoint of the line.
 *      \param y1 The y coordinate of the first endpoint of the line.
 *      \param z1 The z coordinate of the first endpoint of the line.
 *      \param x2 The x coordinate of the second endpoint of the line.
 *      \param y2 The y coordinate of the second endpoint of the line.
 *      \param z2 The z coordinate of the second endpoint of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color The reference variable to the color of the Line.
 */
void Background::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float yaw, float pitch, float roll, ColorFloat color) {
    Line * l = new Line(x1,y1,z1,x2,y2,z2,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(l);
    drawableMutex.unlock();
}

/*!
 * \brief Procedurally draws a Line to the Background.
 * \details Initializes a new Line based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 *      \param x1 The x coordinate of the first endpoint of the line.
 *      \param y1 The y coordinate of the first endpoint of the line.
 *      \param z1 The z coordinate of the first endpoint of the line.
 *      \param x2 The x coordinate of the second endpoint of the line.
 *      \param y2 The y coordinate of the second endpoint of the line.
 *      \param z2 The z coordinate of the second endpoint of the line.
 *      \param yaw The yaw of the line.
 *      \param pitch The pitch of the line.
 *      \param roll The roll of the line.
 *      \param color Array of ColorFloats for the Line's vertices.
 */
void Background::drawLine(float x1, float y1, float z1, float x2, float y2, float z2, float yaw, float pitch, float roll, ColorFloat color[]) {
    Line * l = new Line(x1,y1,z1,x2,y2,z2,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(l);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Line to the Background.
 * \details Initializes a new Line based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Line's center location.
 * \param y The y coordinate of the Line's center location.
 * \param z The z coordinate of the Line's center location.
 * \param length Length of the Line.
 * \param yaw The Line's yaw rotation.
 * \param pitch The Line's pitch rotation.
 * \param roll The Line's roll rotation.
 * \param color ColorFloat for the Line's vertices.
 */
void Background::drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color) {
    Line * l = new Line(x,y,z,length,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(l);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Line to the Background.
 * \details Initializes a new Line based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Line's center location.
 * \param y The y coordinate of the Line's center location.
 * \param z The z coordinate of the Line's center location.
 * \param length Length of the Line.
 * \param yaw The Line's yaw rotation.
 * \param pitch The Line's pitch rotation.
 * \param roll The Line's roll rotation.
 * \param color Array of ColorFloats for the Line's vertices.
 */
void Background::drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color[]) {
    Line * l = new Line(x,y,z,length,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(l);
    drawableMutex.unlock();
}

 /*!
  * \brief Draws a single pixel, specified in x,y format.
  * \details This function alters the value at the specified x, y offset within the Background's buffer variable.
  * \note (0,0) signifies the <b>center</b> of the Background.
  *   \param x The x-position of the pixel.
  *   \param y The y-position of the pixel.
  *   \param color The color of the point.
  */
void Background::drawPixel(float x, float y, ColorInt c) {
    if (abs((int)x) > (myWidth / 2) || abs((int)y) > (myHeight / 2)) {
        TsglErr("Pixel x and y coordinates must be within Background dimensions.");
        return;
    }
    pixelBufferMutex.lock();
    int intX = (int) x + myWidth / 2;
    int intY = (int) y + myHeight / 2;
    int outR;
    int outG;
    int outB;
    int outA;
    // first, if pixel hasn't been written since last draw cycle, just draw
    if (pixelTextureBuffer[(intY * myWidth + intX) * 4 + 3] == 0) {
        outR = c.R; outG = c.G; outB = c.B; outA = c.A;
    } else {
        // if new alpha is 255, just replace. otherwise, alpha blend
        if (c.A == 255) {
            outR = c.R; outG = c.G; outB = c.B; outA = 255;
        } else {
            int destA = (float) pixelTextureBuffer[(intY * myWidth + intX) * 4 + 3] / 255;
            float srcA = (float) c.A / 255;
            float oA = srcA + (destA * (1 - srcA));
            outR = (c.R * srcA + pixelTextureBuffer[(intY * myWidth + intX) * 4] * destA * (1 - srcA)) / oA;
            outG = (c.G * srcA + pixelTextureBuffer[(intY * myWidth + intX) * 4 + 1] * destA * (1 - srcA)) / oA;
            outB = (c.B * srcA + pixelTextureBuffer[(intY * myWidth + intX) * 4 + 2] * destA * (1 - srcA)) / oA;
            outA = (int) (oA * 255);
        }
    }

    pixelTextureBuffer[(intY * myWidth + intX) * 4] = outR;
    pixelTextureBuffer[(intY * myWidth + intX) * 4 + 1] = outG;
    pixelTextureBuffer[(intY * myWidth + intX) * 4 + 2] = outB;
    pixelTextureBuffer[(intY * myWidth + intX) * 4 + 3] = outA;

    newPixelsDrawn = true;
    pixelBufferMutex.unlock();
}

/*!\brief Procedurally draws a Polyline to the Background.
 * \details Initializes a new Polyline based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Polyline's center location.
 * \param y The y coordinate of the Polyline's center location.
 * \param z The z coordinate of the Polyline's center location.
 * \param numVertices The number of vertices to be drawn for the Polyline.
 * \param lineVertices Float array containing the Polyline's vertices (x1,y1,z1,x2,y2,z2,x3...).
 * \param yaw The Polyline's yaw rotation.
 * \param pitch The Polyline's pitch rotation.
 * \param roll The Polyline's roll rotation.
 * \param color ColorFloat for the Polyline's vertices.
 */
void Background::drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color) {
    Polyline * p = new Polyline(x,y,z,numVertices,lineVertices,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(p);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Polyline to the Background.
 * \details Initializes a new Polyline based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Polyline's center location.
 * \param y The y coordinate of the Polyline's center location.
 * \param z The z coordinate of the Polyline's center location.
 * \param numVertices The number of vertices to be drawn for the Polyline.
 * \param lineVertices Float array containing the Polyline's vertices (x1,y1,z1,x2,y2,z2,x3...).
 * \param yaw The Polyline's yaw rotation.
 * \param pitch The Polyline's pitch rotation.
 * \param roll The Polyline's roll rotation.
 * \param color Array of ColorFloats corresponding to the Polyline's vertices.
 */
void Background::drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]) {
    Polyline * p = new Polyline(x,y,z,numVertices,lineVertices,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(p);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Rectangle to the Background.
 * \details Initializes a new Rectangle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Rectangle's center location.
 * \param y The y coordinate of the Rectangle's center location.
 * \param z The z coordinate of the Rectangle's center location.
 * \param width Width of the Rectangle.
 * \param height Height of the Rectangle.
 * \param yaw The Rectangle's yaw rotation.
 * \param pitch The Rectangle's pitch rotation.
 * \param roll The Rectangle's roll rotation.
 * \param color ColorFloat for the Rectangle's vertices.
 * \param outlined Boolean indicating if the Rectangle should be outlined or not, defaulting to not.
 */
void Background::drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    Rectangle * r = new Rectangle(x,y,z,width,height,yaw,pitch,roll,color);
    r->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(r);
    drawableMutex.unlock();
}
/*!\brief Procedurally draws a Rectangle to the Background.
 * \details Initializes a new Rectangle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Rectangle's center location.
 * \param y The y coordinate of the Rectangle's center location.
 * \param z The z coordinate of the Rectangle's center location.
 * \param width Width of the Rectangle.
 * \param height Height of the Rectangle.
 * \param yaw The Rectangle's yaw rotation.
 * \param pitch The Rectangle's pitch rotation.
 * \param roll The Rectangle's roll rotation.
 * \param color Array of ColorFloats corresponding to the Rectangle's vertices.
 * \param outlined Boolean indicating if the Rectangle should be outlined or not, defaulting to not.
 */
void Background::drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    Rectangle * r = new Rectangle(x,y,z,width,height,yaw,pitch,roll,color);
    r->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(r);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a RegularPolygon to the Background.
 * \details Initializes a new RegularPolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the RegularPolygon's center location.
 * \param y The y coordinate of the RegularPolygon's center location.
 * \param z The z coordinate of the RegularPolygon's center location.
 * \param radius Distance from the RegularPolygon's center to each of its vertices.
 * \param points Number of sides on the RegularPolygon.
 * \param yaw The RegularPolygon's yaw rotation.
 * \param pitch The RegularPolygon's pitch rotation.
 * \param roll The RegularPolygon's roll rotation.
 * \param color ColorFloat for the RegularPolygon's vertices.
 * \param outlined Boolean indicating if the RegularPolygon should be outlined or not, defaulting to not.
 */
void Background::drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    RegularPolygon * r = new RegularPolygon(x,y,z,radius,sides,yaw,pitch,roll,color);
    r->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(r);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a RegularPolygon to the Background.
 * \details Initializes a new RegularPolygon based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the RegularPolygon's center location.
 * \param y The y coordinate of the RegularPolygon's center location.
 * \param z The z coordinate of the RegularPolygon's center location.
 * \param radius Distance from the RegularPolygon's center to each of its vertices.
 * \param points Number of sides on the RegularPolygon.
 * \param yaw The RegularPolygon's yaw rotation.
 * \param pitch The RegularPolygon's pitch rotation.
 * \param roll The RegularPolygon's roll rotation.
 * \param color Array of ColorFloats corresponding to the RegularPolygon's vertices.
 * \param outlined Boolean indicating if the RegularPolygon should be outlined or not, defaulting to not.
 */
void Background::drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    RegularPolygon * r = new RegularPolygon(x,y,z,radius,sides,yaw,pitch,roll,color);
    r->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(r);
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Square to the Background.
 * \details Initializes a new Square based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Square's center location.
 * \param y The y coordinate of the Square's center location.
 * \param z The z coordinate of the Square's center location.
 * \param sidelength Length of each side of the Square.
 * \param yaw The Square's yaw rotation.
 * \param pitch The Square's pitch rotation.
 * \param roll The Square's roll rotation.
 * \param color ColorFloat for the Square's vertices.
 * \param outlined Boolean indicating if the Square should be outlined or not, defaulting to not.
 */
void Background::drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    Square * s = new Square(x,y,z,sidelength,yaw,pitch,roll,color);
    s->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Square to the Background.
 * \details Initializes a new Square based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Square's center location.
 * \param y The y coordinate of the Square's center location.
 * \param z The z coordinate of the Square's center location.
 * \param sidelength Length of each side of the Square.
 * \param yaw The Square's yaw rotation.
 * \param pitch The Square's pitch rotation.
 * \param roll The Square's roll rotation.
 * \param color Array of ColorFloats corresponding to the Square's vertices.
 * \param outlined Boolean indicating if the Square should be outlined or not, defaulting to not.
 */
void Background::drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    Square * s = new Square(x,y,z,sidelength,yaw,pitch,roll,color);
    s->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Star to the Background.
 * \details Initializes a new Star based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Star's center location.
 * \param y The y coordinate of the Star's center location.
 * \param z The z coordinate of the Star's center location.
 * \param radius Distance from the Star's center to each of its points.
 * \param points Number of points on the Star.
 * \param yaw The Star's yaw rotation.
 * \param pitch The Star's pitch rotation.
 * \param roll The Star's roll rotation.
 * \param color ColorFloat for the Star's vertices.
 * \param ninja Boolean indicating if the Star should be mirror-symmetrical or just rotationally symmetrical.
 * \param outlined Boolean indicating if the Star should be outlined or not, defaulting to not.
 */
void Background::drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color, bool ninja, bool outlined) {
    Star * s = new Star(x,y,z,radius,points,yaw,pitch,roll,color,ninja);
    s->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    drawableMutex.unlock();   
}

/*!\brief Procedurally draws a Star to the Background.
 * \details Initializes a new Star based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Star's center location.
 * \param y The y coordinate of the Star's center location.
 * \param z The z coordinate of the Star's center location.
 * \param radius Distance from the Star's center to each of its points.
 * \param points Number of points on the Star.
 * \param yaw The Star's yaw rotation.
 * \param pitch The Star's pitch rotation.
 * \param roll The Star's roll rotation.
 * \param color Array of ColorFloats corresponding to the Star's vertices.
 * \param ninja Boolean indicating if the Star should be mirror-symmetrical or just rotationally symmetrical.
 * \param outlined Boolean indicating if the Star should be outlined or not, defaulting to not.
 */
void Background::drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color[], bool ninja, bool outlined) {
    Star * s = new Star(x,y,z,radius,points,yaw,pitch,roll,color,ninja);
    s->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
    drawableMutex.unlock();   
}

/*!\brief Procedurally draws Text to the Background.
 * \details Initializes a new Text based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Text's center location.
 * \param y The y coordinate of the Text's center location.
 * \param z The z coordinate of the Text's center location.
 * \param text String containing the characters to be rendered.
 * \param fontFilename String containing the filename for the font with which the Text will be rendered.
 * \param size The Text's size, relative to worldspace.
 * \param yaw The Text's yaw rotation.
 * \param pitch The Text's pitch rotation.
 * \param roll The Text's roll rotation.
 * \param color ColorFloat for the Text.
 */
void Background::drawText(float x, float y, float z, std::string text, std::string fontFilename, float size, float yaw, float pitch, float roll, const ColorFloat &color) {
    std::wstring wsTmp(text.begin(), text.end());
    std::wstring ws = wsTmp;
    drawText(x,y,z,ws,fontFilename,size,yaw,pitch,roll,color);
}

/*!\brief Procedurally draws Text to the Background.
 * \details Initializes a new Text based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x The x coordinate of the Text's center location.
 * \param y The y coordinate of the Text's center location.
 * \param z The z coordinate of the Text's center location.
 * \param text Wide string containing the characters to be rendered.
 * \param fontFilename String containing the filename for the font with which the Text will be rendered.
 * \param size The Text's size, relative to worldspace.
 * \param yaw The Text's yaw rotation.
 * \param pitch The Text's pitch rotation.
 * \param roll The Text's roll rotation.
 * \param color ColorFloat for the Text.
 */
void Background::drawText(float x, float y, float z, std::wstring text, std::string fontFilename, float size, float yaw, float pitch, float roll, const ColorFloat &color) {
    Text * t = new Text(x,y,z,text,fontFilename,size,yaw,pitch,roll,color);
    drawableMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Triangle to the Background.
 * \details Initializes a new Triangle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x1 The x coordinate of the Triangle's first vertex location.
 * \param y1 The y coordinate of the Triangle's first vertex location.
 * \param z1 The z coordinate of the Triangle's first vertex location.
 * \param x2 The x coordinate of the Triangle's second vertex location.
 * \param y2 The y coordinate of the Triangle's second vertex location.
 * \param z2 The z coordinate of the Triangle's second vertex location.
 * \param x3 The x coordinate of the Triangle's third vertex location.
 * \param y3 The y coordinate of the Triangle's third vertex location.
 * \param z3 The z coordinate of the Triangle's third vertex location.
 * \param yaw The Triangle's yaw rotation.
 * \param pitch The Triangle's pitch rotation.
 * \param roll The Triangle's roll rotation.
 * \param color ColorFloat for the Triangle's vertices.
 * \param outlined Boolean indicating if the Triangle should be outlined or not, defaulting to not.
 */
void Background::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    Triangle * t = new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,yaw,pitch,roll,color);
    t->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a Triangle to the Background.
 * \details Initializes a new Triangle based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param x1 The x coordinate of the Triangle's first vertex location.
 * \param y1 The y coordinate of the Triangle's first vertex location.
 * \param z1 The z coordinate of the Triangle's first vertex location.
 * \param x2 The x coordinate of the Triangle's second vertex location.
 * \param y2 The y coordinate of the Triangle's second vertex location.
 * \param z2 The z coordinate of the Triangle's second vertex location.
 * \param x3 The x coordinate of the Triangle's third vertex location.
 * \param y3 The y coordinate of the Triangle's third vertex location.
 * \param z3 The z coordinate of the Triangle's third vertex location.
 * \param yaw The Triangle's yaw rotation.
 * \param pitch The Triangle's pitch rotation.
 * \param roll The Triangle's roll rotation.
 * \param color Array of ColorFloats corresponding to the Triangle's vertices.
 * \param outlined Boolean indicating if the Triangle should be outlined or not, defaulting to not.
 */
void Background::drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    Triangle * t = new Triangle(x1,y1,z1,x2,y2,z2,x3,y3,z3,yaw,pitch,roll,color);
    t->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a TriangleStrip to the Background.
 * \details Initializes a new TriangleStrip based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the TriangleStrip's center location.
 * \param centerY The y coordinate of the TriangleStrip's center location.
 * \param centerZ The z coordinate of the TriangleStrip's center location.
 * \param numVertices The number of vertices to be drawn for the TriangleStrip.
 * \param x Float array containing the TriangleStrip's x vertices.
 * \param y Float array containing the TriangleStrip's y vertices.
 * \param z Float array containing the TriangleStrip's z vertices.
 * \param yaw The TriangleStrip's yaw rotation.
 * \param pitch The TriangleStrip's pitch rotation.
 * \param roll The TriangleStrip's roll rotation.
 * \param color ColorFloat for the TriangleStrip's vertices.
 * \param outlined Boolean indicating if the TriangleStrip should be outlined or not, defaulting to not.
 */
void Background::drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color, bool outlined) {
    TriangleStrip * t = new TriangleStrip(centerX,centerY,centerZ,numVertices,x,y,z,yaw,pitch,roll,color);
    t->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

/*!\brief Procedurally draws a TriangleStrip to the Background.
 * \details Initializes a new TriangleStrip based on the parameter values, and then adds it to the Array of Drawables to be rendered.
 * \param centerX The x coordinate of the TriangleStrip's center location.
 * \param centerY The y coordinate of the TriangleStrip's center location.
 * \param centerZ The z coordinate of the TriangleStrip's center location.
 * \param numVertices The number of vertices to be drawn for the TriangleStrip.
 * \param x Float array containing the TriangleStrip's x vertices.
 * \param y Float array containing the TriangleStrip's y vertices.
 * \param z Float array containing the TriangleStrip's z vertices.
 * \param yaw The TriangleStrip's yaw rotation.
 * \param pitch The TriangleStrip's pitch rotation.
 * \param roll The TriangleStrip's roll rotation.
 * \param color Array of ColorFloats corresponding to the TriangleStrip's vertices.
 * \param outlined Boolean indicating if the TriangleStrip should be outlined or not, defaulting to not.
 */
void Background::drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color[], bool outlined) {
    TriangleStrip * t = new TriangleStrip(centerX,centerY,centerZ,numVertices,x,y,z,yaw,pitch,roll,color);
    t->setIsOutlined(outlined);
    drawableMutex.lock();
    myDrawables->push(t);  // Push it onto our drawing buffer
    drawableMutex.unlock();
}

 /*!
  * \brief Gets the color of the pixel drawn on the current Background at the given x and y.
  * \note (0,0) signifies the <b>center</b> of the Background's texture.
  *      \param x The x-position of the pixel to grab.
  *      \param y The y-position of the pixel to grab.
  * \return A ColorInt containing the color of the pixel at (x,y).
  */
ColorInt Background::getPixel(float x, float y) {
    if (abs(x) > (myWidth/2) || abs(y) > (myHeight/2)) {
        TsglErr("Accessor x and y must be within Canvas parameters.");
        return ColorInt(0,0,0,0);
    }
    readPixelMutex.lock();
    int intX = (int) x * (framebufferWidth / myWidth) + framebufferWidth/2;
    int intY = (int) y * (framebufferWidth / myWidth) + framebufferHeight/2;
    int off = 3 * (intY * framebufferWidthPadded + intX);
    ColorInt c = ColorInt(readPixelBuffer[off], readPixelBuffer[off + 1], readPixelBuffer[off + 2], 255);
    readPixelMutex.unlock();
    return c;
}

/*! \brief Mutator for the color used to clear the Background when clear() is called.
 *  \details Sets the clear color to the parameter ColorFloat.
 *  \param c ColorFloat assigned to the clear color of the Background.
 */
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
    glDeleteTextures(1, &intermediateFBO);
    glDeleteFramebuffers(1, &intermediateTexture);
    glDeleteTextures(1, &multisampledTexture);
    glDeleteFramebuffers(1, &multisampledFBO);
}

}