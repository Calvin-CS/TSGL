//
//  LCS.cpp
//
//  Created by Sarah Parsons on 11/23/19.
//
//  Updated by Grey Ballard on 7/31/21
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

// function for generating random string of length len from given alphabet
string gen_random(int len) {
    string s = "", alphabet = "ACTG";
    for (int i = 0; i < len; ++i) {
        s += alphabet[rand() % alphabet.length()];
    }
    return s;
}

int main(int argc, char* argv[]) {

    // parse inputs
    // usage: ./LCS THREADS CAPACITY ITEMS TRACEPATH
    int num_threads = (argc > 1) ? atoi(argv[1]) : 2; 
    string x = (argc > 2) ? argv[2] : gen_random(5);
    string y = (argc > 3) ? argv[3] : gen_random(5);
    string path = (argc > 4) ? argv[4] : "No";
    
    // print parameters to terminal
    cout << "1st string: " << x << endl;
    cout << "2nd string: " << y << endl;
    int m = x.length(), n = y.length();
    cout << "# of Threads: " << num_threads << endl;
    omp_set_num_threads(num_threads);
    cout << "Trace path? " << path << endl; 
    std::cout << "# Diagonals: " << m+n-1 << endl << endl;
    
    
    // allocate memory for DP table
    int** LCS = (int**) malloc((m+1) * sizeof(int*));
    for(int i = 0; i <= m; i++) {
        LCS[i] = (int*) malloc((n+1) * sizeof(int));
    }
    
    // initialize 1st row and column to 0s
    for(int i = 0; i <= m; i++)
        LCS[i][0] = 0;
    for(int j = 0; j <= n; j++)
        LCS[0][j] = 0; 

    // construct canvas 
    Canvas c(0, 0, 1200, 800, "0-1 Knapsack Problem",WHITE);
    c.start();   

    //Create a Background ptr to draw 2D Visualizations
    Background *bgp = c.getBackground();  
    
    // set parameters for drawing table
    float x1 = -250, x2 = 250, y1 = -250, y2 = 250, offset = 30; 
    float size = x2-x1, row_wid = size / m, col_wid = size / n; // for table entries
    float cr_wid = row_wid/2 < offset ? row_wid/2 : offset; // for circles around x chars
    float cc_wid = col_wid/2 < offset ? col_wid/2 : offset; // for circles around y chars
    
    // draw title and parameters to canvas   
    bgp->drawText(0, y2+75, 0, "LCS Problem", FONT,30,0,0,0,RED);
    bgp->drawText(x1-200, y2, 0, "# Threads: " + to_string(num_threads), FONT,22,0,0,0,BLACK);
    bgp->drawText(x1-200, y2-40, 0, "# Diagonals: " + to_string(m+n-1),FONT,22,0,0,0,BLACK);
    bgp->drawText(0, y1-50, 0, "String x: " + x, FONT,22,0,0,0,BLACK);
    bgp->drawText(0, y1-90, 0, "String y: " + y, FONT,22,0,0,0,BLACK);
    
    
    // draw x- and y-axes and lines within grid based on column and row widths
    for (int i=0; i<m; i++){
        bgp->drawLine(x1,y2-i*row_wid,0,x2,y2-i*row_wid,0,0,0,0,BLACK);
        bgp->drawText(x1-30,y2-(i+0.5)*row_wid,0,to_string(x[i]),FONT,23,0,0,0,RED);
    }
    bgp->drawLine(x1,y1,0,x2,y1,0,0,0,0,BLACK);
    
    for (int j=0; j<n; j++){
        bgp->drawLine(x1+(j*col_wid),y1,0,x1+(j*col_wid),y2,0,0,0,0,BLACK);
        bgp->drawText(x1+(j+0.5)*col_wid,y2+30,0,to_string(y[j]),FONT,23,0,0,0,RED);
    }
    bgp->drawLine(x2,y1,0,x2,y2,0,0,0,0,BLACK);
    
    // fill in table one anti-diagonal at a time
    double seconds, tic = omp_get_wtime();
    int start_row, diag_length;
    // loop over anti-diagonals
    for(int d = 1; d < m+n; d++) {
        // determine starting row and length of anti-diagonal
        if(d <= m) {
            start_row = d;
            diag_length = d < n ? d : n;
        } else {
            start_row = m;
            diag_length = d < n ? m : m+n-d;
        }
        // parallelize over entries in anti-diagonals
        #pragma omp parallel for
        for(int k = 1; k <= diag_length; k++) {
            int i = start_row - k + 1;
            int j = d - start_row + k;
            if(x[i-1] == y[j-1]) {
                LCS[i][j] = LCS[i-1][j-1] + 1;
            } else {
                LCS[i][j] = LCS[i-1][j] > LCS[i][j-1] ? LCS[i-1][j] : LCS[i][j-1];
            }
            // draw value in entry after pause
            c.sleepFor(1);
            bgp->drawText(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,to_string(LCS[i][j]),FONT,20,0,0,0,BLACK);
        }
    }
    seconds = omp_get_wtime() - tic;
    
    
    // print LCS table
    for(int i = 0; i <=m; i++) {
        for(int j = 0; j<=n; j++) 
            cout << LCS[i][j] << " ";
        cout << endl;
    }
    cout << endl;
    cout << "LCS Value: " << LCS[m][n] << endl;
    cout << "Time: " << seconds << endl;
    
    // draw LCS Value to canvas
    bgp->drawText(x1-200, y2-200, 0, "LCS Value: " + to_string(LCS[m][n]), FONT,24,0,0,0,RED);
    bgp->drawText(x1-200, y2-250, 0, "Time: " + to_string((int) seconds) + " secs", FONT,24,0,0,0,BLACK);
    
    // traceback if requested
    if(path != "No") {
        int i = m, j = n;
        // draw ellipse around entry    
        c.sleepFor(1);    
        bgp->drawEllipse(x1+(j-.5)*col_wid,y2-(i-.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
        bgp->drawEllipse(x1+(j-.5)*col_wid,y2-(i-.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
        bgp->drawText(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,to_string(LCS[i][j]),FONT,20,0,0,0,BLACK);
        while(i > 0 && j > 0) {  
            c.sleepFor(1);
            // determine previous entry
            if(x[i-1] == y[j-1]) {
                // draw circle around character in each string
                bgp->drawEllipse(x1-30,y2-(i-0.5)*row_wid,0,cr_wid,cr_wid,0,0,0,BLACK);
                bgp->drawEllipse(x1-30,y2-(i-0.5)*row_wid,0,cr_wid-2,cr_wid-2,0,0,0,WHITE);
                bgp->drawText(x1-30,y2-(i-0.5)*row_wid,0,to_string(x[i-1]),FONT,23,0,0,0,RED);
                bgp->drawEllipse(x1+(j-0.5)*col_wid,y2+30,0,cc_wid,cc_wid,0,0,0,BLACK);
                bgp->drawEllipse(x1+(j-0.5)*col_wid,y2+30,0,cc_wid-2,cc_wid-2,0,0,0,WHITE);
                bgp->drawText(x1+(j-0.5)*col_wid,y2+30,0,to_string(y[j-1]),FONT,23,0,0,0,RED);
                if(i > 1 && j > 1) {
                    // draw diagonal line and redraw previous circle
                    bgp->drawLine(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,x1+(j-1.5)*col_wid,y2-(i-1.5)*row_wid,0,0,0,0,RED);
                    bgp->drawEllipse(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
                    bgp->drawEllipse(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
                    bgp->drawText(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,to_string(LCS[i][j]),FONT,20,0,0,0,BLACK);
                }
                i--; j--;
            } else {
                LCS[i-1][j] > LCS[i][j-1] ? i-- : j--;
            }
            if(i > 0 && j > 0) {
                // draw ellipse around entry        
                bgp->drawEllipse(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,col_wid/2,row_wid/2,0,0,0,RED);
                bgp->drawEllipse(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,col_wid/2-2,row_wid/2-2,0,0,0,WHITE);
                bgp->drawText(x1+(j-0.5)*col_wid,y2-(i-0.5)*row_wid,0,to_string(LCS[i][j]),FONT,20,0,0,0,BLACK);
            }
        }
        // handle last character
        /*if(x[i-1] == y[j-1]) {
            // draw circle around character in each string
            c.sleepFor(1);
            bgp->drawEllipse(x1-30,y2-(i-0.5)*row_wid,0,cr_wid,cr_wid,0,0,0,BLACK);
            bgp->drawEllipse(x1-30,y2-(i-0.5)*row_wid,0,cr_wid-2,cr_wid-2,0,0,0,WHITE);
            bgp->drawText(x1-30,y2-(i-0.5)*row_wid,0,to_string(x[i-1]),FONT,23,0,0,0,RED);
            bgp->drawEllipse(x1+(j-0.5)*col_wid,y2+30,0,cc_wid,cc_wid,0,0,0,BLACK);
            bgp->drawEllipse(x1+(j-0.5)*col_wid,y2+30,0,cc_wid-2,cc_wid-2,0,0,0,WHITE);
            bgp->drawText(x1+(j-0.5)*col_wid,y2+30,0,to_string(y[j-1]),FONT,23,0,0,0,RED);
        }*/
  
    }
    
    // free memory
    for(int i = 0; i <= m; i++)
        free(LCS[i]);
    free(LCS); 
    
    c.wait(); // wait for the user to close the canvas
    
}


