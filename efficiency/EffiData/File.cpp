#include "stdafx.h"

File::File(std::string f): _filename(f), filestream()
{
	filestream.open(f, std::ios_base::app | std::ios_base::in | std::ios_base::out);
	if (!filestream.is_open())
	{
		std::cout << "Error opening file";
		throw ("Unable to open file:"+f);
	}
}

File::~File() { forceClose(); }

void File::writeLine(std::string s){
		
	assert(filestream.is_open());
	//Apparently if you don't clear the EOF bit...
		//it refuses to write.
	filestream.clear() ; 
	filestream<<s<<std::endl;
	assert(!filestream.bad());
}

std::string File::readLine() { 
	std::string t;
	std::getline(filestream, t);
	return t;
}

void File::forceClose()
{
	if(filestream.is_open())
		filestream.close();
}