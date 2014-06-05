// Shape provides a base class from which to extend other drawable shapes
//
// Last Modified: Patrick Crain, 6/5/2014

#ifndef SHAPE_H_
#define SHAPE_H_

class Shape {
protected:
	int myR, myG, myB; 							//Color data for the shape
	bool useDefaultColor;						//Whether or not the shape uses the global drawing color
public:
	inline Shape();								//Default constructor for the Shape
	inline Shape(int r, int g, int b);			//Explicit constructor for the Shape
	inline virtual ~Shape();					//Default destructor for the Shape
	inline void setColor(int r, int g, int b);	//Mutator for the Shape's color
	inline int getColorR();						//Accessor for the red component of the Shape's color
	inline int getColorG();						//Accessor for the green component of the Shape's color
	inline int getColorB();						//Accessor for the blue component of the Shape's color
	inline bool getUsesDefaultColor();			//Accessor for useDefaultColor
	inline virtual void draw() = 0;				//Abstract method for actually drawing the shape
};


/*
 * Default constructor for the Shape class
 * Returns: a new Shape using the global drawing color
 */
Shape::Shape() {
	myR = -1;	//Set all colors to -1 so our accessors don't return a color
	myG = -1;
	myB = -1;
	useDefaultColor = true;
}

/*
 * Explicit constructor for the Shape class
 * Parameters:
 * 		r, the red component
 * 		g, the green component
 * 		b, the blue component
 * Returns: a new Shape with the specified color
 */
Shape::Shape(int r, int g, int b) {
	myR = r;
	myG = g;
	myB = b;
	useDefaultColor = false;	//We no longer use the global drawing color (if we did before)
}

//Destructor stub for the Shape class
Shape::~Shape() {}

/*
 * setColor sets the color for the Shape class
 * Parameters:
 * 		r, the red component
 * 		g, the red component
 * 		b, the red component
 */
void Shape::setColor(int r, int g, int b) {
	myR = r;
	myG = g;
	myB = b;
	useDefaultColor = false;	//We no longer use the global drawing color (if we did before)
}

// Accessor functions for the individual components of the shape's color
int Shape::getColorR() { return myR; }
int Shape::getColorG() { return myG; }
int Shape::getColorB() { return myB; }

// Accessor function for useDefaultColor
bool Shape::getUsesDefaultColor() { return useDefaultColor; }

#endif /* SHAPER_H_ */
