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
    numberOfVertices = verticalSections*horizontalSections*2;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myRadius = radius;
    attribMutex.unlock();
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
			addVertex(myRadius*(sin((a*PI)/(verticalSections/2)))*(sin((b*PI)/horizontalSections)), myRadius*(cos((a*PI)/(verticalSections/2))), myRadius*(cos((b*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c);
			addVertex(myRadius*(sin((a*PI)/(verticalSections/2)))*(sin(((b+1)*PI)/horizontalSections)), myRadius*(cos((a*PI)/(verticalSections/2))), myRadius*(cos(((b+1)*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c);
		}
	}
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
    numberOfVertices = verticalSections*horizontalSections*2;
    vertices = new GLfloat[numberOfVertices * 3];
    colors = new GLfloat[numberOfVertices * 4];
    myRadius = radius;
    attribMutex.unlock();
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
			addVertex(myRadius*(sin((a*PI)/(verticalSections/2)))*(sin((b*PI)/horizontalSections)), myRadius*(cos((a*PI)/(verticalSections/2))), myRadius*(cos((b*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c[b]);
			addVertex(myRadius*(sin((a*PI)/(verticalSections/2)))*(sin(((b+1)*PI)/horizontalSections)), myRadius*(cos((a*PI)/(verticalSections/2))), myRadius*(cos(((b+1)*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2))), c[b]);
		}
	}
}

/**
 * \brief Sets the Sphere to a new color.
 * \param c The new ColorGLfloat.
 */
void Sphere::setColor(ColorGLfloat c) {
    // attribMutex.lock();
    // for(int i = 0; i < numberOfVertices; i++) {
    //     vertices[i*6 + 2] = c.R;
    //     vertices[i*6 + 3] = c.G;
    //     vertices[i*6 + 4] = c.B;
    //     vertices[i*6 + 5] = c.A;
    // }
    // attribMutex.unlock();
}

/**
 * \brief Sets the Sphere to an array of new colors.
 * \param c An array of new ColorGLfloats.
 */
void Sphere::setColor(ColorGLfloat c[]) {
    // attribMutex.lock();
    // for(int i = 0; i < numberOfVertices; i++) {
    //     vertices[i*6 + 2] = c[i].R;
    //     vertices[i*6 + 3] = c[i].G;
    //     vertices[i*6 + 4] = c[i].B;
    //     vertices[i*6 + 5] = c[i].A;
    // }
    // attribMutex.unlock();
}

/**
 * \brief Gets an array of the Sphere's fill vertex colors.
 * \return c An array of ColorGLfloats.
 * \warning This method allocates memory. The caller is responsible for deallocating it.
 */
// ColorGLfloat* Sphere::getColor() {
//     ColorGLfloat * c = new ColorGLfloat[numberOfVertices];
//     for(int i = 0; i < numberOfVertices; i++) {
//         c[i] = ColorGLfloat(vertices[i*6 + 2], vertices[i*6 + 3], vertices[i*6 + 4], vertices[i*6 + 5]);
//     }
//     return c;
// }

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
    GLfloat ratio = radius/myRadius;
    myRadius = radius;
    for (int i = 0; i < numberOfVertices * 3; i++) {
        vertices[i] *= ratio;
    }
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
	for(int b=0;b<horizontalSections;b++)
	{
		for(int a=0;a<verticalSections;a++)
		{
            vertices[(b*verticalSections + a) * 2 * 3] = myRadius*(sin((a*PI)/(verticalSections/2)))*(sin((b*PI)/horizontalSections));
            vertices[(b*verticalSections + a) * 2 * 3 + 1] = myRadius*(cos((a*PI)/(verticalSections/2)));
            vertices[(b*verticalSections + a) * 2 * 3 + 2] = myRadius*(cos((b*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2)));
            vertices[(b*verticalSections + a) * 2 * 3 + 3] = myRadius*(sin((a*PI)/(verticalSections/2)))*(sin(((b+1)*PI)/horizontalSections));
            vertices[(b*verticalSections + a) * 2 * 3 + 4] = myRadius*(cos((a*PI)/(verticalSections/2)));
            vertices[(b*verticalSections + a) * 2 * 3 + 5] = myRadius*(cos(((b+1)*PI)/horizontalSections))*(sin((a*PI)/(verticalSections/2)));
		}
	}
    attribMutex.unlock();
}

/*!
 * \brief Destructor for the Sphere.
 */
Sphere::~Sphere() { }

}