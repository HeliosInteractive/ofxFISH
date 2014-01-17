#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxSimpleTimer.h"
#include "ofxFISH_User.h"
#include "ofxThreadedXml.h"

class ofxFISH
{
public : 
	ofxFISH() { } 
	~ofxFISH() { } 

	void setup ( string _xmlPath ) ; 
	void update ( ) ; 
	void xmlLoadTimerComplete( int &args ) ; 
	void xmlDataReady ( int &args ) ; 
	void draw( ) ; 

	string xmlPath ; 
	ofxThreadedXml session_xml ; 
	ofxXmlSettings copyXml ;
	ofxSimpleTimer xmlLoadtimer ; 

	//Some Events to trigger to our the app
	ofEvent< string > NEW_USER_DETECTED ; 
	ofEvent< string > USER_CANCELLED ; 

	//Basic User Management
	ofxFISH_User currentUser ; 
	ofxFISH_User previousUser ; 


};