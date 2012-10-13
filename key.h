#ifndef _KEY
#define _KEY
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>

#define KEY1_RATIO 36
#define KEY2_RATIO 50
#define KEY3_RATIO 64
#define KEY_RADIUS 7

using namespace std;
using namespace cv;

class Key {
public:
    
    IplImage *activeKeyTemplate, *inactiveKeyTemplate;
    
    float scaleFactor;
    Point location;
    CvRect boundingBox;
    int radius;
    
    /* Function: UpdatePosition
     * ------------------------
     * given the coordinates of the two boundary markers, this function will set the value of 
     * 'location.'
     */
    void Update (Point white, Point black) {
        int x_offset = (white.x - black.x)*scaleFactor/100;
        int y_offset = (white.y - black.y)*scaleFactor/100;
        location.x = black.x + x_offset;
        location.y = black.y + y_offset;
        
        boundingBox.x = location.x - radius;
        boundingBox.y = location.y - radius;
        boundingBox.width = radius*2;
        boundingBox.height = radius*2;
    }
    
    
    /* Function: GrabInactiveKeyTemplates
     * ----------------------------------
     * given the coordinates of the two boundary markers, this function will set the value of
     * 'location.'
     */
    void GrabInactiveKeyTemplate (IplImage * currentFrame) {
        cvSetImageROI (currentFrame, boundingBox);
        inactiveKeyTemplate = cvCloneImage(currentFrame);
        cvResetImageROI (currentFrame);
    }
    
    /* Function: GrabActiveKeyTemplates
     * ----------------------------------
     * given the coordinates of the two boundary markers, this function will set the value of
     * 'location.'
     */
    void GrabActiveKeyTemplate (IplImage * currentFrame) {
        cvSetImageROI (currentFrame, boundingBox);
        activeKeyTemplate = cvCloneImage(currentFrame);
        cvResetImageROI (currentFrame);
    }
    
    
    Key (int sf, int r) {
        scaleFactor = sf;
        radius = r;
    }
};

#endif