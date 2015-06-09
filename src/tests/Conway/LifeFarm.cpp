/*
 * AntFarm.cpp
 *
 *  Created on: May 28, 2015
 *      Author: pretzel
 */

#include "LifeFarm.h"

LifeFarm::LifeFarm(int w, int h, Canvas* c, bool randomize) {
  framecount = 0;
  listend = 0;
  width = w;
  height = h;
  size = w * h;
  livelist = new int[size];
  int maxsize = 1+h*(1+w);
  currentstate = new int[maxsize];
  for (int i = 0; i < maxsize; ++i)
    currentstate[i] = 0;
  nextstate = new int[maxsize];
  for (int i = 0; i < maxsize; ++i)
    nextstate[i] = 0;
  for (int i = 0; i < size; ++i)
    livelist[i] = 0;

  currentstart = currentstate;  //Pointers to the beginning of the arrays
  nextstart = nextstate;

  alive = new bool*[h]();
  for (int i = 0; i < h; ++i) {
    alive[i] = new bool[w];
    for (int j = 0; j < w; ++j)
      alive[i][j] = false;
  }
  if (randomize) {
    for (int i = 9*h/10; i > h/10; --i) {
      bool newrow = true;
      for (int j = 9*w/10; j > w/10; --j) {
        if ((rand() % 2) > 0) {
          if (newrow) {
            *currentstate = (i + 1);
            ++currentstate;
            newrow = false;
          }
          *currentstate = -(j + 1);
          ++currentstate;
          addAnt(j,i);
        }
      }
    }
    *currentstate = 0;
    currentstate = currentstart;
  }
  else
    initGun();
  can = c;
  neighbors = new int*[h]();
  for (int i = 0; i < h; ++i)
    neighbors[i] = new int[w];
  bgcolor = c->getBackgroundColor();
  drawdead = false;
}

void LifeFarm::initGun() {
  int w = width/2, h = height/2;
  currentstart = currentstate;
  *currentstate++ = h+1;
  *currentstate++ = -(w+25);
  addAnt(w+25,h+1);
  *currentstate++ = h+2;
  *currentstate++ = -(w+25);
  *currentstate++ = -(w+23);
  addAnt(w+25,h+2);
  addAnt(w+23,h+2);
  *currentstate++ = h+3;
  *currentstate++ = -(w+36);
  *currentstate++ = -(w+35);
  *currentstate++ = -(w+22);
  *currentstate++ = -(w+21);
  *currentstate++ = -(w+14);
  *currentstate++ = -(w+13);
  addAnt(w+36,h+3);
  addAnt(w+35,h+3);
  addAnt(w+22,h+3);
  addAnt(w+21,h+3);
  addAnt(w+14,h+3);
  addAnt(w+13,h+3);
  *currentstate++ = h+4;
  *currentstate++ = -(w+36);
  *currentstate++ = -(w+35);
  *currentstate++ = -(w+22);
  *currentstate++ = -(w+21);
  *currentstate++ = -(w+16);
  *currentstate++ = -(w+12);
  addAnt(w+36,h+4);
  addAnt(w+35,h+4);
  addAnt(w+22,h+4);
  addAnt(w+21,h+4);
  addAnt(w+16,h+4);
  addAnt(w+12,h+4);
  *currentstate++ = h+5;
  *currentstate++ = -(w+22);
  *currentstate++ = -(w+21);
  *currentstate++ = -(w+17);
  *currentstate++ = -(w+11);
  *currentstate++ = -(w+2);
  *currentstate++ = -(w+1);
  addAnt(w+22,h+5);
  addAnt(w+21,h+5);
  addAnt(w+17,h+5);
  addAnt(w+11,h+5);
  addAnt(w+2,h+5);
  addAnt(w+1,h+5);
  *currentstate++ = h+6;
  *currentstate++ = -(w+25);
  *currentstate++ = -(w+23);
  *currentstate++ = -(w+18);
  *currentstate++ = -(w+17);
  *currentstate++ = -(w+15);
  *currentstate++ = -(w+11);
  *currentstate++ = -(w+2);
  *currentstate++ = -(w+1);
  addAnt(w+25,h+6);
  addAnt(w+23,h+6);
  addAnt(w+18,h+6);
  addAnt(w+17,h+6);
  addAnt(w+15,h+6);
  addAnt(w+11,h+6);
  addAnt(w+2,h+6);
  addAnt(w+1,h+6);
  *currentstate++ = h+7;
  *currentstate++ = -(w+25);
  *currentstate++ = -(w+17);
  *currentstate++ = -(w+11);
  addAnt(w+25,h+7);
  addAnt(w+17,h+7);
  addAnt(w+11,h+7);
  *currentstate++ = h+8;
  *currentstate++ = -(w+16);
  *currentstate++ = -(w+12);
  addAnt(w+16,h+8);
  addAnt(w+12,h+8);
  *currentstate++ = h+9;
  *currentstate++ = -(w+14);
  *currentstate++ = -(w+13);
  addAnt(w+14,h+9);
  addAnt(w+13,h+9);
  *currentstate = 0;
  currentstate = currentstart;
}

