/*
 * ShadedVoronoi.cpp
 */

#include "ShadedVoronoi.h"

using namespace tsgl;

ShadedVoronoi::ShadedVoronoi(Canvas& can) : Voronoi(can) {
  const int WW = can.getWindowWidth(),                    // Set the screen sizes
            WH = can.getWindowHeight();               // Create a mapping of control point values
  myKValue2 = new int[WW * WH]();                         // Create a mapping of more control point values
}

void ShadedVoronoi::draw(Canvas& can) {
  const int WW = can.getWindowWidth(),                    // Set the screen sizes
            WH = can.getWindowHeight();
  #pragma omp parallel for
  for (int i = 0; i < WW; i++) {                          // For each individual point...
    for (int j = 0; j < WH; j++) {
      int myBestK = -1, myNextBestK = -1;
      float myBDist = 9999, myNBDist = 9999;              // Reset the best distance
      for (int k = 0; k < MY_POINTS; k++) {               // Find the closest control point
        float myXD = i - myX[k], myYD = j - myY[k];
        float myDist = sqrt(myXD * myXD + myYD * myYD);   // Calculate the distance from each control point
        if (myDist < myBDist) {                           // If it's the closest one
          myNBDist = myBDist;                             // Update the next best distance and control point
          myNextBestK = myBestK;
          myBDist = myDist;                               // Update the best distance and control point
          myBestK = k;
        } else if (myDist < myNBDist) {                   // If it's the second closest one
          myNBDist = myDist;                              // Just update the next best distance / CP
          myNextBestK = k;
        }
      }
      myKValue[i * WH + j] = myBestK;
      myKValue2[i * WH + j] = myNextBestK;
      can.drawPoint(i, j, myColor[myBestK]);              // Draw the point with the closest control's color
    }
  }
  #pragma omp parallel for
  for (int i = 0; i < WW; i++) {                          // For each individual point...
    for (int j = 0; j < WH; j++) {
      int k = myKValue[i * WH + j];                       // Find its closest control point
      int nk = myKValue2[i * WH + j];                     // Then find its second closest
      float xd1 = i - myX[k];
      float yd1 = j - myY[k];
      float d1 = xd1 * xd1 + yd1 * yd1;                   // Find the distance to it closest
      float xkd = myX[k] - myX[nk];
      float ykd = myY[k] - myY[nk];
      float kd = xkd * xkd + ykd * ykd;                   // Find the distance between the CPs themselves
      float shading = sqrt(d1 / kd);
      clamp(shading,0,1);
      can.drawPoint(i, j, ColorFloat(0, 0, 0, shading));  // Draw the point with the closest control's color
    }
  }
}

ShadedVoronoi::~ShadedVoronoi() {
  delete [] myKValue2;
  myKValue2 = NULL;
}

