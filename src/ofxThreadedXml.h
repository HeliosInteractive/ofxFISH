#include "ofxXmlSettings.h"
#include "ofMain.h"

class ofxThreadedXml : public ofThread
{
public : 
	ofxThreadedXml( ) { } 
	~ofxThreadedXml( ) { } 

	bool bSendEvent ; 

	ofEvent< int > XML_DATA_READY ; 
	ofxXmlSettings xml ; 
	string xmlPath ; 

	void load ( string _xmlPath ) 
	{
		xmlPath = _xmlPath ; 
		startThread( true , true ) ; 
	}

	void threadedFunction ( ) 
	{
		while ( isThreadRunning() != 0 ) 
		{
			if ( lock() ) 
			{
				bool bResult = xml.load( xmlPath ) ; 
				ofLogVerbose( " result from loading : '" ) << xmlPath << "' is " << bResult << endl ;
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
			ofNotifyEvent( XML_DATA_READY , args ) ; 
			bSendEvent = false ;
		}
	}
};