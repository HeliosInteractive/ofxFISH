#include "ofxFISH.h"


void ofxFISH::setup( string _jsonFolderPath ) 
{
	ofSetLogLevel( OF_LOG_VERBOSE ) ; 
	jsonFolderPath = _jsonFolderPath;
	jsonLoadTimer.setup( 10000 , "RFID__SCAN_TIMER" , 5 ) ;
	ofAddListener( jsonLoadTimer.TIMER_COMPLETE , this , &ofxFISH::jsonLoadTimerComplete ) ; 
	ofAddListener( sessionJson.JSON_DATA_READY , this , &ofxFISH::sessionDataReady ) ;
	ofAddListener( visitorJson.JSON_DATA_READY , this , &ofxFISH::visitorDataReady ) ;

	currentUser.firstName = "DEFAULT CURRENT" ; 
	previousUser.firstName = "DEFAULT PREVIOUS" ;

	sessionJson.bActive = true ; 
	bSessionActive = false ; 

	if ( ofFile::doesFileExist( "session.json" ) == false ) 
	{
		ofLogError( "session.json does NOT EXIST ! Copying from backup... " ) << endl ; 
		ofFile::copyFromTo(  jsonFolderPath + "session - backup.json" , jsonFolderPath +"session.json" , false , true ) ; 
	}
}


void ofxFISH::sessionDataReady( int &args )
{
	ofLogVerbose( " JSON DATA READY " ) << " about to copy... " << endl ; 
	
	//Copy the XML data so we can work with it without fear that it will be overwritten
	string jsonValue = " " ; 
	copyJson = sessionJson.json ; 
	cout << copyJson.toStyledString() << endl ; 

	cout << endl ;
	cout << "-------------" << endl ;
	cout << endl ; 
	cout << " SESSION " << copyJson[ "session_id" ].asString() << endl ; 
	cout << " TIMESTAMP " << copyJson[ "timestamp" ].asInt() << endl ; 
	
	beginSession( ) ; 
}


void ofxFISH::visitorDataReady ( int &args ) 
{
	ofLogVerbose ( "ofxFISH::visitorDataReady!" ) ; 
}

void ofxFISH::beginSession( ) 
{
	if ( bSessionActive == true ) 
	{
		ofLogError( " ofxFishBeginSession()  BUT session is ALREADY ACTIVE ! ending... " ) ; 
		endSession() ; 
	}

	bSessionActive = true ; 
}

void ofxFISH::endSession( ) 
{
	if ( bSessionActive == false ) 
	{
		ofLogError( "ofxFISH::endSession attempting to END SESSION when one is not active... ABORTING " )  ;
		return ; 
	}

	bSessionActive = false ;
	ofFile::removeFile( jsonFolderPath + "/session.json" ) ; 
}

void ofxFISH::update( ) 
{
	jsonLoadTimer.update( ) ; 
	sessionJson.update( ) ; 
	visitorJson.update( ) ; 
}

void ofxFISH::draw( ) 
{
	currentUser.draw( 15 , 35 ) ; 
	previousUser.draw( 500 , 35 ) ; 

	jsonLoadTimer.draw( 15 , 150 ) ; 
}

void ofxFISH::jsonLoadTimerComplete( int &args ) 
{
	if ( sessionJson.bActive == true ) 
	{
		ofLogVerbose( "SESSION JSON is active ! Loading now... " ) << endl ; 
		sessionJson.load( jsonFolderPath + "/session.json" ) ;
	}
	else if ( visitorJson.bActive ) 
	{
		ofLogVerbose( "VISITOR JSON is active ! Loading now... " ) << endl ; 
		visitorJson.load( jsonFolderPath + "/visitor.json" ) ; 
	}
}