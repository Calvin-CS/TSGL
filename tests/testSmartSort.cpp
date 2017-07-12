/*
 * testSmartSort.cpp
 *
 * Usage: ./testSmartSort <numElements> <numThreads>
 */

#include <tsgl.h>
#include <omp.h>

using namespace tsgl;

const int MARGIN = 8;    // Border for drawing

enum MergeState {
  S_MERGE = 1,
  S_SHIFT = 2,
  S_WAIT = 3,
  S_DONE = 4,
  S_HIDE = 5
};

struct sortData {
  ColorFloat color;            //Color of the thread
  MergeState state;            //Current state of the threads
  int first, last,             //Start and end of our block
    left, right,               //Indices of two numbers to compare
    fi, hi, li,                //Indices of first middle and last numbers in a set
    depth;                     //Current depth of the merge
  int* a;                      //Array of numbers to sort
  int seg, segs;               //Current / total segments
  int size;

  sortData(int* arr, int f, int l, ColorFloat c) {
    fi = hi = li = 0;        //Initialize indices
    left = right = 0;        //Initialize bounds
    color = c;               //Set the color
    a = arr;                 //Get a pointer to the array we'll be sorting
    first = f;               //Set the first element we need to worry about
    last = l;                //Set the last element we need to worry about
    depth = 0;               //We start at depth 0
    seg = 0; segs = 1;       //We start on segment -1, with a total of 1 segment
    while(segs < (l-f)) {    //If the current number of segments is more than the # of elements, we're done
      ++depth;               //Otherwise, increment the depth...
      segs *= 2;             //...and double the number of segments
    }
    state = S_SHIFT;         //Start Merging
    size = 2;
  }

  void restart(int l) {
    depth = 0;
    hi = last;
    right = hi+1;
    last = li = l;
    fi = left = first;
    state = S_MERGE;
    size *= 2;
  }

  void sortStep() {
    int tmp, pivot, jump;
    switch(state) {
      case S_SHIFT:
        pivot = jump = segs/2;
        fi = first; li = last;
        hi = (fi + li) / 2;    //Set our half index to the median of our first and last
        for (tmp = depth; tmp > 0; --tmp) {
          jump /= 2;
          if (seg < pivot) {
            pivot -= jump;
            li = hi;           //Set out last index to our old half index
          } else {
            pivot += jump;
            fi = hi+1;         //Set out first index to our old half index plus one
          }
          hi = (fi + li) / 2;  //Set our new half index to the median of our first and last
        }
        left = fi; right = hi+1;
        state = S_MERGE;           //We're ready to start Merging
        break;
      case S_MERGE:
        if (left > right || right > last) {
          seg = 0;                 //Reset our segment(s)
          segs /= 2;               //We're now using half as many segments
          state = (depth-- == 0) ? S_WAIT : S_SHIFT;
        } else if (right > li) {
          ++seg; state = S_SHIFT;  //Move on to the next segment and recalculate our first and last indices
        } else if (left <= hi && a[left] < a[right]) {
          ++left;
        } else {
          tmp = a[right];
          for (int x = right; x > left; --x)
            a[x] = a[x-1];
          a[left] = tmp;
          ++left; ++right; ++hi;
        }
        break;
      default:
        break;
    }
  }
};

/*!
 * \brief Visualization of the bottom-up mergesort algorithm.
 * \details Utilizes the sortData struct and sorts a number of items using the mergesort algorithm.
 * \details Uses lines to represent the items being sorted.
 * \details At the start, the items being sorted are all divided.
 * \details Once items have been sorted in one divided section, then sections are merged and the process repeats itself.
 * \details Different colors represent different sections being sorted.
 * \details Once all items have been sorted and merged, the animation stops and all lines are colored white.
 */
void smartSortFunction(Canvas& can, int threads, int size) {
  const int IPF = 1;      // Iterations per frame
    int* numbers = new int[size];       // Array to store the data
    for (int i = 0; i < size; i++)
      numbers[i] = rand() % (can.getWindowHeight() - MARGIN);

    int bs = size / threads;
    int ex = size % threads;
    sortData** sd = new sortData*[threads];
    int f = 0;
    int l = (ex == 0) ? bs-1 : bs;
    for (int i = 0; i < threads; ++i) {
      sd[i] = new sortData(numbers,f,l,Colors::highContrastColor(i));
      f = l+1;
      if (i < ex-1) l += (bs + 1);
      else          l += bs;
    }
    #pragma omp parallel num_threads(threads)
    {
        int tid = omp_get_thread_num();
        while (can.isOpen()) {
            can.sleep();
            if (sd[tid]->state == S_WAIT) {  //Merge waiting threads
              if ((tid % sd[tid]->size) > 0)
                sd[tid]->state = S_DONE;
              else {
                int next = tid+sd[tid]->size/2;
                if (next < threads && sd[next]->state == S_DONE) {
                  sd[next]->state = S_HIDE;
                  sd[tid]->restart(sd[next]->last);
                }
              }
            }
            for (int i = 0; i < IPF; i++)
              sd[tid]->sortStep();
            can.pauseDrawing();  //Tell the Canvas to stop updating the screen temporarily
            int start = MARGIN/2 + sd[tid]->first, height;
            int cwh = can.getWindowHeight() - MARGIN/2;
            ColorFloat color;
            if (sd[tid]->state != S_HIDE) {
              //Draw a black rectangle over our portion of the screen to cover up the old drawing
              can.drawRectangle(start,0,start + sd[tid]->last - sd[tid]->first,cwh,can.getBackgroundColor());
              for (int i = sd[tid]->first; i < sd[tid]->last; ++i, ++start) {
                  height = numbers[i];
                  if (sd[tid]->state == S_WAIT || sd[tid]->state == S_DONE)
                    color = WHITE;
                  else {
                    if (i == sd[tid]->right || i == sd[tid]->left)
                      color = WHITE;
                    else if (i < sd[tid]->left)
                      color = sd[tid]->color;
                    else if (i >= sd[tid]->fi && i <= sd[tid]->li)
                      color = Colors::blend(sd[tid]->color, WHITE, 0.5f);
                    else
                      color = Colors::blend(sd[tid]->color, BLACK, 0.5f);
                  }
                  can.drawLine(start, cwh - height, start, cwh, color);
              }
            }
            can.resumeDrawing();  //Tell the Canvas it can resume updating
        }
    }
    for (int i = 0; i < threads; ++i)
      delete sd[i];
    delete [] sd;
  delete [] numbers;
}

//Takes in command line arguments for the window width and height
//as well as for the number of threads to use
int main(int argc, char* argv[]) {
    int s = (argc > 1) ? atoi(argv[1]) : 1024;
    if (s < 10) s = 10;
    int w = s + MARGIN;
    int h = w/2;

    int threads, t = (argc > 2) ? atoi(argv[2]) : omp_get_num_procs();
    for (threads = 1; threads < t; threads *=2);  //Force threads to be a power of 2

    Canvas c(-1, -1, w, h, "Bottom-up Merge Sort");
    c.setBackgroundColor(BLACK);
    c.run(smartSortFunction, threads, s);
}
