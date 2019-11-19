#include "Line.h"

namespace tsgl {

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param color The reference variable to the color of the Line.
 * \return A new Line with the specified endpoints and color.
 */
Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color) : Polyline(2) {
    addVertex(x1, y1, color);
    addVertex(x2, y2, color);
}

/*!
 * \brief Explicitly constructs a new Line.
 * \details This is the constructor for the Line class.
 *      \param x1 The x coordinate of the first endpoint.
 *      \param y1 The y coordinate of the first endpoint.
 *      \param x2 The x coordinate of the second endpoint.
 *      \param y2 The y coordinate of the second endpoint.
 *      \param color An array for the colors of the line endpoints.
 * \return A new Line with the specified endpoints and colors.
 */
Line::Line(int x1, int y1, int x2, int y2, const ColorFloat color[]) : Polyline(2) {
    addVertex(x1, y1, color[0]);
    addVertex(x2, y2, color[1]);
}

/**
 * \brief Moves one end of the Line.
 * \details Moves the end of the line originally specified by (x1, y1).
 *   \param x The new x coordinate.
 *   \param y The new y coordinate.
 */
void Line::setFirstEnd(float x, float y) {
  attribMutex.lock();
  vertices[0] = x;
  vertices[1] = y;
  attribMutex.unlock();
}

/**
 * \brief Moves one end of the Line.
 * \details Moves the end of the line originally specified by (x2, y2).
 *   \param x The new x coordinate.
 *   \param y The new y coordinate.
 */
void Line::setSecondEnd(float x, float y) {
  attribMutex.lock();
  vertices[6] = x;
  vertices[7] = y;
  attribMutex.unlock();
}

/**
 * \brief Calculates the length of the line.
 * \details Finds the distance between the two ends of the line.
 * \return Length of the Line.
 */
float Line::getLength() {
  attribMutex.lock();
  float length = sqrt((vertices[0]-vertices[2])*(vertices[0]-vertices[2])+(vertices[1]-vertices[3])*(vertices[1]-vertices[3]));
  attribMutex.unlock();
  return length;
}
}
