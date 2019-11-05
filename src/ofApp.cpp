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
        
        string newName;
        vector<string> nameTypes;
        vector<float> durations;
        
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
            durations.push_back(system_events[i]["dur"].asFloat());
        }
        float maxDuration = *max_element(durations.begin(), durations.end());
        
        // COLOR PALETTE
        vector<ofColor> palette = {ofColor(213, 4, 217),ofColor(85, 0, 194),ofColor( 125, 122, 191),ofColor(242, 105, 56),ofColor(160,194,30),ofColor(122,245,200),ofColor(219,202,0)};
        
        
        
        // One new color for each task type
        vector<int> taskCounter;
        vector<int> taskTotals;
        for(int t=0;t<tasks.size();t++){
            // Colors
            int rColor = ofRandom(palette.size());
            clusterColors.push_back(palette[rColor]);
            palette.erase(palette.begin()+rColor);
            // Origin
            float angle = t*ofDegToRad(360.0/tasks.size());
            // dist is how far out the clusters are from 0,0
            int dist = 200;
            clusterPos.push_back(ofVec3f(cos(angle)*dist, sin(angle)*dist,0));
            // init task counter
            taskTotals.push_back(0);
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
            
            colors.push_back(clusterColors[t]);
            
            //            float angle = ofRandom(ofDegToRad(360.0));
            //            float xOffset = cos(angle)*taskCounter[t]/(tasks[t]*0.005);
            //            float yOffset = sin(angle)*taskCounter[t]/(tasks[t]*0.005);
            //            pos.push_back(ofVec3f(clusterPos[t].x+xOffset,clusterPos[t].y+yOffset,0));
            taskTotals[t] = taskTotals[t]+1;
            
            // Duration (dur) corresponding to size and alpha
            rad.push_back(system_events[i]["dur"].asFloat());
            float alpha = ofMap(rad[i],0,maxDuration,250,10);
            colors[i] = ofColor(colors[i].r,colors[i].g,colors[i].b,alpha);
            //            rad[i] = width+(system_events[i]["dur"].asFloat()*0.001);
            rad[i]= ofRandom(width/2,width*2);
            //            sphere.setRadius(rad);
            
        }
        
        for(int t=0;t<tasks.size();t++){
            // Size of the cluster depends on amount of tasks of that type
            clusterRad.push_back(width*60 + taskTotals[t]/150);
        }
        
        // Set cluster sizes
        for (Json::ArrayIndex i = 0; i < systemEventsSize; ++i)
        {
            int t = distance(nameTypes.begin(), find(nameTypes.begin(), nameTypes.end(), names[i]));
            taskCounter[t] = taskCounter[t]+1;
            
            float spread = clusterRad[t]-10;
            float angle = ofRandom(ofDegToRad(360.0));
            float xOffset = cos(angle)*taskCounter[t]/(tasks[t]/spread);
            float yOffset = sin(angle)*taskCounter[t]/(tasks[t]/spread);
            pos.push_back(ofVec3f(clusterPos[t].x+xOffset,clusterPos[t].y+yOffset,0));
        }
        
        //-------------- Not used right now
        //        Get unique names: GPUTask, ImageDecodeTask, RasterTask, RunTask
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
    //    blur.setup(ofGetWidth(), ofGetHeight(), 30, .9, 2);
    
    // Post processing
    post.init(ofGetWidth(), ofGetHeight());
    ZoomBlurPass::Ptr zoom;
    zoom = post.createPass<ZoomBlurPass>();
    zoom->setEnabled(true);
    zoom->setDecay(0.5);
    zoom->setWeight(1);
    //    ZoomBlurPass(const ofVec2f& aspect, bool arb, float centerX = 0.5, float centerY = 0.5,
    //                   float exposure = 0.48, float decay = 0.9, float density = 0.25,
    //                   float weight = 0.25, float clamp = 1);
    
    
}

//--------------------------------------------------------------
void ofApp::update(){
    
}

//--------------------------------------------------------------
void ofApp::draw(){
    
    
    
    //    blur.begin();
    
    ofBackground(30);
    //    cam.begin();
    
    post.begin(cam);
    
    
    for (int t = 0; t < tasks.size(); t++){
        
        // moving around clusters tests
        //        clusterPos[t].x = clusterPos[t].x + ofMap( ofNoise( ofGetElapsedTimef()/5.0, -1000), 0, 1, 0, 50);
        //        clusterPos[t].y = clusterPos[t].y + ofMap( ofNoise( ofGetElapsedTimef()/5.0, 1000), 0, 1, 0, 50);
        
        cluster(clusterPos[t], clusterColors[t], clusterRad[t]);
    }
    
    for (int i = 0; i < systemEventsSize; ++i)
    {
        blob(pos[i], colors[i], rad[i]);
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
    
    //    // Microscope outline
    ofSetColor(10, 10, 10);
    ofDrawCircle(0,0,400);
    
    
    post.end();
}

//--------------------------------------------------------------
void ofApp::blob(ofVec3f pos, ofColor color, float radius){
    // Spheres
    //    float spinX = sin(ofGetElapsedTimef()*.35f);
    //    float spinY = cos(ofGetElapsedTimef()*.075f);
    //    sphere.setPosition(pos.x, pos.y, pos.z);
    //    sphere.rotate(spinX, 1.0, 0.0, 0.0);
    //    sphere.rotate(spinY, 0, 1.0, 0.0);
    
    // get all the faces from the icoSphere, handy when you want to copy
    // individual vertices or tweak them a little ;)
    //    vector<ofMeshFace> triangles = sphere.getMesh().getUniqueFaces();
    
    
    // Draws the events (small circles) in each cluster
    
    ofFill();
    ofSetColor(color);
    
    
    float startAngle = ofGetElapsedTimef() * TWO_PI/10.0; // angle = time (in seconds) * speed (in radians/second)
    
    float angleOffset = pos.x * (4.0*TWO_PI);
    float angle = startAngle + angleOffset;
    float scale = sin(angle*2.0) * 10.0;
    ofDrawCircle(pos.x+scale*cos(angle), pos.y+scale*sin(angle),radius);
    
    
    //    ofDrawCircle(pos.x,pos.y,radius);
}
void ofApp::cluster(ofVec3f pos, ofColor color, float radius){
    
    // Draws the cluster outlines
    
    ofNoFill();
    ofSetColor(color);
    //    ofDrawCircle(pos.x, pos.y, radius);
    
    ofPolyline polyline1;
    ofPoint center(pos.x,pos.y);
    polyline1.arc(center, radius, radius, 0, 360);
    polyline1.setClosed(true);
    ofPolyline copy = polyline1;
    
    float startAngle = ofGetElapsedTimef() * TWO_PI/40.0; // angle = time (in seconds) * speed (in radians/second)
    for (int i=0; i<polyline1.size(); i++) {
        float angleOffset = float(i)/polyline1.size() * (100.0*TWO_PI);
        float angle = startAngle + angleOffset;
        float scale = sin(angle*2.0) * 10.0;
        copy[i].x = polyline1[i].x + scale * cos(angle);
        copy[i].y = polyline1[i].y + scale * sin(angle);
    }
    
    copy.draw();
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
