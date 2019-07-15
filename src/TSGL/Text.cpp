#include "Text.h"
#include "iostream"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Text instance.
 * \details This is the constructor for the Text class.
 *      \param text The string to draw.
 *      \param loader A reference pointer to the TextureHandler with which to load the font.
 *      \param x The x coordinate.
 *      \param y The y coordinate.
 *      \param fontsize The size of the text in pixels.
 *      \param color A reference to the ColorFloat to use.
 * \return A new Text instance with the specified string, position, and color.
 */
Text::Text(std::wstring text, int x, int y, unsigned int fontsize, const ColorFloat &color) {
    isTextured = true;  // Let the Canvas know we're a textured object
    myString = text;
    myLoader = new TextureHandler();
    myX = x;
    myY = y;
    myFontSize = fontsize;
    myColor = color;
    myRotation = 0;
    myCenterX = 0;
    myCenterY = 0;
    vertices = new float[32];                                        // Allocate the vertices
    myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
}

/*!
 * \brief Draw the Text.
 * \details This function actually draws the Text to the Canvas. 
 */
void Text::draw() {
    vertices[0]  = myX;                                                     // Pre-init the array with the start coords
    vertices[1]  = myY;

    vertices[2] = vertices[10] = vertices[18] = vertices[26] = myColor.R;   // Texture color of the coords
    vertices[3] = vertices[11] = vertices[19] = vertices[27] = myColor.G;   // (Default to opaque white)
    vertices[4] = vertices[12] = vertices[20] = vertices[28] = myColor.B;
    vertices[5] = vertices[13] = vertices[21] = vertices[29] = myColor.A;

    vertices[6]  = vertices[7] = 0.0f;          // Texture coords of top left
    vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
    vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
    vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right

    myLoader->drawText(myString, myFontSize, vertices, myCenterX, myCenterY, myRotation);
}

/*!
 * \brief Alter the Text's string
 * \details This function changes myString to the parameter text
 *  \param text The text to change myString to.
 */
void Text::setText(std::wstring text) {
    myString = text;
    myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
}

/*!
 * \brief Alter the Text's color
 * \details This function changes myColor to the parameter ColorFloat
 *  \param color The ColorFloat to change myColor to.
 */
void Text::setColor(const ColorFloat& color) {
    myColor = color;
}

/*!
 * \brief Alter the Text's lower left hand corner location
 * \details This function changes myX and myY to the parameter x and y.
 *  \param x The new x-coordinate for myX.
 *  \param y The new y-coordinate for myY.
 */
void Text::setBottomLeftCorner(int x, int y) {
    int deltaX = x - myX;
    int deltaY = y - myY;
    myCenterX += deltaX;
    myCenterY += deltaY;
    myX = x;
    myY = y;
}

/*!
 * \brief Alter the Text's lower left hand corner location
 * \details This function changes myX and myY to the parameter x and y.
 *  \param x The new x-coordinate for myX.
 *  \param y The new y-coordinate for myY.
 */
void Text::setCenter(int x, int y) {
    int deltaX = x - myCenterX;
    int deltaY = y - myCenterY;
    myX += deltaX;
    myY += deltaY;
    myCenterX = x;
    myCenterY = y;
}

/*!
 * \brief Alter the Text's font size
 * \details This function changes myFontSize to the parameter fontsize.
 *  \param fontsize The new fontsize.
 */
void Text::setFontSize(int fontsize) {
    myFontSize = fontsize;
    myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
}

/*!
 * \brief Mutator for the rotation of the Text.
 * \details Rotates the Text vertices around centerX, centerY.
 * \param radians Float value denoting how many radians to rotate the Text.
 */
void Text::setRotation(float radians) {
    myRotation = radians;
}

void Text::setFont(std::string filename) {
    myLoader->loadFont(filename);
    myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
}

Text::~Text() {
    delete myLoader;
    delete[] vertices;
}



}