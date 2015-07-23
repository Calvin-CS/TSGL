/*
 * testProducerConsumer.cpp
 *
 *  Created on: Jul 23, 2015
 *      Author: cpd5
 */

#include <tsgl.h>

using namespace tsgl;

int main() {
  Canvas c(0, 0, 800, 800, "Producer-Consumer", FRAME);
  c.start();
  c.drawRectangle(100, 200, 500, 300, BLACK, false);
  c.wait();
}

