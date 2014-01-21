#include "ofxJSONElement.h"
#include "ofMain.h"

class ofxThreadedJSON : public ofThread
{
public : 
	ofxThreadedJSON( ) 
	{
		bActive = false ; 
		bSendEvent = false ; 
	} 
	~ofxThreadedJSON( ) { } 

	bool bSendEvent ; 
	bool bActive ;
	ofEvent< int > JSON_DATA_READY ; 
	ofxJSONElement json ; 
	string jsonPath ; 

	void load ( string _jsonPath ) 
	{
		jsonPath = _jsonPath ; 
		startThread( true , true ) ; 
	}

	void threadedFunction ( ) 
	{
		while ( isThreadRunning() != 0 ) 
		{
			if ( lock() ) 
			{
				bool bResult = json.open( jsonPath ) ; 
				ofLogVerbose( " result from loading : '" ) << jsonPath << "' is " << bResult << endl ;
				if ( bResult == true ) 
					bSendEvent = true ;

				unlock( ) ; 
				stopThread( ) ; 
			}
		}
	}

	void update ( )
	{
		if ( bSendEvent == true ) 
		{
			int args = 5 ; 
			ofNotifyEvent( JSON_DATA_READY , args ) ; 
			bSendEvent = false ;
		}
	}
};