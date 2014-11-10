//@author A0086591U
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

File::File()//Don't use it pls.
{ }

File::File(File&& other): _filename(std::move(other._filename)), filestream(std::move(other.filestream))
{}

File * File::operator =(File && other)
{
	_filename = std::move(other._filename);
	filestream = std::move(other.filestream);
	return this;
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

ostream& operator<<(ostream &os, File &f){
	string t;
	while(f.filestream.good())
	{
		std::getline(f.filestream, t);
		os<<t<<std::endl;
	}
	return os;
}


std::string File::toString(){
	std::stringstream ss;
	ss<<*this;
	return ss.str();
}
