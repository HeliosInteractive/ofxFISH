#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "ofxFISH_User.h"
#include "ofxThreadedJSON.h"
#include "ofxJSONElement.h"

class ofxFISH
{
public : 
	ofxFISH() { } 
	~ofxFISH() { } 

	void setup ( string _jsonFolderPath ) ; 
	void update ( ) ; 
	void draw( ) ; 

	void jsonLoadTimerComplete( int &args ) ; 
	void sessionDataReady ( int &args ) ;
	void visitorDataReady ( int &args ) ;

	bool bSessionActive ; 
	void beginSession( ) ;
	void endSession( ) ; 

	string jsonFolderPath ; 
	ofxThreadedJSON sessionJson ; 
	ofxThreadedJSON visitorJson ; 

	ofxSimpleTimer jsonLoadTimer ;
	ofxJSONElement copyJson ;

	//Some Events to trigger to our the app
	ofEvent< string > NEW_USER_DETECTED ; 
	ofEvent< string > USER_CANCELLED ; 

	//Basic User Management
	ofxFISH_User currentUser ; 
	ofxFISH_User previousUser ; 


};