LifeFarm::~LifeFarm() {
  delete alive;
  delete neighbors;
  delete livelist;
  delete currentstate;
  delete nextstate;
}

inline void LifeFarm::addAnt(int x, int y) {
  livelist[listend++] = y*width+x;
  alive[y][x] = true;
}

void LifeFarm::moveAnts() {
  moveAntsOld();
//  moveAntsNew();
}

void LifeFarm::moveAntsOld() {
  //Compute this frame's color
  const int P1 = 7, P2 = 11, P3 = 17;
//  std::cout << ++framecount << std::endl;
  int r = (framecount*P1/50 % 255);
  if (r < 128)  r = 255 - r;
  int g = (framecount*P2/50 % 255);
  if (g < 128)  g = 255 - g;
  int b = (framecount*P3/50 % 255);
  if (b < 128)  b = 255 - b;
  ColorFloat fcolor = ColorInt(r,g,b,255);

  //Clear the each cell's neighbor list
  for (int i = 0; i < height; ++i)
    for (int j = 0; j < width; ++j)
      neighbors[i][j] = 0;

  //Populate the neighbor list of each living cell
    int tid = 0, nthreads = 1;
    for (int i = tid; i < listend ; i += nthreads) {
      int n = livelist[i];
      int row = n / width, col = n % width;
      int xm = col - 1;
        if (xm < 0) xm = width - 1;
      int xp = col+1;
        if (xp > width-1) xp = 0;
      int ym = row-1;
        if (ym < 0) ym = height - 1;
      int yp = row+1;
        if (yp > height-1) yp = 0;
      ++neighbors[ym][xm];
      ++neighbors[row][xm];
      ++neighbors[yp][xm];
      ++neighbors[ym][col];
      ++neighbors[yp][col];
      ++neighbors[ym][xp];
      ++neighbors[row][xp];
      ++neighbors[yp][xp];
    }
//  }

  //Reset the end of the list for the next iteration
  listend = 0;

  //Redraw any cell whose living status has changed, and repopulate the living list
  bool lives, lived;
  for (int row = 0; row < height; ++row) {
    for (int col = 0; col < width; ++ col) {
      lived = alive[row][col];
      lives = ( (neighbors[row][col] == 3) || ( lived && (neighbors[row][col] == 2) ));
      if (lives != lived) {
        if (lives) {
          can->drawPoint(col, row, fcolor);
          addAnt(col,row);
        }
        else if (drawdead)
          can->drawPoint(col, row, bgcolor);
        alive[row][col] = lives;
      } else if (lives)
        addAnt(col,row);
    }
  }
}


void LifeFarm::moveAntsNew() {
  life(currentstate,nextstate);
  //Swap pointers to the begnning of the current and next state arrays
  int* temp = nextstart;
  nextstart = currentstart;
  currentstart = temp;
  //Move the pointers to the beginning of the newly-swapped arrays
  currentstate = currentstart;
  nextstate = nextstart;
  return;
}

void LifeFarm::setDrawdead(bool b) {
  drawdead = b;
}


