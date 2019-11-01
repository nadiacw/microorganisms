#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    
    std::string file = "../../query-analyser/scores/system_events.json";
    
    // Now parse the JSON
    bool parsingSuccessful = json.open(file);
    
    if (parsingSuccessful)
    {
        //        ofLogNotice("ofApp::setup") << json.getRawString();
    }
    else
    {
        ofLogNotice("ofApp::setup")  << "Failed to parse JSON" << endl;
    }
    
    // Setup graphics
    float width     = ofGetWidth() * .001;
    float height    = ofGetHeight() * .001;
    
    //    ofLog() << json["events"][3]["name"];
    
    if (json["events"].isArray())
    {
        //        Structure of a system event:
        //        {
        //            "name": "RunTask",
        //            "ts": 836309714551,
        //            "dur": 22
        //        }
        const Json::Value& system_events = json["events"];
        systemEventsSize = system_events.size();
        vector<string> names;
        vector<int> tasks;
        
        string newName;
        vector<string> nameTypes;
        
        // Find out how many name types there are in the data
        for (Json::ArrayIndex i = 0; i < systemEventsSize; ++i)
        {
            newName = system_events[i]["name"].asString();
            vector<string>::iterator it = find(nameTypes.begin(), nameTypes.end(), newName);
            if (it==nameTypes.end()){
                // New name type, add it to the list
                nameTypes.push_back(newName);
                tasks.push_back(0);
            }
            else{
                // Name already exists, which does it belong to?
                int index = distance(nameTypes.begin(), it);
                tasks[index]++;
            }
        }
        
        // COLOR PALETTE
        vector<ofColor> palette = {ofColor(70,77,115),ofColor(153,178,221),ofColor(225,169,135),ofColor(229,75,75),ofColor(160,232,175)};
        
        // One new color for each task type
        vector<ofColor> allColors;
        vector<ofVec3f> origin;
        vector<int> taskCounter;
        for(int i=0;i<tasks.size();i++){
            // Colors
            int rColor = ofRandom(palette.size());
            allColors.push_back(palette[rColor]);
            palette.erase(palette.begin()+rColor);
            // Origin
            float angle = i*ofDegToRad(360.0/tasks.size());
            // dist is how far out the clusters are from 0,0
            int dist = 200;
            origin.push_back(ofVec3f(cos(angle)*dist, sin(angle)*dist,0));
            // init task counter
            taskCounter.push_back(0);
        }
        
        
        
        // Now generate positions based on the amount of data
        for (Json::ArrayIndex i = 0; i < systemEventsSize; ++i)
        {
            // Fill names array
            names.push_back(system_events[i]["name"].asString());
            
            //-------------- Not used right now
            // Map time (ts)
            //            double ts_mapped = ofMap(system_events[i]["ts"].asDouble(), system_events[0]["ts"].asDouble(), system_events[systemEventsSize-1]["ts"].asDouble(), -500, 500);
            //            pos.push_back( ofVec3f(0, 0,ts_mapped));
            //--------------
            
            // type number for each data instance
            int t = distance(nameTypes.begin(), find(nameTypes.begin(), nameTypes.end(), names[i]));
            
            colors.push_back(allColors[t]);
            
            float angle = ofRandom(ofDegToRad(360.0));
            float xOffset = cos(angle)*taskCounter[t]/(tasks[t]*0.005);
            float yOffset = sin(angle)*taskCounter[t]/(tasks[t]*0.005);
//            float xOffset = cos(angle)*taskCounter[t]/50;
//            float yOffset = sin(angle)*taskCounter[t]/50;
            pos.push_back(ofVec3f(origin[t].x+xOffset,origin[t].y+yOffset,0));
            taskCounter[t] = taskCounter[t]+1;
            
            
            
            //            // 4 system event names: GPUTask, ImageDecodeTask, RasterTask, RunTask
            //            // different color for each name, hardcoded for these four types, for now
            //            if(names[i] == "GPUTask") {
            //                // Color
            //                colors.push_back(allColors[0]);
            //                // Position
            //                float angle = ofRandom(ofDegToRad(360.0));
            //                float xOffset = cos(angle)*task1/tasks[0];
            //                float yOffset = sin(angle)*task1/tasks[0];
            //                pos.push_back(ofVec3f(origin[0].x+xOffset,origin[0].y+yOffset,0));
            //                task1 ++;
            //            }
            //            else if(names[i] == "ImageDecodeTask") {
            //                // Color
            //                colors.push_back(allColors[1]);
            //                // Position
            //                float angle = ofRandom(ofDegToRad(360.0));
            //                float xOffset = cos(angle)*task2/tasks[1];
            //                float yOffset = sin(angle)*task2/tasks[1];
            //                pos.push_back(ofVec3f(origin[1].x+xOffset,origin[1].y+yOffset,0));
            //                task2 ++;
            //            }
            //            else if(names[i] == "RasterTask") {
            //                // Color
            //                colors.push_back(allColors[2]);
            //                // Position
            //                float angle = ofRandom(ofDegToRad(360.0));
            //                float xOffset = cos(angle)*task3/tasks[2];
            //                float yOffset = sin(angle)*task3/tasks[2];
            //                pos.push_back(ofVec3f(origin[2].x+xOffset,origin[2].y+yOffset,0));
            //                task3++;
            //            }
            //            else if(names[i] == "RunTask") {
            //                // Color
            //                colors.push_back(allColors[3]);
            //                // Position
            //                float angle = ofRandom(ofDegToRad(360.0));
            //                float xOffset = cos(angle)*task4/tasks[3];
            //                float yOffset = sin(angle)*task4/tasks[3];
            //                pos.push_back(ofVec3f(origin[3].x+xOffset,origin[3].y+yOffset,0));
            //                task4 ++;
            //            }
            
            // Duration (dur)
            rad = width+system_events[i]["dur"].asInt();
            sphere.setRadius(rad);
            
        }
        
        //-------------- Not used right now
        //        Get unique names
        //        vector<string> uniqueNames;
        //        set<string> s;
        //        for( unsigned i = 0; i < systemEventsSize; ++i ) s.insert( names[i] );
        //        uniqueNames.assign( s.begin(), s.end() );
        //
        //        for(int i=0; i < uniqueNames.size(); i++)
        //            ofLog() << uniqueNames[i] << endl;
        //--------------
        
        
        
    }
    
    // Setup easyCam
    ofSetVerticalSync(true);
    
    // this uses depth information for occlusion
    // rather than always drawing things on top of each other
    ofEnableDepthTest();
    
    ofSetCircleResolution(64);
    bHelpText = false;
    
    // Blur
    //    blur.setup(ofGetWidth(), ofGetHeight(), 20, .2, 2);
    
    // Post processing
    //    post.init(ofGetWidth(), ofGetHeight());
    //    post.createPass<DofPass>()->setEnabled(true);
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackground(30);
    cam.begin();
    
    
    
    //    blur.begin();
    
    //    post.begin(cam);
    for (int i = 0; i < systemEventsSize; ++i)
    {
        blob(pos[i], colors[i]);
    }
    //    post.end();
    
    //    blur.end();
    
    //    blur.draw();
    
    
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
    
    // Microscope outline
    ofSetColor(10, 10, 10);
    ofDrawCircle(0,0,400);
}

