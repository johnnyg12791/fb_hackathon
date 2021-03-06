#ifndef _UKE
#define _UKE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "boundaryMarker.h"
#include "key.h"
#include "body.h"

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>

#define SMALLEST_RES_WIDTH 92
#define RES_STEP 6
#define HEIGHT_OF_LINE 4 //This is actually the center - height/HEIGHT_OF_LINE, so 4 means its 3/4 up the screen, -4 means 1/4 up the screen

using namespace std;
using namespace cv;


#define F_CHORD 1 //3 //2,3 //1,2,3
#define C_CHORD 2 //0 //1,2 //0,1,2
#define G_CHORD 3 //1 //1,3 //0,1,3
#define A_MINOR 4 //2 //0,2 //0,2,3
#define E_MINOR 5 //0,1 //0,3 //0,1,2,3

#define C_CHORD_FILEPATH "C.wav"
#define G_CHORD_FILEPATH "G.wav"
#define F_CHORD_FILEPATH "F.wav"
#define A_MINOR_FILEPATH "A.wav"
#define E_MINOR_FILEPATH "E.wav"


class Uke {
private:
    /*top and bottom boundarymarkers*/
    BoundaryMarker boundaryMarker_white, boundaryMarker_black;
    
    IplImage **BoundaryMarkerImages_White;
    IplImage **BoundaryMarkerImages_Black;
    int numOfBoundaryMarkerImages;
    
    /*the match matrices*/
    Mat **matchMatrices_White;
    Mat **matchMatrices_Black;
    int numOfMatchMatrices;
    
    /*the keys*/
    Key *keys [NUM_OF_KEYS];
    Body body;
    
    /*map from fingers to chords*/
    map<int*, int> keysToChords;
    
    
public:
    
    
    bool isCalibrated;
    
    /* Function: UpdateBoundaryMarkers 
     * -------------------------------
     * given the stats from FindBoundaryMarkers, this function will set the initial parameters in the boundarymarker objects.
     * this function should be used during DETECTION, not during tracking.
     */
    void SetBoundaryMarkers (    IplImage *whiteTemplate, Point whiteCorner,
                                 IplImage *blackTemplate, Point blackCorner) {
        boundaryMarker_white.Set (whiteTemplate, whiteCorner);
        boundaryMarker_black.Set (blackTemplate, blackCorner);
    }
    
    //sets up the map from keys to chords
    void InitKeyChordMap(){
        int arr0 [4] = {0,0,0,0};
        keysToChords[arr0] = C_CHORD;

        int arr [4] = {0,0,0,1};
        keysToChords[arr] = F_CHORD;
        int arr2 [4] = {0,0,1,1};
        keysToChords[arr2] = F_CHORD;
        int arr3 [4] = {0,1,1,1};
        keysToChords[arr3] = F_CHORD;
        int arr4 [4] = {1,0,0,0};
        keysToChords[arr4] = C_CHORD;
        int arr5 [4] = {0,1,1,0};
        keysToChords[arr5] = C_CHORD;
        int arr6 [4] = {1,1,1,0};
        keysToChords[arr6] = C_CHORD;
        int arr7 [4] = {0,1,0,0};
        keysToChords[arr7] = G_CHORD;
        int arr8 [4] = {0,1,0,1};
        keysToChords[arr8] = G_CHORD;
        int arr9 [4] = {1,1,0,1};
        keysToChords[arr9] = G_CHORD;
        int arr10 [4] = {0,0,1,0};
        keysToChords[arr10] = A_MINOR;
        int arr11 [4] = {1,0,1,0};
        keysToChords[arr11] = A_MINOR;
        int arr12 [4] = {1,0,1,1};
        keysToChords[arr12] = A_MINOR;
        int arr13 [4] = {0,0,1,0};
        keysToChords[arr13] = E_MINOR;
        int arr14 [4] = {1,0,1,0};
        keysToChords[arr14] = E_MINOR;
        int arr15 [4] = {1,0,1,1};
        keysToChords[arr15] = E_MINOR;
    }
 
