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
    
    IplImage *fullResTemplate;
    
    Uke *uke;
    
public:

    
    
    
    /* Function: DetectUke
     * -------------------
     * this function will have the Uke object locate the Uke. returns 1 when it works, 0 otherwise.
     */
    int DetectUke () {
        uke->DetectBoundaryMarkers (currentFrame);
        uke->DrawUkeFrame (currentFrame);
    }
    
    int DetectUke (IplImage *testImage) {
        uke->DetectBoundaryMarkers (testImage);
        uke->DrawUkeFrame (testImage);
    }
    
    int TrackUke () {
        uke->TrackBoundaryMarkers (currentFrame);
        uke->DrawUkeFrame (currentFrame);
    }
    
    void CalibrateUke () {
        uke->GrabBoundaryMarkerTemplates ();
        uke->GrabInactiveKeyTemplates ();
        int key = 0;
        while (key != 'c') {
            GetNextFrame ();
            DisplayFrame ();
            key = cvWaitKey(30);
        }
        uke->GrabActiveKeyTemplates ();
    }
    
    
    
    
    
    
    
    /* Function: DetermineFingering
     * ----------------------------
     * this function will track the uke and determine the fingering...
     */
    int DetermineFingering () {
        return 0;
    }
    
    
    
    
    
    
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
    
    
    
    /* Function: constructor
     * ---------------------
     * sets up communication with the webcam;
     */
    Virtualele (char * newMainDisplayName, IplImage *fullResTemplate_white, IplImage *fullResTemplate_black )
    {
        capture = cvCreateCameraCapture (-1);
        GetNextFrame ();
        mainDisplayName = newMainDisplayName;
        
        uke = new Uke (currentFrame, fullResTemplate_white, fullResTemplate_black);
        cout << "# # # INITIALIZATION COMPLETE # # #\n";
    }
    
    
    
    /* Function: destructor
     * --------------------
     */
    ~Virtualele () {
    }
    
    
};

#endif
