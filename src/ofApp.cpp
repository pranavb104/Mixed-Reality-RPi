#include "ofxCv.h"
#include "ofApp.h"
#include "ofBitmapFont.h"


//--------------------------------------------------------------
void ofApp::setup(){
	ofSetLogLevel(OF_LOG_VERBOSE);
	ofSetLogLevel("ofThread", OF_LOG_ERROR);
	consoleListener.setup(this);
	ofBackground(0);
	ofHideCursor();
	/*if(wiringPiSetup() == -1){
		printf("Error on wiringPi setup\n");
	}
	pinMode(9,INPUT);
	*/

	//OMXPlayer settings
	ofxOMXPlayerSettings psettings;
	psettings.useHDMIForAudio = false;
	psettings.enableTexture = false;
	psettings.enableLooping = true;
	psettings.enableAudio = false;
	
	//setup OMXPlayer
	omxPlayer.setup(psettings);
	startVideo = true;
	processPixel = true;

	//setup cam parameters
	string cameraIntrinsics = "logitech.yml";
	string markerFile = "marker.xml";
	
	//Already defined in .h file
	settings.sensorWidth = 640;
	settings.sensorHeight = 480;
	settings.framerate = 30;
	settings.enableTexture = true;
	settings.enablePixels = true;
	doPixels=false;
	videoTexture.allocate(settings.sensorWidth, settings.sensorHeight, GL_RGBA);
	
	//setup OMX video grabber
	videoGrabber.setup(settings);
	videoGrabber.setSensorCrop(25,25,60,60);
	
	//Load ofPixel object to store RGb values from OMXGrabber
	rgbPixel.allocate(640,480,OF_PIXELS_RGB);
	//load marker
	aruco.setUseHighlyReliableMarker(markerFile);
	
	//init 
	aruco.setThreaded(true);
	aruco.setupXML(cameraIntrinsics, settings.sensorWidth, settings.sensorHeight);
	
	//Load flashlight
	flash.load("flash.jpg");
	flash.resize(400,400);

}

//--------------------------------------------------------------
void ofApp::update(){
	
	//processPixel = digitalRead(9);
	
	if(processPixel){
		if(videoGrabber.isFrameNew()){
			ofPixels pixels = videoGrabber.getPixels(); //Grab pixels
			arucoDetect(pixels); //A new function to convert RGBA to RGB becoz Aruco requires an RGB input.
		}
	}else {
		if(videoGrabber.isFrameNew()){
			//ofPixels pixels = videoGrabber.getPixels(); //Grab pixels
			//videoTexture.loadData(pixels,GL_RGBA);
			//arucoDetect(pixels);
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw(){

	
	if(!processPixel){
		//videoTexture.draw(250, 150, settings.sensorWidth, settings.sensorHeight);
	}
	
	if((aruco.getNumMarkers()<1) && processPixel){
		ofPushMatrix();
		ofTranslate(ofGetWidth()/2,ofGetHeight()/2);
		//ofTranslate(3*ofGetWidth()/4,ofGetHeight()/2);
		flash.draw(-flash.getWidth()/2,-flash.getHeight()/2);
		ofPopMatrix();
		if(!startVideo){
			omxPlayer.close();
			startVideo = true;
		}
	}
	
	if(processPixel){
		for (auto& mark: aruco.getMarkers()) {
			switch(mark.id){
				case 21:
					if(startVideo){ 
						string videoPath = ofToDataPath("merc2.mov",true);
						omxPlayer.loadMovie(videoPath);
						omxPlayer.restartMovie();
						maskOn = true;
						startVideo = false;
					}
					omxPlayer.draw(0,0,704,480);
					break;
				case 3:
					if(startVideo){ 
						string videoPath = ofToDataPath("merc3.mov",true);
						omxPlayer.loadMovie(videoPath);
						omxPlayer.restartMovie();
						maskOn = true;
						startVideo = false;
					}
					omxPlayer.draw(0,0,704,480);
					break;
				case 5:
					if(startVideo){ 
						string videoPath = ofToDataPath("merc4.mov",true);
						omxPlayer.loadMovie(videoPath);
						omxPlayer.restartMovie();
						maskOn = true;
						startVideo = false;
					}
					omxPlayer.draw(0,0,704,480);
					break;
				default:
					//do something here
					break;
					
			}
			if(omxPlayer.isOpen()){
				//omxPlayer.draw(0,100,704,480);
			}
		}
	}
	
	
	/*stringstream info;
	info << "APP FPS: " << ofGetFrameRate() << "\n";
	info << "PIXELS ENABLED: " << processPixel << "\n";
	info << "Markers detected: "  << aruco.getNumMarkers() << "\n";
	info << "Pin Mode: " << digitalRead(9) << "\n";
	
	ofDrawBitmapStringHighlight(info.str(), 100, 100, ofColor::black, ofColor::yellow);
	*/


}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	ofLog(OF_LOG_VERBOSE, "%c keyPressed", key);
	
	switch(key){
		case('p'):
			doPixels = !doPixels;
			if (!doPixels) 
			{
				videoGrabber.disablePixels();
			}else
			{
				videoGrabber.enablePixels();
			}
			break;
		case('o'):
			processPixel = !processPixel;
			break;
		case('f'):
			ofToggleFullscreen();
			break;
		case '=':
		case '+':
			ofLogVerbose() << "increaseVolume";
			omxPlayer.increaseVolume();
			break;
		case('z'):
			videoGrabber.zoomIn();
			break;
		case('r'):
			videoGrabber.resetZoom();
			break;
		case('s'):
			videoGrabber.setSensorCrop(25,25,60,60);
			break;
		case('d'):
			videoGrabber.setSensorCrop(0,0,100,100);
			break;
	}
}

void ofApp::setFboMask(){
	
	//Allocate mask to video player height and width
	mask.allocate(omxPlayer.getWidth(),omxPlayer.getHeight());
	
	mask.begin();
	ofClear(0);
	//ofDrawCircle(400,200,500);
	ofDrawCircle(3*ofGetWidth()/8,100+(3*ofGetHeight()/8),3*ofGetHeight()/8);
	mask.end();
	
	omxPlayer.getTextureReference().setAlphaMask(mask.getTexture());
	
}

void ofApp::arucoDetect(ofPixels pixels){
	//Since the pixels are RGBA type, need to convert them to RGB 
	int width = pixels.getWidth();
	int height = pixels.getHeight();
	for(int col=0; col<width; col++){
		for(int row=0; row<height; row++){
			long int base = (row*width + col)*4 ;
			long int rgb_base = (row*width + col)*3;
			rgbPixel[rgb_base] = pixels[base];
			rgbPixel[rgb_base+1] = pixels[base+1];
			rgbPixel[rgb_base+2] = pixels[base+2];
		}
	}
	
	aruco.detectBoards(rgbPixel); //Aruco detectBoard
}

void ofApp::onCharacterReceived(KeyListenerEventData& e)
{
	keyPressed((int)e.character);
}
