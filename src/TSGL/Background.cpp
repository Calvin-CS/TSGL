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
    attribMutex.unlock();
}

 /*!
  * \brief Draw the Background.
  * \details This function actually draws the Background to the Canvas.
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
    
    glBindFramebuffer(GL_READ_FRAMEBUFFER, multisampledFBO);
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
    glBlitFramebuffer(0, 0, myWidth, myHeight, 0, 0, myWidth, myHeight, GL_COLOR_BUFFER_BIT, GL_NEAREST);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

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
    attribMutex.lock();
    shapeShader = shapeS;
    textShader = textS;
    textureShader = textureS;    
    complete = true;
    attribMutex.unlock();
    glfwMakeContextCurrent(0);
}

void Background::drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color) {
    Square * s = new Square(x,y,z,sidelength,yaw,pitch,roll,color);
    attribMutex.lock();
    myDrawables->push(s);  // Push it onto our drawing buffer
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
    delete myDrawables;
    glDeleteTextures(1, &multisampledTexture);
    glDeleteFramebuffers(1, &multisampledFBO);
}

}