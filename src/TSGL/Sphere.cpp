#include "Sphere.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new Sphere.
  * \details Explicit constructor for a Sphere object.
  *   \param x The x coordinate of the center of the Sphere.
  *   \param y The y coordinate of the center of the Sphere.
  *   \param z The z coordinate of the center of the Sphere.
  *   \param radius The Sphere's radius
  *   \param yaw The Sphere's yaw.
  *   \param pitch The Sphere's pitch.
  *   \param roll The Sphere's roll.
  *   \param c A ColorGLfloat for the Sphere's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Sphere with a buffer for storing the specified numbered of vertices.
  */
Sphere::Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c)  : Object3D(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    verticalSections = 36;
    horizontalSections = 20;
    numberOfVertices = numberOfOutlineVertices = verticalSections*horizontalSections*2+1;
    outlineGeometryType = GL_LINES;
    edgesOutlined = false;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    myZScale = radius;
    attribMutex.unlock();  
	for(float b=0;b<horizontalSections;b++)
	{
		for(float a=0;a<verticalSections;a++)
		{
			addVertex(sin((a*PI)/(verticalSections/2))*sin((b*PI)/horizontalSections), cos((a*PI)/(verticalSections/2)), cos((b*PI)/horizontalSections)*sin((a*PI)/(verticalSections/2)), ColorGLfloat(c.R * (1 - 1 * sin(a/verticalSections * PI) / 2), c.G * (1 - 1 * sin(a/verticalSections * PI) / 2), c.B * (1 - 1 * sin(a/verticalSections * PI) / 2), c.A));
			addVertex(sin((a*PI)/(verticalSections/2))*sin(((b+1)*PI)/horizontalSections), cos((a*PI)/(verticalSections/2)), cos(((b+1)*PI)/horizontalSections)*sin((a*PI)/(verticalSections/2)), ColorGLfloat(c.R * (1 - 1 * sin(a/verticalSections * PI) / 2), c.G * (1 - 1 * sin(a/verticalSections * PI) / 2), c.B * (1 - 1 * sin(a/verticalSections * PI) / 2), c.A));
		}
	}
    addVertex(0, 1, 0, ColorGLfloat(c.R, c.G, c.B, c.A));
}

 /*!
  * \brief Explicitly constructs a new Sphere.
  * \details Explicit constructor for a Sphere object.
  *   \param x The x coordinate of the center of the Sphere.
  *   \param y The y coordinate of the center of the Sphere.
  *   \param z The z coordinate of the center of the Sphere.
  *   \param radius The distance from the center of the Sphere's base to each vertex of the base.
  *   \param yaw The Sphere's yaw.
  *   \param pitch The Sphere's pitch.
  *   \param roll The Sphere's roll.
  *   \param c An array of ColorGLfloats for the Sphere's vertex colors.
  * \warning An invariant is held where if radius isn't positive then an error message is given.
  * \return A new Sphere with a buffer for storing the specified numbered of vertices.
  */
Sphere::Sphere(float x, float y, float z, GLfloat radius, float yaw, float pitch, float roll, ColorGLfloat c[])  : Object3D(x, y, z, yaw, pitch, roll)  {
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
    }
    attribMutex.lock();
    geometryType = GL_TRIANGLE_STRIP;
    verticalSections = 36;
    horizontalSections = 20;
    numberOfVertices = numberOfOutlineVertices = verticalSections*horizontalSections*2 + 1;
    outlineGeometryType = GL_LINES;
    edgesOutlined = false;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    myZScale = radius;
    attribMutex.unlock();
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
			addVertex((sin((a*PI)/(verticalSections/2)))*(sin((b*PI)/horizontalSections)), (cos((a*PI)/(verticalSections/2))), (cos((b*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c[b]);
			addVertex((sin((a*PI)/(verticalSections/2)))*(sin(((b+1)*PI)/horizontalSections)), (cos((a*PI)/(verticalSections/2))), (cos(((b+1)*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c[b]);
		}
	}
    addVertex(0, 1, 0, c[horizontalSections]);
}

/**
 * \brief Mutates the Sphere's radius.
 * \param radius The new radius of the Sphere.
 */
void Sphere::setRadius(float radius) {
    if (radius <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius = radius;
    myXScale = radius;
    myYScale = radius;
    myZScale = radius;
    attribMutex.unlock();
}

/**
 * \brief Mutates the Sphere's radius by the parameter amount.
 * \param delta The amount by which to change the radius of the Sphere.
 */
void Sphere::changeRadiusBy(float delta) {
    if (myRadius + delta <= 0) {
        TsglDebug("Cannot have a Sphere with radius less than or equal to 0.");
        return;
    }
    attribMutex.lock();
    myRadius += delta;
    myXScale += delta;
    myYScale += delta;
    myZScale += delta;
    attribMutex.unlock();
}

/**
 * \brief Sets the Sphere to a new color.
 * \param c The new ColorGLfloat.
 */
void Sphere::setColor(ColorGLfloat c) {
    attribMutex.lock();
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
			colors[(b*verticalSections + a)*2*4] = c.R * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 1] = c.G * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 2] = c.B * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 3] = c.A;
			colors[(b*verticalSections + a)*2*4 + 4] = c.R * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 5] = c.G * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 6] = c.B * (1 - 1 * sin(((float)a)/verticalSections * PI) / 2);
            colors[(b*verticalSections + a)*2*4 + 7] = c.A;
		}
	}
    colors[horizontalSections*verticalSections*2*4] = c.R;
    colors[horizontalSections*verticalSections*2*4+1] = c.G;
    colors[horizontalSections*verticalSections*2*4+2] = c.B;
    colors[horizontalSections*verticalSections*2*4+3] = c.A;
    attribMutex.unlock();
}

/**
 * \brief Sets the Sphere to an array of new colors.
 * \param c An array of new ColorGLfloats.
 * \details The array should have 20 ColorGLfloats minimum, one for each horizontal section. 
 */
void Sphere::setColor(ColorGLfloat c[]) {
    attribMutex.lock();
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
			colors[(b*verticalSections + a)*2*4] = c[b].R;
            colors[(b*verticalSections + a)*2*4 + 1] = c[b].G;
            colors[(b*verticalSections + a)*2*4 + 2] = c[b].B;
            colors[(b*verticalSections + a)*2*4 + 3] = c[b].A;
			colors[(b*verticalSections + a)*2*4 + 4] = c[b].R;
            colors[(b*verticalSections + a)*2*4 + 5] = c[b].G;
            colors[(b*verticalSections + a)*2*4 + 6] = c[b].B;
            colors[(b*verticalSections + a)*2*4 + 7] = c[b].A;
		}
	}
    colors[horizontalSections*verticalSections*2*4] = c[horizontalSections].R;
    colors[horizontalSections*verticalSections*2*4+1] = c[horizontalSections].G;
    colors[horizontalSections*verticalSections*2*4+2] = c[horizontalSections].B;
    colors[horizontalSections*verticalSections*2*4+3] = c[horizontalSections].A;
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Sphere.
 */
Sphere::~Sphere() { }

}