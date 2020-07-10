/*
 * Background.h extends Drawable and provides a class for drawing a background.
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
class Background : public Drawable {
protected:
    GLint myWidth, myHeight;
    GLuint myTexture;
    GLuint myFramebuffer;
    GLuint myRenderbufferObject;
    Array<Drawable*> * myDrawables;
    Shader * textShader;
    Shader * shapeShader;
    Shader * textureShader;
public:
    Background(float x, float y, float z, GLint width, GLint height, float yaw, float pitch, float roll, const ColorFloat &c = WHITE);

    virtual void draw(Shader * shader); 

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

    virtual void selectShaders(unsigned int sType);

    virtual void defineShaders(Shader * shapeS, Shader * textS, Shader * textureS);

    virtual void drawSquare(float z);

    virtual ~Background();
};

}

#endif /* BACKGROUND_H_ */