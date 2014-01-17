#include "ofxFISH.h"


void ofxFISH::setup( string _xmlPath ) 
{
	ofSetLogLevel( OF_LOG_VERBOSE ) ; 
	xmlPath = _xmlPath;
	xmlLoadtimer.setup( 10000 , "RFID_TIMER" , 5 ) ;
	ofAddListener( xmlLoadtimer.TIMER_COMPLETE , this , &ofxFISH::xmlLoadTimerComplete ) ; 
	ofAddListener( session_xml.XML_DATA_READY , this , &ofxFISH::xmlDataReady ) ; 

	currentUser.firstName = "DEFAULT CURRENT" ; 
	previousUser.firstName = "DEFAULT PREVIOUS" ; 
}

void ofxFISH::xmlDataReady( int &args )
{
	ofLogVerbose( " XML DATA READY " ) << " about to copy... " << endl ; 
	
	//Copy the XML data so we can work with it without fear that it will be overwritten
	string xmlValue = " " ; 
	session_xml.xml.copyXmlToString( xmlValue ) ;
	copyXml.loadFromBuffer( xmlValue ); 

	//Push into the latest user...
	int numUsers = copyXml.getNumTags ( "visitor" ) ; 
	copyXml.pushTag( "visitor" , numUsers -1 ) ; 
		ofxFISH_User tempUser ; 
		tempUser.populateFromXml( &copyXml ) ; 

		if ( tempUser.rfid.compare( currentUser.rfid ) != 0 ) 
		{
			previousUser = currentUser ; 
			currentUser.populateFromXml( &copyXml ) ; 
			ofNotifyEvent( NEW_USER_DETECTED , currentUser.rfid ) ; 
		}
		else
		{
			//Same ID as current user... ignore for now
		}
	//reset the XML
	copyXml.popTag( ) ; 
}

void ofxFISH::update( ) 
{
	xmlLoadtimer.update( ) ; 
	session_xml.update( ); 
}

void ofxFISH::draw( ) 
{
	currentUser.draw( 15 , 15 ) ; 
	previousUser.draw( 500 , 15 ) ; 
}

void ofxFISH::xmlLoadTimerComplete( int &args ) 
{
	session_xml.load( xmlPath ) ; 

}