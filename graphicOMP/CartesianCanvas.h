/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain, Mark Vander Stel
 * Last Modified: Patrick Crain, 6/20/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

#include "Canvas.h"
#include "Function.h"									// For drawing math functions on the screen

class CartesianCanvas : public Canvas {
	typedef long double Decimal;						// Define the variable type to use for coordinates
private:
	bool	canZoom;									// If we can zoom at all
	Decimal cartWidth,									// maxX-minX, maxY-minY
			cartHeight;
	Decimal minX, maxX, minY, maxY;						// Bounding Cartesian coordinates for the window
	Decimal pixelWidth, pixelHeight;					// cartWidth/window.w(), cartHeight/window.h()
	bool	zoomed;										// Whether we've zoomed since last draw
protected:
	void HandleIO();									// Handler for mouse events
public:
	CartesianCanvas(unsigned int b);					// Default constructor for our CartesianCanvas
	CartesianCanvas(int xx, int yy, int w, int h, Decimal xMin,	Decimal yMin,
					Decimal xMax, Decimal yMax, unsigned int b, char *t = (char*)"");	// Explicit constructor for our CartesianCanvas

	void drawAxes(Decimal x, Decimal y, Decimal dx, Decimal dy);			// Draws axes crossing at the input coordinates
	void drawFunction(const Function* f);									// Draws the Function on the screen
	void drawLine(Decimal x1, Decimal y1, Decimal x2,
					Decimal y2, RGBfloatType color = BLACK);				// Draws a line at the given coordinates with the given color
	void drawPoint(Decimal x, Decimal y, RGBfloatType color = BLACK);		// Draws a point at the given coordinates with the given color
	void drawRectangle(Decimal x, Decimal y, Decimal w,
						Decimal h, RGBfloatType color = BLACK);				// Draws a rectangle at the given coordinates with the given dimensions and color
	void drawShinyPolygon(int size, int x[], int y[], RGBfloatType color[]);// Draws an arbitrary polygon with colored vertices
//	void drawText(const char * s, int x, int y, RGBfloatType color = BLACK);// Draws a string of text at the given position, with the given color
	void drawTriangle(int x1, int y1, int x2, int y2,
						int x3, int y3,	RGBfloatType color = BLACK);		// Draws a triangle with the given vertices and color

	bool	getCanZoom()		{ return canZoom; }							// Accessor for canZoom
	void	getCartesianCoordinates(int screenX, int screenY,
									Decimal &cartX, Decimal &cartY);		// Returns the equivalent Cartesian coordinates for the specified screen ones
	Decimal getCartHeight()		{ return cartHeight; }						// Accessor for cartHeight
	Decimal getCartWidth()		{ return cartWidth; }						// Accessor for cartWidth
	Decimal getPixelWidth()		{ return pixelWidth; }						// Accessor for pixelWidth
	Decimal getPixelHeight()	{ return pixelHeight; }						// Accessor for pixelHeight
	Decimal getMaxY() 			{ return maxY; }							// Accessor for maxY
	Decimal getMaxX() 			{ return maxX; }							// Accessor for maxX
	Decimal getMinX() 			{ return minX; }							// Accessor for minX
	Decimal getMinY() 			{ return minY; }							// Accessor for minY
	void	getScreenCoordinates(Decimal cartX, Decimal cartY,
									int &screenX, int &screenY);			// Returns the equivalent screen coordinates for the specified Cartesian ones
	bool	getZoomed()			{ return zoomed; }							// Accessor for zoomed

	void recomputeDimensions(Decimal xMin, Decimal yMin, Decimal xMax, Decimal yMax);	// Recomputes CartesianCanvas' size variables
	void setZoomed(bool z)		{ zoomed = z; }								// Mutator for zoomed
	void setCanZoom(bool z)		{ canZoom = z; }							// Mutator for canZoom
};

#endif /* CARTESIANCANVAS_H_- */
