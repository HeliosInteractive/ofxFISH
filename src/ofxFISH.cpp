#include "ofxFISH.h"



void ofxFISH::setup( string _jsonFolderPath , string _domain , string _stationId , string _authToken ) 
{
	jsonFolderPath = _jsonFolderPath;
	jsonLoadTimer.setup( 500 , "RFID__SCAN_TIMER" , 5 ) ;
	ofAddListener( jsonLoadTimer.TIMER_COMPLETE , this , &ofxFISH::jsonLoadTimerComplete ) ; 
	ofAddListener( sessionJson.JSON_DATA_READY , this , &ofxFISH::sessionDataReady ) ;
	ofAddListener( visitorJson.JSON_DATA_READY , this , &ofxFISH::visitorDataReady ) ;

	domain = _domain ; 
	stationId = _stationId ; 
	authToken = _authToken ; 

	ofLogNotice( "creating new instance of ofxFISH " ) << endl << "domain - " << domain << endl << "station - " << stationId << endl << "authToken - " << authToken << endl ; 
	user.first_name = "DEFAULT CURRENT" ; 
	previousUser.first_name = "DEFAULT PREVIOUS" ; 

	sessionJson.bActive = true ; 

	if ( ofFile::doesFileExist( "session.json" ) == false ) 
	{
		ofLogError( "session.json does NOT EXIST ! Copying from backup... " ) << endl ; 
		ofFile::copyFromTo(  jsonFolderPath + "session - backup.json" , jsonFolderPath +"session.json" , false , true ) ; 
	}

	idIncrement= 0 ;

	appState = WAITING_FOR_SESSION ; 

	connectionAttempts = 0 ; 
	connectionMaxAttempts = 3 ; ; 

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
	
	request = ofxJSONElement() ;; 
	request.clear() ;

	request["auth_id"] = authToken + "3" ; //"1QAarRlaZK11jlcTDbQ5ULc9M";
	request["id"] = tag_id ; 
	request["id_type"] = "rfid";

	string url = domain + "module/regdata/rdq";  
	ofLogNotice( "SENDING OUT JSON ---- " ) << endl << request.getRawString( true ) << endl ; 
	response = securePostJson( url ,request.getRawString( false )) ; // _json ) ; //copyJson.getRawString(false) ) ; //  _json ) ; //copyJson.getRawString(false) );
	ofLogNotice("response : ") << "'" << response.getRawString( true ) << "'" << endl ;

	bool bSuccess = false ; 
	if ( response != "" ) 
	{
		bool bResult = user.populateFromJSON( response ) ; 
		if ( bResult == true  )
			bSuccess = true ; 		
	}

	if ( bSuccess ) 
	{
		ofLogNotice( " USER IS NOW " ) << user.toString() ; 
	}
	else
	{
		ofLogError( "Could not parse user data ! " ) ;
	}
}

string ofxFISH::securePostJson( string reference , string json )
{
	try {
		Poco::URI uri(reference);

		const Poco::Net::Context::Ptr context( new Poco::Net::Context( Poco::Net::Context::CLIENT_USE, "", "", "", Poco::Net::Context::VERIFY_NONE) );
		Poco::Net::HTTPSClientSession session(uri.getHost(), uri.getPort(), context);
		string path(uri.getPathAndQuery());

		Poco::Net::HTTPRequest req(Poco::Net::HTTPRequest::HTTP_POST, path, Poco::Net::HTTPMessage::HTTP_1_1);
		req.setKeepAlive(false);
		std::string reqBody(json);
		req.setContentType("application/json");
		req.setContentLength( reqBody.length() );
		session.sendRequest(req) << reqBody;

		Poco::Net::HTTPResponse res;
		istream& rs = session.receiveResponse(res);
		string result;
		Poco::StreamCopier::copyToString(rs, result);
		return result;
	}
	catch (Poco::Exception& exc) {
		ofLogError("ERROR", exc.displayText());
			return "";
	}  
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
	}

	idIncrement++ ; 

	ofxJSONElement json ; 
	json.open( "session.json" ) ;

	
	stringstream ss ; 
	ss << "{" << '"' << "session_id" << '"' << ": " << '"' << "x-y-1-" <<  idIncrement << '"' << "}" ; 

	cout << "JSON FILE IS : " << endl << ss.str() << endl ; 

	string jsonOutput = ss.str() ; 
	ofBuffer jsonBuffer ; 
	jsonBuffer.set( ss.str() ) ; 
	
	
	string fullLength = jsonFolderPath + "session.json" ; 
	bool bFolderSuccess = ofBufferToFile(  jsonFolderPath + "/session.json" , jsonBuffer , false ) ; 
	bool bLocalSuccess = ofBufferToFile(  "session.json" , jsonBuffer , false ) ; 

	sessionJson.bActive = false ; 
	visitorJson.bActive = true ; 
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

void ofxFISH::resetSession ( ) 
{
	
	//MATCH THIS
	//endSession( ) ; 

	/*
	switch ( appState ) 
	{
	case WAITING_FOR_SESSION : 
		ofFile::removeFile( jsonFolderPath + "session.json" ) ; 
		break ; 

	case WAITING_FOR_VISITOR : 
		stringstream ss ; 
			ss << "{" << '"' << "session_id" << '"' << ": " << '"' << "x-y-1-" <<  idIncrement << '"' << "}" ; 

			cout << "JSON FILE IS : " << endl << ss.str() << endl ; 

			string jsonOutput = ss.str() ; 
			ofBuffer jsonBuffer ; 
			jsonBuffer.set( ss.str() ) ; 
			ofFile::removeFile( jsonFolderPath + "session.json" ) ; 
			string fullLength = jsonFolderPath + "session.json" ; 
			bool bFolderSuccess = ofBufferToFile(  jsonFolderPath + "/session.json" , jsonBuffer , false ) ; 
			bool bLocalSuccess = ofBufferToFile(  "session.json" , jsonBuffer , false ) ; 
			break ; 

	case VISITOR_INFO_RECIEVED : 
		break ; 
	}*/

	ofFile::removeFile( jsonFolderPath + "/visitor.json" ) ; 
	ofFile::removeFile( jsonFolderPath + "/session.json" ) ; 
}

void ofxFISH::exit( ) 
{
	resetSession( ) ; 
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