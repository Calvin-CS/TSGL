//
// Basic Shape class build on top of FLTK library.
//
// Last Modified: Patrick Crain, 6/4/2014

#ifndef SHAPE_H_
#define SHAPE_H_

class Shape {
protected:
	int myR, myG, myB; //Color data
	bool useDefaultColor;
public:
	inline Shape();
	inline Shape(int r, int g, int b);
	inline virtual ~Shape();
	inline void setColor(int r, int g, int b);
	inline int getColorR();
	inline int getColorG();
	inline int getColorB();
	inline bool getUsesDefaultColor();
	inline virtual void draw() = 0;
};

Shape::Shape() {
	useDefaultColor = true;
	setColor(0,0,0);
}
Shape::Shape(int r, int g, int b) {
	useDefaultColor = false;
	setColor(r,g,b);
}
Shape::~Shape() {}
//Sets the color for drawing
void Shape::setColor(int r, int g, int b) {
	myR = r;
	myG = g;
	myB = b;
}
int Shape::getColorR() { return myR; }
int Shape::getColorG() { return myG; }
int Shape::getColorB() { return myB; }
bool Shape::getUsesDefaultColor() { return useDefaultColor; }

#endif /* SHAPER_H_ */
