#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/* for opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;


int main () {


  CvCapture * capture = cvCreateCameraCapture (-1);
  cvGrabFrame (capture);
  IplImage *img = cvRetrieveFrame (capture);


  cvNamedWindow ("DISPLAY");
  cvResizeWindow ("DISPLAY", 640, 640);

  cvShowImage ("DISPLAY", img);

  int key = 0;
  while (key != 'q') {
    key = cvWaitKey(30);
  }

  return 0;
}
