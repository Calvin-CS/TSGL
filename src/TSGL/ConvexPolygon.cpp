#include "ConvexPolygon.h"

namespace tsgl {

ConvexPolygon::ConvexPolygon(int numVertices, const ColorFloat& color) : Polygon(numVertices, color) { }

//Testing methods
void ConvexPolygon::runTests() {
    TsglDebug("Testing ConvexPolygon class...");
    tsglAssert(testAddVertex(), "Unit test for adding vertices failed");
    TsglDebug("Unit tests for ConvexPolygon complete.");
    std::cout << std::endl;
}

bool ConvexPolygon::testAddVertex() {
    int passed = 0;
    int failed = 0;
    ConvexPolygon c1(4, BLACK);  //Start with a polygon that has the capacity for four vertices
    //Test 1: Adding a single vertex
    if(c1.size == 8 && c1.current == 0) {   //As it should be...
      c1.addVertex(20, 30);  //Add a vertex; only the current should change...
      if(c1.size == 8 && c1.current == 2) {
        passed++;
      } else {
        failed++;
        TsglErr("Test 1, One vertex for testAddVertex() failed!");
      }
    }

    //Test 2: Filling the polygon vertices completely
    //Add three more vertices, checking if the current has changed correspondingly
    c1.addVertex(20, 30);
    c1.addVertex(20, 30);
    c1.addVertex(20, 30);

    //The size should've remained the same but the current should now equal the size
    if(c1.size == 8 && c1.current == 8) {
      passed++;
    } else {
      failed++;
      TsglErr("Test 2, Filling vertices completely for testAddVertex() failed!");
    }

    //Test 3: Attempting to add another vertex to a filled vertices....
    c1.addVertex(20, 30);
    //The size and current should've stayed the same
    if(c1.size == 8 && c1.current == 8) {
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
