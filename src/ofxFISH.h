#pragma once

#include "ofMain.h"
#include "ofxSimpleTimer.h"
#include "ofxFISH_User.h"
#include "ofxThreadedJSON.h"
#include "ofxJSONElement.h"

/* 

Credit to Surya Buchwald ( aka Momo The Monster @ http://mmmlabs.com/w/  )
for his contribution to the JSON HTTP POSTING 

*/

#include <Poco/Net/HTTPClientSession.h>
#include <Poco/Net/HTTPRequest.h>
#include <Poco/Net/HTTPResponse.h>
#include <Poco/StreamCopier.h>
#include <Poco/Path.h>
#include <Poco/URI.h>
#include <Poco/Exception.h>
#include <Poco/Net/HTTPSClientSession.h>

class ofxFISH
{
public : 
	ofxFISH() { } 
	~ofxFISH() { } 

	enum FISH_APP_FLOW
	{
		WAITING_FOR_SESSION = 0 , 
		WAITING_FOR_VISITOR = 1 , 
		VISITOR_INFO_RECIEVED = 2
	};

	void setup ( string _jsonFolderPath ,string _domain , string _station , string _authToken  ) ; 
	void update ( ) ; 
	void draw( ) ; 
	void exit( ) ; 

	void jsonLoadTimerComplete( int &args ) ; 
	void sessionDataReady ( int &args ) ;
	void visitorDataReady ( int &args ) ;

	//Easy start / end points
	void beginSession( ) ;
	void endSession( ) ; 
	void resetSession( ) ; 

	//Global Variables
	string domain ; 
	string stationId ; 
	string authToken ;
	string jsonFolderPath ; 
	//Increment each user for the going through
	int idIncrement ; 
	
	//Probably overkill but the JSON is loaded in a thread and dispatches an event when ready.
	ofxThreadedJSON sessionJson ; 
	ofxThreadedJSON visitorJson ; 

	ofxSimpleTimer jsonLoadTimer ;
	ofxJSONElement copyJson ;

	//Some Events to trigger to our the app
	ofEvent< string > NEW_USER_DATA_COLLECTED ; 
	ofEvent< string > NEW_USER_DATA_INCOMPLETE ;

	//Basic User Management
	ofxFISH_User user ; 
	ofxFISH_User previousUser ; 

	//Know where the app is 
	FISH_APP_FLOW appState ; 

	//Contacting the FISH server...
	ofxJSONElement request ; 
	ofxJSONElement response ; 
	string securePostJson( string url , string json ) ; 

	//Need to try connecting a few times. Just in case :)
	int connectionAttempts ; 
	int connectionMaxAttempts ; 
	ofxSimpleTimer requestTimer ; 
	void requestTimerComplete ( int & args ) ; 
	
	bool validateEmail( string email_address ) ; 
	


};