    /*
     * returns -1 if there is no chord (play no sound sample), returns the constant mapping to the chord otherwise.
     */
    int GetCurrentChord (IplImage *currentFrame) {
        if(body.isActive()){
            cout << "- - - STRUM OCCURRED - - - ";
            int arr [4];
            for (int i=0;i<4;i++) {
                keys[i]->DetermineActivity (currentFrame);
                arr[i] = keys[i]->isActive ();
                cout << "arr[" << i << "] = " << arr[i] << ", ";
            }
            
            int number = 1*arr[0] + 2*arr[1] + 4*arr[2] + 8*arr[3];
            
            switch (number) {
            case 0:
            case 5:
            case 9:
                return C_CHORD;
            case 1:
            case 4:
            case 8:
                return G_CHORD;
            case 2:
            case 6:
            case 10:
                return F_CHORD;
            case 3:
            case 7:
            case 11:
                return E_MINOR;
            default:
                return E_MINOR;
            }
            
            
        }
        else {
            cout << "body is not active...\n";
            return -1;
        }
        return 0;
    }
    
    /* Function: UpdateBoundaryMarkers
     * -------------------------------
     * given the stats from FindBoundaryMarkers, this function will update the boundary markers appropriately.
     * this function should be used during TRACKING, not during detection.
     */
    void UpdateBoundaryMarkers (IplImage * currentFrame, Point white_tl, Point black_tl) {
        boundaryMarker_white.Update (white_tl);
        boundaryMarker_black.Update (black_tl);
        for (int i=0;i<NUM_OF_KEYS;i++) {
            keys[i]->Update (boundaryMarker_white.center, boundaryMarker_black.center);
        }
        body.UpdateLocation (currentFrame, boundaryMarker_white.center, boundaryMarker_black.center);
        
    }
    
    /* Function: DrawUkeFrame
     * ----------------------
     * Draws the 2 rectangles around our targets and also puts a line in-between them
     * Also tries to put the circles 
     */
    
