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
    CvRect BoundaryMarker_T, BoundaryMarker_B;
    
    IplImage **BoundaryMarkerImages;
    
    int numOfBoundaryMarkerImages;
    
    
    /*the match matrices*/
    MatchMatrix **matchMatrices;
    int numOfMatchMatrices;
    
public:
    
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
            if ((min < bestMatchMinVal) && (max > bestMatchMaxVal)) {
                bestMatchMinVal = min;
                bestMatchMaxVal = max;
                bestMatchMaxLoc.x = maxLoc.x;
                bestMatchMaxLoc.y = maxLoc.y;
                bestMatchMinLoc.x = minLoc.x;
                bestMatchMinLoc.y = minLoc.y;
                cout << "best match updated!\n";
            }
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
    
    
    IplImage * scan_test (IplImage *testImage, IplImage * templateImage) {
        MatchMatrix* resultMat = new MatchMatrix (10, 10);
        GenerateMatchMatrix (testImage, templateImage, resultMat);
        double min = 100000, max = 0;
        cv::Point minLoc, maxLoc;
        Mat testMat ();
        cout << "HERE\n";
        minMaxLoc (resultMat->matrix, &min, &max, &minLoc, &maxLoc, Mat());
        
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
        cout << "largestResWidth: " << largestResWidth << endl;
        numOfBoundaryMarkerImages = (largestResWidth - SMALLEST_RES_WIDTH + 1) / 3;
        BoundaryMarkerImages = new IplImage *[numOfBoundaryMarkerImages];
        for (int i = largestResWidth, j = 0; i > SMALLEST_RES_WIDTH; i = i - RES_STEP, j++) {
            cout << "i: " << i << endl;
            BoundaryMarkerImages[j] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate,  BoundaryMarkerImages[j], 1);
            cout << i << "th boundary image: width, height = " << BoundaryMarkerImages[j]->width << ", " << BoundaryMarkerImages[j]->height << "\n";
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
        cout << "BEFORE " << endl;
        InitBoundaryMarkerImages(fullSizeTemplate);
        cout << "AFTER " << endl;

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
