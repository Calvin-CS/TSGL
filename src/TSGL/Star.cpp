#include "Star.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Star with monocolored fill or outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param color The color of the star.
  *   \param filled Whether the star should be filled
  *     (set to true by default).
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat color, bool filled, bool ninja) : ConcavePolygon(points*2, filled, !filled) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), color);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), color);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), color);
  }
}

 /*!
  * \brief Explicitly constructs a new Star with multicolored fill or outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param color An array of colors for the star.
  *   \param filled Whether the star should be filled
  *     (set to true by default).
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat color[], bool filled, bool ninja) : ConcavePolygon(points*2, filled, !filled) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), color[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), color[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), color[i]);
  }
}

 /*!
  * \brief Explicitly constructs a new Star with different monocolored fill and outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param fillColor The color of the star's fill.
  *   \param outlineColor The color of the star's outline.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor, bool ninja) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), fillColor, outlineColor);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), fillColor, outlineColor);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), fillColor, outlineColor);
  }
}

 /*!
  * \brief Explicitly constructs a new Star with multicolored fill and monocolored outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param fillColor An array of colors for the star's fill.
  *   \param outlineColor The color of the star's outline.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor, bool ninja) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), fillColor[i], outlineColor);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), fillColor[i], outlineColor);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), fillColor[i], outlineColor);
  }
}

 /*!
  * \brief Explicitly constructs a new Star with monocolored fill and multicolored outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param fillColor The color of the star's fill.
  *   \param outlineColor An array of colors for the star's outline.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat fillColor, ColorFloat outlineColor[], bool ninja) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), fillColor, outlineColor[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), fillColor, outlineColor[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), fillColor, outlineColor[i]);
  }
}

 /*!
  * \brief Explicitly constructs a new Star with different multicolored fill and outline.
  * \details This function draws a star with the given center,
  *   radius, points, and color.
  *   \param x The x coordinate of the star's center.
  *   \param y The y coordinate of the star's center.
  *   \param radius The radius of the star in pixels.
  *   \param points The number of points to use in the star.
  *   \param fillColor An array of colors for the star's fill.
  *   \param outlineColor An array of colors for the star's outline.
  *   \param ninja The ninja setting of the star, making the star points spin differently if true
  *     (set to false by default).
  */
Star::Star(float x, float y, float radius, int points, ColorFloat fillColor[], ColorFloat outlineColor[], bool ninja) : ConcavePolygon(points*2, true, true) {
  //TODO: maybe take "ninja" out, decide how we want the stars to be
  myRadius = radius;
  myPoints = points;
  float delta = 2.0f / points * PI;
  for(int i = 0; i < points; ++i) {
    addVertex(x+(radius/2)*cos(i*delta), y+(radius/2)*sin(i*delta), fillColor[i], outlineColor[i]);
    if( ninja )
      addVertex(x+(radius*cos(i*delta)), y+(radius*sin(i*delta)), fillColor[i], outlineColor[i]);
    else
      addVertex(x+(radius*cos((i+0.5)*delta)), y+(radius*sin((i+0.5)*delta)), fillColor[i], outlineColor[i]);
  }
}
}