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
        
        /*
        scanTopLeft.x = center.x - SCAN_RATIO*width/2;
        scanTopLeft.y = center.y - SCAN_RATIO*height/2;
        scanBottomRight.x = center.x + SCAN_RATIO*width/2;
        scanBottomRight.y = center.y + SCAN_RATIO*height/2;
         */
        /*
        if (scanTopLeft.x < 0) scanTopLeft.x = 0;
        if (scanTopLeft.y < 0) scanTopLeft.y = 0;
        
        if (scanBottomRight.x >= imageWidth) scanBottomRight.x = imageWidth - 1;
        if (scanBottomRight.y >= imageHeight) scanBottomRight.y = imageHeight - 1;
         */
        
    }
    
    void Set (Point tLeft, int r_index, int dimension) {
        resolutionIndex = r_index;
        width = dimension;
        height = dimension;
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
    
    void init (int Height, int Width) {
        imageHeight = Height;
        imageWidth = Width;
    }
    
    BoundaryMarker () {
    }

};

#endif