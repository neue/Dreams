#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define NUM_PTS 10000

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
        void draw();
        void loadSketch(ofxXmlSettings sketchXML);
		
		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);		

		ofxXmlSettings XML;
		ofTrueTypeFont TTF;

		string xmlStructure;
		string message;

		ofPoint dragPts[NUM_PTS];
    
    
        struct rhonPoint {
            ofPoint pnt;
            rhonPoint(const float x, float y, float z){
                pnt.set(x,y,z);
            }

        };

        struct rhonLine {
            vector<rhonPoint> rhonPoints;
        };

        struct rhonSketch {
            vector<rhonLine*> rhonLines;
            ofPoint pos;
        };
    
        vector<rhonSketch*> rhonSketches;

		int pointCount;
		int lineCount;
		int lastTagNumber;
    
        float offsetX;
        float offsetY;
        float offsetZ;
    
    float timeCode;

    
    ofEasyCam cam; // add mouse controls for camera movement

};

