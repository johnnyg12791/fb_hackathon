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
    
    
    /* Function: GetSummedSquaredDifference
     * ------------------------------------
     * given the current frame and the index of the template that we are using, as well
     * as the coordinates within the image at which we are calculating, this function will return
     * the summed-squared-difference between the template and that image region.
     */

    int GetSummedSquaredDifference (IplImage *currentFrame, IplImage *currentTemplate, int x, int y) {
        
        int totalSum = 0;
        
        char *imagePosition = currentFrame->imageData + x*currentFrame->widthStep + y;
        char *templatePosition = currentTemplate->imageData;
        
        for (int i=0;i<currentTemplate->width;i++) {
            for (int j=0;j<currentTemplate->height;j++) {
                
                totalSum += pow( (float) (*(imagePosition + j) - *(templatePosition + j)) , 2);
            }
            imagePosition += currentFrame->widthStep;
            templatePosition += currentTemplate->widthStep;
        }
   
        return totalSum;
    }
    
    /* Function: GenerateMatchMatrix 
     * -----------------------------
     * given the current frame and the 'resolution index' of the match template to 
     * use, this function will scan that match template over the entire image and 
     * fill the appropriate member of 'matchMatrices'
     */
    void GenerateMatchMatrix (IplImage * currentFrame, int index) {
        MatchMatrix *currentMatchMatrix = matchMatrices [index];
        IplImage    *currentTemplate = BoundaryMarkerImages [index];
        
        for (int i=0;i<currentMatchMatrix->GetRows();i++) {
            for (int j=0;j<currentMatchMatrix->GetCols();j++) {
                currentMatchMatrix->SetEntry(i, j, GetSummedSquaredDifference (currentFrame, currentTemplate, i, j));
                
            }
        }
        
        
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
        int numOfBoundaryMarkerImages = (largestResWidth - SMALLEST_RES_WIDTH) / 3;
        BoundaryMarkerImages = new IplImage *[numOfBoundaryMarkerImages];
        for (int i = largestResWidth; i > SMALLEST_RES_WIDTH; i = i - RES_STEP) {
            BoundaryMarkerImages[i] = cvCreateImage(cvSize(i, i), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate,  BoundaryMarkerImages[i], 1);
            
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
