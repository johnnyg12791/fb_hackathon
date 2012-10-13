/* for opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

#include "virtualele.h"


#define FULL_RES_TEMPLATE_NAME_WHITE "template.bmp"
#define FULL_RES_TEMPLATE_NAME_BLACK "template_black.bmp"

#define TEST_IMAGE_NAME "test_img.jpg"
using namespace std;


int main () {
    
    /*windows to display*/
    cvNamedWindow ("MAIN_DISPLAY");
    cvResizeWindow ("MAIN_DISPLAY", 640, 640);

    IplImage *testImage = cvLoadImage (TEST_IMAGE_NAME);//TEST_IMAGE_NAME, -1);
    IplImage *fullResTemplate_white = cvLoadImage (FULL_RES_TEMPLATE_NAME_WHITE, 0);
    IplImage *fullResTemplate_black = cvLoadImage (FULL_RES_TEMPLATE_NAME_BLACK, 0);

    Virtualele virtualele ("MAIN_DISPLAY", fullResTemplate_white, fullResTemplate_black);
    
    
    /*### Routine #1: find the ukulele ###*/
//    int found = 0;
//    while (!found) {
    
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
