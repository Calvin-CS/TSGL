#include <VisualTaskQueue.h>

namespace tsgl {

VisualTaskQueue::VisualTaskQueue(int elements, int sideLength, float aspect, int spacing, int borderLength) {
    showingLegend = false;
    space = spacing;
    border = borderLength;
    totalElements = elements;
    rowLength = ceil(sqrt(totalElements/aspect));  //Number of elements per row
    blockSize = sideLength;
    vcan = new Canvas(0,-1,2*border+(blockSize+space)*rowLength,2*border+(blockSize+space)*elements/rowLength,"Thread colors");
    vcan->start();
	reset();
}

VisualTaskQueue::~VisualTaskQueue() {
  delete vcan;
  if (showingLegend)
    delete lcan;
}

void VisualTaskQueue::showLegend(int threads) {
  bool canContinue = false;
  #pragma omp critical
  {
    if (!showingLegend) {
      showingLegend = true;
      canContinue = true;
    }
  }
  if (canContinue) {
    const int TEXTW = 24, GAP = 4;
    if (threads == -1)
      threads = omp_get_num_threads();

    //Ugly calculations :(
    int offset = border+space;
    int xStart = border;
    int xDelta = TEXTW*2;
    int yStart = TEXTW + offset;
    int yDelta = blockSize+space;
    int oheight = vcan->getWindowHeight();
    int myHeight = TEXTW + (threads+1) * yDelta;
    if (myHeight > oheight)
      myHeight = oheight;
    int perColumn = (myHeight-yStart)/yDelta;
    int yCutoff = yStart + yDelta*perColumn-blockSize;
    int myWidth = 2*border + ((threads)/perColumn)*xDelta+blockSize+TEXTW;
  #ifdef _WIN32
	if (myWidth < 116);  //Magic number for Windows windows...
	myWidth = 116;
  #endif

    //Actually draw things
    lcan = new Canvas(vcan->getWindowX()+vcan->getWindowWidth(),vcan->getWindowY(),myWidth,myHeight,"");
	std::cout << lcan->getWindowWidth();
    lcan->start();
    lcan->drawText("Legend:",TEXTW/2,TEXTW,TEXTW,BLACK);
    int xx = xStart, yy = yStart;
    for (int i = 0; i < threads; ++i) {
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
    Colors::blend(
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
  lcan->wait();
  if (showingLegend) {
    if (vcan->getIsOpen())
      vcan->close();
    vcan->wait();
  }
}

}
