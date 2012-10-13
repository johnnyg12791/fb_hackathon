#ifndef _KEY
#define _KEY
#include <iostream>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>

#define NUM_OF_KEYS 4
#define KEY1_RATIO 36
#define KEY2_RATIO 50
#define KEY3_RATIO 64
#define KEY4_RATIO 74
#define KEY_RADIUS 13

using namespace std;
using namespace cv;

class Key {
public:
    
    IplImage *activeKeyTemplate, *inactiveKeyTemplate;
    IplImage *currentImage;
    
    float scaleFactor;
    Point location;
    CvRect boundingBox;
    int radius;
    bool activity;
    
    
    bool isActive () {
        return activity;
    }
    
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
    
    
    /* Function: GrabInactiveKeyTemplate
     * ----------------------------------
     * given the coordinates of the two boundary markers, this function will set the value of
     * 'location.'
     */
    void GrabInactiveKeyTemplate (IplImage * currentFrame) {
        cvSetImageROI (currentFrame, boundingBox);
        inactiveKeyTemplate = cvCreateImage (cvSize(boundingBox.width, boundingBox.height), currentFrame->depth, currentFrame->nChannels);
        cvCopy(currentFrame, inactiveKeyTemplate);
        cvResetImageROI (currentFrame);
        cout << "inactiveKeyTemplate.width, height = " << inactiveKeyTemplate->width << ", " << inactiveKeyTemplate->height << "\n";
    }
    
    /* Function: GrabActiveKeyTemplates
     * ----------------------------------
     * given the coordinates of the two boundary markers, this function will set the value of
     * 'location.'
     */
    void GrabActiveKeyTemplate (IplImage * currentFrame) {
        cvSetImageROI (currentFrame, boundingBox);
        activeKeyTemplate = cvCreateImage (cvSize(boundingBox.width, boundingBox.height), currentFrame->depth, currentFrame->nChannels);
        cvCopy(currentFrame, activeKeyTemplate);
        cvResetImageROI (currentFrame);
        
        cout << "activeKeyTemplate.width, height = " << activeKeyTemplate->width << ", " << activeKeyTemplate->height << "\n";
    }
    
    
    
    
    double FindMatrixDistance (IplImage *img1, IplImage *img2) {
        double total = 0;
        char * img1Read = img1->imageData;
        char * img2Read = img2->imageData;
        assert ( (img1->width == img2->width) && (img1->height == img2->height) );
                
        for (int i=0;i<img1->height;i++) {
            for (int j=0;j<img1->width;j++) {
                total +=    pow(    (double) (*(img1Read+(j*3)) - *(img2Read+(j*3))), 2) +
                            pow(    (double) (*(img1Read+(j*3)+1) - *(img2Read+(j*3)+1)), 2) +
                            pow(    (double) (*(img1Read+(j*3)+2) - *(img2Read+(j*3)+2)), 2);
                
            }
            img1Read += img1->widthStep;
            img2Read += img2->widthStep;
        }
        return total;
    }
    
    /*Function: DetermineActivity
     * --------------------------
     */
    void DetermineActivity (IplImage * currentFrame) {
        cvSetImageROI (currentFrame, boundingBox);
        currentImage = cvCloneImage (inactiveKeyTemplate);
        cvCopy(currentFrame, currentImage);
        cvResetImageROI (currentFrame);
        
        
        double activeDistance      = FindMatrixDistance (currentImage, activeKeyTemplate);
        double inactiveDistance    = FindMatrixDistance (currentImage, inactiveKeyTemplate);
        
        if (activeDistance < inactiveDistance) {
            activity = true;
            cout << "KEY IS ACTIVE!!!!\n";
        }
        else {
            cout << "---------------------------\n";
        }activity = false;
    }
    
    
    
    Key (int sf, int r) {
        activity = false;
        scaleFactor = sf;
        radius = r;
    }
};

#endif