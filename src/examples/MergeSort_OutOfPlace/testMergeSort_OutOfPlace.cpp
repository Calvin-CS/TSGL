// TO EXECUTE: ./mergesort random_seed list_length num_processors s|p (s for sequential merge and p for parallel merge)

#include <iostream>
#include <algorithm>
#include <climits>
#include <cassert>
#include <tsgl.h>
#include <Text.h>
#include <Canvas.h>
#include <Color.h>
#include <iostream>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;
using namespace tsgl;


float PAUSE = .05;   // controls how fast the visualization moves
float RHEIGHT = .36; // controls rectangle height
float AWIDTH = .9;   // controls array width

void mergesort(int* a, int* b, int n, int bc, char merge_type, Canvas& can, Rectangle** recta, Rectangle** rectb, ColorFloat* color);
void medianofunion(int* a, int n, int& ia, int* b, int m, int& ib);
void recmerge(int* a, int n, int* b, int m, int* c, int bc, Canvas& can, Rectangle** recta, Rectangle** rectb, Rectangle** rectc, ColorFloat* color);

void SeqMergeSort(int* a, int* b, int size, Canvas& can, Rectangle** recta, Rectangle** rectb, ColorFloat* color);
void SeqMerge(int* a, int size1, int* b, int size2, int* c, Canvas& can, Rectangle** recta, Rectangle** rectb, Rectangle** rectc,ColorFloat* color);


int main(int argc, char** argv) {

    // parse inputs
    if(argc < 4 || argc > 5) {
        cout << "Usage: ./a.out seed length nthreads (merge: s|p)" << endl;
        return 1;
    }
    srand(atoi(argv[1]));
    int length = atoi(argv[2]);
    int nthreads = atoi(argv[3]);
    char merge_type = 'p';
    if(argc == 5) merge_type = argv[4][0];
    if (merge_type != 's' && merge_type != 'p'){
        cout << "Usage: ./a.out seed length nthreads (merge: s|p)" << endl;
        return 1;
    }
    omp_set_num_threads(nthreads);

    // set base case size for switching from parallel to sequential
    int bc = length / nthreads;

    // allocate memory
    int* input = new int[length]; // array to be sorted
    int* output = new int[length]; // output array
    int* correct = new int[length]; // copy for checking

    // set height and width of display
    int h = 800;
    int w = 1331;

    // initialize canvas, rectangles, and colors
    Canvas can(0, 0, w, h, "Parallel Merge Sort", BLACK);
    Rectangle** inrectangles = new Rectangle*[length]; 
    Rectangle** outrectangles = new Rectangle*[length]; 
    ColorFloat* color = new ColorFloat[nthreads + 1];
    
    // intialize the colors
    for(int i = 0; i < nthreads; ++i){
        color[i] = Colors::highContrastColor(i);
    }

    // start visualization
    can.start();

    // generate input values for array and initalize the rectangles
    float start = -can.getWindowWidth() * AWIDTH / 2.0;
    float width = can.getWindowWidth() * AWIDTH / length;
    for (int i = 0; i < length; i++) {
        // save initial values in output for copyless mergesort and in separate array correct for checking
        input[i] = output[i] = correct[i] = saferand(2,can.getWindowHeight());

        inrectangles[i] = new Rectangle(start + i * width, (h/4) + 50, -50, width, input[i]*RHEIGHT, 0, 0, 0, GRAY); 
        inrectangles[i]->setIsOutlined(false);
        can.add(inrectangles[i]);

        outrectangles[i] = new Rectangle(start + i * width, -(h/4), -50, width, output[i]*RHEIGHT, 0, 0, 0, GRAY); 
        outrectangles[i]->setIsOutlined(false);       
        can.add(outrectangles[i]);
    }

    // sort array using mergesort 
    #pragma omp parallel shared(can)
    {
        #pragma omp single
        mergesort(input, output, length, bc, merge_type, can, inrectangles, outrectangles, color);
    }

    // sort array using STL 
    sort(correct,correct+length);

    // check outputs, make sure they match
    for(int i = 0; i < length; i++) {
        assert(output[i] == correct[i]);
    }

    // wait for user to close window
    can.wait();

    // clean up memory
    for (int i = 0; i < length; i++) {
        delete outrectangles[i];
        delete inrectangles[i];
    }
    delete [] color;
    delete [] outrectangles;
    delete [] inrectangles;
    delete [] correct;
    delete [] output;
    delete [] input;

    return 0;

}

// sorts array a of length n, tmp is workspace of length n,
// bc is base case size to switch to STL sort
void mergesort(int* a, int* b, int n, int bc, char merge_type, Canvas& can, Rectangle** recta, Rectangle** rectb, ColorFloat* color)
{
    if(n <= bc) {
        // run sequential mergesort
        SeqMergeSort(a, b, n, can, recta, rectb, color); 
        return;
    }
    
    // sort left and right recursively
    int mid = n / 2;
    #pragma omp task shared(can)
    mergesort(b, a, mid, bc, merge_type, can, rectb, recta, color);
    #pragma omp task shared(can)
    mergesort(b + mid, a + mid, n - mid, bc, merge_type, can, rectb + mid, recta + mid, color);
    #pragma omp taskwait

    if (merge_type == 's') {
        // run sequential merge
        SeqMerge(a, mid, a+mid, n-mid, b, can, recta, recta+mid, rectb, color);
    } else {
        // run parallel merge
        recmerge(a, mid, a+mid, n-mid, b, bc, can, recta, recta+mid, rectb, color);
    }
    
}

