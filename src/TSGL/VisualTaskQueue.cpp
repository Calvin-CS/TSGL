#include <VisualTaskQueue.h>

namespace tsgl {

VisualTaskQueue::VisualTaskQueue(int elements, int sideLength, float aspect, int spacing, int borderLength) {
    showingLegend = false;
    space = spacing;
    border = borderLength;
    totalElements = elements;
    rowLength = ceil(sqrt(totalElements/aspect));  //Number of elements per row
    blockSize = sideLength;
    vcan = new Canvas(0,-1,2*border+(blockSize+space)*rowLength,2*border+(blockSize+space)*elements/rowLength,"");
    reset();
    vcan->start();
}

VisualTaskQueue::~VisualTaskQueue() {
  delete vcan;
  if (showingLegend)
    delete lcan;
}

void VisualTaskQueue::showLegend(int t) {
  if (!showingLegend) {
    const int TEXTW = 24, GAP = 4;
    showingLegend = true;

    //Ugly calculations :(
    int offset = border+space;
    int xStart = border;
    int xDelta = TEXTW*2;
    int yStart = TEXTW + offset;
    int yDelta = blockSize+space;
    int oheight = vcan->getWindowHeight();
    int myHeight = TEXTW + t * yDelta;
    if (myHeight > oheight)
      myHeight = oheight;
    int perColumn = (myHeight-yStart)/yDelta;
    int yCutoff = yStart + yDelta*((myHeight-yStart)/yDelta)-blockSize;
    int myWidth = 2*border + ((t-1)/perColumn)*xDelta+blockSize+TEXTW;

    //Actually draw things
    lcan = new Canvas(vcan->getWindowX()+vcan->getWindowWidth(),vcan->getWindowY(),myWidth,myHeight,"");
    lcan->start();
    lcan->drawText("Legend:",TEXTW/2,TEXTW,TEXTW,BLACK);
    int xx = xStart, yy = yStart;
    for (int i = 0; i < t; ++i) {
      lcan->drawRectangle(xx,yy,xx+blockSize,yy+blockSize,Colors::highContrastColor(i));
      lcan->drawText(to_string(i),xx+blockSize+GAP,yy+blockSize,TEXTW/2);
      yy += yDelta;
      if (yy > yCutoff) {
        yy = yStart;
        xx += xDelta;
      }
    }
  }
}

void VisualTaskQueue::update(int index, VQState state) {
  int x = index % rowLength;
  int y = index / rowLength;
  vcan->drawRectangle(
    border+x*(blockSize+space),border+y*(blockSize+space),
    border+x*(blockSize+space)+blockSize,border+y*(blockSize+space)+blockSize,
    Colors::blendedColor(
      Colors::highContrastColor(omp_get_thread_num()),(state == RUNNING) ? BLACK : WHITE,0.5f
    ),true
  );
}

void VisualTaskQueue::reset() {
  for (int i = 0; i < totalElements; ++i) {
    int x = i % rowLength;
    int y = i / rowLength;
    vcan->drawRectangle(
      border+x*(blockSize+space),border+y*(blockSize+space),
      border+x*(blockSize+space)+blockSize,border+y*(blockSize+space)+blockSize,
      WHITE,true
    );
  }
}

void VisualTaskQueue::close() {
  if (lcan->getIsOpen())
    lcan->close();
  if (vcan->getIsOpen())
    vcan->close();
  lcan->wait();
  vcan->wait();
}

}
