#include "logger.h"

Logger:: Logger(){
	//set up vars
	isActivated_ = false;
}
Logger:: ~Logger(){

}
void
Logger:: log(string str){
	if (isActivated_){
		cout << str << endl;
	}
}

void
Logger:: set_is_activated(bool isActivated){
	isActivated_ = isActivated;
}