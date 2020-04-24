// #include "Image.h"

// namespace tsgl {

//  /*!
//   * \brief Explicitly constructs a new Image.
//   * \details This is the explicit constructor for the Image class.
//   *   \param filename The filename of the image to load.
//   *   \param loader A reference pointer to the TextureHandler with which to load the image.
//   *   \param x The x coordinate of the left of the Image.
//   *   \param y The y coordinate of the top of the Image.
//   *   \param width The width of the Image.
//   *   \param height The height of the Image.
//   *   \param alhpa The alpha of the Image.
//   * \return A new Image is drawn with the specified coordinates, dimensions, and transparency.
//   * \note <B>IMPORTANT</B>: In CartesianCanvas, *y* specifies the bottom, not the top, of the image.
//   */
// Image::Image(std::string filename, TextureHandler &loader, int x, int y, int width, int height, float alpha) {
//   isTextured = true;  // Let the Canvas know we're a textured object
//   myTexture = 0;      // Fix no texture initialization warning
//   myWidth = width; myHeight = height;
//   if (myWidth <= 0 || myHeight <= 0) {
//     TextureHandler::getDimensions(filename,myWidth,myHeight);
//   }
//   myCenterX = x + myWidth / 2;
//   myCenterY = y + myHeight / 2;
//   setRotationPoint(myCenterX, myCenterY);
//   currentRotation = 0;
//   myFile = filename;
//   myLoader = &loader;
//   vertices = new float[32];
//   vertices[0] = x;
//   vertices[1] = y;
//   vertices[8] = x + myWidth;
//   vertices[9] = y;
//   vertices[16] = x;
//   vertices[17] = y + myHeight;
//   vertices[24] = x + myWidth;
//   vertices[25] = y + myHeight;
//   vertices[2] = vertices[10] = vertices[18] = vertices[26] = 1.0f;  // Texture color of the coords
//   vertices[3] = vertices[11] = vertices[19] = vertices[27] = 1.0f;
//   vertices[4] = vertices[12] = vertices[20] = vertices[28] = 1.0f;
//   vertices[5] = vertices[13] = vertices[21] = vertices[29] = alpha;
//   vertices[6] = vertices[7] = 0.0f;           // Texture coords of top left
//   vertices[14] = 1.0f, vertices[15] = 0.0f;   // Texture coords of top right
//   vertices[22] = 0.0f, vertices[23] = 1.0f;   // Texture coords of bottom left
//   vertices[30] = vertices[31] = 1.0f;         // Texture coords of bottom right
// }

//  /*!
//   * \brief Draw the Image.
//   * \details This function actually draws the Image to the Canvas.
//   */
// void Image::draw() {
//   unsigned int w, h;
//   myLoader->loadPicture(myFile, w, h, myTexture);

//   glBindTexture(GL_TEXTURE_2D, myTexture);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glBufferData(GL_ARRAY_BUFFER, 32 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
//   glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// }

// /*!
//  * \brief Mutator for the center coordinates of the Image.
//  * \details Alters the values of the myCenterX and myCenterY private variables.
//  * \param x Float value for the new center x-coordinate.
//  * \param y Float value for the new center y-coordinate.
//  * \warning This will also alter the Image's rotation point if and only if the 
//  *          old rotation point was at the Image's old center.
//  */
// void Image::setCenter(float x, float y) {
//   attribMutex.lock();
//   if(myRotationPointX == myCenterX && myRotationPointY == myCenterY) {
//     setRotationPoint(x, y);
//   }
//   if(myCenterX != x || myCenterY != y) {
//     float deltaX = x - myCenterX;
//     float deltaY = y - myCenterY;
//     for(int i = 0; i < 4; i++) {
//       vertices[8*i] += deltaX;
//       vertices[8*i+1] += deltaY;
//     }
//     myCenterX = x;
//     myCenterY = y;
//   }
//   attribMutex.unlock();
// }

// /*!
//  * \brief Mutator for the coordinates of the Image.
//  * \details Alters all coordinate variables for the Image.
//  * \param deltaX Float value of how much to alter the y-coordinates.
//  * \param deltaY Float value of how much to alter the y-coordinates.
//  * \warning This will also alter the Image's rotation point if and only if the 
//  *          old rotation point was at the Image's old center.
//  */
// void Image::moveImageBy(float deltaX, float deltaY) {
//   attribMutex.lock();
//   if(myRotationPointX == myCenterX && myRotationPointY == myCenterY) {
//     myRotationPointX += deltaX;
//     myRotationPointY += deltaY;
//   }
//   if(deltaX != 0 || deltaY != 0) {
//     for(int i = 0; i < 4; i++) {
//       vertices[8*i] += deltaX;
//       vertices[8*i+1] += deltaY;
//     }
//     myCenterX += deltaX;
//     myCenterY += deltaY;
//   }
//   attribMutex.unlock();
// }

// /*!
//  * \brief Mutator for the rotation of the Image.
//  * \details Rotates the Image corners around myRotationPointX, myRotationPointY.
//  * \param radians Float value denoting how many radians to rotate the Image.
//  */
// void Image::setRotation(float radians) {
//   if(radians != currentRotation) {
//     attribMutex.lock();
//     float pivotX = myRotationPointX;
//     float pivotY = myRotationPointY;
//     float s = sin(radians - currentRotation);
//     float c = cos(radians - currentRotation);
//     currentRotation = radians;
//     for(int i = 0; i < 4; i++) {
//       float x = vertices[8*i];
//       float y = vertices[8*i+1];
//       x -= pivotX;
//       y -= pivotY;
//       float xnew = x * c - y * s;
//       float ynew = x * s + y * c;

//       x = xnew + pivotX;
//       y = ynew + pivotY;
//       vertices[8*i] = x;
//       vertices[8*i+1] = y;
//     }
//     attribMutex.unlock();
//   }
// }

// /*!
//  * \brief Alters the file the Image draws.
//  * \details Alters the values of the myFile, myWidth, myHeight, and mutates vertices.
//  * \param filename New string value for myFile.
//  * \param width New width of the Image.
//  * \param height New height of the Image.
//  */
// void Image::changeFileName(std::string filename, int width, int height) {
//   attribMutex.lock();
//   myFile = filename;
//   myWidth = width; myHeight = height;
//   if (myWidth <= 0 || myHeight <= 0) {
//     TextureHandler::getDimensions(filename,myWidth,myHeight);
//   }
//   vertices[0] = myCenterX - myWidth/2;
//   vertices[1] = myCenterY - myHeight/2;
//   vertices[8] = myCenterX + myWidth/2;
//   vertices[9] = myCenterY - myHeight/2;
//   vertices[16] = myCenterX - myWidth/2;
//   vertices[17] = myCenterY + myHeight/2;
//   vertices[24] = myCenterX + myWidth/2;
//   vertices[25] = myCenterY + myHeight/2;
//   attribMutex.unlock();
// }

// }
