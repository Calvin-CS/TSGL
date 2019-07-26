#include "Polygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Polygon.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices the number of vertices the complete Polygon will have.
  * \warning An invariant is held where if numVertices is less than 3 then an error message is given.
  * \return A new Polygon with a buffer for storing the specified numbered of vertices.
  */
Polygon::Polygon(int numVertices)  : Shape()  {
  attribMutex.lock();
  if (numVertices < 3) {
    TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
  }
  attribMutex.unlock();
}

/*!
 *  \brief This method actually draws the Polygon
 *  \details Depending on isFilled and hasOutline, draws either a series of connected lines outlining the Polygon or a filled version.
 */
void Polygon::draw()  {
  if(isFilled) {
    glBufferData(GL_ARRAY_BUFFER, numberOfVertices * 6 * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(geometryType, 0, numberOfVertices);
  }
  if(hasOutline) {
    glBufferData(GL_ARRAY_BUFFER, numberOfOutlineVertices * 6 * sizeof(float), outlineVertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINE_LOOP, 0, numberOfOutlineVertices);
  }
}

 /*!
  * \brief Adds another vertex to a Polygon.
  * \details This function initializes the next vertex in the Polygon and adds it to a Polygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param color The reference variable of the color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Polygon::addVertex(float x, float y, const ColorFloat &color) {
  if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
  }
  if(isFilled) {
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
  }
  if(hasOutline) {
    outlineVertices[current] = x;
    outlineVertices[current + 1] = y;
    outlineVertices[current + 2] = color.R;
    outlineVertices[current + 3] = color.G;
    outlineVertices[current + 4] = color.B;
    outlineVertices[current + 5] = color.A;
  }
  current += 6;
  if (current == numberOfVertices*6) {
    init = true;
    attribMutex.lock();
    float minX = 0, maxX = 0;
    float minY = 0, maxY = 0;
    if(hasOutline) {
      minX = maxX = outlineVertices[0];
      //Find min and max X
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        if( outlineVertices[i*6] < minX )
        minX = outlineVertices[i*6];
        else if( outlineVertices[i*6] > maxX )
        maxX = outlineVertices[i*6];
      }
      minY = maxY = outlineVertices[1];
      //Find min and max X
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        if( outlineVertices[i*6+1] < minY )
        minY = outlineVertices[i*6+1];
        else if( outlineVertices[i*6+1] > maxY )
        maxY = outlineVertices[i*6+1];
      }
    } else if (isFilled) {
      minX = maxX = vertices[0];
      //Find min and max X
      for(int i = 0; i < numberOfVertices; i++) {
        if( vertices[i*6] < minX )
        minX = vertices[i*6];
        else if( vertices[i*6] > maxX )
        maxX = vertices[i*6];
      }

      minY = maxY = vertices[1];
      //Find min and max X
      for(int i = 0; i < numberOfVertices; i++) {
        if( vertices[i*6+1] < minY )
        minY = vertices[i*6+1];
        else if( vertices[i*6+1] > maxY )
        maxY = vertices[i*6+1];
      }
    }
    
    myCenterX = (minX+maxX)/2;
    myCenterY = (minY+maxY)/2;

    setRotationPoint(myCenterX, myCenterY);

    attribMutex.unlock();
  }
}

 /*!
  * \brief Adds another vertex to a ConcavePolygon.
  * \details This function initializes the next vertex in the ConcavePolygon and adds it to a ConcavePolygon buffer.
  *      \param x The x position of the vertex.
  *      \param y The y position of the vertex.
  *      \param fillColor The reference variable of the fill color of the vertex.
  *      \param outlineColor The reference variable of the outline color of the vertex.
  * \note This function does nothing if the vertex buffer is already full.
  * \note A message is given indicating that the vertex buffer is full.
  */
void Polygon::addVertex(float x, float y, const ColorFloat &fillColor, const ColorFloat &outlineColor) {
  if (init) {
    TsglDebug("Cannot add anymore vertices.");
    return;
  }
  if(isFilled) {
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = fillColor.R;
    vertices[current + 3] = fillColor.G;
    vertices[current + 4] = fillColor.B;
    vertices[current + 5] = fillColor.A;
  }
  if(hasOutline) {
    outlineVertices[current] = x;
    outlineVertices[current + 1] = y;
    outlineVertices[current + 2] = outlineColor.R;
    outlineVertices[current + 3] = outlineColor.G;
    outlineVertices[current + 4] = outlineColor.B;
    outlineVertices[current + 5] = outlineColor.A;
  }
  current += 6;
  if (current == numberOfVertices*6) {
    init = true;
    attribMutex.lock();
    float minX = 0, maxX = 0;
    float minY = 0, maxY = 0;
    if(hasOutline) {
      minX = maxX = outlineVertices[0];
      //Find min and max X
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        if( outlineVertices[i*6] < minX )
        minX = outlineVertices[i*6];
        else if( outlineVertices[i*6] > maxX )
        maxX = outlineVertices[i*6];
      }
      minY = maxY = outlineVertices[1];
      //Find min and max X
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        if( outlineVertices[i*6+1] < minY )
        minY = outlineVertices[i*6+1];
        else if( outlineVertices[i*6+1] > maxY )
        maxY = outlineVertices[i*6+1];
      }
    } else if (isFilled) {
      minX = maxX = vertices[0];
      //Find min and max X
      for(int i = 0; i < numberOfVertices; i++) {
        if( vertices[i*6] < minX )
        minX = vertices[i*6];
        else if( vertices[i*6] > maxX )
        maxX = vertices[i*6];
      }

      minY = maxY = vertices[1];
      //Find min and max X
      for(int i = 0; i < numberOfVertices; i++) {
        if( vertices[i*6+1] < minY )
        minY = vertices[i*6+1];
        else if( vertices[i*6+1] > maxY )
        maxY = vertices[i*6+1];
      }
    }
    
    myCenterX = (minX+maxX)/2;
    myCenterY = (minY+maxY)/2;

    setRotationPoint(myCenterX, myCenterY);

    attribMutex.unlock();
  }
}

