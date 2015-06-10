/*
 * testSmartSort.cpp
 *
 *  Created on: May 27, 2015
 *      Author: cpd5
 */

#include <tsgl.h>
#include <omp.h>
using namespace tsgl;

enum MergeState {
  S_CALC = 1,
  S_MERGE = 2,
  S_SHIFT = 3,
  S_WAIT = 4,
  S_DONE = 5
};

struct sortData {
  MergeState state;            //Current state of the threads
  int first, last,             //Start and end of block
    left, right,               //Indices of two numbers to compare
    fi, hi, li,                //Indices of first middle and last numbers in a set
    depth;                     //Current depth of the merge
  int* a;                      //Array of numbers to sort
  int sorted;                  //Number of elements in nums sorted
  int seg, segs;               //Current / total segments

  sortData(int* arr, int f, int l) {
    int tmp;
    a = arr;               //Get a pointer to the array we'll be sorting
    first = f;             //Set the first element we need to worry about
    last = l;              //Set the last element we need to worry about
    depth = sorted = 0;    //We start at depth 0 with nothing sorted
    left = first; li = last; //Set our starting index to first and our ending index to last
    fi = first; hi = last;  //Set our start and halfway index to our last index
    seg = -1; segs = 1;     //We start on segment 0, with a total of 1 segment
    while (true) {
      tmp = (left + hi)/2; //Set our temp index to the median of our first and half index
      if (tmp == hi)       //If temp and last half are the same, we're done
        break;
      li = hi;             //Set out last index to our old half index
      hi = tmp;            //Set our new half index to our temp index
      ++depth;             //Otherwise, increment the depth and repeat the process
      segs *= 2;           //We have twice as many segments now
      if ( (segs*2) >= (l-f) )
        break;
    }
    right = hi+1;          //Set our second index to hi + 1
    state = S_SHIFT;       //Start Merging
  }

  void sortStep() {
    int tmp, pivot, delta;
    switch(state) {
      case S_SHIFT:
        ++seg;                 //Increment our segment
        tmp = depth;
        pivot = segs/2;
        delta = segs/2;
        left = first; li = last;
        hi = (left + li) / 2;    //Set our new half index to the median of our first and last
        while (tmp > 0) {
          delta /= 2;
          if (seg < pivot) {
            pivot -= delta;
            li = hi;             //Set out last index to our old half index
          } else {
            pivot += delta;
            left = hi+1;             //Set out first index to our old half index
          }
          hi = (left + li) / 2;    //Set our new half index to the median of our first and last
          if (left == li)
            break;
          --tmp;
        }
        right = hi+1;
        if (right > li)
          right = li;
        fi = left;
        if (left <= right)
          state = S_MERGE;       //We're ready to start Merging
        break;
      case S_MERGE:
        //Assuming all elements between i1 & hi are sorted, and all elements between
        //i2 & li are sorted, this will run until all elements between i1 & li are sorted
        if (right > li) {
          sorted = li;           //All elements up to li are now sorted
          if (sorted >= last) {  //If we've reached the end...
            if (depth-- == 0) {  //If we're at depth 0, we're done for now; otherwise, decrement depth
              state = S_WAIT;
              break;
            }
            sorted = 0;          //Reset sorted
            seg = -1;            //Reset our segment(s)
            segs /= 2;           //We're now using half as many segments
          }
          state = S_SHIFT;       //Move on to the next segment and recalculate our first and last indices
          break;
        } else if (left <= hi && a[left] < a[right]) {
          ++left;
        } else {
          tmp = a[right];
          for (int x = right; x > left; --x) {
            a[x] = a[x-1];
          }
          a[left] = tmp;
          ++left; ++right; ++hi;
        }
        break;
      case S_WAIT:
        break;
      default:
        break;
    }
  }
};

void merge(int* a, int fi1, int fi2, int li) {
  int n = fi1, tmp;
  int li1 = fi2-1, li2 = li;
  while (fi1 < li1 || fi2 < li2) {
    if (fi2 == li2 || (fi1 < li1 && a[fi1] < a[fi2]) ) {
      tmp = a[n];
      a[n] = a[fi1];
      a[fi1] = tmp;
      ++fi1;
    } else {
      tmp = a[n];
      a[n] = a[fi2];
      a[fi2] = tmp;
      ++fi2;
    }
    ++n;
  }
}

void sort(int* a, int fi, int li) {
  if (fi < li) {
    int hi = (fi+li)/2;
    sort(a, fi, hi);
    sort(a, hi, li);
    merge(a, fi, hi, li);
  }
}

void smartSortFunction(Canvas& can, int threads) {
	  const int SIZE = 515,   // Size of the data pool (set to 550 by default)
	            IPF = 10;     // Iterations per frame
    int numbers[SIZE];      // Array to store the data
    for (int i = 0; i < SIZE; i++)
        numbers[i] = rand() % (can.getWindowHeight() - 40);
    can.setBackgroundColor(GREY);

    int bs = SIZE / threads;
    int ex = SIZE % threads;
    sortData** tstate = new sortData*[threads];
    int f, l;
    for (int i = 0; i < threads; ++i) {
      if (ex == 0 || i < ex) {
        f = i*bs;
        l = f + bs - 1;
      } else {
        f = ex*bs + (i-ex)*(bs+1);
        l = f + bs;
      }
      tstate[i] = new sortData(numbers,f,l);
    }
//    #pragma omp parallel
//    {
//        int tid = omp_get_thread_num();
        while (can.getIsOpen()) {
            can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
//            #pragma omp parallel num_threads(threads)
//            {
              for (int i = 0; i < IPF; i++) {
                for (int j = 0; j < threads; ++j) {
                  tstate[j]->sortStep();
                }
              }
//            }
            can.clear();
            int start = 50, width = 1, height;
            int cwh = can.getWindowHeight() - 20;
            ColorFloat color;
//            if (tid == 0) {
                for (int i = 0; i < SIZE; i++, start += width * 2) {
                    height = numbers[i];
//                    if (i == tstate[0]->hi)
//                      color = ColorInt(0, MAX_COLOR, MAX_COLOR);
//                    else
                     if (i == tstate[0]->left || i == tstate[0]->right)
                      color = ColorInt(0, MAX_COLOR, 0);
                    else if (i >= tstate[0]->fi && i <= tstate[0]->li)
                      color = ColorInt(MAX_COLOR, MAX_COLOR, 0);
                    else
                      color = ColorInt(MAX_COLOR, 0, 0);
                    can.drawRectangle(start, cwh - height, start + width, cwh, color);
                }
//            }
        }
//    }
    for (int i = 0; i < threads; ++i) {
      delete tstate[i];
    }
    delete [] tstate;
}

//Takes in command line arguments for the window width and height
int main(int argc, char* argv[]) {
    int w = (argc > 1) ? atoi(argv[1]) : 1200;
    int h = (argc > 2) ? atoi(argv[2]) : 900;
    if (w <= 0 || h <= 0) {     //Checked the passed width and height if they are valid
      w = 1200;
      h = 900;                  //If not, set the width and height to a default value
    }
    int t = (argc > 3) ? atoi(argv[3]) : 4;
    if (t < 1) t= 1;
    t = 1;
    Canvas c(0, 0, w, h, "", FRAME);
    c.setBackgroundColor(GREY);
    c.start();
    smartSortFunction(c,t);   //Pass it as an argument
    c.wait();
}
