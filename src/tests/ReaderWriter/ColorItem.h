/**
 * ColorItem.h provides a ColorItem class for the Reader-Writer visualization.
 * ColorItem is a drawable object with a color, Canvas, and coordinates.
 */

#ifndef COLORITEM_H_
#define COLORITEM_H_

//#include <tsgl.h>
#include "../../TSGL/tsgl.h"
using namespace tsgl;

/**
 * \class ColorItem
 * \brief Represents a colored square
 * \details Has a color, coordinates, and Canvas
 */
class ColorItem {
public:
	ColorItem(); //Default constructor
	ColorItem(Canvas & can, ColorInt color, int index);
	ColorItem& operator=(const ColorItem& original);
	bool operator==(ColorItem& original);
	int getX() { return myX; } 				//Access item's x coordinate
	int getY() { return myY; } 				//Access item's y coordinate
	int getId() { return myId; } 			//Access item's id
	ColorInt getColor() { return myColor; } //Access stored ColorInt
	void draw(); 							//Draw ColorItem onto the Canvas
	static const int width, dataX, dataY, dataHeight, dataWidth; //constants for display

private:
	ColorInt myColor;
	int myX, myY; 	//Top left coorindates
	int myId; 		//Id number, also index in Vec
	Canvas * myCan; //Handle to the Canvas
};

#endif /*COLORITEM_H_*/
