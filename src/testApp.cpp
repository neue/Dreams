#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);

	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading sketch.xml";

	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile("sketch2.xml") )  {loadSketch(XML,-1040,-600,-120);}
	if( XML.loadFile("sketch.xml") )   {loadSketch(XML,-1040,-600,-120);}
	if( XML.loadFile("cat.xml") )      {loadSketch(XML,-1040,-600,-120);}
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
    offsetZ = 0;
}

//--------------------------------------------------------------
void testApp::update(){
    timeCode += 0.01;
    offsetZ += 0.4;
    if(offsetZ > 2000){
        offsetZ = -2500;
        corrupt(visibleSketch);
        visibleSketch = (visibleSketch+1)%rhonSketches.size();
        ofLog() << "Sketch:" << visibleSketch;
    }
    //ofLog() << "Z:" << offsetZ;
}

//--------------------------------------------------------------
void testApp::draw(){

	cam.begin();

	//---------
	//Lets draw the stroke as a continous line

    sketchBrightness = measure(-200,0,offsetZ,offsetY);
    
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

    glTranslatef(offsetX, offsetY, offsetZ);

        glPushMatrix();
        glRotatef(timeCode*5, timeCode* -3, timeCode*8, timeCode*7);

        for(int i = 0; i < rhonSketches.size();i++){
        if(i== visibleSketch){
            for(int i2 = 0; i2 < rhonSketches[i]->rhonLines.size();i2++){
                ofBeginShape();
                    for(int i3=0;i3 < rhonSketches[i]->rhonLines[i2]->rhonPoints.size();i3++){
                    ofVertex(rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.x+rhonSketches[i]->pos.x,
                             rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.y+rhonSketches[i]->pos.y,
                             rhonSketches[i]->rhonLines[i2]->rhonPoints[i3].pnt.z+rhonSketches[i]->pos.z
                             );
                    }
                ofEndShape(false);
            }
            
        }
        }
        ofEndShape(false);
        
        glPopMatrix();
    glPopMatrix();
    
    //drawZeroPoint();
    
    drawCrosshair(-300,-300,0,30);
    drawCrosshair(-300,-300,-500,30);
    drawCrosshair(300,-300,-500,30);
    drawCrosshair(300,-300,0,30);
    
    drawCrosshair(-300,300,0,30);
    drawCrosshair(-300,300,-500,30);
    drawCrosshair(300,300,-500,30);
    drawCrosshair(300,300,0,30);

    
    ofFill();
    cam.end();

	ofSetColor(0, 0, 255, 200);
    TTF.drawString("Distance: "+ofToString(sketchBrightness), 170, 12);
    

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

//--------------------------------------------------------------
void testApp::drawCrosshair(float x, float y, float z,float size){
    ofSetColor(255,255,255);
    ofBeginShape();
        ofVertex((size*-1)+x,0+y,0+z);
        ofVertex(size+x,0+y,0+z);
    ofEndShape(false);
    ofBeginShape();
        ofVertex(0+x,size+y,0+z);
        ofVertex(0+x,(size*-1)+y,0+z);
    ofEndShape(false);
    ofBeginShape();
        ofVertex(0+x,0+y,(size*-1)+z);
        ofVertex(0+x,0+y,size+z);
    ofEndShape(false);
}

//--------------------------------------------------------------

float testApp::measure(float x1,float y1,float x2,float y2){
    float dx = x1 - x2;
    float dy = y1 - y2;
    
    
    
    return (((sqrt( dx * dx + dy * dy ))-0)/(FOG_DIST-0) * (0+255) - 255)* -1;
}

//--------------------------------------------------------------

void testApp::corrupt(int sketchToCorrupt){
    ofLog() << "Corrupting";
        for(int i2 = 0; i2 < rhonSketches[sketchToCorrupt]->rhonLines.size();i2++){
            for(int i3=0;i3 < rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints.size();i3++){
                if (ofRandom(0,300) < 2) {
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x += ofRandom(-6,6);
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y += ofRandom(-6,6);
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z += ofRandom(-6,6);
                }
                if (ofRandom(0,6000) < 2) {
                    ofLog() << "BIG ONE";

                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x += ofRandom(-60,60);
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y += ofRandom(-60,60);
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z += ofRandom(-60,60);
                }
            }
        }
            

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
    if (key == 'c'){corrupt(visibleSketch);}
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

