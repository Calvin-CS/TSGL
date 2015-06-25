#include <VisualTaskQueue.h>

namespace tsgl {

VisualTaskQueue::VisualTaskQueue(int elements, int sideLength, float aspect, int spacing, int borderLength) {
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
  if (vcan->getIsOpen())
    vcan->close();
  vcan->wait();  //Close our progress bar if we're done
}

}
