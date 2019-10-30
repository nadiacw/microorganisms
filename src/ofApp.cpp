#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    std::string file = "data.json";
    
    // Now parse the JSON
    bool parsingSuccessful = json.open(file);
    
    if (parsingSuccessful)
    {
        ofLogNotice("ofApp::setup") << json.getRawString();
    }
    else
    {
        ofLogNotice("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    // Setup graphics
    float width     = ofGetWidth() * .005;
    float height    = ofGetHeight() * .005;
    sphere.setRadius( width );
    
    
    ofLog() << json["events"];
    ofLog() << json["events"][3]["ts"];
    
    if (json["events"].isArray())
    {
        const Json::Value& events = json["events"];
        eventSize = events.size();
        for (Json::ArrayIndex i = 0; i < eventSize; ++i)
        {
            double ts = events[i]["ts"].asDouble();
            double ts0 = events[0]["ts"].asDouble();
            double tsN = events[eventSize-1]["ts"].asDouble();
            double ts_mapped = ofMap(ts, ts0, tsN, -500, 500);
            colors.push_back( ofColor(ofRandom(0,255),ofRandom(0,255),ofRandom(0,255)));
            ofLog() << "ts_mapped: " << ts_mapped;
            pos.push_back( ofVec3f(0, 0,ts_mapped));
        }
    }
    
    // Setup easyCam
    ofSetVerticalSync(true);

    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();

    ofSetCircleResolution(64);
    bHelpText = true;
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(0);
    cam.begin();
    
    for (int i = 0; i < eventSize; ++i)
    {
        ofSetColor(colors[i]);
        blob(pos[i]);
    }
    
    
    
    // cam help
    if (bHelpText) {
        ofDrawGrid(20,10,true,true,true,true);
           stringstream ss;
           ss << "FPS: " << ofToString(ofGetFrameRate(),0) <<endl<<endl;
           ss << "MODE: " << (cam.getOrtho()?"ORTHO":"PERSPECTIVE")<<endl;
           ss << "MOUSE INPUT ENABLED: " << (cam.getMouseInputEnabled()?"TRUE":"FALSE")<<endl;
           ss << "INERTIA ENABLED: " << (cam.getInertiaEnabled()?"TRUE":"FALSE")<<endl;
           ss << "ROTATION RELATIVE Y AXIS: " << (cam.getRelativeYAxis()?"TRUE":"FALSE")<<endl;
           ss << endl;
           ss << "Toogle camera projection mode (ORTHO or PERSPECTIVE):"<< endl;
           ss << "    press space bar."<< endl;
           ss << "Toggle mouse input:"<<endl;
           ss << "    press 'c' key."<< endl;
           ss << "Toggle camera inertia:"<<endl;
           ss << "    press 'i' key."<< endl;
           ss << "Toggle rotation relative Y axis:"<<endl;
           ss << "    press 'y' key."<< endl;
           ss << "Toggle this help:"<<endl;
           ss << "    press 'h' key."<< endl;
           ss << endl;
           ss << "camera x,y rotation:" <<endl;
           ss << "    LEFT MOUSE BUTTON DRAG inside yellow circle"<<endl;
           ss << endl;
           ss << "camera z rotation or roll"<<endl;
           ss << "    LEFT MOUSE BUTTON DRAG outside yellow circle"<<endl;

           ss << endl;
           ss << "move over x,y axis / truck and boom:"<<endl;
           ss << "    LEFT MOUSE BUTTON DRAG + m"<<endl;
           ss << "    MIDDLE MOUSE BUTTON PRESS"<<endl;
           ss << endl;
           ss << "move over z axis / dolly / zoom in or out:"<<endl;
           ss << "    RIGHT MOUSE BUTTON DRAG"<<endl;
           ss << "    VERTICAL SCROLL"<<endl<<endl;
           if (cam.getOrtho()) {
               ss << "    Notice that in ortho mode zoom will be centered at the mouse position." << endl;
           }
           ofDrawBitmapString(ss.str().c_str(), 20, 20);
       }
}

//--------------------------------------------------------------
void ofApp::blob(ofVec3f pos){
    float spinX = sin(ofGetElapsedTimef()*.35f);
    float spinY = cos(ofGetElapsedTimef()*.075f);
    sphere.setPosition(pos.x, pos.y, pos.z);
    sphere.rotate(spinX, 1.0, 0.0, 0.0);
    sphere.rotate(spinY, 0, 1.0, 0.0);
    
    // get all the faces from the icoSphere, handy when you want to copy
    // individual vertices or tweak them a little ;)
    //    vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
    
    // now draw
    sphere.draw();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key) {
      case ' ':
            cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
            break;
        case 'C':
        case 'c':
            cam.getMouseInputEnabled() ? cam.disableMouseInput() : cam.enableMouseInput();
            break;
        case 'F':
        case 'f':
            ofToggleFullscreen();
            break;
        case 'H':
        case 'h':
            bHelpText ^=true;
            break;
        case 'I':
        case 'i':
            cam.getInertiaEnabled() ? cam.disableInertia() : cam.enableInertia();
            break;
        case 'Y':
        case 'y':
            cam.setRelativeYAxis(!cam.getRelativeYAxis());
            break;
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){
    
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){
    
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 
    
}
