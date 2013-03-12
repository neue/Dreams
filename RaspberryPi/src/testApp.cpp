#include "testApp.h"

#define WIDTH 720
#define HEIGHT 480

//--------------------------------------------------------------
void testApp::setup(){
	ofBackground(0,0,0);
    ofHideCursor();
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    screenFbo.allocate(WIDTH, HEIGHT,GL_RGBA);
    // looks in /bin/data
    //chrom_abb.load("shaders/chrom_abb.vert", "shaders/chrom_abb.frag");

	//-----------
	//the string is printed at the top of the app
	//to give the user some feedback
	message = "loading sketch.xml";

	//we load our settings file
	//if it doesn't exist we can still make one
	//by hitting the 's' key
	if( XML.loadFile("Predator.xml") )  {loadSketch(XML,-580,-340,-40);}
	if( XML.loadFile("x47b.xml") )   {loadSketch(XML,-580,-320,60);}
	if( XML.loadFile("cray.xml") )      {loadSketch(XML,-880,-560,540);}
	if( XML.loadFile("BigDog.xml") ) {loadSketch(XML,-540,-280,-80);}
    if( XML.loadFile("Kennan.xml") )  {loadSketch(XML,-240,-500,260);}


	//we initalize some of our variables
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
    
    offsetX = 0.0;
    offsetY = 0.0;
    offsetZ = 0.0;
    angle   = 0.0;
    rotateX = 0.0;
    rotateY = 0.0;
    rotateZ = 0.0;
    gridDistort = 0.0;
    

	TTF.loadFont("mono.ttf", 7);
    
    visibleSketch = 0;
    ofLog() << "Screen Width:" << ofGetScreenWidth() << "Screen Height:" << ofGetScreenHeight();
    
    offsetZ = 500;
    corruptionAmplitude = 0.5;
    corruptionTimer = 200.0;
    corruptionCount = 0;

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
                //ofLog() << " Points:" << numPtTags;
                
                int totalToRead = MIN(numPtTags, NUM_PTS);
                
                for(int i2 = 0; i2 < numPtTags; i2++){
                    int x = sketchXML.getValue("pt:x", 0, i2);
                    int y = sketchXML.getValue("pt:y", 0, i2);
                    int z = sketchXML.getValue("pt:z", 0, i2);
                    newRhonLine->rhonPoints.push_back(rhonPoint(x, y, z));
                    dragPts[i2].set(x, y,z);
                    pointCount++;
                }
            }
            sketchXML.popTag();
            newRhonSketch->rhonLines.push_back(newRhonLine);
        }
        //ofLog() << "Lines:" << newRhonSketch->rhonLines.size();
	}
    newRhonSketch->pos.set(anchorX,anchorY,anchorZ);
    rhonSketches.push_back(newRhonSketch);
}

//--------------------------------------------------------------
void testApp::update(){
    //timeCode += 0.01;
    timeCode = ofGetLastFrameTime();
    offsetZ +=  timeCode * 100;
    if(offsetZ > 2000){
        offsetZ = -2500;
        visibleSketch = (visibleSketch+1)%rhonSketches.size();
        ofLog() << "Sketch:" << visibleSketch;
    }
    //ofLog() << "Z:" << offsetZ;
    
    angle   +=  timeCode * 70;
    rotateX +=  timeCode * 100;
    rotateY +=  timeCode * -80;
    rotateZ +=  timeCode * 89;
    
    corruptionTimer -= 0.1;
    if(corruptionTimer < 0){
        corrupt(visibleSketch);
        corruptionTimer = ofRandom(30.0,200.0);
    }
    
}

