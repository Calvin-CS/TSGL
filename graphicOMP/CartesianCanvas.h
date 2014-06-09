/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/9/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

class CartesianCanvas : public Canvas {
	typedef void (*fcall)(Canvas* const);	//Define a type for our callback function pointer
private:
	double minX, maxX, minY, maxY;
	double cartWidth, cartHeight;
	double pixelWidth, pixelHeight;
public:
	inline CartesianCanvas(fcall c, int b);										//Default constructor for our Canvas
	inline CartesianCanvas(fcall c, int xx, int yy, int w, int h,
			double xMin, double yMin, double xMax, double yMax, int b, char *t);
	inline void getScreenCoordinates(double cartX, double cartY, double &screenX, double &screenY);
	inline void getCartesianCoordinates(double screenX, double screenY, double &cartX, double &cartY);
	inline double getPixelWidth() { return pixelWidth; }
	inline double getpixelHeight() { return pixelHeight; }
	inline Point drawPoint(int x, int y);								//Draws a point at the given coordinates
	inline Point drawPointColor(int x, int y, int r, int g, int b);		//Draws a point at the given coordinates with the given color
	inline Line drawLine(int x1, int y1, int x2, int y2);				//Draws a line at the given coordinates
	inline Line drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b);	//Draws a line at the given coordinates with the given color
};

/*
 * Default constructor for the CartesianCanvas class
 * Parameter:
 * 		c, a callback to the user's own draw function
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 CartesianCanvas with 1-1 pixel correspondence and central origin
 */
CartesianCanvas::CartesianCanvas(fcall c, int b = -1) : Canvas (c, b) {
	minX = -400;
	maxX = 400;
	minY = -300;
	maxY = 300;
	cartWidth = maxX-minX;
	cartHeight = maxY-minY;
	pixelWidth = cartWidth / monitorWidth;
	pixelHeight = cartHeight / monitorHeight;
}

CartesianCanvas::CartesianCanvas(fcall c, int xx, int yy, int w, int h,
			double xMin, double yMin, double xMax, double yMax, int b = -1, char* t = 0) :
			Canvas(c, xx, yy, w, h, b, t) {
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	cartWidth = maxX-minX;
	cartHeight = maxY-minY;
	pixelWidth = cartWidth / monitorWidth;
	pixelHeight = cartHeight / monitorHeight;
}

void CartesianCanvas::getScreenCoordinates(double cartX, double cartY, double &screenX, double &screenY) {
	screenX = (cartX-minX)/cartWidth*monitorWidth;
	screenY = (cartY-minY)/cartHeight*monitorHeight;
}

void CartesianCanvas::getCartesianCoordinates(double screenX, double screenY, double &cartX, double &cartY) {
	cartX = (screenX*cartWidth)/monitorWidth + minX;
	cartY = (screenY*cartHeight)/monitorHeight + minY;
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the given position
 */
Point CartesianCanvas::drawPoint(int x, int y) {
	double actualX, actualY;
	getScreenCoordinates(x,y,actualX,actualY);
	Point* p = new Point(actualX,actualY);	//Creates the Point with the specified coordinates
	myShapes->push(p);			//Push it onto our drawing queue
	return *p;					//Return a pointer to our new Point
}

/*
 * drawPointColor draws a point at the given coordinates with the given color
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new point at the given position with the specified color
 */
Point CartesianCanvas::drawPointColor(int x, int y, int r, int g, int b) {
	double actualX, actualY;
	getScreenCoordinates(x,y,actualX,actualY);
	Point* p = new Point(actualX,actualY,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(p);					//Push it onto our drawing queue
	return *p;							//Return a pointer to our new Point
}

/*
 * drawLine draws a line at the given coordinates
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 	Returns: a new line with the given coordinates
 */
Line CartesianCanvas::drawLine(int x1, int y1, int x2, int y2) {
	double actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2);//Creates the Point with the specified coordinates
	myShapes->push(l);				//Push it onto our drawing queue
	return *l;						//Return a pointer to our new Point
}

/*
 * drawLineColor draws a line at the given coordinates with the given color
 * Parameters:
 * 		x1, the x position of the start of the line
 * 		y1, the y position of the start of the line
 *		x2, the x position of the end of the line
 * 		y2, the y position of the end of the line
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 * 	Returns: a new line with the given coordinates and the specified color
 */
Line CartesianCanvas::drawLineColor(int x1, int y1, int x2, int y2, int r, int g, int b) {
	double actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(l);						//Push it onto our drawing queue
	return *l;								//Return a pointer to our new Point
}

#endif /* CARTESIANCANVAS_H_- */