// merges sorted arrays a (length n) and b (length m) into array c (length n+m),
// bc is base case size to switch to STL merge
void recmerge(int* a, int n, int* b, int m, int* c, int bc, Canvas& can, Rectangle** recta, Rectangle** rectb, Rectangle** rectc, ColorFloat* color) {
    if(n+m<=bc) {
        // run sequential merge
        SeqMerge(a, n, b, m, c, can, recta, rectb, rectc, color);
        return;
    }

    // compute median of union with i elements of a and j elements of b <= median
    int i, j;
    medianofunion(a, n, i, b, m, j);

    // merge left and right recursively
    #pragma omp task shared(can)
    recmerge(a, i, b, j, c, bc, can, recta, rectb, rectc, color);
    #pragma omp task shared(can)
    recmerge(a+i, n-i, b+j, m-j, c+i+j, bc, can, recta + i, rectb + j, rectc + i + j, color);
    #pragma omp taskwait
    
}

   
// computes median of union of array a of length n and array b of length m
// assuming elements of a and b are already internally sorted; upon return:
// ma is number of elements in a less than or equal to median of union,
// mb is number of elements in b less than or equal to median of union
void medianofunion(int *a, int n, int& ma, int *b, int m, int& mb) 
{
    // enforce that a is smaller of two arrays
    if(n > m) {
        medianofunion(b,m,mb,a,n,ma);
        return;
    }

    // run binary search in array a
    int i = 0;
    int j = n;
    while (i <= j) {
        // get middle two elements of each array (use extremes to handle corner cases)
        ma = (i + j) / 2;
        mb = (n + m + 1) / 2 - ma;
        int la = (ma > 0) ? a[ma - 1]: INT_MIN; 
        int lb = (mb > 0) ? b[mb - 1] : INT_MIN;
        int ra = (ma < n) ? a[ma] : INT_MAX;
        int rb = (mb < m) ? b[mb] : INT_MAX;

        // check for complete (la <= {ra,rb} and lb <= {ra,rb})
        if (la <= rb && lb <= ra)
            return;

        // continue search
        if (la > rb) 
            j = ma - 1;
        else // lb > ra
            i = ma + 1;
    }
}

// Sequential Merge 
void SeqMerge(int* a, int size1, int* b, int size2, int* dest, Canvas& can, Rectangle** recta, Rectangle** rectb, Rectangle** rectc, ColorFloat* color){
    int i = 0, j = 0, k = 0;
    ColorFloat thrColor = color[omp_get_thread_num()];

    while(i < size1 && j < size2){
        can.sleepFor(PAUSE);

        if (a[i] <= b[j]) {
            dest[k] = a[i];

            // writing
            rectc[k]->setHeight(dest[k]*RHEIGHT);
            rectc[k]->setColor(WHITE); 

            // reading
            recta[i]->setColor(WHITE);

            i++;
            k++;

            can.sleepFor(PAUSE);

            // color in previous element
            rectc[k - 1]->setColor(thrColor);
            recta[i - 1]->setColor(Colors::blend(thrColor, WHITE, 0.5f)); 

        } else {
            dest[k] = b[j];

            // writing
            rectc[k]->setHeight(dest[k]*RHEIGHT);
            rectc[k]->setColor(WHITE); 

            // reading
            rectb[j]->setColor(WHITE);

            j++;
            k++;

            can.sleepFor(PAUSE);

            // color in previous element
            rectc[k - 1]->setColor(thrColor);
            rectb[j - 1]->setColor(Colors::blend(thrColor, WHITE, 0.5f)); 
            
        }
    }

    while(i < size1){
        can.sleepFor(PAUSE);

        dest[k] = a[i];

        // writing
        rectc[k]->setHeight(dest[k]*RHEIGHT);
        rectc[k]->setColor(WHITE);

        // reading
        recta[i]->setColor(WHITE);

        i++;
        k++;

        can.sleepFor(PAUSE);
        
        // color in previous element
        rectc[k - 1]->setColor(thrColor);
        recta[i - 1]->setColor(Colors::blend(thrColor, WHITE, 0.5f)); 

    }

    while(j < size2){
        can.sleepFor(PAUSE);

        dest[k] = b[j];
    
        // writing
        rectc[k]->setHeight(dest[k]*RHEIGHT);
        rectc[k]->setColor(WHITE);

        // reading
        rectb[j]->setColor(WHITE);

        j++;
        k++;

        can.sleepFor(PAUSE);

        // color in previous element
        rectc[k - 1]->setColor(thrColor);  
        rectb[j - 1]->setColor(Colors::blend(thrColor, WHITE, 0.5f)); 

        
    }
}

// Sequential Mergesort
void SeqMergeSort(int* a, int* b, int size, Canvas& can, Rectangle** recta, Rectangle** rectb, ColorFloat* color){
    // quick return in base case
    if(size <= 1) return;

    // sort recursively and merge
    int mid = size / 2;
    SeqMergeSort(b, a, mid, can, rectb, recta, color);
    SeqMergeSort(b + mid, a + mid, size - mid, can, rectb + mid, recta + mid, color);
    SeqMerge(a, mid, a + mid, size - mid, b, can, recta, recta+mid, rectb, color); 
    
}