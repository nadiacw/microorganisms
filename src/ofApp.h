#pragma once

#include "ofMain.h"
#include "ofxJSON.h"
#include "ofxBlur.h"
#include "ofxPostProcessing.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void blob(ofVec3f pos, ofColor color);
    
    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    ofxJSONElement json;
    int systemEventsSize;
    
    // graphics
    vector<ofColor> colors;
    vector<ofVec3f> pos;
    
    ofSpherePrimitive sphere;
    int rad;
    
    // easyCam
    bool bHelpText;
    ofEasyCam cam; // add mouse controls for camera movement
    
    // blur
    ofxBlur blur;
    
    // Post processing
    ofxPostProcessing post;
};
