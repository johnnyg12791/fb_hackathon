#ifndef _VIRTUALELE
#define _VIRTUALELE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class Virtualele {
private:
    CvCapture * capture;
    IplImage * currentFrame;
    char * mainDisplayName;
    
    Uke uke;
    
public:
    
    /* Function: constructor
     * ---------------------
     * sets up communication with the webcam;
     */
    Virtualele (char * newMainDisplayName)
    {
        capture = cvCreateCameraCapture (-1);
        mainDisplayName = newMainDisplayName;
    }
    
    
    /* Function: GetNextFrame
     * ----------------------
     * this will fill 'currentFrame' with updated information.
     */
    void GetNextFrame () {
        cvGrabFrame (capture);
        currentFrame = cvRetrieveFrame (capture);
    }
    
    
    /* Function: DisplayFrame
     * this function will display 'currentFrame' on the main display.
     */
    void DisplayFrame () {
        cvShowImage (mainDisplayName, currentFrame);
    }
    
};

#endif