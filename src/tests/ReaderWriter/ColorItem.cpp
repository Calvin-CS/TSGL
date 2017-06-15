#include "ColorItem.h"

const int ColorItem::width = 20; //Width of each object
const int ColorItem::dataX = 200, ColorItem::dataY = 670; //Bottom left coordinates of the data area
const int ColorItem::dataHeight = 600, ColorItem::dataWidth = 200; //Dimensions of the data area

/**
 * \brief Default constructor for the ColorItem class.
 * \return: the constructed ColorItem
 */
ColorItem::ColorItem() {
	myColor = ColorInt(0, 0, 0);
	myX = myY = myId = 0;
	myCan = NULL;
}

/**
 * \brief Explicit constructor for the ColorItem class.
 * \param can, a handle to the Canvas
 * \param color, a ColorInt to store this ColorItem's color
 * \return: the constructed ColorItem
 */
ColorItem::ColorItem(Canvas & can, ColorInt color, int index) {
	myCan = &can;
	myColor = color;
	myId = index;
	myX = ColorItem::dataX + index%(200/width) * width; // start of data + column
	myY = ColorItem::dataY - (index/(200/width) + 1) * width; // start of data + row
}

/**
 * \brief Copy constructor
 * \details Copies color, canvas, and coordinates to this ColorItem and returns this
 * \param original, the ColorItem to copy from
 * \return: this ColorItem
 */
ColorItem& ColorItem::operator=(const ColorItem& original) {
	myX = original.myX;
	myY = original.myY;
	myColor = original.myColor;
	myCan = original.myCan;
	return *this;
}

/**
 * \brief operator== checks equivalence between this and another ColorItem
 * \param other, the ColorItem for comparison
 * \return: a boolean, true if all fields are equivalent, false otherwise
 */
bool ColorItem::operator==(ColorItem& other) {
	if( myCan != other.myCan )
		return false;
	if( myX != other.myX )
		return false;
	if( myY != other.myY )
		return false;
	return true;
}

/**
 * \brief Draws the ColorItem to the Canvas
 */
void ColorItem::draw() {
	myCan->drawRectangle(myX, myY, myX+width, myY+width, myColor, true);
}
