#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(255,255,255);

	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading sketch.xml";

	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile("sketch2.xml") )  {loadSketch(XML,-1040,-600,-120);}
	if( XML.loadFile("sketch.xml") )   {loadSketch(XML,-1040,-600,-120);}
	if( XML.loadFile("cat.xml") )      {loadSketch(XML,-1040,-600,-3120);}
	if( XML.loadFile("spongebob.xml") ){loadSketch(XML,-1040,-300,-120);}

	//we initalize some of our variables
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
    
     offsetX = 0.0;
     offsetY = 0.0;
     offsetZ = 0.0;
	TTF.loadFont("mono.ttf", 7);
    
    visibleSketch = 0;
}

//--------------------------------------------------------------
void testApp::loadSketch(ofxXmlSettings sketchXML, float anchorX, float anchorY, float anchorZ){
    rhonSketch * newRhonSketch = new rhonSketch();
    
	int numLines = sketchXML.getNumTags("line");
    
	if(numLines > 0){
        for(int i = 0;i < numLines;i++){
            rhonLine * newRhonLine = new rhonLine();
            sketchXML.pushTag("line", i);
            int numPtTags = sketchXML.getNumTags("pt");
            if(numPtTags > 0){
                ofLog() << " Numpointtags:" << numPtTags;
                
                int totalToRead = MIN(numPtTags, NUM_PTS);
                
                for(int i2 = 0; i2 < numPtTags; i2++){
                    int x = sketchXML.getValue("pt:x", 0, i2);
                    int y = sketchXML.getValue("pt:y", 0, i2);
                    int z = sketchXML.getValue("pt:z", 0, i2);
                    newRhonLine->rhonPoints.push_back(rhonPoint(x, y, z*-1));
                    dragPts[i2].set(x, y,z);
                    pointCount++;
                }
            }
            sketchXML.popTag();
            newRhonSketch->rhonLines.push_back(newRhonLine);
        }
        ofLog() << "Lines:" << newRhonSketch->rhonLines.size();
	}
    newRhonSketch->pos.set(anchorX,anchorY,anchorZ);
    rhonSketches.push_back(newRhonSketch);
    
}

//--------------------------------------------------------------
void testApp::update(){
    timeCode += 0.01;
    
}

//--------------------------------------------------------------
void testApp::draw(){
	cam.begin();

	//---------
	//Lets draw the stroke as a continous line
    
    sketchBrightness = measure(0,0,offsetX,offsetY);
    
	ofSetColor(sketchBrightness,sketchBrightness,sketchBrightness);
	ofNoFill();
//	ofBeginShape();
//	for(int i = 0; i < pointCount; i++){
//		ofVertex(dragPts[i].x+offsetX+ofRandom(-0.2,0.2),
//                 dragPts[i].y+offsetY+ofRandom(-0.2,0.2),
//                 dragPts[i].z+offsetZ+ofRandom(-0.2,0.2)
//                 );
//	}
//    ofEndShape(false);
    glPushMatrix();
    glRotatef(sin(timeCode)*180, 0, 1, 0);

    for(int i = 0; i < rhonSketches.size();i++){
    if(i== visibleSketch){
        for(int i2 = 0; i2 < rhonSketches[i]->rhonLines.size();i2++){
            ofBeginShape();
                for(int i3=0;i3 < rhonSketches[i]->rhonLines[i2]->rhonPoints.size();i3++){
                ofVertex(rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.x+rhonSketches[i]->pos.x+offsetX,
                         rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.y+rhonSketches[i]->pos.y+offsetY,
                         rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.z+rhonSketches[i]->pos.z+offsetZ
                         );
                }
            ofEndShape(false);
        }
        
    }
    }
	ofEndShape(false);
    
    glPopMatrix();
    
    drawZeroPoint();
    
    ofFill();
    cam.end();

	ofSetColor(0, 0, 0, 200);
    TTF.drawString("Points: "+ofToString(sketchBrightness), 170, 12);
    

}

//--------------------------------------------------------------
void testApp::drawZeroPoint(){
    ofSetColor(255,0,0);
    ofBeginShape();
    ofVertex(-100,0,0);
    ofVertex(100,0,0);
    ofEndShape(false);
    ofBeginShape();
    ofVertex(0,100,0);
    ofVertex(0,-100,0);
    ofEndShape(false);
    ofBeginShape();
    ofVertex(0,0,-100);
    ofVertex(0,0,100);
    ofEndShape(false);
}

float testApp::measure(float x1,float y1,float x2,float y2){
    float dx = x1 - x2;
    float dy = y1 - y2;
    
    
    
    return ((sqrt( dx * dx + dy * dy ))-0)/(1000-0) * (255-0) + 0;
}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    if (key == 't'){offsetX += 20;}
    if (key == 'y'){offsetX -= 20;}
    if (key == 'g'){offsetY += 20;}
    if (key == 'h'){offsetY -= 20;}
    if (key == 'b'){offsetZ += 20;}
    if (key == 'n'){offsetZ -= 20;}
    if (key == 's'){visibleSketch += 1;}
    if (key == 'a'){visibleSketch -= 1;}
    ofLog() << " x:"<< offsetX << " y:"<< offsetY <<" z:"<< offsetZ ;
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){


}


//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}

