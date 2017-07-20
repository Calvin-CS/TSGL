/*
 * testUnits.cpp runs the unit tests for the TSGL library.
 *
 * Usage: ./testUnits
 */

#include "tsgl.h"

using namespace tsgl;

int main() {
  //Quick note about the tests...
   std::cout << "NOTE: " << std::endl;
   TsglDebug("This color means either a set of tests is being initiated or tests have passed.");
   TsglErr("This color means tests have failed.");
   //Begin Unit testing
   std::cout << "Begin unit testing...." << std::endl << std::endl;
   Canvas::runTests();   // Canvas (Image test included)
   TextureHandler::runTests();   // TextureHandler
   ConcavePolygon::runTests();   // ConcavePolygon
   ConvexPolygon::runTests();    // ConvexPolygon
   CartesianCanvas::runTests();  // CartesianCanvas
   std::cout << std::endl;
   TsglDebug("All Unit Tests have completed!");
}
