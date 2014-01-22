#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "ofxJSONElement.h"

class ofxFISH_User
{
public : 
	ofxFISH_User() 
	{
		reset() ; 
	} 
	
	~ofxFISH_User() { }


	void reset() ; 
	bool IsEmailValid( string email_address ) ; 
	void populateFromXml( ofxXmlSettings * xml ) ; 
	
	string toString() ; 

	string tag_id ; 
	string email ; 
	string firstName ; 
	string lastName;


};