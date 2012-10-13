#include <iostream>
#include <stdlib.h>
#Include <stdio.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

class Virtualele {
private:
    CvCapture * capture;
    IplImage * currentFrame;
    
    
public:
    /* Function: constructor
     * ---------------------
     * sets up communication with the webcam;
     */
    Virtualele ()
    {
        capture = cvCreateCameraCapture (-1);
    }
    
    /* Function: GetNextFrame
     * ----------------------
     * this will fill 'currentFrame' with updated information.
     */
    GetNextFrame () {
        cvGrabFrame (capture);
        currentFrame = cvRetreiveFrame (capture);
    }
}
