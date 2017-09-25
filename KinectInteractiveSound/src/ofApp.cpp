#include "ofApp.h"

using namespace std;

int minPixelVal = 150;
int maxPixelVal = 165;

float maxSpeedVal = 1.0f;
float minSpeedVal = 0.3f;

int customThresholdPixelVal = 100;

//--------------------------------------------------------------
void ofApp::setup(){
  //Uncomment for verbose info from libfreenect2
  //ofSetLogLevel(OF_LOG_VERBOSE);
    
  ofBackground(30, 30, 30);
  
  // See if we have a valid Kinect hooked to the system.
  ofxKinectV2 tmp;
  vector <ofxKinectV2::KinectDeviceInfo> deviceList = tmp.getDeviceList();
  
  panel.setup("", "settings.xml", 10, 100);
  
  // Modify this if there are multiple Kinects hooked into the system.
  if (deviceList.size() > 0) {
    kinect = new ofxKinectV2();
    kinect -> open(deviceList[0].serial);
    panel.add(kinect->params);
  }
  
  // Load track.
  track1.load("howtostillmind.mp3");
  track1.setVolume(0.75f);
  track1.setLoop(true);
  
  // Start playing the track.
  // TODO: Hook it up with Touch OSC later.
  track1.play();
  
  panel.loadFromFile("settings.xml");
}

void ofApp::updateSound() {
  float newSpeed = ofMap(avgBrightness, minPixelVal, maxPixelVal, maxSpeedVal, minSpeedVal, true);
  track1.setSpeed(newSpeed);
}

//--------------------------------------------------------------
void ofApp::update(){
  int sumX = 0;
  int sumY = 0;
  int numPixels = 0;
  int sumBrightness = 0;
  
  if (kinect != NULL){
    kinect->update();
    if( kinect->isFrameNew() ){
      
      // Get depth pixels.
      ofPixels pixels = kinect -> getDepthPixels();
      
      // Create the texture from Kinect pixels.
      depthTexture.loadData(pixels);
      int height = depthTexture.getHeight();
      int width = depthTexture.getWidth();
      
      for (int x = 0; x < width; x += pixelSkip) {
        for (int y = 0; y < height; y += pixelSkip) {
          
          // Index of this pixel in 1D array.
          int pixelIndex = x + y * width;
          
          // Is the current pixelVal > customThresholdPixelVal?
          // This is to start averaging out the brightness pixels
          // for calculating average brightness.
          int pixelVal = (int) pixels[pixelIndex];
          if (pixelVal > customThresholdPixelVal) {
            sumX += x;
            sumY += y;
            sumBrightness += pixels[pixelIndex];
            numPixels++;
          }
        }
      }
      
      // Valid pixels within that threshold? Update sound based on this
      // data.
      if (numPixels != 0) {
        // Calculate average pixel values.
        avgX = sumX / numPixels;
        avgY = sumY / numPixels;
        avgBrightness = sumBrightness / numPixels;
        // Update the sound.
        updateSound();
      }
    }
  }
}

//--------------------------------------------------------------
void ofApp::draw(){
  // Draw the depth image.
  depthTexture.draw(0, 0);
  
  ofPushStyle();
  
  // Draw a circle at (avgX, avgY)
  ofSetColor(ofColor::purple);
  ofFill();
  ofDrawCircle(avgX, avgY, 20);
  ofDrawBitmapString(avgBrightness, ofGetWidth()/2, ofGetHeight()/2);
  
  ofPopStyle();
  
  // Threshold panel.
  panel.draw();
}
