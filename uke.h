#ifndef _UKE
#define _UKE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>



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
        
        for (int i=0;i<currentTemplate->rows;i++) {
            for (int j=0;j<currentTemplate->cols;j++) {
                
                
                totalsum += 
                
            }
        }
   
            
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
        
        for (int i=0;i<currentMatchMatrix->rows;i++) {
            for (int j=0;j<currentMatchMatrix->cols;j++) {
             
                currentMatchMatrix[i,j] = GetSummedSquaredDifference (currentFrame, currentTemplate, int i, int j);
                
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
    
    
    /* Function: constructor
     * ---------------------
     * the constructor takes in an IplImage containing the full-sized template; it will 
     * then generate an array containing all of the different resolutions of this template 
     * and, in addition, initialize the array of 'matchmatrices'
     */
     
     
    Uke (IplImage *fullSizeTemplate) {
        BoundaryMarkerImage = cvCopyImage (bm);
        
    }
    
    
}



#endif