//--------------------------------------------------------------
void ofApp::blob(ofVec3f pos, ofColor color){
    // Spheres
    //    float spinX = sin(ofGetElapsedTimef()*.35f);
    //    float spinY = cos(ofGetElapsedTimef()*.075f);
    //    sphere.setPosition(pos.x, pos.y, pos.z);
    //    sphere.rotate(spinX, 1.0, 0.0, 0.0);
    //    sphere.rotate(spinY, 0, 1.0, 0.0);
    
    // get all the faces from the icoSphere, handy when you want to copy
    // individual vertices or tweak them a little ;)
    //    vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
    
    // now draw
    ofSetColor(color);
    //    sphere.draw();
    
    // Circles
    ofDrawCircle(pos.x,pos.y,rad);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    //    switch(key) {
    //        case ' ':
    //            cam.getOrtho() ? cam.disableOrtho() : cam.enableOrtho();
    //            break;
    //        case 'C':
    //        case 'c':
    //            cam.getMouseInputEnabled() ? cam.disableMouseInput() : cam.enableMouseInput();
    //            break;
    //        case 'F':
    //        case 'f':
    //            ofToggleFullscreen();
    //            break;
    //        case 'H':
    //        case 'h':
    //            bHelpText ^=true;
    //            break;
    //        case 'I':
    //        case 'i':
    //            cam.getInertiaEnabled() ? cam.disableInertia() : cam.enableInertia();
    //            break;
    //        case 'Y':
    //        case 'y':
    //            cam.setRelativeYAxis(!cam.getRelativeYAxis());
    //            break;
    //    }
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