/**
 * \brief Sets the Polygon to a new color.
 * \param c The new ColorFloat.
 */
void Polygon::setColor(ColorFloat c) {
  if(isFilled) {
    for(int i = 0; i < numberOfVertices; i++) {
      vertices[i*6 + 2] = c.R;
      vertices[i*6 + 3] = c.G;
      vertices[i*6 + 4] = c.B;
      vertices[i*6 + 5] = c.A;
    }
  }
  if(hasOutline) {
    for(int i = 0; i < numberOfOutlineVertices; i++) {
      outlineVertices[i*6 + 2] = c.R;
      outlineVertices[i*6 + 3] = c.G;
      outlineVertices[i*6 + 4] = c.B;
      outlineVertices[i*6 + 5] = c.A;
    }
  }
}

/**
 * \brief Sets the Polygon to an array of new colors.
 * \param c An array of new ColorFloats.
 */
void Polygon::setColor(ColorFloat c[]) {
  if(isFilled) {
    for(int i = 0; i < numberOfVertices; i++) {
      vertices[i*6 + 2] = c[i].R;
      vertices[i*6 + 3] = c[i].G;
      vertices[i*6 + 4] = c[i].B;
      vertices[i*6 + 5] = c[i].A;
    }
  }
  if(hasOutline) {
    for(int i = 0; i < numberOfOutlineVertices; i++) {
      outlineVertices[i*6 + 2] = c[i].R;
      outlineVertices[i*6 + 3] = c[i].G;
      outlineVertices[i*6 + 4] = c[i].B;
      outlineVertices[i*6 + 5] = c[i].A;
    }
  }
}

/**
 * \brief Sets the Polygon to new single fill and outline colors.
 * \param fillColor A new ColorFloat for the Polygon's fill.
 * \param outlineColor A new ColorFloat for the Polygon's outline.
 */
void Polygon::setColor(ColorFloat fillColor, ColorFloat outlineColor) {
  if(!isFilled || !hasOutline) {
    TsglErr("Polygon isn't filled and outlined.");
    return;
  }
  for(int i = 0; i < numberOfVertices; i++) {
    vertices[i*6 + 2] = fillColor.R;
    vertices[i*6 + 3] = fillColor.G;
    vertices[i*6 + 4] = fillColor.B;
    vertices[i*6 + 5] = fillColor.A;
  }
  for(int i = 0; i < numberOfOutlineVertices; i++) {
    outlineVertices[i*6 + 2] = outlineColor.R;
    outlineVertices[i*6 + 3] = outlineColor.G;
    outlineVertices[i*6 + 4] = outlineColor.B;
    outlineVertices[i*6 + 5] = outlineColor.A;
  }
}

