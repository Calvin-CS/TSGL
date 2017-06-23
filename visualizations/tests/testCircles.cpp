#include <tsgl.h>
using namespace tsgl;

int main(int argc, char *argv[]) {
  Canvas myCan(0, 0, 500, 500, "Testing Circles");
  myCan.start();

  for(int x = 50; x < 500; x+=100) {
    for(int y = 50; y < 500; y+=100) {
      Circle * newCircle = new Circle(x, y, (x/10)+y/50, (x/10)+y/50);
      myCan.add(newCircle);
    }
  }

  //Conclusion: it is about reasonable for a circle's number of sides to == its radius

  myCan.wait();

  return 0;
}
