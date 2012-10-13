/* for opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "virtualele.h"


#define FULL_RES_TEMPLATE_NAME "template.bmp"
#define TEST_IMAGE_NAME "test2.bmp"
using namespace std;


int main () {
    
    /*windows to display*/
    cvNamedWindow ("MAIN_DISPLAY");
    cvResizeWindow ("MAIN_DISPLAY", 640, 640);

    IplImage *testImage = cvLoadImage ( currentScreen)//TEST_IMAGE_NAME, -1);
    IplImage *fullResTemplate = cvLoadImage (FULL_RES_TEMPLATE_NAME, CV_LOAD_IMAGE_GRAYSCALE);
    Virtualele virtualele ("MAIN_DISPLAY", fullResTemplate);
    
    
    /*### Routine #1: find the ukulele ###*/
//    int found = 0;
//    while (!found) {
    
    IplImage *testImage;
    int found = virtualele.LocateUke ();
    //cvShowImage ("MAIN_DISPLAY", testImage);
    virtualele.DisplayFrame ();
    /*### Routine #2: track and play ###*/
    int key = 0;
    while (key != 0) {
        
        virtualele.DetermineFingering ();
        
        key = cvWaitKey (30);
    }
    
    /*### Delay Until End ###*/
    key = 0;
    while (key != 'q') {
        //virtualele.GetNextFrame ();
        //virtualele.DisplayFrame ();
        key = cvWaitKey(30);
    }

  return 0;
}
