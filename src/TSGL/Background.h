/*
 * Background.h extends Drawable and provides a class for drawing TSGL primitives procedurally onto a background.
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "Drawable.h"       // For extending our Shape object
#include "Array.h"          // Our own array for buffering drawing operations
#include "Square.h"
#include "Util.h"           // Needed constants and has cmath for performing math operations

namespace tsgl {

/*! \class Background
 *  \brief Draw a Background for the Canvas with colored pixels.
 *  \details Background is a class for holding colored pixel data.
 */
class Background {
protected:
    GLint myWidth, myHeight;
    GLuint multisampledTexture;
    GLuint multisampledFBO;
    GLuint RBO;
    Array<Drawable*> * myDrawables;
    Shader * textShader;
    Shader * shapeShader;
    Shader * textureShader;
    ColorFloat baseColor;
    bool complete;
    bool toClear;
    std::mutex attribMutex;

    virtual void selectShaders(unsigned int sType);
public:
    Background(GLint width, GLint height, const ColorFloat &c = WHITE);

    virtual void init(Shader * shapeS, Shader * textS, Shader * textureS, GLFWwindow * window);

    virtual bool isInitialized() { return complete; }

    virtual void draw(); 

    /*!
    * \brief Accessor for the width of the Background.
    * \details Returns the value of the myWidth private variable, a GLint.
    */
    virtual GLint getWidth() { return myWidth; }

    /*!
    * \brief Accessor for the height of the Background.
    * \details Returns the value of the myHeight private variable, a GLint.
    */
    virtual GLint getHeight() { return myHeight; }

    virtual ColorInt getPixel(int row, int col);

    virtual void drawPixel(int row, int col, ColorInt c);

    virtual void drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color);

    virtual void clear() { toClear = true; }

    virtual void setClearColor(ColorFloat c);

    virtual ColorFloat getClearColor() { return baseColor; }

    virtual ~Background();
};

}

#endif /* BACKGROUND_H_ */