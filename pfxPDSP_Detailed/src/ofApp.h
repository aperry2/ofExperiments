#pragma once

#include "ofMain.h"
#include "ofxPDSP.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
    void mouseMoved(int x, int y);
  
    // pdsp modules
    ofxPDSPEngine engine;
    
    pdsp::VAOscillator osc;
    pdsp::LFO lfo;
};
