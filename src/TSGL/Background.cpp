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
Background::Background(float x, float y, float z, GLint width, GLint height, float yaw, float pitch, float roll, const ColorFloat &c)  
: Drawable(x, y, z, yaw, pitch, roll)  {
    if (width <= 0 || height <= 0) {
        TsglDebug("Cannot have a Background with non-positive width or height.");
    }
    attribMutex.lock();
    shaderType = TEXTURE_SHADER_TYPE;
    myXScale = myWidth = width;
    myYScale = myHeight = height;
    vertices = new GLfloat[30];
    vertices[0]  = vertices[1] = vertices[6] = vertices[10] = vertices[16]  = vertices[20] = -0.5; // x + y
    vertices[5] = vertices[11] = vertices[15] = vertices[21] = vertices[25] = vertices[26] = 0.5; // x + y
    vertices[2] = vertices[7] = vertices[12] = vertices[17] = vertices[22] = vertices[27] = 0; // z
    vertices[3]  = vertices[13]  = vertices[14] = vertices[23] = vertices[24] = vertices[29] = 0.0; // texture coord x + y
    vertices[4] = vertices[8] = vertices[9] = vertices[18] = vertices[19] = vertices[28] = 1.0; // texture coord x + y

    myDrawables = new Array<Drawable*>(myWidth * myHeight * 2);

    myAlpha = 1.0;

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    init = true;  
    attribMutex.unlock();
}

 /*!
  * \brief Draw the Background.
  * \details This function actually draws the Background to the Canvas.
  */
void Background::draw(Shader * shader) {
    if (!init) {
        TsglDebug("Vertex buffer is not full.");
        return;
    }

    // gen framebuffer
    glGenFramebuffers(1, &myFramebuffer);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, myFramebuffer);
    glDrawBuffer(GL_COLOR_ATTACHMENT0);

    glViewport(0,0,myWidth,myHeight);

    // gen texture, attach to framebuffer
    glGenTextures(1, &myTexture);
    glBindTexture(GL_TEXTURE_2D, myTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, myWidth, myHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, myTexture, 0);

    // render buffer object
    // glGenRenderbuffers(1, &myRenderbufferObject);
    // glBindRenderbuffer(GL_RENDERBUFFER, myRenderbufferObject);
    // glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, myWidth, myHeight); // use a single renderbuffer object for both a depth AND stencil buffer.
    // glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, myRenderbufferObject); // now actually attach it

    GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
    glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers

    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        TsglErr("ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
    glClearColor(1,0,0,1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

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
    
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    selectShaders(TEXTURE_SHADER_TYPE);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(myRotationPointX, myRotationPointY, myRotationPointZ));
    model = glm::rotate(model, glm::radians(myCurrentYaw), glm::vec3(0.0f, 0.0f, 1.0f));
    model = glm::rotate(model, glm::radians(myCurrentPitch), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(myCurrentRoll), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::translate(model, glm::vec3(myCenterX - myRotationPointX, myCenterY - myRotationPointY, myCenterZ - myRotationPointZ));
    model = glm::scale(model, glm::vec3(myXScale, myYScale, myZScale));

    unsigned int modelLoc = glGetUniformLocation(shader->ID, "model");
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

    glBindTexture(GL_TEXTURE_2D, myTexture);	// use the color attachment texture as the texture of the quad plane
    glPixelStorei(GL_UNPACK_ALIGNMENT,4);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 5, vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glFlush();                                   // Flush buffer data to the actual draw buffer


    glDeleteTextures(1, &myTexture);
    glDeleteFramebuffers(1, &myFramebuffer);
}

 /*!
  * \brief Gets the color of the pixel drawn on the current Background at the given row and column.
  * \note (0,0) signifies the <b>top-left</b> of the Background's texture.
  *      \param row The row (y-position) of the pixel to grab.
  *      \param col The column (x-position) of the pixel to grab.
  * \return A ColorInt containing the color of the pixel at (col,row).
  */
ColorInt Background::getPixel(int row, int col) {
    return ColorInt(0,0,0,255);
}

 /*!
  * \brief Draws a single pixel, specified in row,column format.
  * \details This function alters the value at the specified row, column offset within the Background's buffer variable.
  * \note (0,0) signifies the <b>top-left</b> of the Background.
  *   \param row The row (y-position) of the pixel.
  *   \param col The column (x-position) of the pixel.
  *   \param color The color of the point.
  */
void Background::drawPixel(int row, int col, ColorInt c) {
    attribMutex.lock();

    attribMutex.unlock();
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

void Background::defineShaders(Shader * shapeS, Shader * textS, Shader * textureS) {
    attribMutex.lock();
    shapeShader = shapeS;
    textShader = textS;
    textureShader = textureS;
    attribMutex.unlock();
}

void Background::drawSquare(float z) {
    Square * s = new Square(0,0,z,200,0,0,0,BLUE);
    myDrawables->push(s);  // Push it onto our drawing buffer
}

/*!
* \brief Destructor for the Background.
*/
Background::~Background() {
    myDrawables->clear();
    delete myDrawables;
}

}