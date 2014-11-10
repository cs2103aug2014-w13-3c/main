//@author A0086591U
#ifndef UNDO_H_
#define UNDO_H_

#include <iostream>
#include <string>
#include <stack>
#include <vector>

using namespace std;

class Undo{

public:

	// find inverse function of an executed function to be push to undo stack
	static string matchInverseFunction(vector<string> commandStringVector);

	static vector<string> undo();

private:

	static stack< vector<string> > undoStack;

};
#endif