    void DrawUkeFrame (IplImage *testImage) {
        Mat testImageMat (testImage);
        rectangle (     testImageMat,
                        boundaryMarker_white.topLeft,
                        Point (boundaryMarker_white.topLeft.x + boundaryMarker_white.width, boundaryMarker_white.topLeft.y + boundaryMarker_white.height),
                        Scalar (0, 255, 0, 0),
                        3,
                        8,
                        0 );
        
        rectangle (     testImageMat,
                        boundaryMarker_black.topLeft,
                        Point (boundaryMarker_black.topLeft.x + boundaryMarker_black.width, boundaryMarker_black.topLeft.y + boundaryMarker_black.height),
                        Scalar (0, 0, 255, 0),
                        3,
                        8,
                        0 );
        
        
        boundaryMarker_black.center.y -= boundaryMarker_black.height/HEIGHT_OF_LINE;
        boundaryMarker_white.center.y -= boundaryMarker_white.height/HEIGHT_OF_LINE;

        
        line (          testImageMat,
                        boundaryMarker_black.center,
                        boundaryMarker_white.center,
                        Scalar (255, 0, 0, 0),
                        3, 8, 0);
        
        for (int i=0;i<NUM_OF_KEYS;i++) {
            keys[i]->location.y -= boundaryMarker_black.height/HEIGHT_OF_LINE;
            circle (    testImageMat,
                        keys[i]->location,
                        keys[i]->radius,
                        Scalar (0, 0, 128, 0),
                        2, 8, 0);
            
        }
        
        rectangle (     testImageMat,
                        Point (body.boundingBox.x, body.boundingBox.y),
                        Point (body.boundingBox.x + body.boundingBox.width, body.boundingBox.y + body.boundingBox.height),
                        Scalar (0, 255, 0, 0),
                   3, 8, 0);
        
        IplImage newImage = testImageMat;
        cvShowImage("MAIN_DISPLAY", &newImage);
        
    }
    
    
    /* Function: FindBoundaryMarkers
     * -----------------------------
     * this function will initially locate and set the two boundary markers.
     */
    void DetectBoundaryMarkers (IplImage *currentFrame) {
    
        CvPoint bestMatch_white, bestMatch_black;
        double bestMatchVal_white = 1e+10, bestMatchVal_black = 1e+10;
        int bestMatchResolution_white = -1;
        int bestMatchResolution_black = -1;
        
        
        for (int i=0;i<numOfBoundaryMarkerImages;i++) {
            double min_white = 1e+10, max_white = 0;
            double min_black = 1e+10, max_black = 0;
            cv::Point minLoc_white, maxLoc_white;
            cv::Point minLoc_black, maxLoc_black;
            
            GenerateMatchMatrix (currentFrame, BoundaryMarkerImages_White[i], matchMatrices_White[i]);
            GenerateMatchMatrix (currentFrame, BoundaryMarkerImages_Black[i], matchMatrices_Black[i]);
            
            minMaxLoc (*matchMatrices_White[i], &min_white, &max_white, &minLoc_white, &maxLoc_white, Mat());
            minMaxLoc (*matchMatrices_Black[i], &min_black, &max_black, &minLoc_black, &maxLoc_black, Mat());

            if (min_white < bestMatchVal_white) {
                bestMatchVal_white = min_white;
                bestMatch_white.x = minLoc_white.x;
                bestMatch_white.y = minLoc_white.y;
                bestMatchResolution_white = i;
            }
            
            if (min_black < bestMatchVal_black) {
                bestMatchVal_black = min_black;
                bestMatch_black.x = minLoc_black.x;
                bestMatch_black.y = minLoc_black.y;
                bestMatchResolution_black = i;
            }

        }
        
        
        SetBoundaryMarkers (    BoundaryMarkerImages_White[bestMatchResolution_white], bestMatch_white, 
                                BoundaryMarkerImages_Black[bestMatchResolution_black], bestMatch_black);
        
        body.init (currentFrame, boundaryMarker_white.center, boundaryMarker_black.center, boundaryMarker_white.width);
    }
    
    
    /* Function: TrackBoundaryMarkers
     * ------------------------------
     * given a newly captured frame, this function will determine the most likely position for the boundary markers
     * given their position in the previous frame. this is the primary function for TRACKING.
     */
    void TrackBoundaryMarkers (IplImage *currentFrame) {

        double min_white = 1e+10, max_white = 0;
        double min_black = 1e+10, max_black = 0;
        cv::Point minLoc_white, maxLoc_white;
        cv::Point minLoc_black, maxLoc_black;
        
        cvSetImageROI (currentFrame, boundaryMarker_white.scanRegion);
        GenerateMatchMatrix (currentFrame, boundaryMarker_white.bmTemplate, boundaryMarker_white.matchMatrix);
        minMaxLoc(*boundaryMarker_white.matchMatrix, &min_white, &max_white, &minLoc_white, &maxLoc_white, Mat());
        Point newLocation_white;
        newLocation_white.x = minLoc_white.x + boundaryMarker_white.scanRegion.x;
        newLocation_white.y = minLoc_white.y + boundaryMarker_white.scanRegion.y;
        
        cvSetImageROI (currentFrame, boundaryMarker_black.scanRegion);
        GenerateMatchMatrix (currentFrame, boundaryMarker_black.bmTemplate, boundaryMarker_black.matchMatrix);
        minMaxLoc(*boundaryMarker_black.matchMatrix, &min_black, &max_black, &minLoc_black, &maxLoc_black, Mat());
        Point newLocation_black;
        newLocation_black.x = minLoc_black.x + boundaryMarker_black.scanRegion.x;
        newLocation_black.y = minLoc_black.y + boundaryMarker_black.scanRegion.y;
        
        UpdateBoundaryMarkers (currentFrame, newLocation_white, newLocation_black);
        
        cvResetImageROI (currentFrame);
    }
            
    
    /* Function: GenerateMatchMatrix
     * -----------------------------
     * given a region of an image to scan over (scanImage), and a template to scan with (templageImage), this function will
     * fill out the matrix 'resultMat' so that it reflects the summed-squared-difference.
     */
    void GenerateMatchMatrix (IplImage * scanImage, IplImage* templateImage, Mat *resultMat) {
        
        
        /*speed it up - take care of the template initially*/
        Mat scanImageMat (scanImage);
        Mat scanImageMatGray;
        Mat templateImageMat (templateImage);
        
        cvtColor (scanImageMat, scanImageMatGray, CV_RGB2GRAY);
        
        if (isCalibrated) {
            Mat templateImageMatGray;
            cvtColor (templateImageMat, templateImageMatGray, CV_RGB2GRAY);
            matchTemplate (scanImageMatGray, templateImageMatGray, *resultMat, CV_TM_SQDIFF_NORMED);
        }
        else {
            matchTemplate (scanImageMatGray, templateImageMat, *resultMat, CV_TM_SQDIFF_NORMED);
        }
        
    }
    
    
    
    
    /* Function: GrabBoundaryMarkerTemplates 
     * -------------------------------------
     */
    void GrabBoundaryMarkerTemplates (IplImage * currentFrame) {
        boundaryMarker_white.SetBmTemplate (currentFrame);
        boundaryMarker_black.SetBmTemplate (currentFrame);
    }
    
