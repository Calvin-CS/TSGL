#include "ConvexPolygon.h"

namespace tsgl {

ConvexPolygon::ConvexPolygon(int numVertices, bool filled, bool outlined) : Polygon(numVertices) {
    setup(numVertices, filled, outlined);
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat color, bool filled) : Polygon(numVertices) {
    setup(numVertices, filled, !filled);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color);
    }
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat color[], bool filled) : Polygon(numVertices) {
    setup(numVertices, filled, !filled);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], color[i]);
    }
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor);
    }
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor);
    }
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor, ColorFloat outlineColor[]) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor, outlineColor[i]);
    }
}

ConvexPolygon::ConvexPolygon(int numVertices, int x[], int y[], ColorFloat fillColor[], ColorFloat outlineColor[]) : Polygon(numVertices) {
    setup(numVertices, true, true);
    for (int i = 0; i < numVertices; i++) {
        addVertex(x[i], y[i], fillColor[i], outlineColor[i]);
    }
}

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
