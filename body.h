#ifndef _BODY
#define _BODYf

#include <iostream>
#include <math.h>
/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace cv;

#define OFFSET_FACTOR 0.2
#define STRUM_THRESHOLD 800

class Body {
public:
    CvRect boundingBox;
    int size;
    IplImage *previousImage;
    IplImage *currentImage;
    
    
    void UpdateLocation (IplImage * currentFrame, Point bmCenter1, Point bmCenter2) {
        
        Point center;
        if (bmCenter1.x > bmCenter2.x) {
            
            center.x = bmCenter1.x + 150;
            center.y = bmCenter1.y;
            //center.x = bmCenter1.x + x_offset;
            //center.y = bmCenter1.y - y_offset;
        }
        else {
            center.x = bmCenter2.x + 150;
            center.y = bmCenter2.y;
        }
        
        boundingBox.x = center.x + (size/2);
        boundingBox.y = center.y - (size/2);
        if (boundingBox.x >= currentFrame->width) boundingBox.x = currentFrame->width - 1;
        if (boundingBox.y <= 0) boundingBox.y = 0;
        if (boundingBox.x + boundingBox.width >= currentFrame->width) boundingBox.width = currentFrame->width - boundingBox.x - 1;
        if (boundingBox.y + boundingBox.height >= currentFrame->height) boundingBox.height = currentFrame->height - boundingBox.y - 1;
        boundingBox.width = size;
        boundingBox.height = size;
        
        cout << "boundgBox.x, y, width, height = " << boundingBox.x << ", " << boundingBox.y << ", " << boundingBox.width << ", " << boundingBox.height << "\n";
        
        previousImage = cvCloneImage (currentImage);
        cvSetImageROI(currentFrame, boundingBox);
        cvCopy (currentFrame, currentImage);
        cvResetImageROI (currentFrame);;
    }
    
    bool isActive () {
        int total = 0;
        
        if ( (previousImage->width == currentImage->width) && (previousImage->height == currentImage->height) ) {
            char *curRead = currentImage->imageData;
            char *prevRead = previousImage->imageData;
        
            for (int i=0;i<currentImage->height;i++) {
                for (int j=0;j<currentImage->width;j++) {
                
                    if (*(curRead + j*3) != *(prevRead+j*3)) {
                        total++;
                    }
                
                }
                curRead  += currentImage->widthStep;
                prevRead += previousImage->widthStep;
            }
        
            cout << "strum total = " << total << "\n";
        
            if (total > STRUM_THRESHOLD) return true;
            else return false;
        }
        else return false;
            
    }


    void init (IplImage *currentFrame, Point bmCenter1, Point bmCenter2, int dimension) {
        cout << "# # # BEGIN INIT # # # \n";
        size = 50;
        currentImage = cvCreateImage (cvSize(size, size), currentFrame->depth, currentFrame->nChannels);
        
        UpdateLocation (currentFrame, bmCenter1, bmCenter2);
        
        cvSetImageROI(currentFrame, boundingBox);
        cout << "currentFrame->depth, nChannels = " << currentFrame->depth << ", " << currentFrame->nChannels << "\n";
        cout << "currentImage->depth, nChannels = " << currentImage->depth << ", " << currentImage->nChannels << "\n";
        cvCopy (currentFrame, currentImage);
        
        
        cvResetImageROI(currentFrame);
        previousImage = cvCloneImage (currentImage);
        
    }
    Body () {
    }
};

#endif 