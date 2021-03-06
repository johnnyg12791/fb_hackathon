#ifndef _VIRTUALELE
#define _VIRTUALELE

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "uke.h"
#include "musicPlayer.h"

/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>


class Virtualele {
private:
    CvCapture * capture;
    IplImage * currentFrame;
    char * mainDisplayName;
    
    IplImage *fullResTemplate;
    
    IplImage *displayFrame;
    
    Uke *uke;
    
public:

    
    
    
    /* Function: DetectUke
     * -------------------
     * this function will have the Uke object locate the Uke. returns 1 when it works, 0 otherwise.
     */
    void DetectUke () {
        uke->isCalibrated = false;
        uke->DetectBoundaryMarkers (currentFrame);
    }
    
    void DetectUke (IplImage *testImage) {
        uke->DetectBoundaryMarkers (testImage);
    }
    
    void TrackUke () {
        uke->TrackBoundaryMarkers (currentFrame);
    }
    
    void CalibrateUke () {
        uke->isCalibrated = true;
        uke->GrabBoundaryMarkerTemplates (currentFrame);
        uke->GrabInactiveKeyTemplates (currentFrame);
        int key = 0;
        while (key != 'c') {
            GetNextFrame ();
            TrackUke ();
            DisplayFrameMarked ();
            key = cvWaitKey(30);
        }
        uke->GrabActiveKeyTemplates (currentFrame);
    }
    
    
    
    
    /* Function: DetermineFingering
     * ----------------------------
     * this function will track the uke and determine the fingering...
     */
    void DetermineFingering () {
        uke->DetermineFingering (currentFrame);
    }
    
    int GetCurrentChord () {
        return uke->GetCurrentChord (currentFrame);
    }
    
    /* Function: GetNextFrame
     * ----------------------
     * this will fill 'currentFrame' with updated information.
     */
    void GetNextFrame () {
        cvGrabFrame (capture);
        currentFrame = cvRetrieveFrame (capture);
        displayFrame = cvCloneImage(currentFrame);
    }
    
    
    /* Function: DisplayFrame
     * ----------------------
     * this function will display 'currentFrame' on the main display.
     */
    void DisplayFrameRegular () {
        cvShowImage (mainDisplayName, displayFrame);
    }
    void DisplayFrameMarked () {
        uke->DrawUkeFrame (displayFrame);
    }
    
    
    void PlayNote (int note) {
        
        MusicPlayer *player = new MusicPlayer();
        
        switch (note) {
            case F_CHORD:
                player->play (F_CHORD_FILEPATH);
                break;
            case C_CHORD:
                player->play (C_CHORD_FILEPATH);
                break;
            case G_CHORD:
                player->play (G_CHORD_FILEPATH);
                break;
            case A_MINOR:
                player->play (A_MINOR_FILEPATH);
                break;
            case E_MINOR:
                player->play (E_MINOR_FILEPATH);
                break;
            //default:??
                
                
        }

        //player->play("");
    }
    
    
    /* Function: constructor
     * ---------------------
     * sets up communication with the webcam;
     */
    Virtualele (char * newMainDisplayName, IplImage *fullResTemplate_white, IplImage *fullResTemplate_black )
    {
        capture = cvCreateCameraCapture (-1);
        GetNextFrame ();
        mainDisplayName = newMainDisplayName;
        
        uke = new Uke (currentFrame, fullResTemplate_white, fullResTemplate_black);
        cout << "# # # INITIALIZATION COMPLETE # # #\n";
    }
    
    
    
    /* Function: destructor
     * --------------------
     */
    ~Virtualele () {
    }
    
    
};

#endif
