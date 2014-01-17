#include "ofMain.h"
#include "ofxXmlSettings.h"

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
	void draw ( float x , float y ) ; 
	string rfid ; 
	string email ; 
	string firstName ; 
	string lastName;

	bool bFinished ; 


};