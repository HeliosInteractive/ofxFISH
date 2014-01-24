#include "ofMain.h"
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
	bool populateFromJSON( ofxJSONElement json ) ; 
	
	string toString() ; 

	string tag_id ; 
	string email ; 
	string first_name ; 
	string last_name;


};