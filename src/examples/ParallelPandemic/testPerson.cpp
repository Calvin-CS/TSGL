/*
 * testPandemic.cpp
 *
 * Usage: ./testPandemic [numPersons] [infectionRate]
            1 <= numPersons <= 100, defaults to 50
            0 <= infectionRate <= 100, defaults to 20
 */

#include <tsgl.h>
#include <cmath>
#include "Person.h"

using namespace tsgl;

void personFunction(Canvas& can) {
   Person * p1 = new Person(0, 0, 50, susceptible);

   p1->draw(can);

    while (can.isOpen()) {
        can.sleep();
    }

    // Deallocate all object memory
    delete p1;

}

int main(int argc, char* argv[]){
    Canvas c(0, -1, 1820, 620, "Pandemic Simulation");
    c.setBackgroundColor(WHITE);
    c.run(personFunction);
}