//--------------------------------------------------------------
void testApp::draw(){
    screenFbo.begin();
	ofBackground(0,0,0);

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
        glRotatef(angle, rotateX, rotateY, rotateZ);
    

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
    drawGrid(140, 93, -430);
    
    ofFill();
    cam.end();

	//ofSetColor(130, 93, 130);
    //TTF.drawString("FPS: "+ofToString(ofGetFrameRate()), 170, 50);
    screenFbo.end();
    
    if (ofRandom(1,4000) < 10) {
        aberrationX = ofRandom(2,10);
    } else {
        aberrationX = 1;
    }    
    
    ofEnableAlphaBlending();
    ofEnableBlendMode(OF_BLENDMODE_ADD);
    ofSetColor(255, 0, 0);
    screenFbo.draw(aberrationX*-1, aberrationY*-1);
    ofSetColor(0, 255, 0);
    screenFbo.draw(0, 0);
    ofSetColor(0, 0, 255);
    screenFbo.draw(aberrationX, aberrationY);


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
void testApp::drawGrid(float x, float y, float z){
    gridBrightness = 0;
    for (int g1 = 0; g1 < MAX_GRID; g1++) {
        gridBrightness += 50;

        for (int g2 = 0; g2< MAX_GRID; g2++) {

            for (int g3 = 0; g3 < MAX_GRID; g3++) {

                ofSetColor(gridBrightness,gridBrightness,gridBrightness);
                drawCrosshair(
                              x+g3*GRID_SPACINGX-(GRID_SPACINGX*MAX_GRID/2)+ofRandom(gridDistort*-1,gridDistort),
                              y+g2*GRID_SPACINGY-(GRID_SPACINGY*MAX_GRID/2)+ofRandom(gridDistort*-1,gridDistort),
                              z+g1*GRID_SPACINGX-(GRID_SPACINGX*MAX_GRID/2)+ofRandom(gridDistort*-1,gridDistort),
                              15);
            }
        }
        
    }
    if(gridDistort > 0.0) {
        gridDistort -= 0.2;
    } else {
        gridDistort = 0.0;
    }
}

//--------------------------------------------------------------

float testApp::measure(float x1,float y1,float x2,float y2){
    float dx = x1 - x2;
    float dy = y1 - y2;
    return (((sqrt( dx * dx + dy * dy ))-0)/(FOG_DIST-0) * (0+255) - 255)* -1;
}

//--------------------------------------------------------------

void testApp::corrupt(int sketchToCorrupt){

    corruptionCount++;
    ofLog() << "Corruptions:" << corruptionCount;
    
    float liquidness = 2;
	float speedDampen = 5;
    gridDistort = 10;
    for(int i2 = 0; i2 < rhonSketches[sketchToCorrupt]->rhonLines.size();i2++){
        if (ofRandom(0,500) < 2) {
            corruptionBurnoff = corruptionAmplitude;
            ofLog() << "corruptionBurnoff:"<< corruptionBurnoff;

            for(int i3=0;i3 < rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints.size();i3++){

                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x += ofSignedNoise(rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x/liquidness, rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y/liquidness,rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z/liquidness, ofGetElapsedTimef()/speedDampen)*corruptionBurnoff;
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y += ofSignedNoise(rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z/liquidness, rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x/liquidness,rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y/liquidness, ofGetElapsedTimef()/speedDampen)*corruptionBurnoff;
                    rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z += ofSignedNoise(rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.y/liquidness, rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.z/liquidness,rhonSketches[sketchToCorrupt]->rhonLines[i2]->rhonPoints[i3].pnt.x/liquidness, ofGetElapsedTimef()/speedDampen)*corruptionBurnoff;
                if(corruptionBurnoff > 0.01){
                    corruptionBurnoff = corruptionBurnoff * 0.95;
                } else if(corruptionBurnoff < 0.01) {
                    corruptionBurnoff = 0.0;
                }
                
            }
            corruptionAmplitude += 1.0;

        }
    }

    
//	for(int i = 0; i < verts.size(); i++){
//		verts[i].x += 
//		verts[i].y += ofSignedNoise(verts[i].z/liquidness, verts[i].x/liquidness,verts[i].y/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
//		verts[i].z += ofSignedNoise(verts[i].y/liquidness, verts[i].z/liquidness,verts[i].x/liquidness, ofGetElapsedTimef()/speedDampen)*amplitude;
//	}

    
    
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

