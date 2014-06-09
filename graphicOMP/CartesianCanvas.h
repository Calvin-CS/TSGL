/*
 * CartesianCanvas.h provides a Canvas with a Cartesian coordinate system for ease of plotting
 *
 * Authors: Patrick Crain
 * Last Modified: Patrick Crain, 6/9/2014
 */

#ifndef CARTESIANCANVAS_H_
#define CARTESIANCANVAS_H_

class CartesianCanvas : public Canvas {
	typedef long double Type;
	typedef void (*cfcall)(CartesianCanvas* const);	//Define a type for our callback function pointer
private:
	Type minX, maxX, minY, maxY;
	Type cartWidth, cartHeight;
	Type pixelWidth, pixelHeight;
	cfcall cartesianUpdateFunc;
	Type xError, yError;
public:
	inline CartesianCanvas(cfcall c, int b);										//Default constructor for our Canvas
	inline CartesianCanvas(cfcall c, int xx, int yy, int w, int h,
			Type xMin, Type yMin, Type xMax, Type yMax, int b, char *t);
	inline void callUpdate();											//Actually calls updateFunc (needed to avoid typing errors)
	inline void getScreenCoordinates(Type cartX, Type cartY, Type &screenX, Type &screenY);
	inline void getCartesianCoordinates(Type screenX, Type screenY, Type &cartX, Type &cartY);
	inline Type getPixelWidth() { return pixelWidth; }				//Accessor for pixelWidth
	inline Type getPixelHeight() { return pixelHeight; }				//Accessor for pixelHeight
	inline Type getMinX() { return minX; }							//Accessor for minX
	inline Type getMaxX() { return maxX; }							//Accessor for maxX
	inline Type getMinY() { return minY; }							//Accessor for minY
	inline Type getMaxY() { return maxY; }							//Accessor for maxY
	inline Point drawPoint(Type x, Type y);							//Draws a point at the given coordinates
	inline Point drawPointColor(Type x, Type y,
			int r, int g, int b);										//Draws a point at the given coordinates with the given color
	inline Line drawLine(Type x1, Type y1, Type x2, Type y2);	//Draws a line at the given coordinates
	inline Line drawLineColor(Type x1, Type y1, Type x2,
			Type y2, int r, int g, int b);							//Draws a line at the given coordinates with the given color
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
	xError = 1.0f/monitorWidth;
	yError = 1.0f/monitorHeight;
	pixelWidth = (cartWidth-xError) / monitorWidth;
	pixelHeight = (cartHeight-yError) / monitorHeight;
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
			Type xMin, Type yMin, Type xMax, Type yMax, int b = -1, char* t = 0) :
			Canvas(NULL, xx, yy, w, h, b, t) {
	if (c == NULL) {
		cartesianUpdateFunc = [](CartesianCanvas* c){};					//Empty lambda function that does nothing
	} else
		cartesianUpdateFunc = c;										//Adds a callback to the user's own draw function
	minX = xMin;
	minY = yMin;
	maxX = xMax;
	maxY = yMax;
	xError = 1.0f/monitorWidth;
	yError = 1.0f/monitorHeight;
	cartWidth = (maxX-minX)-xError;
	cartHeight = (maxY-minY)-yError;
	pixelWidth = (cartWidth) / (monitorWidth+xError);
	pixelHeight = (cartHeight) / (monitorHeight+yError);
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
void CartesianCanvas::getScreenCoordinates(Type cartX, Type cartY, Type &screenX, Type &screenY) {
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
void CartesianCanvas::getCartesianCoordinates(Type screenX, Type screenY, Type &cartX, Type &cartY) {
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
Point CartesianCanvas::drawPoint(Type x, Type y) {
	Type actualX, actualY;
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
Point CartesianCanvas::drawPointColor(Type x, Type y, int r, int g, int b) {
	Type actualX, actualY;
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
Line CartesianCanvas::drawLine(Type x1, Type y1, Type x2, Type y2) {
	Type actualX1, actualY1,actualX2, actualY2;
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
Line CartesianCanvas::drawLineColor(Type x1, Type y1, Type x2, Type y2, int r, int g, int b) {
	Type actualX1, actualY1,actualX2, actualY2;
	getScreenCoordinates(x1,y1,actualX1,actualY1);
	getScreenCoordinates(x2,y2,actualX2,actualY2);
	Line* l = new Line(actualX1,actualY1,actualX2,actualY2,r,g,b);	//Creates the Point with the specified coordinates and color
	myShapes->push(l);						//Push it onto our drawing queue
	return *l;								//Return a pointer to our new Point
}

#endif /* CARTESIANCANVAS_H_- */
