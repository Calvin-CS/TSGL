/*
 * testUnits.cpp runs the unit tests for the TSGL library.
 *
 *  Created on: Jun 4, 2015
 *      Author: cpd5
 */

#include "tsgl.h"

using namespace tsgl;

int main() {
   std::cout << "NOTE: " << std::endl;
   TsglDebug("This color means either a set of tests is being initiated or tests have passed.");
   TsglErr("This color means tests have failed.");
   std::cout << "Begin unit testing...." << std::endl << std::endl;
   Canvas::runTests();
   TextureHandler::runTests();
   ConcavePolygon::runTests();
}
