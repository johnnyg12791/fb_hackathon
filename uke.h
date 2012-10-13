#ifndef _UKE
#define _UKE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SMALLEST_RES_WIDTH 92
#define RES_STEP 3

using namespace std;
using namespace cv;


class MatchMatrix {
public:
    cv::Mat matrix;
    CvPoint globalMax;
    int globalMaxVal;
    
    
    CvPoint GetGlobalMax () {
        return globalMax;
    }
    
    MatchMatrix () {
    }
    MatchMatrix (int r, int c) {
        matrix.create (r, c, CV_8U);
        globalMaxVal = -1;
    }
};


class Uke {
private:
    /*top and bottom boundarymarkers*/
    CvRect BoundaryMarker_Max, BoundaryMarker_Min;
    
    IplImage **BoundaryMarkerImages;
    int numOfBoundaryMarkerImages;
    
    /*the match matrices*/
    MatchMatrix **matchMatrices;
    int numOfMatchMatrices;
    
public:
    
    
    /* Function: UpdateBoundaryMarkers 
     * -------------------------------
     * given the stats from FindBoundaryMarkers, this function will update the boundary markers appropriately
     */
    void UpdateBoundaryMarkers (Point Max, Point Min, int resolutionIndex) {
        BoundaryMarker_Max.x        = Max.x;
        BoundaryMarker_Max.y        = Max.y;
        BoundaryMarker_Max.width   = BoundaryMarkerImages [resolutionIndex]->width;
        BoundaryMarker_Max.height   = BoundaryMarkerImages [resolutionIndex]->height;
        
        BoundaryMarker_Min.x        = Min.x;
        BoundaryMarker_Min.y        = Min.y;
        BoundaryMarker_Min.width    = BoundaryMarkerImages [resolutionIndex]->width;
        BoundaryMarker_Min.height   = BoundaryMarkerImages [resolutionIndex]->height;
    }
    
    
    /* Function: FindBoundaryMarkers
     * -----------------------------
     * this function will initially locate and set the two boundary markers.
     */
    void FindBoundaryMarkers (IplImage *currentFrame) {
    
        
        CvPoint bestMatchMaxLoc, bestMatchMinLoc;
        int bestMatchMinVal = 10000, bestMatchMaxVal = -1;
        int bestMatchResolution = -1;
        
        
        for (int i=0;i<numOfBoundaryMarkerImages;i++) {
            cout << "# # # i = " << i << " # # # \n";
            double min = 100000, max = 0;
            cv::Point minLoc, maxLoc;
            GenerateMatchMatrix (currentFrame, BoundaryMarkerImages[i], matchMatrices[i]);
            minMaxLoc (matchMatrices[i]->matrix, &min, &max, &minLoc, &maxLoc, Mat());
            
            /*note: this isnt guaranteed to work... but in a constrained environment, we hope it will.*/
            if ((max > bestMatchMaxVal)) {
                bestMatchMinVal = min;
                bestMatchMaxVal = max;
                bestMatchMaxLoc.x = maxLoc.x;
                bestMatchMaxLoc.y = maxLoc.y;
                bestMatchMinLoc.x = minLoc.x;
                bestMatchMinLoc.y = minLoc.y;
                bestMatchResolution = i;
            }
        }
        
        UpdateBoundaryMarkers (bestMatchMaxLoc, bestMatchMinLoc, bestMatchResolution);
        
    }
    
    void DrawBoundaryMarkers (IplImage *testImage) {
        Mat testImageMat (testImage);

        cout << "DrawBoundaryMarkers " << endl;
        rectangle (   testImageMat,
                        Point (BoundaryMarker_Max.x, BoundaryMarker_Max.y),
                        Point (BoundaryMarker_Max.x + BoundaryMarker_Max.width, BoundaryMarker_Max.y + BoundaryMarker_Max.height),
                        Scalar (0, 255, 0, 0),
                        3,
                        8,
                        0 );
        
        rectangle (   testImageMat,
                   Point (BoundaryMarker_Min.x, BoundaryMarker_Min.y),
                   Point (BoundaryMarker_Min.x + BoundaryMarker_Min.width, BoundaryMarker_Min.y + BoundaryMarker_Min.height),
                   Scalar (0, 128, 0, 0),
                   3,
                   8,
                   0 );
        
        IplImage newImage = testImageMat;
        
        cvShowImage ("MAIN_DISPLAY", &newImage);
        int key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
    }
            
    
    /* Function: GenerateMatchMatrix
     * -----------------------------
     * given a region of an image to scan over (scanImage), and a template to scan with (templageImage), this function will
     * fill out the matrix 'resultMat' so that it reflects the summed-squared-difference.
     */
    void GenerateMatchMatrix (IplImage * scanImage, IplImage* templateImage, MatchMatrix *resultMat) {
        
        Mat scanImageMat (scanImage);
        Mat scanImageMatGray;
        cvtColor (scanImageMat, scanImageMatGray, CV_RGB2GRAY);
        Mat templateImageMat (templateImage);
        matchTemplate (scanImageMatGray, templateImageMat, resultMat->matrix, CV_TM_SQDIFF_NORMED);
        
    }
      
    
    /* Function: InitMatchMatrices
     * ---------------------------
     * this function will initialize the array 'matchMatrices' so that it contains 
     * the appropriate *number* and *resolutions* of match matrices; it will also set 
     * the 'numOfMatchMatrices
     */
    void InitMatchMatrices (IplImage *targetImage) {
        int width = targetImage->width;
        int height = targetImage->height;
        matchMatrices = new MatchMatrix* [numOfBoundaryMarkerImages];
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            int rows = (width - BoundaryMarkerImages[i]->width) + 1;
            int cols = (height - BoundaryMarkerImages[i]->height) + 1;
            matchMatrices[i] = new MatchMatrix (rows, cols);
        }
        numOfMatchMatrices = numOfBoundaryMarkerImages;
    }
    
    
    /* Function: InitBoundaryMarkerImages
     * ---------------------------
     * this function will initialize the array 'BoundaryMarkerImages' with
     * different resampled resultions; it will also set
     * the 'numOfBoundaryMarkerImages
     */
    void InitBoundaryMarkerImages (IplImage *fullSizeTemplate) {
        
        int largestResWidth = fullSizeTemplate->width;
        numOfBoundaryMarkerImages = (largestResWidth - SMALLEST_RES_WIDTH + 1) / RES_STEP;
        BoundaryMarkerImages = new IplImage *[numOfBoundaryMarkerImages];
        for (int i = largestResWidth, j = 0; i > SMALLEST_RES_WIDTH; i = i - RES_STEP, j++) {
            BoundaryMarkerImages[j] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate,  BoundaryMarkerImages[j], 1);
        }
     
    }
    
    
    /* Function: constructor
     * ---------------------
     * the constructor takes in an IplImage containing the full-sized template; it will 
     * then generate an array containing all of the different resolutions of this template 
     * and, in addition, initialize the array of 'matchmatrices'
     */
     
    Uke () {
    }
    Uke (IplImage *currentFrame, IplImage *fullSizeTemplate) {
        numOfBoundaryMarkerImages = 0;
        numOfMatchMatrices = 0;
        InitBoundaryMarkerImages(fullSizeTemplate);

        InitMatchMatrices (currentFrame);
        
        
        
    }
    
    ~Uke () {
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            delete BoundaryMarkerImages[i];
        }
        delete BoundaryMarkerImages;
        
        for (int i = 0; i < numOfMatchMatrices; i++) {
            delete matchMatrices [i];
        }
        delete matchMatrices;
    }
    
    
};



#endif
