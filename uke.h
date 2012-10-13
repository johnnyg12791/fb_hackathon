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
    
    IplImage *BoundaryMarkerImage;
    
    
public:
    
    /* Function: FindBoundaryMarkers
     * -----------------------------
     * this function will initially locate and set the two boundary markers.
     */
    void FindBoundaryMarkers (IplImage *currentFrame) {
    
        for (int i=0;i<NUM_OF_TEMPLATES;i++) {
            GenerateTemplateMatrix (currentFrame,)
        }
        
        
    }
    
    
    Uke (IplImage *bm) {
        BoundaryMarkerImage = cvCopyImage (bm);
    }
    
    
}



#endif