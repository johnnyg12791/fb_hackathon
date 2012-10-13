//
//  musicPlayer.h
//  
//
//  Created by Sebastian Schuster on 10/13/12.
//
//

#ifndef _MUSICPLAYER
#define _MUSICPLAYER


#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>




/*for opencv*/
#include <opencv2/opencv.hpp>
#include <opencv2/highgui/highgui.hpp>



class MusicPlayer {
private:
    
    
public:
    MusicPlayer() {
    }
    
    ~MusicPlayer() {
    }
    
   
    
    int play( char *filename ) {
        char command[256];
        int status;
        
        /* create command to execute */
        sprintf( command, "afplay %s", filename );
        
        cout << command << endl;
        
        /* play sound */
        status = system( command );
        
        return status;
    }

    
    
    
    

    
    
};



#endif
