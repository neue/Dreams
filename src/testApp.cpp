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
	if( XML.loadFile("sketch2.xml") ){
		message = "sketch.xml loaded!";
	}else{
		message = "unable to load sketch.xml check data/ folder";
	}


	//we initalize some of our variables
	lastTagNumber	= 0;
	pointCount		= 0;
	lineCount		= 0;
    
     offsetX = 0.0;
     offsetY = 0.0;
     offsetZ = 0.0;



	//-------
	//this is a more advanced use of ofXMLSettings
	//we are going to be reading multiple tags with the same name
    rhonSketch * newRhonSketch = new rhonSketch();

	//lets see how many <STROKE> </STROKE> tags there are in the xml file
	int numDragTags = XML.getNumTags("line:pt");
	int numLines = XML.getNumTags("line");

    
    

	//if there is at least one <STROKE> tag we can read the list of points
	//and then try and draw it as a line on the screen
	if(numLines > 0){

		//we push into the last STROKE tag
		//this temporarirly treats the tag as
		//the document root.
        
        for(int i = 0;i < numLines;i++){
            rhonLine * newRhonLine = new rhonLine();

        
            XML.pushTag("line", i);

                //we see how many points we have stored in <PT> tags
                int numPtTags = XML.getNumTags("pt");

                if(numPtTags > 0){
                    ofLog() << " Numpointtags:" << numPtTags;

                    int totalToRead = MIN(numPtTags, NUM_PTS);

                    for(int i2 = 0; i2 < numPtTags; i2++){
                        int x = XML.getValue("pt:x", 0, i2);
                        int y = XML.getValue("pt:y", 0, i2);
                        int z = XML.getValue("pt:z", 0, i2);
                        newRhonLine->rhonPoints.push_back(rhonPoint(x, y, z*-1));
                        dragPts[i2].set(x, y,z);
                        pointCount++;
                    }
                }
            //this pops us out of the STROKE tag
            //sets the root back to the xml document
            XML.popTag();
            newRhonSketch->rhonLines.push_back(newRhonLine);

        }
        ofLog() << "Lines:" << newRhonSketch->rhonLines.size();

	}
    newRhonSketch->pos.set(-1040,-600,-120);
    rhonSketches.push_back(newRhonSketch);

	//load a monospaced font
	//which we will use to show part of the xml structure
	TTF.loadFont("mono.ttf", 7);
}

//--------------------------------------------------------------
void testApp::loadSketch(ofxXmlSettings sketchXML){
    timeCode += 0.01;
    
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
	ofSetColor(0,0,0);
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
	ofEndShape(false);
    
    glPopMatrix();

    
    ofFill();
    cam.end();

	ofSetColor(0, 0, 0, 200);
    TTF.drawString("Points: "+ofToString(pointCount), 170, 12);


}

//--------------------------------------------------------------
void testApp::keyPressed  (int key){
    if (key == 't'){offsetX += 20;}
    if (key == 'y'){offsetX -= 20;}
    if (key == 'g'){offsetY += 20;}
    if (key == 'h'){offsetY -= 20;}
    if (key == 'b'){offsetZ += 20;}
    if (key == 'n'){offsetZ -= 20;}
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

