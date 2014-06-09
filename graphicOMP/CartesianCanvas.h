/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/9/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

class CartesianCanvas : public Canvas {
	typedef void (*cfcall)(CartesianCanvas* const);	//Define a type for our callback function pointer
private:
	double minX, maxX, minY, maxY;
	double cartWidth, cartHeight;
	double pixelWidth, pixelHeight;
	cfcall cartesianUpdateFunc;
public:
	inline CartesianCanvas(cfcall c, int b);										//Default constructor for our Canvas
	inline CartesianCanvas(cfcall c, int xx, int yy, int w, int h,
			double xMin, double yMin, double xMax, double yMax, int b, char *t);
	inline void callUpdate();											//Actually calls updateFunc (needed to avoid typing errors)
	inline void getScreenCoordinates(double cartX, double cartY, double &screenX, double &screenY);
	inline void getCartesianCoordinates(double screenX, double screenY, double &cartX, double &cartY);
	inline double getPixelWidth() { return pixelWidth; }				//Accessor for pixelWidth
	inline double getPixelHeight() { return pixelHeight; }				//Accessor for pixelHeight
	inline double getMinX() { return minX; }							//Accessor for minX
	inline double getMaxX() { return maxX; }							//Accessor for maxX
	inline double getMinY() { return minY; }							//Accessor for minY
	inline double getMaxY() { return maxY; }							//Accessor for maxY
	inline Point drawPoint(double x, double y);							//Draws a point at the given coordinates
	inline Point drawPointColor(double x, double y,
			int r, int g, int b);										//Draws a point at the given coordinates with the given color
	inline Line drawLine(double x1, double y1, double x2, double y2);	//Draws a line at the given coordinates
	inline Line drawLineColor(double x1, double y1, double x2,
			double y2, int r, int g, int b);							//Draws a line at the given coordinates with the given color
};

/*
 * Default constructor for the CartesianCanvas class
 * Parameter:
 * 		c, a callback to the user's own draw function
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * Returns: a new 800x600 CartesianCanvas with 1-1 pixel correspondence and central origin
 */
CartesianCanvas::CartesianCanvas(cfcall c, int b = -1) : Canvas(NULL, b) {
	if (c == NULL) {
		cartesianUpdateFunc = [](CartesianCanvas* c){};					//Empty lambda function that does nothing
	} else
		cartesianUpdateFunc = c;										//Adds a callback to the user's own draw function
	minX = -400;
	maxX = 400;
	minY = -300;
	maxY = 300;
	cartWidth = maxX-minX;
	cartHeight = maxY-minY;
	pixelWidth = cartWidth / monitorWidth;
	pixelHeight = cartHeight / monitorHeight;
}

/*
 * Explicit constructor for the CartesianCanvas class
 * Parameters:
 * 		c, a callback to the user's own draw function
 * 		xx, the x position of the CartesianCanvas window
 * 		yy, the y position of the CartesianCanvas window
 * 		w, the x dimension of the CartesianCanvas window
 * 		h, the y dimension of the CartesianCanvas window
 * 		xMin, the real number corresponding to the left edge of the CartesianCanvas
 * 		YMin, the real number corresponding to the top edge of the CartesianCanvas
 * 		xMax, the real number corresponding to the right edge of the CartesianCanvas
 * 		xMax, the real number corresponding to the bottom edge of the CartesianCanvas
 * 		b, the buffer size for the Shapes (-1 = no limit)
 * 		t, the title of the window
 * Returns: a new CartesianCanvas with the specified positional/scaling data and title
 */
CartesianCanvas::CartesianCanvas(cfcall c, int xx, int yy, int w, int h,
			double xMin, double yMin, double xMax, double yMax, int b = -1, char* t = 0) :
			Canvas(NULL, xx, yy, w, h, b, t) {
	if (c == NULL) {
		cartesianUpdateFunc = [](CartesianCanvas* c){};					//Empty lambda function that does nothing
	} else
		cartesianUpdateFunc = c;										//Adds a callback to the user's own draw function
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	cartWidth = maxX-minX;
	cartHeight = maxY-minY;
	pixelWidth = cartWidth / monitorWidth;
	pixelHeight = cartHeight / monitorHeight;
}

/*
 * callUpdate simply calls the update function for the CartesianCanvas class
 */
void CartesianCanvas::callUpdate() {
	cartesianUpdateFunc(this);		//Call the user's callback to do work on the Canvas
}

/*
 * getScreenCoordinates takes a pair of Cartesian coordinates and translates them to on-screen coordinates
 * Parameters:
 * 		cartX, the Cartesian x coordinate
 * 		cartY, the Cartesian y coordinate
 * 		screenX, a reference variable to be filled with cartX's window position
 * 		screenY, a reference variable to be filled with cartY's window position
 */
void CartesianCanvas::getScreenCoordinates(double cartX, double cartY, double &screenX, double &screenY) {
	screenX = (cartX-minX)/cartWidth*monitorWidth;
	screenY = (cartY-minY)/cartHeight*monitorHeight;
}

/*
 * getCartesianCoordinates takes a pair of on-screen coordinates and translates them to Cartesian coordinates
 * Parameters:
 * 		screenX, the window's x coordinate
 * 		screenY, the window's y coordinate
 * 		cartX, a reference variable to be filled with screenX's Cartesian position
 * 		cartY, a reference variable to be filled with screenY's Cartesian position
 */
void CartesianCanvas::getCartesianCoordinates(double screenX, double screenY, double &cartX, double &cartY) {
	cartX = (screenX*cartWidth)/monitorWidth + minX;
	cartY = (screenY*cartHeight)/monitorHeight + minY;
}

/*
 * drawPoint draws a point at the given coordinates
 * Parameters:
 * 		x, the x position of the point
 * 		y, the y position of the point
 * 	Returns: a new point at the Cartesian-adjusted position
 */
Point CartesianCanvas::drawPoint(double x, double y) {
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
 * 	Returns: a new point at the Cartesian-adjusted position with the specified color
 */
Point CartesianCanvas::drawPointColor(double x, double y, int r, int g, int b) {
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
 * 	Returns: a new line with Cartesian-adjusted coordinates
 */
Line CartesianCanvas::drawLine(double x1, double y1, double x2, double y2) {
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
 * 	Returns: a new line with Cartesian-adjusted coordinates and the specified color
 */
Line CartesianCanvas::drawLineColor(double x1, double y1, double x2, double y2, int r, int g, int b) {
	double actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(l);						//Push it onto our drawing queue
	return *l;								//Return a pointer to our new Point
}

#endif /* CARTESIANCANVAS_H_- */
