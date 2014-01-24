#include "ofxFISH_User.h"

void ofxFISH_User::reset( ) 
{
	tag_id = "no rfid" ;
	email = "no email" ;
	first_name = "jon" ; 
	last_name = "snow" ; 
}

// Quick trace of all attribtues
string ofxFISH_User::toString () 
{
	stringstream ss ; 
	ss <<  " name : " << first_name << " " << last_name << endl << "email : " << email << endl << "tag_id : " << tag_id << endl ; 
	
	return ss.str() ; 
}

bool ofxFISH_User::populateFromJSON( ofxJSONElement json ) 
{
	//Need to see if JSON isn't empty
	if ( json != "" ) 
	{
		int numGuests = json["guests"][0]["email"].size() ; 

		//If it's not empty we need to make sure we have at least an email...
		if ( numGuests < 1 ) 
		{
			ofLogError( "Not enough guest data. No email !" ) ; 
			return false ; 
		}

		cout << " # guests... " << numGuests << endl ; 
		email =			json["guests"][0]["email"].asString() ; 
		first_name =	json["guests"][0]["first_name"].asString() ; 
		last_name =		json["guests"][0]["last_name"].asString() ; 
		return true ; 
	}
	else
	{
		ofLogError( " ofxFISH_User::populateFromJSON() . JSON is empty ! " ) ; 
		return false ;
	}
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
