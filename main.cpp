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
    
    int key = 0;
    bool currentlyTracking = false;
    while (true) {
        virtualele.GetNextFrame ();
        key = cvWaitKey (30);
        
        if (key == 'r') {
            virtualele.DetectUke ();
            currentlyTracking = true;
        }
        else if (key == 'c') {
            virtualele.CalibrateUke ();
        }
        else if (key == ' ') {
            return 0;
        }
        else {
            if (currentlyTracking) {
                virtualele.TrackUke ();
                virtualele.DetermineFingering ();
                int currentChord = virtualele.GetCurrentChord ();
                if (currentChord != -1) {
                    virtualele.PlayNote (currentChord);
                }
            }
        }
        
        if (currentlyTracking) virtualele.DisplayFrameMarked ();
        else virtualele.DisplayFrameRegular ();
    }
    return 0;
}
