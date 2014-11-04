#ifndef PREDPARSER_H_
#define PREDPARSER_H_

#include <string>
#include "parser.h"
#include <functional>
#include "boost/any.hpp"
#include <memory>


typedef std::function<bool(boost::any& e)> pred;
typedef std::function<pred(pred A, pred B)> boolop;

class expected: std::exception{
protected:
	string expect;
	int location;
public:
	expected(string s, int position);
	expected operator+(const int& rhs);
};

class parseTreeNode{
protected:
	std::unique_ptr<parseTreeNode> left;
	std::unique_ptr<parseTreeNode> right;
	boolop op;
	pred predicate;
	bool is_predicate;

public:
	parseTreeNode(pred p);
	parseTreeNode(parseTreeNode * left, boolop mid, parseTreeNode * right);
	pred toPredicate();
};

pred parsePredicate(std::string);

//helper functions
int findBracketMatchFail(string str);
parseTreeNode* parsePred(string str);
pred parseConditional(string str);
boolop parseOp(string str);
std::tuple<string, string, string> splitPredicate(string str);
std::tuple<string, string, string> splitConditional(string str);
pair<string, string> splitOp(string str);

#endif