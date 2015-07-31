/*
 * ShadedVoronoi.h
 */

#ifndef SHADEDVORONOI_H_
#define SHADEDVORONOI_H_

#include "Voronoi.h"

/*!
 * \class ShadedVoronoi
 * \brief Voronoi diagram, only shaded!
 * \details Creates a Shaded Voronoi diagram.
 * \details Similar to a Voronoi diagram, but with some parts shaded in.
 * \details Subclass of the Voronoi class.
 * \see Voronoi class.
 */
class ShadedVoronoi : public Voronoi {
private:
  int * myKValue2;
  ColorFloat myColor[MY_POINTS];

public:

  /*!
   * \brief Explicitly construct a ShadedVoronoi object.
   * \details Explicit constructor for a ShadedVoronoi object.
   * \param can Reference to the Canvas to draw to.
   * \return The constructed ShadedVoronoi object.
   */
  ShadedVoronoi(Canvas& can);

  /*!
   * \brief Draw the ShadedVoronoi object.
   * \details Actually draws the ShadedVoronoi object onto the Canvas.
   * \param can Reference to the Canvas to draw to.
   */
  void draw(Canvas& can);

  /*!
   * \brief Destroy a ShadedVoronoi object.
   * \details Destructor for a ShadedVoronoi object.
   * \return The memory allocated to a ShadedVoronoi object.
   */
  virtual ~ShadedVoronoi();
};

#endif /* SHADEDVORONOI_H_ */
