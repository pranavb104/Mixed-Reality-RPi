#pragma once

#include "ofMain.h"
#include "ofxAruco.h"
#include "highlyreliablemarkers.h"
#include "ofAppEGLWindow.h"
#include "TerminalListener.h"
#include "ofxOMXVideoGrabber.h"
#include "ImageFilterCollection.h"
#include "ofxOMXPlayer.h"
#include "wiringPi.h"

class ofApp : public ofBaseApp, public KeyListener{

	public:
		void setup();
		void update();
		void draw();
		void keyPressed(int key);
		void setFboMask();
		void arucoDetect(ofPixels pixels);
		
	void onCharacterReceived(KeyListenerEventData& e);
	TerminalListener consoleListener;
	
	//OMX Player
	ofxOMXPlayer omxPlayer;
	bool startVideo;
	string videoPath;
	
	//OMX Grabber
	ofxOMXVideoGrabber videoGrabber;
	ofTexture videoTexture;
	ofxOMXCameraSettings settings;
	bool doPixels, processPixel;

	ofxAruco aruco;
	//bool showMarkers;
	ofPixels rgbPixel;
	ofImage marker, flash;
	
	ofFbo mask;
	bool maskOn;
};
