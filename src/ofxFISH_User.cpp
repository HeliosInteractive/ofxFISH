#include "ofxFISH_User.h"

void ofxFISH_User::reset( ) 
{
	tag_id = "no rfid" ;
	email = "no email" ;
}

string ofxFISH_User::toString () 
{
	stringstream ss ; 
	ss <<  " name : " << firstName << " " << lastName << endl << "email:" << email << endl << "tag_id" << tag_id << endl ; 
	
	return ss.str() ; 
	//ofDrawBitmapStringHighlight( ss.str() , x , y ) ; 
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
