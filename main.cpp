/* for opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "virtualele.h"

using namespace std;


int main () {
    
    /*windows to display*/
    cvNamedWindow ("MAIN_DISPLAY");
    cvResizeWindow ("MAIN_DISPLAY", 640, 640);

    Virtualele virtualele ("MAIN_DISPLAY");

    
    /*### Delay Until End ###*/
    int key = 0;
    while (key != 'q') {
        virtualele.GetNextFrame ();
        virtualele.DisplayFrame ();
        key = cvWaitKey(30);
    }

  return 0;
}
