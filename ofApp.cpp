#include "ofApp.h"

/*
    If you are struggling to get the device to connect ( especially Windows Users )
    please look at the ReadMe: in addons/ofxKinect/README.md
*/

//--------------------------------------------------------------
void ofApp::setup() {
    //---------------------KINECT_PART_START--------------------------//
	ofSetLogLevel(OF_LOG_VERBOSE);
	
	// enable depth->video image calibration
	kinect.setRegistration(true);
    
	kinect.init();
	//kinect.init(true); // shows infrared instead of RGB video image
	//kinect.init(false, false); // disable video image (faster fps)
	
	kinect.open();		// opens first available kinect
	// print the intrinsic IR sensor values
	if(kinect.isConnected()) {
		ofLogNotice() << "sensor-emitter dist: " << kinect.getSensorEmitterDistance() << "cm";
		ofLogNotice() << "sensor-camera dist:  " << kinect.getSensorCameraDistance() << "cm";
		ofLogNotice() << "zero plane pixel size: " << kinect.getZeroPlanePixelSize() << "mm";
		ofLogNotice() << "zero plane dist: " << kinect.getZeroPlaneDistance() << "mm";
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.init();
	kinect2.open();
#endif
	
	grayImage.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
	
	nearThreshold = 190;
	farThreshold = 145;
	
	ofSetFrameRate(40);
	
	// 20 the tilt on startup
	angle = 20;
	kinect.setCameraTiltAngle(angle);
    //---------------------KINECT_PART_END--------------------------//

}

//--------------------------------------------------------------
void ofApp::update() {
    //---------------------KINECT_PART_START--------------------------//

	ofBackground(100, 100, 100);
	kinect.update();
    
	// there is a new frame and we are connected
	if(kinect.isFrameNewDepth()) {
		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels());
			ofPixels & pix = grayImage.getPixels();
			int numPixels = pix.size();
        
			for(int i = 0; i < numPixels; i++) {
				if(pix[i] < nearThreshold && pix[i] > farThreshold) {
					pix[i] = 255;
				} else {
					pix[i] = 0;
				}
			}
        for (int x = 0; x < 30; x++)
        {
            for (int y = 0; y < 30; y++)
            {
                samples[x][y] = pix[80+(640)*(y*16)+x*16] == 255;
                if(samples[x][y]){
                    cout << "true" << x<< " " << y << endl;
                    myGrid.setColor(x, y, true);
                }else{
                    myGrid.setColor(x, y, false);
                }
            }
        }
       
        grayImage.getPixels().mirror(false, true);

		// update the cv images
		grayImage.flagImageChanged();
		contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
	}
	
#ifdef USE_TWO_KINECTS
	kinect2.update();
#endif
    //---------------------KINECT_PART_END--------------------------//
}

//--------------------------------------------------------------
void ofApp::draw() {
    //---------------------KINECT_PART_START--------------------------//
	ofSetColor(255, 255, 255);
    
		grayImage.draw(490, 10, 640, 480);
        myGrid.draw();
		contourFinder.draw(-70, 10, 640, 480);
		
#ifdef USE_TWO_KINECTS
		kinect2.draw(420, 320, 400, 300);
#endif

	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
    
	reportStream<< "set near threshold " << nearThreshold << " (press: + -)" << endl
	<< "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
	<< ", fps: " << ofGetFrameRate() << endl
	<< "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl;

    if(kinect.hasCamTiltControl()) {
    	reportStream << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
        << "press 1-5 & 0 to change the led mode" << endl;
    }
	ofDrawBitmapString(reportStream.str(), 20, 652);
    //---------------------KINECT_PART_END--------------------------//
}


//--------------------------------------------------------------
void ofApp::exit() {
    //---------------------KINECT_PART_START--------------------------//

	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	
#ifdef USE_TWO_KINECTS
	kinect2.close();
#endif
    //---------------------KINECT_PART_END--------------------------//
}

//--------------------------------------------------------------
void ofApp::keyPressed (int key) {
    //---------------------KINECT_PART_START--------------------------//
	switch (key) {
			
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;
			
		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;
			
		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
			
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
			
		case 'o':
			kinect.setCameraTiltAngle(angle); // go back to prev tilt
			kinect.open();
			break;
			
		case 'c':
			kinect.setCameraTiltAngle(0); // zero the tilt
			kinect.close();
			break;
			
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;
			
		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
    //---------------------KINECT_PART_END--------------------------//
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
	
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{

}
