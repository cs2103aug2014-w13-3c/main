#ifndef _FILE
#define _FILE

#include <string>
#include <fstream>
#include <iostream>
#include <assert.h>

class File{ //RAII wrapper around fstream

	//Files cannot be copied, because fstreams cannot be copied.
	//If you don't do this, the default copy constructor will attempt to copy all variables, and filestream is not copyable...
	//Resulting in a compile error.
private:
	File(const File& that);

protected:
	std::string _filename;
	std::fstream filestream;
	
public:
	File(std::string f);
	~File();

	void writeLine(std::string s);
	std::string readLine();

	std::string filename() { return _filename; };

	void forceClose();
};

#endif