    /* Function: GrabInactiveKeyTemplates
     * ----------------------------------
     */
    void GrabInactiveKeyTemplates (IplImage * currentFrame) {
        for (int i=0;i<NUM_OF_KEYS;i++) {
            keys[i]->GrabInactiveKeyTemplate (currentFrame);
        }
    }

    /* Function: GrabActiveKeyTemplates
     * ----------------------------------
     */
    void GrabActiveKeyTemplates (IplImage * currentFrame) {
        for (int i=0;i<NUM_OF_KEYS;i++) {
            keys[i]->GrabActiveKeyTemplate (currentFrame);
        }
    }
    
    
    
    /* Function: InitMatchMatrices
     * ---------------------------
     * this function will initialize the array 'matchMatrices' so that it contains 
     * the appropriate *number* and *resolutions* of match matrices; it will also set 
     * the 'numOfMatchMatrices
     */
    void InitMatchMatrices (IplImage *targetImage) {
        int imageWidth = targetImage->width;
        int imageHeight = targetImage->height;
        matchMatrices_White = new Mat* [numOfBoundaryMarkerImages];
        matchMatrices_Black = new Mat* [numOfBoundaryMarkerImages];
        
        for (int i = 0; i < numOfBoundaryMarkerImages; i++) {
            
            int rows = imageWidth - BoundaryMarkerImages_White[i]->width + 1;
            int cols = imageHeight - BoundaryMarkerImages_White[i]->height + 1;
            matchMatrices_White[i] = new Mat (rows, cols, CV_8UC1);
            matchMatrices_Black[i] = new Mat (rows, cols, CV_8UC1);
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
    
    
    /* DETERMINE FINGERING */
    void DetermineFingering (IplImage *currentFrame) {
        if (isCalibrated) {
            cout << "current fingering: ";
            for (int i=0;i<NUM_OF_KEYS;i++) {
                keys[i]->DetermineActivity (currentFrame);
                cout << keys[i]->isActive () << ", ";
            }
            cout << "\n";
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

        boundaryMarker_white.init (currentFrame->height, currentFrame->width);
        boundaryMarker_black.init (currentFrame->height, currentFrame->width);
        
        numOfBoundaryMarkerImages = 0;
        numOfMatchMatrices = 0;
        InitBoundaryMarkerImages(fullSizeTemplate_white, fullSizeTemplate_black);
        InitMatchMatrices (currentFrame);
        InitKeyChordMap();
        
        keys[0] = new Key (KEY1_RATIO, KEY_RADIUS);
        keys[1] = new Key (KEY2_RATIO, KEY_RADIUS);
        keys[2] = new Key (KEY3_RATIO, KEY_RADIUS);
        keys[3] = new Key (KEY4_RATIO, KEY_RADIUS);
        
        
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
