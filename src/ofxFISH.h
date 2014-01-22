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
	void exit( ) ; 

	void jsonLoadTimerComplete( int &args ) ; 
	void sessionDataReady ( int &args ) ;
	void visitorDataReady ( int &args ) ;

	void beginSession( ) ;
	void endSession( ) ; 

	int idIncrement ; 
	string jsonFolderPath ; 
	ofxThreadedJSON sessionJson ; 
	ofxThreadedJSON visitorJson ; 

	ofxSimpleTimer jsonLoadTimer ;
	ofxJSONElement copyJson ;

	//Some Events to trigger to our the app
	ofEvent< string > NEW_USER_DETECTED ; 
	ofEvent< string > USER_CANCELLED ; 

	//Basic User Management
	ofxFISH_User user ; 
	ofxFISH_User previousUser ; 

		
	enum FISH_APP_FLOW
	{
		WAITING_FOR_SESSION = 0 , 
		WAITING_FOR_VISITOR = 1 , 
		VISITOR_INFO_RECIEVED = 2
	};

	FISH_APP_FLOW appState ; 

	void urlResponse ( ofHttpResponse &response ) ; 

};