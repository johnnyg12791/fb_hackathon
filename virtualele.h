#ifndef _VIRTUALELE
#define _VIRTUALELE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "uke.h"

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class Virtualele {
private:
    CvCapture * capture;
    IplImage * currentFrame;
    char * mainDisplayName;
    
    Uke *uke;
    
public:
    
    
    /* Function: GetNextFrame
     * ----------------------
     * this will fill 'currentFrame' with updated information.
     */
    void GetNextFrame () {
        cvGrabFrame (capture);
        currentFrame = cvRetrieveFrame (capture);
        //cvCvtColor (currentFrame, currentFrame, CV_BGR2GRAY);
    }
    
    
    /* Function: DisplayFrame
     * ----------------------
     * this function will display 'currentFrame' on the main display.
     */
    void DisplayFrame () {
        cvShowImage (mainDisplayName, currentFrame);
    }
    
    
    
    /* Function: LocateUke
     * -------------------
     * this function will have the Uke object locate the Uke.
     */
    int LocateUke () {
        return 0;
    }
    
    /* Function: DetermineFingering
     * ----------------------------
     * this function will track the uke and determine the fingering...
     */
    int DetermineFingering () {
        return 0;
    }
    
    
    /* Function: constructor
     * ---------------------
     * sets up communication with the webcam;
     */
    Virtualele (char * newMainDisplayName, IplImage *fullResTemplate)
    {
        capture = cvCreateCameraCapture (-1);
        GetNextFrame ();
        mainDisplayName = newMainDisplayName;
        
        uke = new Uke (currentFrame, fullResTemplate);
    }
    
    
    
    /* Function: destructor
     * --------------------
     */
    ~Virtualele () {
    }
    
    
};

#endif
