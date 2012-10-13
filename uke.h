#ifndef _UKE
#define _UKE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SMALLEST_RES_WIDTH 53
#define RES_STEP 3

using namespace std;


class MatchMatrix {
private:
    int ** matrix;
    int rows;
    int cols;
    CvPoint GlobalMax;
    int GlobalMaxValue;
    
public:
    
    int GetRows () {
        return rows;
    }
    int GetCols () {
        return cols;
    }
    
    void SetEntry (int i, int j, int val) {
        assert ( ((i >= 0) && (i < rows)) && ((j >= 0) && (j < cols)) );
        matrix [i][j] = val;
        if (val > GlobalMaxValue) {
            GlobalMaxValue = val;
            GlobalMax.x = i;
            GlobalMax.y = j;
        }
            
    }
    
    CvPoint GetGlobalMax () {
        return GlobalMax;
    }
    
    MatchMatrix () {
    }
    MatchMatrix (int r, int c) {
        rows = r;
        cols = c;
        matrix = new int* [rows];
        for (int i=0;i<rows;i++) {
            matrix [i] = new int [cols];
        }
    }
    
    ~MatchMatrix () {
        for (int i=0;i<rows;i++) {
            delete matrix [i];
        }
        delete matrix;
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
    
      //        for (int i=0;i<NUM_OF_TEMPLATES;i++) {
      //    GenerateMatchMatrix (currentFrame, i);
      //}
        
        
    }
    
    
    IplImage * scan_test (IplImage *testImage) {
        GenerateMatchMatrix (testImage, 0);
        IplImage *resultImage = cvCloneImage (testImage);
        
        CvRect result = cvRect (matchMatrices[0]->GetGlobalMax().x, matchMatrices[0]->GetGlobalMax().y, BoundaryMarkerImages[0]->width, BoundaryMarkerImages[0]->height);
        cvRectangle (resultImage, cvPoint(result.x, result.y), cvPoint(result.x + result.width, result.y + result.height), cvScalar( 0, 0, 0), 3, 8);
        
        cvShowImage( "MAIN_DISPLAY", resultImage);
        int key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
        return resultImage;
        
    }
            
                    
                    
    /* Function: GetSummedSquaredDifference
     * ------------------------------------
     * given the current frame and the index of the template that we are using, as well
     * as the coordinates within the image at which we are calculating, this function will return
     * the summed-squared-difference between the template and that image region.
     */

    int GetSummedSquaredDifference (IplImage *currentFrame, IplImage *currentTemplate, int x, int y) {
        cout << "Started GetSummedSquaredDifference" << endl;
        int totalSum = 0;
        
        cvShowImage( "MAIN_DISPLAY", currentFrame);
        int key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
        
        cvShowImage( "MAIN_DISPLAY", currentTemplate);
        
        key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
        
        char *imagePosition = currentFrame->imageData + x *currentFrame->widthStep + y;
        char *templatePosition = currentTemplate->imageData;
        cout << "CurrentTemplate->width: " << currentTemplate->width << ", height: " << "CurrentTemplate->height: " << currentTemplate->height << endl;
        for (int i=0;i<currentTemplate->width;i++) {
            for (int j=0;j<currentTemplate->height;j++) {
                
                totalSum += pow( (float) (*(imagePosition + j) - *(templatePosition + j)) , 2);
                cout << "totalSum = " << totalSum << endl;
            }
            imagePosition += currentFrame->widthStep;
            templatePosition += currentTemplate->widthStep;
        }
        cout << "Finished GetSummedSquaredDifference" << endl;
        return totalSum;
    }
    
    /* Function: GenerateMatchMatrix 
     * -----------------------------
     * given the current frame and the 'resolution index' of the match template to 
     * use, this function will scan that match template over the entire image and 
     * fill the appropriate member of 'matchMatrices'
     */
    void GenerateMatchMatrix (IplImage * currentFrame, int index) {
        cout << "Started generating MatchMatrix" << endl;

        

        
        MatchMatrix *currentMatchMatrix = matchMatrices [index];
        IplImage    *currentTemplate = BoundaryMarkerImages [index];
        
        cvShowImage( "MAIN_DISPLAY", currentTemplate);
        int key = 0;
        while (key != 'q') {
            key = cvWaitKey (30);
        }
        
        cout << "worked" << endl;


        for (int i=0;i<currentMatchMatrix->GetRows();i++) {
            for (int j=0;j<currentMatchMatrix->GetCols();j++) {
                cout << "i: " << i << ", j: " << j << endl;

                currentMatchMatrix->SetEntry(i, j, GetSummedSquaredDifference (currentFrame, currentTemplate, i, j));
                
            }
        }
        cout << "Finished generating MatchMatrix" << endl;

        
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
        int numOfBoundaryMarkerImages = (largestResWidth - SMALLEST_RES_WIDTH + 1) / 3;
        BoundaryMarkerImages = new IplImage *[numOfBoundaryMarkerImages];
        for (int i = largestResWidth; i > SMALLEST_RES_WIDTH; i = i - RES_STEP) {
            cout << "i: " << i << endl;
            BoundaryMarkerImages[i] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate,  BoundaryMarkerImages[i], 1);
            cvShowImage( "MAIN_DISPLAY", BoundaryMarkerImages[i]);
            int key = 0;
            while (key != 'q') {
                key = cvWaitKey (30);
            }

            cout << i << "th boundary image: width, height = " << BoundaryMarkerImages[i]->width << ", " << BoundaryMarkerImages[i]->height << "\n";
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
