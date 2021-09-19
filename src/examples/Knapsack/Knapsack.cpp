//
//  Knapsack.cpp
//  
//
//  Created by Sarah Parsons on 11/23/19.
//  Last updated by Grey Ballard on 9/18/21.
//

#include <tsgl.h>
#include <Line.h>
#include <Canvas.h>
#include <Color.h>
#include <iostream>
#include <vector>
#include <string>
#include <omp.h>
#include <stdlib.h>
using namespace tsgl;
using namespace std;



int main(int argc, char* argv[]) {
    
    // parse inputs
    // usage: ./Knapsack THREADS CAPACITY ITEMS TRACEPATH
    int num_threads = (argc > 1) ? atoi(argv[1]) : 2; 
    int capacity = (argc > 2) ? atoi(argv[2]) : 12; 
    int items = (argc > 3) ? atoi(argv[3]) : 8; 
    string path = (argc > 4) ? argv[4] : "No";

    // print parameters to terminal
    cout << "Capacity: " << capacity << endl;
    cout << "# of Items: " << items << endl;
    cout << "# of Threads: " << num_threads << endl;
    omp_set_num_threads(num_threads);
    cout << "Trace path? " << path << endl; 
    
    // set pause time (secs) to control speed
    float pause = 1.0;   
    
    // allocate memory
    int* weights = (int*) malloc(items * sizeof(int));
    int* values = (int*) malloc(items * sizeof(int));
    int** knap = (int**) malloc((capacity+1) * sizeof(int*));
    for(int k = 0; k <= capacity; k++) {
        knap[k] = (int*) malloc((items+1) * sizeof(int));
    }
    
    // initialize 1st column of table
    for(int j = 0; j <= items; j++) {
        knap[0][j] = 0;
    }
    
    // randomly assign weights and values
    const int MAX_WEIGHT = 5;
    cout << "Weights: " << endl;
    for (int w =0; w < items; w++) {
        weights[w] = 1 + (rand() % min(MAX_WEIGHT, capacity));
        cout << weights[w] << " ";
    }
    cout << endl;  
    const int MAX_VALUE  = 5;  
    cout << "Values: " << endl;
    for (int v =0; v < items; v++) {
        values[v] = 1 + (rand() % MAX_VALUE);
        cout << values[v] << " ";
    }
    cout << endl << endl;

    // construct canvas and draw table with labels
    Canvas c(0, 0, 1200, 800, "0-1 Knapsack Problem",WHITE);
    //c.setBackgroundColor(WHITE); 
    c.start();
    
    //Create a Background ptr to draw 2D Visualizations
    Background *bgp = c.getBackground();
   
    float x1 = -250, x2 = 250, y1 = -250, y2 = 250; // table corners
    float size = x2 - x1, col_wid = size/items, row_wid = size/(capacity+1);
    
    // title and main parameters
    bgp->drawText(0,y2+75,0,"0-1 Knapsack Problem",FONT,30,0,0,0,RED);
    bgp->drawText(x1-200,y2,0,"Capacity: " + to_string(capacity),FONT,22,0,0,0,BLACK);
    bgp->drawText(x1-200,y2-40,0,"# Items: " + to_string(items),FONT,22,0,0,0,BLACK);
    bgp->drawText(x1-200,y2-80,0,"# Threads: " + to_string(num_threads),FONT,22,0,0,0,BLACK);
    
    // horizontal lines and row labels
    bgp->drawText(x2+50,y2+25,0,"Capacity",FONT,20,0,0,0,BLACK);
    for (int i=0; i<=capacity; i++){
        bgp->drawLine(x1,y1+i*row_wid,0,x2,y1+i*row_wid,0,0,0,0,BLACK);
        bgp->drawText(x2+25,y2-(i+.5)*row_wid,0,to_string(i),FONT,20,0,0,0,RED);
    }
    bgp->drawLine(x1,y2,0,x2,y2,0,0,0,0,BLACK);
    
    // vertical lines and column labels
    bgp->drawText(x1-50,y1-30,0,"Items",FONT,20,0,0,0,BLACK);
    bgp->drawText(x1-50,y1-70,0,"Weights",FONT,20,0,0,0,BLACK);
    bgp->drawText(x1-50,y1-110,0,"Values",FONT,20,0,0,0,BLACK);
    for(int j=0;j<items;j++){
        bgp->drawLine(x1+j*col_wid,y1,0,x1+j*col_wid,y2,0,0,0,0,BLACK);
        bgp->drawText(x1+(j+0.5)*col_wid,y1-30,0,to_string(j+1),FONT,20,0,0,0,RED);
        bgp->drawText(x1+(j+0.5)*col_wid,y1-70,0,to_string(weights[j]),FONT,20,0,0,0,BLACK);
        bgp->drawText(x1+(j+0.5)*col_wid,y1-110,0,to_string(values[j]),FONT,20,0,0,0,BLACK);
    }
    bgp->drawLine(x2,y1,0,x2,y2,0,0,0,0,BLACK);
    
    double seconds, tic = omp_get_wtime();
    // loop of columns (items)
    for(int h = 1; h < items+1; h++){
        // parallelize the rows (capacities) for each column
        #pragma omp parallel for 
        for(int k = 0; k <= capacity; k++) {
            if (k-weights[h-1] > -1 && knap[k][h-1] < values[h-1] + knap[k-weights[h-1]][h-1])
                knap[k][h] = values[h-1] + knap[k - weights[h-1]][h - 1];
            else 
                knap[k][h] = knap[k][h-1];
            // print value to entry after a pause
            c.sleepFor(pause);
            bgp->drawText((x1+(h-0.5)*col_wid),(y2-(k+0.5)*row_wid),0,to_string(knap[k][h]),FONT,20,0,0,0,Colors::highContrastColor(omp_get_thread_num(),135));
        }
    }
    seconds = omp_get_wtime() - tic;
    stringstream ss;
    ss.precision(2);
    ss << pause * (capacity+1) * items / seconds; // seq time = pause * num_table_entries

    // draw Knapsack value and time to canvas
    bgp->drawText(x1-200, y2-200, 0, "Knapsack ", FONT,24,0,0,0,BLACK);
    bgp->drawText(x1-200, y2-240, 0, "Value: " + to_string(knap[capacity][items]), FONT,24,0,0,0,BLACK);
    bgp->drawText(x1-200, y2-320, 0, "Time: " + to_string((int) seconds) + " secs",FONT,24,0,0,0,BLACK);
    bgp->drawText(x1-200, y2-360, 0, "Speedup: " + ss.str() + "x",FONT,24,0,0,0,BLACK);

    //Output Knap array to terminal
    cout << "Knap: " << endl;
    for(int k=0;k<capacity+1;k++){
        for(int h=0; h <items+1; h++){
            cout << knap[k][h] << ", ";
        }
        cout << endl;
    }
    cout << endl;
   
    cout << "Knapsack Value: " << knap[capacity][items] << endl;
    cout << "Time: " << seconds << endl;
    cout << "Speedup: " << ss.str() << endl;
    cout << endl;
    
    
    // traceback if requested
    if(path != "No") {
        int k = capacity;
        bool included = false;
        for(int h = items; h > 0; h--) { 
            c.sleepFor(pause); 
            if(included) {
                // if previous item is included, draw line and two ellipses (with values) over entries
                bgp->drawLine(x1+((h-.5)*col_wid),y2-(k+0.5)*row_wid,0,x1+((h+.5)*col_wid),y2-(k+weights[h]+0.5)*row_wid,0,0,0,0,RED);
                bgp->drawEllipse(x1+(h-.5)*col_wid,y2-(k+0.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
                bgp->drawEllipse(x1+(h-.5)*col_wid,y2-(k+0.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
                bgp->drawText(x1+(h-0.5)*col_wid,(y2-(k+0.5)*row_wid),0,to_string(knap[k][h]),FONT,20,0,0,0,BLACK);
                bgp->drawEllipse(x1+(h+.5)*col_wid,y2-(k+weights[h]+0.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
                bgp->drawEllipse(x1+(h+.5)*col_wid,y2-(k+weights[h]+0.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
                bgp->drawText((x1+((h+0.5)*(col_wid))),y2-(k+weights[h]+0.5)*row_wid,0,to_string(knap[k+weights[h]][h+1]),FONT,20,0,0,0,BLACK);
                // draw ellipse around included item
                bgp->drawEllipse(x1+(h+0.5)*col_wid,y1-30,0,.45*col_wid+2,26,0,0,0,RED); 
                bgp->drawEllipse(x1+(h+0.5)*col_wid,y1-30,0,.45*col_wid,24,0,0,0,WHITE); 
                bgp->drawText(x1+(h+0.5)*col_wid,y1-30,0,to_string(h+1),FONT,20,0,0,0,RED);
                included = false;
            } else {
                // if previous item is not included, just draw ellipse around current entry
                bgp->drawEllipse(x1+(h-0.5)*col_wid,y2-(k+0.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
                bgp->drawEllipse(x1+(h-0.5)*col_wid,y2-(k+0.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
                bgp->drawText(x1+(h-0.5)*col_wid,y2-(k+0.5)*row_wid,0,to_string(knap[k][h]),FONT,20,0,0,0,BLACK);
            }
            // check if item is included       
            if (k-weights[h-1] > -1 && knap[k][h-1] < values[h-1] + knap[k-weights[h-1]][h-1]) {             
                k -= weights[h-1];
                included = true;  
            }    
        }
        // check if 1st item is included, draw ellipse around it
        if(included) {
            c.sleepFor(pause);
            bgp->drawEllipse(x1+0.5*col_wid,y1-30,0,.45*col_wid+2,26,0,0,0,RED);
            bgp->drawEllipse(x1+0.5*col_wid,y1-30,0,.45*col_wid,24,0,0,0,WHITE);
            bgp->drawText(x1+0.5*col_wid,y1-30,0,to_string(1),FONT,20,0,0,0,RED);
        }    
    }
    
    // free memory
    for(int k = 0; k <= capacity; k++)
        free(knap[k]);
    free(knap);
    free(values);
    free(weights);
    
    c.wait(); // wait for the user to close the canvas
}


