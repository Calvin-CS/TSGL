/*
 * testMouse.cpp
 *
 * Usage: ./testMouse <width> <height> <numThreads>
 */

#include <tsgl.h>

using namespace tsgl;

inline float dist(float x1, float y1, float x2, float y2) {
  return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}

inline void scatter(float& f, float max) {
  float am = 1.0f - ((rand() % 20000) / 10000.0f);
  f += max*am;
}

/*!
 *
 */
void auraFunction(Canvas& can, int segs) {
  const float SR2 = sqrt(2);
  const int CW = can.getWindowWidth(), CH = can.getWindowHeight();
  const int ccw = CW/2, cch = CH/2;
  const float ROT = 0.01f;
  const float HUEDELTA = 0.01f;
  const float OFF = (2*PI/segs);
  const float START = PI/2;

  int mx, my;
  ColorHSV   *cf = new ColorHSV[segs];
  bool       *drawn = new bool[segs];
  float      *x1 = new float[segs], *y1 = new float[segs],
             *x2 = new float[segs], *y2 = new float[segs];

  float mangle = 0;
  float mdist = 0;

  for (int i = 0; i < segs; ++i) {
    cf[i] = Colors::highContrastColor(i);
    cf[i].A = 0.01f;
  }

  while(can.isOpen()) {
    //Update mouse coordinates
    scatter(mangle,0.02f);
    scatter(mdist,2.0f);
    if (mdist > 100) mdist *= 0.99f;
    mx = can.getMouseX()+mdist*cos(mangle);
    my = can.getMouseY()+mdist*sin(mangle);

    //Update positions of the edges of the triangles
    float ang = START + ROT*can.getFrameNumber();
    for (int i = 0; i < segs; ++i) {
      cf[i].H += HUEDELTA;
        if (cf[i].H > 6.0f) cf[i].H = 0.0f;
      float sang = sin(ang), cang = cos(ang);
      if (fabs(cang) > fabs(sang)) {
        x1[i] = (cang > 0) ? CW : 0;
        y1[i] = cch+cch*sang*SR2;
      } else {
        y1[i] = (sang > 0) ? CH : 0;
        x1[i] = ccw+ccw*cang*SR2;
      }
      ang += OFF;
    }
    for (int i = 0; i < segs; ++i) {
      x2[i] = x1[(i+1)%segs];
      y2[i] = y1[(i+1)%segs];
    }

    //Draw the triangles
    can.pauseDrawing();
    for (int i = 0; i < segs; drawn[i++] = false);
    for (int i = 0; i < segs; ++i) {
      int next = -1;
      float bdist = 999999;
      //Draw the triangles closest to the mouse first (lazy depth test)
      for (int j = 0; j < segs; ++j) {
        if (drawn[j])
          continue;
        float d = dist(mx,my,x1[j],y1[j]) + dist(mx,my,x2[j],y2[j]);
        if (d < bdist) {
          bdist = d;
          next = j;
        }
      }
	  if (next >= 0) {
		can.drawTriangle(mx,my,x1[next],y1[next],x2[next],y2[next],cf[next],true, false);
		drawn[next] = true;
	  }
    }
    can.resumeDrawing();
    can.sleep();
  }

  //Clean up
  delete [] x1; delete [] y1;
  delete [] x2; delete [] y2;
  delete [] drawn; delete [] cf;
}

int main(int argc, char* argv[]) {
  int w = (argc > 1) ? atoi(argv[1]) : -1;
  int h = (argc > 2) ? atoi(argv[2]) : w;
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs()*2;
  Canvas c(-1, -1, w, h, "Aura");
  c.setBackgroundColor(BLACK);
  c.run(auraFunction,t);
}
