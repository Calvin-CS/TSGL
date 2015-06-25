/* testSpectrogram.cpp */

#include <tsgl.h>
using namespace tsgl;

const int B = 16;  //Border
const float PI = 3.14159;
void spectrogramFunction(Canvas& can, Canvas& ocan, std::string fname) {
    const int ow = ocan.getWindowWidth(), oh = ocan.getWindowHeight();
    const int cww = can.getWindowWidth(), cwh = can.getWindowHeight();
    float count[NUM_COLORS+1] = {0}, maxCount = 0;
    int xx[NUM_COLORS+1], yy[NUM_COLORS+1] ,maxx[NUM_COLORS+1], maxy[NUM_COLORS+1];
    ColorFloat col[NUM_COLORS+1];
    ColorFloat black[NUM_COLORS+1](BLACK);
    xx[0] = (ow)/2;
    yy[0] = (oh)/2;
    col[0] = WHITE;
    for (int i = 1; i < NUM_COLORS; ++i) {
      xx[i] = (ow + oh*cos((2*PI*i)/NUM_COLORS))/2;
      yy[i] = (oh + oh*sin((2*PI*i)/NUM_COLORS))/2;
      col[i] = ColorHSV(6.0f*i/255.0f,1.0f,1.0f);
      black[i] = col[i] * 0.25f;
    }
    xx[NUM_COLORS] = xx[1];
    yy[NUM_COLORS] = yy[1];
    col[NUM_COLORS] = col[1];
    black[NUM_COLORS] = black[1];
    black[0] = BLACK;
    for (int k = 0; k < NUM_COLORS+1; ++k) {
      maxx[k] = xx[k];
      maxy[k] = yy[k];
    }
    can.drawImage(fname, 0, 0, cww, cwh);
    for (int j = 0; j < cwh; ++j) {
      can.sleepFor(0.01f);
      if (!can.getIsOpen())
        break;
      for (int i = 0; i < cww; ++i) {
        ColorHSV hsv = can.getPoint(i,j);
        if (hsv.H == hsv.H) { //Check for NAN
          int ihue = (MAX_COLOR*hsv.H/6);
          float weight = 1.0f;
          for (int k = 0; k < MAX_COLOR/2; ++k) {
            int index = (ihue + k) % MAX_COLOR;
            count[index] += weight;
            if (count[index] > maxCount)
              maxCount = count[index];
            index = (ihue + MAX_COLOR - k) % MAX_COLOR;
            count[index] += weight;
            if (count[index] > maxCount)
              maxCount = count[index];
            weight *= 0.95f;
          }
        }
      }
      //NEW WAY
      if (maxCount > 0) {
        for (int k = 0; k < MAX_COLOR; ++k) {
          xx[k+1] = (ow + (1.0f*j)/cwh*count[k]*oh*cos((2*PI*k)/NUM_COLORS)/maxCount)/2;
          yy[k+1] = (oh + (1.0f*j)/cwh*count[k]*oh*sin((2*PI*k)/NUM_COLORS)/maxCount)/2;
          col[k+1] = ColorHSV(6.0f*k/255.0f,(1.0f*count[k])/maxCount,1.0f);
        }
        xx[NUM_COLORS] = xx[1];
        yy[NUM_COLORS] = yy[1];
        col[NUM_COLORS] = col[1];
        ocan.clear();
        ocan.drawConvexPolygon(NUM_COLORS+1,maxx,maxy,black,true);
        ocan.drawConvexPolygon(NUM_COLORS+1,xx,yy,col,true);
      }
      //OLD WAY
//      if (maxCount > 0)
//        for (int k = 0; k < MAX_COLOR; ++k)
//          ocan.drawLine(B,B+k,B+((ow-2*B)*count[k])/maxCount,B+k,ColorHSV((6.0f*k)/MAX_COLOR,1.0f,1.0f));
    }
}

int main(int argc, char* argv[]) {
    std::string fname = argc > 1 ? argv[1] : "../assets/colorful_cars.jpg";
    int w, h;
    TextureHandler::getDimensions(fname,w,h);
    Canvas c(-1, Canvas::getDisplayHeight()-h, w, h ,"");
    c.start();
    Canvas c2(-1, 0, 4*MAX_COLOR+B*2, 4*MAX_COLOR + B*2 ,"");
    c2.start();
    spectrogramFunction(c,c2,fname);
    c.wait();
    c2.wait();
}
