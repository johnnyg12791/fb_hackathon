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
#define RES_STEP 6

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
    
    IplImage **BoundaryMarkerImages_White;
    IplImage **BoundaryMarkerImages_Black;
    int numOfBoundaryMarkerImages;
    
    /*the match matrices*/
    MatchMatrix **matchMatrices_White;
    MatchMatrix **matchMatrices_Black;
    int numOfMatchMatrices;
    
public:
    
    
    /* Function: UpdateBoundaryMarkers 
     * -------------------------------
     * given the stats from FindBoundaryMarkers, this function will update the boundary markers appropriately
     */
    void UpdateBoundaryMarkers (Point Max, Point Min, int resolutionIndexMax, int resolutionIndexMin) {
        BoundaryMarker_Max.x        = Max.x;
        BoundaryMarker_Max.y        = Max.y;
        BoundaryMarker_Max.width   = BoundaryMarkerImages_White [resolutionIndexMax]->width;
        BoundaryMarker_Max.height   = BoundaryMarkerImages_White [resolutionIndexMax]->height;
        
        BoundaryMarker_Min.x        = Min.x;
        BoundaryMarker_Min.y        = Min.y;
        BoundaryMarker_Min.width    = BoundaryMarkerImages_Black [resolutionIndexMin]->width;
        BoundaryMarker_Min.height   = BoundaryMarkerImages_Black [resolutionIndexMin]->height;
    }
    
    
    /* Function: FindBoundaryMarkers
     * -----------------------------
     * this function will initially locate and set the two boundary markers.
     */
    void FindBoundaryMarkers (IplImage *currentFrame) {
    
        
        CvPoint bestMatch_white, bestMatch_black;
        double bestMatchVal_white = 1e+10, bestMatchVal_black = 1e+10;
        int bestMatchResolution_white = -1;
        int bestMatchResolution_black = -1;
        
        
        for (int i=0;i<numOfBoundaryMarkerImages;i++) {
            cout << "# # # i = " << i << " # # # \n";
            double min_white = 1e+10, max_white = 0;
            double min_black = 1e+10, max_black = 0;
            cv::Point minLoc_white, maxLoc_white;
            cv::Point minLoc_black, maxLoc_black;
            
            GenerateMatchMatrix (currentFrame, BoundaryMarkerImages_White[i], matchMatrices_White[i]);
            GenerateMatchMatrix (currentFrame, BoundaryMarkerImages_Black[i], matchMatrices_Black[i]);
            
            minMaxLoc (matchMatrices_White[i]->matrix, &min_white, &max_white, &minLoc_white, &maxLoc_white, Mat());
            minMaxLoc (matchMatrices_Black[i]->matrix, &min_black, &max_black, &minLoc_black, &maxLoc_black, Mat());
            
            /*note: this isnt guaranteed to work... but in a constrained environment, we hope it will.*/

            if (min_white < bestMatchVal_white) {
                bestMatchVal_white = min_white;
                bestMatch_white.x = minLoc_white.x;
                bestMatch_white.y = minLoc_white.y;
                bestMatchResolution_white = i;
                cout << "Did some update, new best resolution " << i << endl;
                cout << "new white min: " << min_white << endl;
            }
            
            if (min_black < bestMatchVal_black) {
                bestMatchVal_black = min_black;
                bestMatch_black.x = minLoc_black.x;
                bestMatch_black.y = minLoc_black.y;
                bestMatchResolution_black = i;
                cout << "new black min: " << min_black << endl;
            }
            
            UpdateBoundaryMarkers (bestMatch_white, bestMatch_black, bestMatchResolution_white, bestMatchResolution_black);

        }
        cout << "finished for loop: " << bestMatchResolution_white << " "  << bestMatchResolution_black << endl;

        UpdateBoundaryMarkers (bestMatch_white, bestMatch_black, bestMatchResolution_white, bestMatchResolution_black);
        DrawBoundaryMarkers (currentFrame);
        cvShowImage ("MAIN_DISPLAY", currentFrame);
        int key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
        
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
        matchMatrices_White = new MatchMatrix* [numOfBoundaryMarkerImages];
        matchMatrices_Black = new MatchMatrix* [numOfBoundaryMarkerImages];
        
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            int rows = (width - BoundaryMarkerImages_White[i]->width) + 1;
            int cols = (height - BoundaryMarkerImages_White[i]->height) + 1;
            matchMatrices_White[i] = new MatchMatrix (rows, cols);
            matchMatrices_Black[i] = new MatchMatrix (rows, cols);
        }
        numOfMatchMatrices = numOfBoundaryMarkerImages;
    }
    
    
    /* Function: InitBoundaryMarkerImages
     * ---------------------------
     * this function will initialize the array 'BoundaryMarkerImages' with
     * different resampled resultions; it will also set
     * the 'numOfBoundaryMarkerImages
     */
    void InitBoundaryMarkerImages (IplImage *fullSizeTemplate_white, IplImage *fullSizeTemplate_black) {
        
        int largestResWidth = fullSizeTemplate_white->width;
        numOfBoundaryMarkerImages = (largestResWidth - SMALLEST_RES_WIDTH + 1) / RES_STEP;
        BoundaryMarkerImages_White = new IplImage *[numOfBoundaryMarkerImages];
        BoundaryMarkerImages_Black = new IplImage *[numOfBoundaryMarkerImages];
        for (int i = largestResWidth, j = 0; i > SMALLEST_RES_WIDTH; i = i - RES_STEP, j++) {
            BoundaryMarkerImages_White[j] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate_white,  BoundaryMarkerImages_White[j], 1);
            
            BoundaryMarkerImages_Black[j] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate_black,  BoundaryMarkerImages_Black[j], 1);
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
    Uke (IplImage *currentFrame, IplImage *fullSizeTemplate_white, IplImage *fullSizeTemplate_black) {
        cout << "BEGIN Uke Init" << endl;
        
        numOfBoundaryMarkerImages = 0;
        numOfMatchMatrices = 0;
        InitBoundaryMarkerImages(fullSizeTemplate_white, fullSizeTemplate_black);
        cout << "After InitBoundaryMarkerImages " << endl;


        InitMatchMatrices (currentFrame);
        cout << "After InitMatchMatrices " << endl;

        
        
    }
    
    ~Uke () {
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            delete BoundaryMarkerImages_White[i];
            delete BoundaryMarkerImages_Black[i];

        }
        delete BoundaryMarkerImages_White;
        delete BoundaryMarkerImages_Black;
        
        for (int i = 0; i < numOfMatchMatrices; i++) {
            delete matchMatrices_White [i];
            delete matchMatrices_Black [i];

        }
        delete matchMatrices_White;
        delete matchMatrices_Black;

    }
    
    
};



#endif
