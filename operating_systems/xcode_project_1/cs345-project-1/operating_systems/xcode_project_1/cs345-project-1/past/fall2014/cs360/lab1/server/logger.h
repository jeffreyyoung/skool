#pragma once

#include <stdlib.h>
#include <string.h>
#include <iostream>

using namespace std;

class Logger {
	
	private:
		bool isActivated_;
	public:
		Logger();
		~Logger();
		void log(string);
		void set_is_activated(bool);
};