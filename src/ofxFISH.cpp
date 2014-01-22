#include "ofxFISH.h"


void ofxFISH::setup( string _jsonFolderPath ) 
{
	jsonFolderPath = _jsonFolderPath;
	jsonLoadTimer.setup( 10000 , "RFID__SCAN_TIMER" , 5 ) ;
	ofAddListener( jsonLoadTimer.TIMER_COMPLETE , this , &ofxFISH::jsonLoadTimerComplete ) ; 
	ofAddListener( sessionJson.JSON_DATA_READY , this , &ofxFISH::sessionDataReady ) ;
	ofAddListener( visitorJson.JSON_DATA_READY , this , &ofxFISH::visitorDataReady ) ;

	user.firstName = "DEFAULT CURRENT" ; 
	previousUser.firstName = "DEFAULT PREVIOUS" ; 

	sessionJson.bActive = true ; 

	if ( ofFile::doesFileExist( "session.json" ) == false ) 
	{
		ofLogError( "session.json does NOT EXIST ! Copying from backup... " ) << endl ; 
		ofFile::copyFromTo(  jsonFolderPath + "session - backup.json" , jsonFolderPath +"session.json" , false , true ) ; 
	}

	idIncrement= 0 ;

	appState = WAITING_FOR_SESSION ; 
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
	//cout << " TIMESTAMP " << copyJson[ "timestamp" ].asInt() << endl ; 
	
//	beginSession( ) ; 
}


void ofxFISH::visitorDataReady ( int &args ) 
{
	ofLogVerbose ( "ofxFISH::visitorDataReady!" ) ; 
	jsonLoadTimer.stop() ; 

	previousUser = user ; 

	copyJson = visitorJson.json ; 
	string tag_id = copyJson["tag_id"].asString() ; 
	ofLogNotice ( "Visitor Data loaded with a tag_id of : " ) << tag_id << endl ;
	user.tag_id = tag_id ; 

	appState = VISITOR_INFO_RECIEVED ;
}

void ofxFISH::beginSession( ) 
{
	if ( appState != WAITING_FOR_SESSION ) 
	{
		switch ( appState ) 
		{
			case WAITING_FOR_VISITOR :
				ofLogError( "Session already began, waiting on Visitor data... " ) ; 
				break ; 

			case VISITOR_INFO_RECIEVED :
				ofLogError( "Session already began. have visitor data... " ) ; 
				break ; 
		}

		return ; 
		//ofLogError( " ofxFishBeginSession()  BUT session is ALREADY ACTIVE ! ending... " ) ; 
		//endSession() ; 
	}

	idIncrement++ ; 

	ofxJSONElement json ; 
	json.open( "session.json" ) ;
	
	//json["session_id"] = "x-y-1-" + ofToString( idIncrement ) ; 
	//json.save(  jsonFolderPath + "session.json" , false ) ; 

	
	stringstream ss ; 
	ss << "{" << '"' << "session_id" << '"' << ": " << '"' << "x-y-1-" <<  idIncrement << '"' << "}" ; 

	cout << "JSON FILE IS : " << endl << ss.str() << endl ; 

	string jsonOutput = ss.str() ; 
	ofBuffer jsonBuffer ; 
	jsonBuffer.set( ss.str() ) ; 
	
	
	string fullLength = jsonFolderPath + "session.json" ; 
//	cout << "fullPath " << fullLength << endl ;
	bool bFolderSuccess = ofBufferToFile(  jsonFolderPath + "/session.json" , jsonBuffer , false ) ; 
	bool bLocalSuccess = ofBufferToFile(  "session.json" , jsonBuffer , false ) ; 
	
//	cout << "bFolderSuccess : " << bFolderSuccess << endl ; 
//	cout << "bLocalSuccess : " << bLocalSuccess << endl;	
	
	sessionJson.bActive = false ; 
	visitorJson.bActive = true ; 
	jsonLoadTimer.delayMillis = 500 ; 

	appState = WAITING_FOR_VISITOR ; 
	
	jsonLoadTimer.start( true , true ) ; 
}

void ofxFISH::endSession( ) 
{
	if ( appState != VISITOR_INFO_RECIEVED ) 
	{
		ofLogError( "ofxFISH::endSession attempting to END SESSION when one is not active... ABORTING " )  ;
		return ; 
	} 

	bool bDeleteResult = ofFile::removeFile( jsonFolderPath + "visitor.json" ) ; 

	stringstream ss ; 
	ss << "{" << '"' << "session_id" << '"' << ": " << '"' << "x-y-1-" <<  idIncrement << '"' << "}" ; 

	cout << "JSON FILE IS : " << ss.str() << endl ; 

	string jsonOutput = ss.str() ; 
	ofBuffer jsonBuffer ; 
	jsonBuffer.set( ss ) ; 
	
	
	bool bFolderSuccess = ofBufferToFile(  jsonFolderPath + "session.json" , jsonBuffer , true ) ;
	cout << "bFolderSuccess : " << bFolderSuccess << endl ; 
	
	appState = WAITING_FOR_SESSION ; 
	//cout << "bLocalSuccess : " << bLocalSuccess << endl;	

}

void ofxFISH::exit( ) 
{
	endSession( ) ; 
}

void ofxFISH::update( ) 
{
	jsonLoadTimer.update( ) ; 
	sessionJson.update( ) ; 
	visitorJson.update( ) ; 
}

void ofxFISH::draw( ) 
{
	stringstream ss ; 
	ss << "FISH APP is currently : " ; 
	switch( appState ) 
	{
		case WAITING_FOR_SESSION : 
			ss << " waiting for session.json " << endl ; 
			break; 
		case WAITING_FOR_VISITOR : 
			ss << " waiting for visitor.json" << endl ; 
			break ; 
		case VISITOR_INFO_RECIEVED : 
			ss << " has visitor info" << endl ; 
			break; 
	}
	ss << endl ; 
	ss << user.toString() << endl ; 
	ss << previousUser.toString() << endl ; 
	ofDrawBitmapStringHighlight( ss.str() , 15 , 40 ) ; 

	jsonLoadTimer.draw( 15 , 15 ) ; 
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