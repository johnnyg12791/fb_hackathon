#ifndef _UKE
#define _UKE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define LARGEST_RES_WIDTH 176
#define LARGEST_RES_HEIGHT 164
#define SMALLEST_RES_WIDTH 53
#define RES_STEP 3


class Uke {
private:
    /*top and bottom boundarymarkers*/
    CvRect BoundaryMarker_T, BoundaryMarker_B;
    
    IplImage **BoundaryMarkerImages;
    
    int numOfBoundaryMarkerImages;
    
    
    /*the match matrices*/
    CvMat **matchMatrices;
    int numbOfMatchMatrices;
    
public:
    
    /* Function: FindBoundaryMarkers
     * -----------------------------
     * this function will initially locate and set the two boundary markers.
     */
    void FindBoundaryMarkers (IplImage *currentFrame) {
    
        for (int i=0;i<NUM_OF_TEMPLATES;i++) {
            GenerateMatchMatrix (currentFrame, i);
        }
        
        
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
        
        for (int i=0;i<currentTemplate->rows;i++) {
            for (int j=0;j<currentTemplate->cols;j++) {
                
                totalSum += (int) pow( (*(imagePosition + j) - *(templatePosition + j), 2);
            }
            imagePosition += currentFrame->widthStep;
            templatePosition += templateFrame->widthSTep;
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
        CVMat       *currentMatchMatrix = matchMatrices [i];
        IplImage    *currentTemplate = BoundaryMarkerImages [i];
        char        * writeLocation = (char*) matchMatrices[i]->data;
        
        for (int i=0;i<currentMatchMatrix->rows;i++) {
            for (int j=0;j<currentMatchMatrix->cols;j++) {
                
                *writeLocation = GetSummedSquaredDifference (currentFrame, currentTemplate, int i, int j);
                writeLocation += sizeof(char);
                
            }
        }
        
        
    }
    
    
    /* Function: InitMatchMatrices
     * ---------------------------
     * this function will initialize the array 'matchMatrices' so that it contains 
     * the appropriate *number* and *resolutions* of match matrices; it will also set 
     * the 'numOfMatchMatrices
     */
    void InitMatchMatrices () {
        
    }
    
    
    /* Function: InitBoundaryMarkerImages
     * ---------------------------
     * this function will initialize the array 'BoundaryMarkerImages' with
     * different resampled resultions; it will also set
     * the 'numOfBoundaryMarkerImages
     */
    void InitBoundaryMarkerImages (IplImage *fullSizeTemplate) {
        int numOfBoundaryMarkerImages = (LARGEST_RES_WIDTH - SMALLEST_RES_WIDTH) / 3;
        BoundaryMarkerImages = new IplImage[numOfBoundaryMarkerImages];
        for (int i = LARGEST_RES_WIDTH; i > SMALLEST_RES_WIDTH; i = i - 3) {
            int height = i * LARGEST_RES_HEIGHT / LARGEST_RES_WIDTH;
            BoundaryMarkerImages[i] = cvCreateImage(CvSize(i, height), IPL_DEPTH_8U, 1);
            cvResize(fullSizeTemplate,  BoundaryMarkerImages[i], 1);
        }
        
    }
    
    
    /* Function: constructor
     * ---------------------
     * the constructor takes in an IplImage containing the full-sized template; it will 
     * then generate an array containing all of the different resolutions of this template 
     * and, in addition, initialize the array of 'matchmatrices'
     */
     
     
    Uke (IplImage *fullSizeTemplate) {
        InitBoundaryMarkerImages(fullSizeTemplate);
        
    }
    
    ~Uke () {
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            delete BoundaryMarkerImages[i];
        }
        delete BoundaryMarkerImages;
    }
    
    
}



#endif