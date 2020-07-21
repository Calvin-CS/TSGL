/*
 * Background.h extends Drawable and provides a class for drawing TSGL primitives procedurally onto a background.
 */

#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "Drawable.h"       // For extending our Shape object
#include "Array.h"          // Our own array for buffering drawing operations
#include "Arrow.h"
#include "Circle.h"
#include "ConcavePolygon.h"
#include "ConvexPolygon.h"
#include "Ellipse.h"
#include "Image.h"
#include "Line.h"
#include "Polyline.h"
#include "Rectangle.h"
#include "RegularPolygon.h"
#include "Square.h"
#include "Star.h"
#include "Text.h"
#include "Triangle.h"
#include "TriangleStrip.h"
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

    // broken
    virtual ColorInt getPixel(int row, int col);

    // broken
    virtual void drawPixel(int row, int col, ColorInt c);

    virtual void drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color, bool doubleArrow = false);

    virtual void drawArrow(float x, float y, float z, float length, float width, float yaw, float pitch, float roll, ColorFloat color[], bool doubleArrow = false);

    virtual void drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawCircle(float x, float y, float z, float radius, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawConcavePolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawConvexPolygon(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawEllipse(float x, float y, float z, float xRadius, float yRadius, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawImage(float x, float y, float z, std::string filename, float width, float height, float yaw, float pitch, float roll, float alpha = 1.0f);

    virtual void drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawLine(float x, float y, float z, float length, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawPolyline(float x, float y, float z, int numVertices, float lineVertices[], float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawRectangle(float x, float y, float z, float width, float height, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawRegularPolygon(float x, float y, float z, float radius, int sides, float yaw, float pitch, float roll, ColorFloat color[]);  

    virtual void drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawSquare(float x, float y, float z, float sidelength, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color, bool ninja = false);

    virtual void drawStar(float x, float y, float z, float radius, int points, float yaw, float pitch, float roll, ColorFloat color[], bool ninja = false);

    virtual void drawText(float x, float y, float z, std::wstring text, std::string fontFilename, unsigned int fontsize, float yaw, float pitch, float roll, const ColorFloat &color);

    virtual void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawTriangle(float x1, float y1, float z1, float x2, float y2, float z2, float x3, float y3, float z3, float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color);

    virtual void drawTriangleStrip(float centerX, float centerY, float centerZ, int numVertices, float x[], float y[], float z[], float yaw, float pitch, float roll, ColorFloat color[]);

    virtual void clear() { toClear = true; }

    virtual void setClearColor(ColorFloat c);

    virtual ColorFloat getClearColor() { return baseColor; }

    virtual ~Background();
};

}

#endif /* BACKGROUND_H_ */