/*
 * testMaster.cpp
 *
 *  Created on: Jun 8, 2015
 *      Author: cpd5
 */

#include <omp.h>
#include <tsgl.h>

int main(int argc, char* argv[]) {
//  int w = (argc > 1) ? atoi(argv[1]) : 640;
//  int h = (argc > 2) ? atoi(argv[2]) : (w*4)/3;
  int t = (argc > 3) ? atoi(argv[3]) : omp_get_num_procs();
  return (t > 0);
}
