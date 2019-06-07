#include "ConvexPolygon.h"

namespace tsgl {

 /*!
  * \brief Explicitly constructs a new ConvexPolygon.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the complete ConvexPolygon will have.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *   \param outlined Whether or not the ConvexPolygon is outlined.
  * \return A new ConvexPolygon with a buffer for storing the specified numbered of vertices.
  */
ConvexPolygon::ConvexPolygon(int numVertices, bool filled, bool outlined) : Polygon(numVertices) {
    setup(numVertices, filled, outlined);
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with monocolored fill or outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color The color of the ConvexPolygon.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *     (set to true by default).
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat color, bool filled) : Polygon(numVertices) {
    setup(numVertices, filled, !filled);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with multicolored fill or outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param color An array of colors for the ConvexPolygon.
  *   \param filled Whether or not the ConvexPolygon is filled.
  *     (set to true by default).
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat color[], bool filled) : Polygon(numVertices) {
    setup(numVertices, filled, !filled);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with different monocolored fill and outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor The color of the ConvexPolygon's fill.
  *   \param outlineColor The color of the ConvexPolygon's outline.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with multicolored fill and monocolored outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor An array of colors for the ConvexPolygon's fill.
  *   \param outlineColor The color of the ConvexPolygon's outline.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with monocolored fill and multicolored outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor The color of the ConvexPolygon's fill.
  *   \param outlineColor An array of colors for the ConvexPolygon's outline.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor[i]);
    }
}

 /*!
  * \brief Explicitly constructs a new ConvexPolygon with different multicolored fill and outline.
  * \details Explicit constructor for a Convex Polygon object.
  *   \param numVertices The number of vertices the ConvexPolygon has.
  *   \param x An array of x values for the vertices.
  *   \param y An array of y values for the vertices.
  *   \param fillColor An array of colors for the ConvexPolygon's fill.
  *   \param outlineColor An array of colors for the ConvexPolygon's outline.
  * \return A new ConvexPolygon with the specified vertices and color.
  */
ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor[i]);
    }
}

/*!
 * \brief private helper method that works with the constructor
 * \details Defines a lot of instance variables, initializes vertices and outlineVertices arrays
 *    \param numVertices Number of vertices on the ConvexPolygon
 *    \param filled Whether or not the ConvexPolygon is filled.
 *    \param outlined Whether or not the ConvexPolygon is outlined.
 */
void ConvexPolygon::setup(int numVertices, bool filled, bool outlined) {
    numberOfVertices = numVertices;
    numberOfOutlineVertices = numVertices;
    size = numberOfVertices * 6;
    if(filled) {
      vertices = new float[size];
    }
    if(outlined) {
      outlineVertices = new float[size];
    }
    geometryType = GL_TRIANGLE_FAN;
    isFilled = filled;
    hasOutline = outlined;
}

/*!
 * \brief Runs the Unit tests.
 * \details Runs the Unit tests for the ConvexPolygon class. addVertex() is tested.
 */
void ConvexPolygon::runTests() {
    TsglDebug("Testing ConvexPolygon class...");
    tsglAssert(testAddVertex(), "Unit test for adding vertices failed");
    TsglDebug("Unit tests for ConvexPolygon complete.");
    std::cout << std::endl;
}

bool ConvexPolygon::testAddVertex() {
    int passed = 0;
    int failed = 0;
    ConvexPolygon c1(4);  //Start with a polygon that has the capacity for four vertices
    //Test 1: Adding a single vertex
    if(c1.size == 24 && c1.current == 0) {   //As it should be...
      c1.addVertex(20, 30, BLACK);  //Add a vertex; only the current should change...
      if(c1.size == 24 && c1.current == 6) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 1, One vertex for testAddVertex() failed!");
      }
    }

    //Test 2: Filling the polygon vertices completely
    //Add three more vertices, checking if the current has changed correspondingly
    c1.addVertex(20, 30, BLACK);
    c1.addVertex(20, 30, BLACK);
    c1.addVertex(20, 30, BLACK);

    //The size should've remained the same but the current should now equal the size
    if(c1.size == 24 && c1.current == 24) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Filling vertices completely for testAddVertex() failed!");
    }

    //Test 3: Attempting to add another vertex to a filled vertices....
    c1.addVertex(20, 30, BLACK);
    //The size and current should've stayed the same
    if(c1.size == 24 && c1.current == 24) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 3, Add more beyond capacity for testAddVertex() failed!");
    }

    if(passed == 3 && failed == 0) {
      TsglDebug("Unit test for adding vertices passed!");
      return true;
    } else {
      TsglErr("This many tests for ConvexPolygon passed: ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many tests for ConvexPolygon failed: ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}

}