void LifeFarm::life(int *current, int *fresh) {
  //Compute this frame's color
  const int P1 = 7, P2 = 11, P3 = 17;
  std::cout << ++framecount << std::endl;
  int r = (framecount*P1/50 % 255);
  if (r < 128)  r = 255 - r;
  int g = (framecount*P2/50 % 255);
  if (g < 128)  g = 255 - g;
  int b = (framecount*P3/50 % 255);
  if (b < 128)  b = 255 - b;
  ColorFloat fcolor = ColorInt(r,g,b,255);

  unsigned bitmap;
  int *next, *prev, *start;
  start = fresh;
  int x, y;
  static bool state[1 << 9];    //Array of dead / alive states
  static bool init = false;

  //Check for init [007 (octal) == 00000111 (binary) -> 3 neighbors]
  if(!init) {
    init = true;
    //Populate a state bitmap
    for(bitmap = 0; bitmap < (1<<9); bitmap++) {
      for(x = y = 0; y < 9; y++)  //For each cell in the 3x3 neighbor grid
        if(bitmap & (1<<y) )         //If bitmap ANDed with 1 bitshifted y times is true, there's a neighbor
          x += 1;                 //Add a neighbor
      if(bitmap & 020) {          //Check if bitmap is alive [020 (octal) = 000010000 (binary) -> center cell is alive]
        if(x == 2 || x == 3)      //If two neighbors or three neighbors, alive; else, dead
          state[bitmap] = true;
        else
          state[bitmap] = false;
      } else {                    //Else, bitmap is dead
        if(x == 3)                //If three neighbors, alive; else, dead
          state[bitmap] = true;
        else
          state[bitmap] = false;
      }
    }
  }

  x = y = 0;  //ADDED

  prev = next = current;   //Set the previous / next cell to the first cell of "this" (the old state)
  bitmap = 0;           //Set bitmap to 0 (empty)
  *fresh = 0;             //Set the first position of the new state to 0 (empty)
  for(;;) {             //For each row
    /* did we write an X co-ordinate? */
    if(*fresh < 0)        //If the next element in the new state is an x coordinate
      fresh++;            //Change new's pointer to the next element
    if(prev == next) {  //If the address of our previous and next cell are the same...
      /* start a new group of rows */
      if(*next == 0) {  //If out next cell is the terminator
        *fresh = 0;       //Set the next state of the new board to the terminator state as well
        fresh = start;
        return;         //We're done!
      }
      y = *next++ + 1;  //Set out y coordinate to 1 plus next's current value, and increment next (which is sorted desc.)
    } else {            //If our previous and next cells are different
      /* move to next row and work out which ones to scan */
      if(*prev == y--)  //Increment prev's pointer if it's the same as our y (after decrementing it) plus 1
        prev++;
      if(*current == y)    //Increment this's pointer if it's the same as our y
        current++;
      if(*next == y-1)  //Increment next's pointer if it's the same as our y minus 1
        next++;
    }
    /* write new row co-ordinate */
    *fresh = y;           //Write the new row coordinate
    for(;;) {           //For each column (cell)
      /* skip to the leftmost cell */
      x = *prev;        //Set our (negative) x coordinate to the max of the current values of prev, this, and next
      if(x > *current)
        x = *current;
      if(x > *next)
        x = *next;
      /* end of line? */
      if(x >= 0)        //If x is non-negative, this is a y coordinate; we're at the end of the line; move on!
        break;
      for(;;) {         //For each consecutive cell
        /* add a column to the bitmap */
        if(*prev == x) {  //If the previous row's cell (the one directly above us) is alive
          bitmap |= 0100; //OR the bitmap with 001000000 (topright)
          prev++;         //Increment the previous row's pointer
        }
        if(*current == x) {  //If the current row's cell is alive
          bitmap |= 0200; //OR the bitmap with 010000000 (midright)
          current++;         //Increment the current row's pointer
        }
        if(*next == x) {  //If the next row's cell (the one directly below us) is alive
          bitmap |= 0400; //OR the bitmap with 100000000 (bottomright)
          next++;         //Increment the next row's pointer
        }
        /* what does this bitmap indicate? */
        if(state[bitmap] && !(y > height-3 || y < 3) && !((-x) > width-3 || (-x) < 3)) { //If our bitmap is now alive
          *(++fresh) = x - 1;         //Increment the cell pointer of our new board, and set the next value to the current x-coordinate minus 1
          can->drawPoint(-(x), y, fcolor); //Draw it
        }
        else if(bitmap == 000)    //If our bitmap is now empty...
          break;                  //No more consecutive living cells; move on to the next explicitly stored pointer
        /* move right */
        bitmap >>= 3;             //Shave off the the leftmost three bits to move our bitmap to the right
        x += 1;                   //Increment our x coordinate
      }
    }
  }
}