/**
 * \brief Gives the Polygon a new monocolored fill and multicolored outline.
 * \param fillColor A new ColorFloat for the Polygon's fill.
 * \param outlineColor A new array of ColorFloats for the Polygon's outline.
 */
void Polygon::setColor(ColorFloat fillColor, ColorFloat outlineColor[]) {
  if(!isFilled || !hasOutline) {
    TsglErr("Polygon isn't filled and outlined.");
    return;
  }
  for(int i = 0; i < numberOfVertices; i++) {
    vertices[i*6 + 2] = fillColor.R;
    vertices[i*6 + 3] = fillColor.G;
    vertices[i*6 + 4] = fillColor.B;
    vertices[i*6 + 5] = fillColor.A;
  }
  for(int i = 0; i < numberOfOutlineVertices; i++) {
    outlineVertices[i*6 + 2] = outlineColor[i].R;
    outlineVertices[i*6 + 3] = outlineColor[i].G;
    outlineVertices[i*6 + 4] = outlineColor[i].B;
    outlineVertices[i*6 + 5] = outlineColor[i].A;
  }
}

/**
 * \brief Gives the Polygon a new multicolored fill and monocolored outline.
 * \param fillColor A new array of ColorFloats for the Polygon's fill.
 * \param outlineColor A new ColorFloat for the Polygon's outline.
 */
void Polygon::setColor(ColorFloat fillColor[], ColorFloat outlineColor) {
  if(!isFilled || !hasOutline) {
    TsglErr("Polygon isn't filled and outlined.");
    return;
  }
  for(int i = 0; i < numberOfVertices; i++) {
    vertices[i*6 + 2] = fillColor[i].R;
    vertices[i*6 + 3] = fillColor[i].G;
    vertices[i*6 + 4] = fillColor[i].B;
    vertices[i*6 + 5] = fillColor[i].A;
  }
  for(int i = 0; i < numberOfOutlineVertices; i++) {
    outlineVertices[i*6 + 2] = outlineColor.R;
    outlineVertices[i*6 + 3] = outlineColor.G;
    outlineVertices[i*6 + 4] = outlineColor.B;
    outlineVertices[i*6 + 5] = outlineColor.A;
  }
}

/**
 * \brief Sets the Polygon to new single fill and outline colors.
 * \param fillColor A new array of ColorFloats for the Polygon's fill.
 * \param outlineColor A new array of ColorFloats for the Polygon's outline.
 */
void Polygon::setColor(ColorFloat fillColor[], ColorFloat outlineColor[]) {
  if(!isFilled || !hasOutline) {
    TsglErr("Polygon isn't filled and outlined.");
    return;
  }
  for(int i = 0; i < numberOfVertices; i++) {
    vertices[i*6 + 2] = fillColor[i].R;
    vertices[i*6 + 3] = fillColor[i].G;
    vertices[i*6 + 4] = fillColor[i].B;
    vertices[i*6 + 5] = fillColor[i].A;
  }
  for(int i = 0; i < numberOfOutlineVertices; i++) {
    outlineVertices[i*6 + 2] = outlineColor[i].R;
    outlineVertices[i*6 + 3] = outlineColor[i].G;
    outlineVertices[i*6 + 4] = outlineColor[i].B;
    outlineVertices[i*6 + 5] = outlineColor[i].A;
  }
}

