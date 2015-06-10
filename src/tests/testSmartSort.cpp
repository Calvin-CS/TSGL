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
  S_MERGE = 1,
  S_SHIFT = 2,
  S_WAIT = 3,
  S_DONE = 4
};

struct sortData {
  MergeState state;            //Current state of the threads
  int first, last,             //Start and end of block
    left, right,               //Indices of two numbers to compare
    fi, hi, li,                //Indices of first middle and last numbers in a set
    depth;                     //Current depth of the merge
  int* a;                      //Array of numbers to sort
  int seg, segs;               //Current / total segments

  sortData(int* arr, int f, int l) {
    int tmp;
    a = arr;                 //Get a pointer to the array we'll be sorting
    first = f;               //Set the first element we need to worry about
    last = l;                //Set the last element we need to worry about
    depth = 0;               //We start at depth 0
    fi = left = first;       //Set our starting index to first
    li = hi = last;          //Set our ending and halfway indices to last
    seg = 0; segs = 1;       //We start on segment -1, with a total of 1 segment
    while (true) {
      tmp = (left + hi)/2;   //Set our temp index to the median of our first and half index
      li = hi;               //Set out last index to our old half index
      hi = tmp;              //Set our new half index to our temp index
      ++depth;               //Otherwise, increment the depth and repeat the process
      segs *= 2;             //We have twice as many segments now
      if ((segs*2) >= (l-f)) //If twice the current number of segments is more than the # of elements
        break;               //We're done
    }
    right = hi+1;            //Set our second index to hi + 1
    state = S_SHIFT;         //Start Merging
  }

  void restart(int l) {
    last = l;
    depth = 0;
    state = S_SHIFT;
  }

  void sortStep() {
    int tmp, pivot, jump;
    switch(state) {
      case S_SHIFT:
        tmp = depth;
        pivot = jump = segs/2;
        fi = first; li = last;
        hi = (fi + li) / 2;    //Set our new half index to the median of our first and last
        while (tmp-- > 0) {
          jump /= 2;
          if (seg < pivot) {
            pivot -= jump;
            li = hi;           //Set out last index to our old half index
          } else {
            pivot += jump;
            fi = hi+1;         //Set out first index to our old half index
          }
          hi = (fi + li) / 2;  //Set our new half index to the median of our first and last
          if (fi == li)
            break;
        }
        left = fi;
        right = hi+1;
        if (right > li)
          right = li;
        state = S_MERGE;         //We're ready to start Merging
        break;
      case S_MERGE:
        if (right > last) {
          seg = 0;               //Reset our segment(s)
          segs /= 2;             //We're now using half as many segments
          state = (depth-- == 0) ? S_WAIT : S_SHIFT;
        } else if (right > li) {
          ++seg; state = S_SHIFT;//Move on to the next segment and recalculate our first and last indices
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
      case S_WAIT:
      default:
        break;
    }
  }
};

void smartSortFunction(Canvas& can, int threads) {
	  const int SIZE = 1100,   // Size of the data pool (set to 550 by default)
	            IPF = 25;      // Iterations per frame
    int numbers[SIZE];       // Array to store the data
    for (int i = 0; i < SIZE; i++)
        numbers[i] = rand() % (can.getWindowHeight() - 40);
    can.setBackgroundColor(GREY);

    int bs = SIZE / threads;
    int ex = SIZE % threads;
    sortData** tstate = new sortData*[threads];
    int f, l, active = 1;
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
    #pragma omp parallel num_threads(threads)
    {
        int tid = omp_get_thread_num();
        while (can.getIsOpen()) {
            can.sleep();  //Removed the timer and replaced it with an internal timer in the Canvas class
            if (tid == 0) {
              bool allwaiting = true;
              for (int i = 0; i < threads; ++i) {
                if (tstate[i]->state != S_WAIT && tstate[i]->state != S_DONE)
                  allwaiting = false;
              }
              if (allwaiting && active < threads) {
                active *= 2;
                for (int i = 0; i < threads; i ++) {
                  if (i % active == 0)
                    tstate[i]->restart(tstate[i+active/2]->last);
                  else
                    tstate[i]->state = S_DONE;
                }
              }
            }
            for (int i = 0; i < IPF; i++)
              tstate[tid]->sortStep();
            can.clear();
            int start = 50 + tstate[tid]->first, width = 1, height;
            int cwh = can.getWindowHeight() - 20;
            ColorFloat color;
            if (tstate[tid]->state != S_DONE) {
              for (int i = tstate[tid]->first; i < tstate[tid]->last; i++, start += width * 1) {
                  height = numbers[i];
                  if (tstate[tid]->state == S_WAIT)
                    color = ColorInt(MAX_COLOR, MAX_COLOR, MAX_COLOR);
                  else {
                    if (i == tstate[tid]->hi)
                      color = ColorInt(0, MAX_COLOR, MAX_COLOR);
                    else if (i == tstate[tid]->left || i == tstate[tid]->right)
                      color = ColorInt(0, MAX_COLOR, 0);
                    else if (i >= tstate[tid]->fi && i <= tstate[tid]->li)
                      color = ColorInt(MAX_COLOR, MAX_COLOR, 0);
                    else
                      color = ColorInt(MAX_COLOR, 0, 0);
                  }
                  can.drawRectangle(start, cwh - height, start + width, cwh, color);
              }
            }
        }
    }
    for (int i = 0; i < threads; ++i)
      delete tstate[i];
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
    int t = (argc > 3) ? atoi(argv[3]) : 8;
    Canvas c(0, 0, w, h, "", FRAME);
    c.setBackgroundColor(GREY);
    c.start();
    smartSortFunction(c,t);   //Pass it as an argument
    c.wait();
}
