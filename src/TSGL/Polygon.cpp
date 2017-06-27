#include "Polygon.h"

namespace tsgl {

Polygon::Polygon(int numVertices) : Shape(numVertices) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    length = numVertices;
    size = length * 6;
    current = 0;
    vertices = new float[size];
    init = false;
    outline = new Polyline(numVertices+1);
}

void Polygon::draw() {
    if (!init) {
      TsglDebug("Cannot draw yet.");
      return;
    }
    glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLE_FAN, 0, length);
}

void Polygon::setCenter(int x, int y) {
  Shape::setCenter(x, y);
  outline->setCenter(x, y);
}

void Polygon::drawOutline() {
  outline->draw();
}

void Polygon::addVertex(int x, int y, const ColorFloat& color, const ColorFloat& outlineColor) {
  Shape::addVertex(x, y, color);
  outline->addVertex(x, y, outlineColor);
  if( init ) {
    outline->addVertex(vertices[0], vertices[1], outlineColor );
  }
}

float* Polygon::getVerticesPointerForRenderer() {
  //TODO: return a pointer to an array of vertices that is formatted correctly for the new renderer
  float* temp = new float[1];
  return temp;
}

void Polygon::runTests() {
    TsglDebug("Testing Polygon class...");
    tsglAssert(testAddVertex(), "Unit test for adding vertices failed");
    TsglDebug("Unit tests for Polygon complete.");
    std::cout << std::endl;
}

bool Polygon::testAddVertex() {
    int passed = 0;
    int failed = 0;
    Polygon c1(4);  //Start with a polygon that has the capacity for four vertices
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
      TsglErr("This many tests for Polygon passed: ");
      std::cout << " " << passed << std::endl;
      TsglErr("This many tests for Polygon failed: ");
      std::cout << " " << failed << std::endl;
      return false;
    }
}

}