/**
 * \brief Gets an array of the Polygon's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Polygon::getFillColor() {
  ColorFloat * c = new ColorFloat[numberOfVertices];
  if(isFilled) {
    for(int i = 0; i < numberOfVertices; i++) {
      c[i] = ColorFloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
    }
  } else {
    TsglErr("Polygon isn't filled.");
    for(int i = 0; i < numberOfVertices; i++) {
      c[i] = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }
  return c;
}

/**
 * \brief Gets an array of the Polygon's fill vertex colors.
 * \return c An array of ColorFloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
ColorFloat* Polygon::getOutlineColor() {
  ColorFloat * c = new ColorFloat[numberOfOutlineVertices];
  if(hasOutline) {
    for(int i = 0; i < numberOfOutlineVertices; i++) {
      c[i] = ColorFloat(outlineVertices[i*6 + 2], outlineVertices[i*6 + 3], outlineVertices[i*6 + 4], outlineVertices[i*6 + 5]);
    }
  } else {
    TsglErr("Polygon isn't outlined.");
    for(int i = 0; i < numberOfOutlineVertices; i++) {
      c[i] = ColorFloat(1.0f, 1.0f, 1.0f, 1.0f);
    }
  }
  return c;
}

/**
 * \brief Alters the Polygon's vertex locations.
 * \param deltaX The difference between the new and old vertex X coordinates.
 * \param deltaY The difference between the new and old vertex Y coordinates.
 * \warning This will also alter the Polygon's rotation point if and only if the 
 *          old rotation point was at the Polygon's old center.
 */
void Polygon::moveShapeBy(float deltaX, float deltaY) {
  attribMutex.lock();
  if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
    myRotationPointX += deltaX;
    myRotationPointY += deltaY;
  }
  if(isFilled) {
    for(int i = 0; i < numberOfVertices; i++) {
      vertices[i*6]     += deltaX; //Transpose x
      vertices[(i*6)+1] += deltaY; //Transpose y
    }
  }
  if(hasOutline) {
    for(int i = 0; i < numberOfOutlineVertices; i++) {
      outlineVertices[i*6]     += deltaX; //Transpose x
      outlineVertices[(i*6)+1] += deltaY; //Transpose y
    }
  }
  myCenterX += deltaX;
  myCenterY += deltaY;
  attribMutex.unlock();
}

/**
 * \brief Moves the Polygon to new coordinates.
 * \param x The new center x coordinate.
 * \param y The new center y coordinate.
 * \warning This will also alter the Polygon's rotation point if and only if the 
 *          old rotation point was at the Polygon's old center.
 */
void Polygon::setCenter(float x, float y) {
    float deltaX = x - myCenterX; //Change for x
    float deltaY = y - myCenterY; //Change for y
    attribMutex.lock();
    if(myCenterX == myRotationPointX && myCenterY == myRotationPointY) {
      setRotationPoint(x,y);
    }
    myCenterX = x;
    myCenterY = y;
    if(isFilled) {
      for(int i = 0; i < numberOfVertices; i++) {
        vertices[i*6]     += deltaX; //Transpose x
        vertices[(i*6)+1] += deltaY; //Transpose y
      }
    }
    if(hasOutline) {
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        outlineVertices[i*6]     += deltaX; //Transpose x
        outlineVertices[(i*6)+1] += deltaY; //Transpose y
      }
    }
    attribMutex.unlock();
}


/*!
 * \brief Mutator for the rotation of the Polygon.
 * \details Rotates the Polygon vertices around myRotationPointX, myRotationPointY.
 * \param radians Float value denoting how many radians to rotate the Polygon.
 */
void Polygon::setRotation(float radians) {
  if(radians != currentRotation) {
    float pivotX = myRotationPointX;
    float pivotY = myRotationPointY;
    float s = sin(radians - currentRotation);
    float c = cos(radians - currentRotation);
    currentRotation = radians;
    if(isFilled) {
      for(int i = 0; i < numberOfVertices; i++) {
        float x = vertices[6*i];
        float y = vertices[6*i+1];
        x -= pivotX;
        y -= pivotY;
        float xnew = x * c - y * s;
        float ynew = x * s + y * c;

        x = xnew + pivotX;
        y = ynew + pivotY;
        vertices[6*i] = x;
        vertices[6*i+1] = y;
      }
    }
    if(hasOutline) {
      for(int i = 0; i < numberOfOutlineVertices; i++) {
        float x = outlineVertices[6*i];
        float y = outlineVertices[6*i+1];
        x -= pivotX;
        y -= pivotY;
        float xnew = x * c - y * s;
        float ynew = x * s + y * c;

        x = xnew + pivotX;
        y = ynew + pivotY;
        outlineVertices[6*i] = x;
        outlineVertices[6*i+1] = y;
      }
    }
  }
}

/*!
 * \brief Destructor for the Polygon.
 */
Polygon::~Polygon() {
  if(hasOutline) { 
    delete[] outlineVertices; 
  }
  if(isFilled) {
    delete[] vertices;
  }
}

}