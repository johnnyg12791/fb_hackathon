#ifndef _BOUNDARY
#define _BOUNDARY
#include <iostream>
/* for opencv */
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <stdlib.h>
#include <stdio.h>

#define SCAN_RATIO 2

using namespace std;
using namespace cv;

class BoundaryMarker {
public:
    
    IplImage *bmTemplate;
    
    /*the boundary itself*/
    Point topLeft;
    Point bottomRight;
    Point center;
    int width, height;
    int resolutionIndex;
    
    /*surrounding scan region*/
    CvRect scanRegion;
    //Point scanTopLeft;
    //Point scanBottomRight;
    Mat *matchMatrix;
    
    int imageHeight, imageWidth;
    
    Point GetTopLeft () {
        return topLeft;
    }
    Point GetBottomRight () {
        return bottomRight;
    }
    void UpdateCenter () {
        center.x = (topLeft.x + bottomRight.x) / 2;
        center.y = (topLeft.y + bottomRight.y) / 2;
    }
    Point GetCenter () {
        return center;
    }
    
    void CalculateScanRegion () {
        scanRegion.x = center.x - SCAN_RATIO*width/2;
        scanRegion.y = center.y - SCAN_RATIO*height/2;
        scanRegion.width = width*SCAN_RATIO;
        scanRegion.height = height*SCAN_RATIO;
   
        
        if (scanRegion.x < 0) scanRegion.x = 0;
        if (scanRegion.y < 0) scanRegion.y = 0;
        if (scanRegion.x + scanRegion.width >= imageWidth) {
            scanRegion.width = imageWidth - scanRegion.x - 1;
        }
        if (scanRegion.y + scanRegion.height >= imageHeight) {
            scanRegion.height = imageHeight - scanRegion.y - 1;
        }
    }
    
    
    /* Function: Set
     * -------------
     * The client supplies this function with the image of the template that detected it and the coordinates of
     * the top left corner where it was located. 
     */
    void Set (IplImage *temporaryTemplate, Point tLeft) {

        bmTemplate = cvCloneImage (temporaryTemplate);
        width = bmTemplate->width;
        height = bmTemplate->height;
        topLeft.x = tLeft.x;
        topLeft.y = tLeft.y;
        bottomRight.x = topLeft.x + width;
        bottomRight.y = topLeft.y + height;
        UpdateCenter ();
        CalculateScanRegion ();
        matchMatrix = new Mat (scanRegion.width - width + 1, scanRegion.height - height + 1, CV_8UC1);
        
    }
    
    
    
    
    void Update (Point tLeft) {
        topLeft.x = tLeft.x;
        topLeft.y = tLeft.y;
        bottomRight.x = topLeft.x + width;
        bottomRight.y = topLeft.y + height;

        UpdateCenter ();
        CalculateScanRegion ();
    }
    
    /* Function: SetBmTemplate
     * -----------------------
     * this function accepts the current frame and extracts from it it's own image.
     */
    void SetBmTemplate (IplImage *currentFrame) {
        cvSetImageROI (currentFrame, cvRect (topLeft.x, topLeft.y, width, height));
        bmTemplate = cvCloneImage(currentFrame);
        cvResetImageROI (currentFrame);
    }
    
    
    
    
    void init (int Height, int Width) {
        imageHeight = Height;
        imageWidth = Width;
    }
    
    BoundaryMarker () {
    }

};

#endif