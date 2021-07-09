//
//  Knapsack.cpp
//
//
//  Created by Sarah Parsons on 11/23/19.
//  Last updated on 12/11/19.
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
    // "Yes"/"No" path parameter for outputting trace at onset of program; default is "No"
    string path = (argc > 2) ? argv[2] : "No";

    //construct canvas with a width of 1200 and height of 700, white background color
    Canvas c(0, 0, 1200, 700, "0-1 Knapsack Problem");
    c.setBackgroundColor(WHITE);

    //Create a Background ptr to draw 2D Visualizations
    Background *bgp = c.getBackground();

    // initialize button for keyboard interaction used to trace to false
    bool leftMouseButtonPressed = false;

    //Bind the left mouse button so that when it's pressed
    //the boolean is set to true.
    c.bindToButton(TSGL_A, TSGL_PRESS,
                   [&leftMouseButtonPressed]() {
                       leftMouseButtonPressed = true;
                   }
                   );

    //Bind the left mouse button again so that when it's released
    //the boolean is set to false.
    c.bindToButton(TSGL_A, TSGL_RELEASE,
                   [&leftMouseButtonPressed]() {
                       leftMouseButtonPressed = false;
                   }
                   );

    // define parameters for 2D grid size (500 x 500)
    c.start();
    int x1 = -150;
    int x2 = 350;
    int y1 = -325;
    int y2 = -320;
    int size = x2 - x1;

    // draw title and 2D grid (yellow) to canvas
    bgp->drawText(-225,300,0,"0-1 Knapsack Problem",FONT,30,0,0,0,BLACK);
    //c.drawRectangle(x1,y1,x2,y2,YELLOW,true);

    // set number of threads as a parameter from command line; default is 2
    int np = (argc > 1) ? atoi(argv[1]) : 2; //atoi(argv[1]);
    omp_set_num_threads(np);

    // draw # threads to canvas
    bgp->drawText(-525,170,0,"# Threads: " + to_string(np),FONT,22,0,0,0,BLACK);

    // define max value and max weight
    const int MAX_VALUE  = 5;
    const int MAX_WEIGHT = 5;

    // initialize Capacity parameter and draw to canvas
    const int Capacity = 12; //(argc > 3) ? atoi(argv[3]) : 8; //atoi(argv[2]);
    cout << "\nCapacity: " << Capacity << "\n";
    bgp->drawText(-525,300,0,"Capacity: " + to_string(Capacity),FONT,22,0,0,0,BLACK);
    double tic, seconds;

    // initialize number of items and determine column and row widths based on items and Capacity
    int items = 8; //(argc > 4) ? atoi(argv[4]) : 6; //atoi(argv[3]);
    int col_wid = size/(items);
    int row_wid = size/(Capacity);
    cout << col_wid << " " << row_wid << endl;

    // draw x- and y-axes and lines within grid based on column and row widths
    bgp->drawText(x2-600+(col_wid/4),y2+100-(row_wid/3),0,"Capacity",FONT,20,0,0,0,BLACK);
    for(int u=1;u<Capacity+1;u++){
        bgp->drawText(x2-600+(col_wid/4),y2+100+((u-0.5)*row_wid),0,to_string(u),FONT,20,0,0,0,BLACK);
    }

    for (int i=0; i<Capacity; i++){
        bgp->drawLine(x1,y2+100+((i)*(row_wid)),0,x2-600,y2+100+((i)*(row_wid)),0,0,0,0,BLACK);
    }


    bgp->drawText(x1-1.5*(col_wid),y1+(row_wid/2),0,"Num_Items",FONT,20,0,0,0,BLACK);
    for(int y=1;y<items+1;y++){
        bgp->drawText((x1+((y-0.5)*(col_wid))),y1+(row_wid/2),0,to_string(y),FONT,20,0,0,0,BLACK);
    }
    for (int j=0; j<items; j++){
        bgp->drawLine(x1+(j*col_wid),y1,0,x1+(j*col_wid),y2+100,0,0,0,0,BLACK);
    }


    //Weights Vector List
    // randomly assign weights based on max weight and draw weights to canvas
    srand(1);
    vector <int> weights(items,0);
    cout << "# of Weights: " << items << endl;
    for (int w =0; w < items; w++) {
        weights[w] = 1 + (rand() % min(MAX_WEIGHT, Capacity));
        bgp->drawText(-525,270,0,"Weights: ",FONT,22,0,0,0,BLACK);
        bgp->drawText(-525+w*25,250,0,"|" + to_string(weights[w]) + "| ",FONT,20,0,0,0,BLACK);
        cout << weights.at(w);}
    cout << endl;

    //Values Vector List
    // randomly assign values based on max value and draw values to canvas
    vector <int> values(items,0);
    cout << "# of Values: " << items << endl;
    for (int v =0; v < (int)values.size(); v++) {
        values[v] = 1 + (rand() % MAX_VALUE);
        bgp->drawText(-525,220,0,"Values: ",FONT,22,0,0,0,BLACK);
        bgp->drawText(-525+v*25,200,0,"|" + to_string(values[v])+"| ",FONT,20,0,0,0,BLACK);
        cout << values.at(v);}
    cout << endl;

    //int dims = Capacity+1;

    //Draw top row
    int knap[Capacity+1][Capacity+1];
    //vector<vector<int>> knap(Capacity+1,vector<int>(Capacity+1));
    int k=0;
    int h=0;

    for(int h=0; h<(int)weights.size()+1; h++) {
        //knap[k][h]=0;
        if(path=="Yes"){bgp->drawText(x1+((h-0.5)*col_wid),y2+100-(row_wid/2),0,"0",FONT,20,0,0,0,BLACK);}
    }

    //Initialize table and draw 0 column
    for(int k=0;k<Capacity+1;k++){
        if(path=="Yes"){bgp->drawText(x1-(col_wid/2),y2+100+((k-0.5)*row_wid),0,"0",FONT,20,0,0,0,BLACK);}
        // for(int h=0; h<1;h++){
        for(int h=0; h<(int)weights.size()+1;h++){
            knap[k][h]=0;
        }}


    tic = omp_get_wtime();

    // Check that canvas is open, Iterate through each column and evaluate the knapsack value, store value in knap array, draw values to canvas and draw lines and circles for tracing if path parameter set to "Yes"
    if(c.isOpen()){
        for(h=1;h<items+1;h++){
            // parallelize the rows for each column
            #pragma omp parallel for
            for(k=1;k<Capacity+1;k++) {
                if (knap[k][h-1] < (values[h-1] + knap[k-weights[h-1]][h-1]) && k-weights[h-1] > -1) {
                    knap[k][h] = values[h-1] + knap[k - weights[h-1]][h - 1];
                    if(path == "Yes"){
                        int r = rand()%MAX_COLOR;
                        int g = rand()%MAX_COLOR;
                        int b = rand()%MAX_COLOR;
                        c.sleepFor(1);
                        bgp->drawCircle(x1+((h-1.5)*(col_wid)),y2+100+(((k-weights[h-1])-0.5)*(row_wid)),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawCircle((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawLine(x1+((h-1.5)*(col_wid)),y2+100+(((k-weights[h-1])-0.5)*(row_wid)),0,(x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,0,0,0,BLACK);
                    }
                c.sleepFor(1);
                bgp->drawText((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,to_string(knap[k][h]),FONT,20,0,0,0,BLACK);
                    }
                else { knap[k][h] = knap[k][h-1];
                    if(path == "Yes"){
                        int r = rand()%MAX_COLOR;
                        int g = rand()%MAX_COLOR;
                        int b = rand()%MAX_COLOR;
                        c.sleepFor(1);
                        bgp->drawCircle(x1+((h-1.5)*(col_wid)),y2+100+((k-0.5)*(row_wid)),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawCircle((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawLine(x1+((h-1.5)*(col_wid)),y2+100+((k-0.5)*(row_wid)),0,(x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,0,0,0,BLACK);

                    }
                c.sleepFor(1);
                bgp->drawText((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,to_string(knap[k][h]),FONT,20,0,0,0,BLACK);

                    }
                }

        }
    }

    //Output Knap array
    cout << "Knap: " << endl;
    for(int k=0;k<Capacity+1;k++){
        for(int h=0; h <(int)weights.size()+1; h++){
            cout << knap[k][h] << ", ";
        }
        cout << endl;
    }

    cout << "Knapsack Value: " << knap[Capacity][items] << endl;

    cout << endl;

    // draw Knapsack Value to canvas
    bgp->drawText(-515,90,0,"Knapsack ",FONT,24,0,0,0,BLACK);
    bgp->drawText(-515,70,0,"Value: " + to_string(knap[Capacity][items]),FONT,24,0,0,0,BLACK);

    seconds = omp_get_wtime() - tic;

    cout << "time: " << seconds << "\n";

    // trace Knapsack Value upon user keying 'A' if trace not already drawn
    while(c.isOpen()){
        c.sleep();
        if(leftMouseButtonPressed==true && path == "No"){
            //Draw Top Row
            //vector<vector<int>> knap(dims,vector<int>(dims));
            int knap[Capacity+1][Capacity+1];
            //vector<vector<int>> knap(Capacity+1,vector<int>(Capacity+1));
            int k=0;
            int h=0;
            for(int h=0; h<(int)weights.size()+1; h++) {
                bgp->drawText(x1+((h-0.5)*col_wid),y2+100-(row_wid/2),0,"0",FONT,20,0,0,0,BLACK);
            }
            //Initialize Table
            for(int k=0;k<Capacity+1;k++){
                bgp->drawText(x1-(col_wid/2),y2+100+((k-0.5)*row_wid),0,"0",FONT,20,0,0,0,BLACK);
                for(int h=0; h<(int)weights.size()+1;h++){
                    knap[k][h]=0;}
                }
            //Iterate through each column and evaluate the knapsack value, draw lines and circles for tracing
            for(h=1;h<items+1;h++){
                #pragma omp parallel for
                for(k=1;k<Capacity+1;k++) {

                    if (knap[k][h-1] < (values[h-1] + knap[k-weights[h-1]][h-1]) && k-weights[h-1] > -1) {
                        knap[k][h] = values[h-1] + knap[k - weights[h-1]][h - 1];
                        int r = rand()%MAX_COLOR;
                        int g = rand()%MAX_COLOR;
                        int b = rand()%MAX_COLOR;
                        c.sleepFor(1);
                        bgp->drawCircle(x1+((h-1.5)*(col_wid)),y2+100+(((k-weights[h-1])-0.5)*(row_wid)),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawCircle((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawLine(x1+((h-1.5)*(col_wid)),y2+100+(((k-weights[h-1])-0.5)*(row_wid)),0,(x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,0,0,0,BLACK);
                        //if(k==8){
                         //   cout << x1+((h-1.5)*(col_wid)) << ", " << y2+(((k-weights[h-1])-0.5)*(row_wid)) << "\n";}

                    }
                    else { knap[k][h] = knap[k][h-1];
                        int r = rand()%MAX_COLOR;
                        int g = rand()%MAX_COLOR;
                        int b = rand()%MAX_COLOR;
                        c.sleepFor(1);
                        bgp->drawCircle(x1+((h-1.5)*(col_wid)),y2+100+((k-0.5)*(row_wid)),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawCircle((x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,(col_wid/6),0,0,0,ColorInt(r,g,b,50));
                        bgp->drawLine(x1+((h-1.5)*(col_wid)),y2+100+((k-0.5)*(row_wid)),0,(x1+((h-0.5)*(col_wid))),(y2+100+((k-0.5)*(row_wid))),0,0,0,0,BLACK);
                        //if(k==8){
                         //   cout << x1+((h-1.5)*(col_wid))<< ", " << y2+((k-0.5)*(row_wid)) << "\n";
                        //}

                    }
                }

            }
        }
    }


    c.wait();
}


