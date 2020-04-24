// #include "Text.h"
// #include "iostream"

// namespace tsgl {

// /*!
//  * \brief Explicitly constructs a new Text instance.
//  * \details This is the constructor for the Text class.
//  *      \param text The string to draw.
//  *      \param loader A reference pointer to the TextureHandler with which to load the font.
//  *      \param x The x coordinate.
//  *      \param y The y coordinate.
//  *      \param fontsize The size of the text in pixels.
//  *      \param color A reference to the ColorFloat to use.
//  * \return A new Text instance with the specified string, position, and color.
//  */
// Text::Text(std::wstring text, float x, float y, unsigned int fontsize, const ColorFloat &color) {
//     isTextured = true;  // Let the Canvas know we're a textured object
//     myString = text;
//     myLoader = new TextureHandler();
//     myX = x;
//     myY = y;
//     myFontSize = fontsize;
//     myColor = color;
//     myRotation = 0;
//     myCenterX = 0;
//     myCenterY = 0;
//     vertices = new float[32];                                        // Allocate the vertices
//     myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);

//     setRotationPoint(myCenterX, myCenterY);
// }

// /*!
//  * \brief Draw the Text.
//  * \details This function actually draws the Text to the Canvas. 
//  */
// void Text::draw() {
//     vertices[0]  = myX;                                                     // Pre-init the array with the start coords
//     vertices[1]  = myY;

//     vertices[2] = vertices[10] = vertices[18] = vertices[26] = myColor.R;   // Texture color of the coords
//     vertices[3] = vertices[11] = vertices[19] = vertices[27] = myColor.G;   // (Default to opaque white)
//     vertices[4] = vertices[12] = vertices[20] = vertices[28] = myColor.B;
//     vertices[5] = vertices[13] = vertices[21] = vertices[29] = myColor.A;

//     vertices[6]  = vertices[7] = 0.0f;          // Texture coords of top left
//     vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
//     vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
//     vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right

//     myLoader->drawText(myString, myFontSize, vertices, myCenterX, myCenterY, myRotation);
// }

// /*!
//  * \brief Alter the Text's string
//  * \details This function changes myString to the parameter text
//  *  \param text The text to change myString to.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::setText(std::wstring text) {
//     myString = text;
//     bool shiftRotationPoint = false;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         shiftRotationPoint = true;
//     }
//     myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
//     if(shiftRotationPoint) {
//         setRotationPoint(myCenterX, myCenterY);
//     }
// }

// /*!
//  * \brief Alter the Text's font size
//  * \details This function changes myFontSize to the parameter fontsize.
//  *  \param fontsize The new fontsize.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::setFontSize(int fontsize) {
//     myFontSize = fontsize;
//     bool shiftRotationPoint = false;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         shiftRotationPoint = true;
//     }
//     myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
//     if(shiftRotationPoint) {
//         setRotationPoint(myCenterX, myCenterY);
//     }
// }

// /*!
//  * \brief Alter the Text's font
//  * \details This function changes myLoader's font to the parameter font.
//  *  \param filename The new font file name.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::setFont(std::string filename) {
//     myLoader->loadFont(filename);
//     bool shiftRotationPoint = false;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         shiftRotationPoint = true;
//     }
//     myLoader->calculateTextCenter(myString, myFontSize, myX, myY, myCenterX, myCenterY);
//     if(shiftRotationPoint) {
//         setRotationPoint(myCenterX, myCenterY);
//     }
// }

// /*!
//  * \brief Alter the Text's lower left hand corner location
//  * \details This function changes myX and myY to the parameter x and y.
//  *  \param x The new x-coordinate for myX.
//  *  \param y The new y-coordinate for myY.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::setBottomLeftCorner(float x, float y) {
//     float deltaX = x - myX;
//     float deltaY = y - myY;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         myRotationPointX += deltaX;
//         myRotationPointY += deltaY;
//     }
//     myCenterX += deltaX;
//     myCenterY += deltaY;
//     myX = x;
//     myY = y;
// }

// /*!
//  * \brief Alter the Text's lower left hand corner location
//  * \details This function changes myX and myY to the parameter x and y.
//  *  \param x The new x-coordinate for myX.
//  *  \param y The new y-coordinate for myY.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::setCenter(float x, float y) {
//     float deltaX = x - myCenterX;
//     float deltaY = y - myCenterY;
//     myX += deltaX;
//     myY += deltaY;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         setRotationPoint(x, y);
//     }
//     myCenterX = x;
//     myCenterY = y;
// }

// /*!
//  * \brief Alter the Text's location by deltaX and deltaY.
//  * \details This function changes all coordinate variables by the parameter deltaX and deltaY
//  *  \param deltaX The amount to change x-coordinates by.
//  *  \param deltaY The amount to change y-coordinates by.
//  * \warning This will also alter the Text's rotation point to the new center if and only if 
//  *          the old rotation point was at the Text's old center.
//  */
// void Text::moveTextBy(float deltaX, float deltaY) {
//     myX += deltaX;
//     myY += deltaY;
//     if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
//         myRotationPointX += deltaX;
//         myRotationPointY += deltaY;
//     }
//     myCenterX += deltaX;
//     myCenterY += deltaY;
// }

// /*!
//  * \brief Mutator for the rotation of the Text.
//  * \details Rotates the Text vertices around centerX, centerY.
//  * \param radians Float value denoting how many radians to rotate the Text.
//  */
// void Text::setRotation(float radians) {
//     // myRotation = radians;
//   if(radians != myRotation) {
//     attribMutex.lock();

//     // distance between myCenter and bottom left corner
//     float deltaX = std::roundf(myCenterX - myX);
//     float deltaY = std::roundf(myCenterY - myY);

//     //deal with rotation variables
//     float s = sin(radians - myRotation);
//     float c = cos(radians - myRotation);
//     myRotation = radians;

//     //rotate myCenter around myRotationPoint
//     float x = myCenterX;
//     float y = myCenterY;
//     x -= myRotationPointX;
//     y -= myRotationPointY;
//     float xnew = x * c - y * s;
//     float ynew = x * s + y * c;
//     x = xnew + myRotationPointX;
//     y = ynew + myRotationPointY;
//     myCenterX = x;
//     myCenterY = y;

//     //calculate new location of bottom left corner
//     myX = myCenterX - deltaX;
//     myY = myCenterY - deltaY;
//     attribMutex.unlock();
//   }
// }

// /*!
//  * \brief Alter the Text's color
//  * \details This function changes myColor to the parameter ColorFloat
//  *  \param color The ColorFloat to change myColor to.
//  */
// void Text::setColor(const ColorFloat& color) {
//     myColor = color;
// }

// Text::~Text() {
//     delete myLoader;
//     delete[] vertices;
// }



// }