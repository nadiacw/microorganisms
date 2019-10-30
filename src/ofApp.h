#pragma once

#include "ofMain.h"
#include "ofxJSON.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    void blob(ofVec3f pos);
    
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
    int eventSize;
    
    // graphics
    vector<ofColor> colors;
    vector<ofVec3f> pos;
    
    ofSpherePrimitive sphere;
    
    // easyCam
//    void drawInteractionArea();
    bool bHelpText;
    ofEasyCam cam; // add mouse controls for camera movement
};
