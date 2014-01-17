#include "ofxFISH_User.h"

void ofxFISH_User::reset( ) 
{
	rfid = "no rfid" ;
	email = "no email" ;
	bFinished = false ;
}

void ofxFISH_User::populateFromXml( ofxXmlSettings * xml ) 
{
	rfid = xml->getValue( "id" , "no rfid" ) ; 
	email = xml->getValue( "email" , " no email" ) ; 
	firstName = xml->getValue( "firstName" , "firstName" ) ; 
	lastName = xml->getValue( "lastName" , "lastName" ); 
	
}

void ofxFISH_User::draw ( float x , float y ) 
{
	stringstream ss ; 
	ss << firstName << " " << lastName << endl << email << endl << rfid << endl ; 
	ofDrawBitmapStringHighlight( ss.str() , x , y ) ; 
}

bool ofxFISH_User::IsEmailValid(string email_address ) 
{
	int atIndex = email_address.find( "@" ) ; 
	int dotIndex = email_address.find( "." ) ; 

	if ( email_address.compare( "no email" ) != 0
		&& atIndex > 0 
		&& dotIndex > 0 )  
	{
		ofLogVerbose( email_address ) << " is valid" << endl; 
		return true ;
	}
	else
	{
		ofLogError( email_address ) << "is not VALID" << endl ; 
		return false ;
	}

}
