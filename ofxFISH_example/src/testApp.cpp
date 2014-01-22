#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
	
	fish.setup( "C:/fish/" ) ;
	ofBackground ( 255 ) ; 
	lastUserChange = -1 ; 

	ofAddListener( fish.NEW_USER_DETECTED , this , &testApp::newFishUserDetected ) ; 
}

//--------------------------------------------------------------
void testApp::update(){
	fish.update( ) ; 

	if ( lastUserChange > 0.0f && ((  ofGetElapsedTimef() - lastUserChange ) > 3.0f ) )
	{ 
		ofBackground ( 255 ) ; 
		lastUserChange = -1 ; 
	}
}



//--------------------------------------------------------------
void testApp::draw(){
	fish.draw( ) ; 
}

void testApp::newFishUserDetected ( string &args ) 
{
	ofBackground( 255 , 0 , 0 ) ; 
	lastUserChange = ofGetElapsedTimef() ;
}

void testApp::exit( ) 
{
	fish.exit() ; 
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
	switch ( key ) 
	{
		case 'e':
		case 'E':
			fish.endSession() ; 
			break ; 

		case 'b':
		case 'B':
			fish.beginSession() ; 
			break ; 
	}
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
