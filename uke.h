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
#define SMALLEST_RES_WIDTH 54
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
            GenerateMatchMatrix (currentFrame, i)
        }
        
        
    }
    
    /* Function: GenerateMatchMatrix 
     * -----------------------------
     * given the current frame and the 'resolution index' of the match template to 
     * use, this function will scan that match template over the entire image and 
     * fill the appropriate member of 'matchMatrices'
     */
    void GenerateMatchMatrix () {
        
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