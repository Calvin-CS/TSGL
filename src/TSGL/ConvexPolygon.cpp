#include "ConvexPolygon.h"

namespace tsgl {

ConvexPolygon::ConvexPolygon(int numVertices) {
    if (numVertices < 3)
      TsglDebug("Cannot have a polygon with fewer than 3 vertices.");
    numberOfVertices = numVertices;
    size = numberOfVertices * 6;
    current = 0;
    vertices = new float[size];
    init = false;
    geometryType = GL_TRIANGLE_FAN;
}

ConvexPolygon::~ConvexPolygon() {
    delete[] vertices;
}

void ConvexPolygon::addVertex(int x, int y, const ColorFloat &color) {
    if (init) {
      TsglDebug("Cannot add anymore vertices.");
      return;
    }
    vertices[current] = x;
    vertices[current + 1] = y;
    vertices[current + 2] = color.R;
    vertices[current + 3] = color.G;
    vertices[current + 4] = color.B;
    vertices[current + 5] = color.A;
    current += 6;
    if (current == size) init = true;
}

// void ConvexPolygon::draw() {
//     if (!init) {
//       TsglDebug("Cannot draw yet.");
//       return;
//     }
//     glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), vertices, GL_DYNAMIC_DRAW);
//     glDrawArrays(GL_TRIANGLE_FAN, 0, length);
// }

// int ConvexPolygon::getNumberOfVertices() {
//     return length;
// }

// float* ConvexPolygon::getVertices() {
//     return vertices;
// }

// GLenum ConvexPolygon::getGeometryType() {
//     return GL_TRIANGLE_FAN;
// }

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
