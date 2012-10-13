#ifndef _MATCHMAT
#define _MATCHMAT
#include <iostream>


class MatchMatrix {
public:
    cv::Mat matrix;
    
    CvPoint GetGlobalMax () {
        return globalMax;
    }
    
    MatchMatrix () {
    }
    MatchMatrix (int imageWidth, int imageHeight, int templateWidth, int templateHeight) {
        matrix.create (imageWidth - templateWidth + 1, imageHeight - templateHeight + 1);
        globalMaxVal = -1;
    }
};

#endif