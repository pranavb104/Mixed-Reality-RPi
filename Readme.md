
## About
A Mixed-Reality device using the small yet powerful RPI Zero 2W。It detect fiducials (aruco-markers) via the RPI dedicated camera and plays 720p resolution videos. All of this running simulataneously on the PI Zero2W.

I've compiled this project in Openframeworks (OFv0.10.0). It was difficult to link the libraries, especially to compile ofxOMXPlayer and ofxOMXCamera. So, ill provide a detailed guide for the right environment and setup instructions. This project greatly relies on the work done by (jvcleave)[https://github.com/jvcleave/ofxOMXCamera] for building the OMX wrapper inside openframeworks. 

## Instructions 
Before downloading the source files, make sure you have your environment setup:
RPI Buster 5.10.17 armv7l
OFv0.10.0

Then download the dependent libraries which are:
ofxAruco
ofxCv
ofxOpenCv
ofxPoco
ofxOMXCamera
ofxOMXPlayer

Before compiling, you would need to make certain changes in your config file located in:
libs/openFrameworksCompiled/project/linuxarmv6l/config.linuxarmv6l.default.mk

Comment out: USE_GLFW_WINDOW = 1 AND USE_PI_LEGACY = 0

And then Add PROJECT_LDFLAGS += -latomic to file: config.make for your project to compile.



