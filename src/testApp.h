#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

#define NUM_PTS 10000
#define FOG_DIST 2000
#define MAX_GRID 4
#define GRID_SPACINGX 300
#define GRID_SPACINGY 225

class testApp : public ofBaseApp{

	public:

		void setup();
		void update();
        void draw();
        void loadSketch(ofxXmlSettings sketchXML, float anchorX, float anchorY, float anchorZ);
		
        void corrupt(int sketchToCorrupt);
    
        void drawZeroPoint();
        void drawCrosshair(float x,float y,float z,float size);
        void drawGrid(float x,float y,float z);
    
        float measure(float x1,float y1,float x2,float y2);
    
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
    
        int visibleSketch;
    
        int sketchBrightness;
    
    float offsetX;
    float offsetY;
    float offsetZ;
    
    float angle;
    float rotateX;
    float rotateY;
    float rotateZ;
    ofFbo screenFbo;

    ofShader chrom_abb;

    int gridBrightness;
    float gridDistort;
    
    float timeCode;

    int aberrationX;
    int aberrationY;
    
    float corruptionAmplitude;
    float corruptionBurnoff;
    float corruptionTimer;
    int corruptionCount;
    
    ofEasyCam cam; // add mouse controls for camera movement

};

