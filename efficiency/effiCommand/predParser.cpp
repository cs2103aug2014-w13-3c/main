#include "predParser.h"
#include <vector>
#include <regex>

using namespace std;

expected::expected(string s, int position): expect(s), location(position)
{}

expected expected::operator+(const int& rhs){
	return expected(expect, location+rhs);
}

parseTreeNode::parseTreeNode(pred p):
	is_predicate(true),
	predicate(p) {}

parseTreeNode::parseTreeNode(parseTreeNode * left, boolop mid,  parseTreeNode * right):
	is_predicate(false), 
	left(unique_ptr<parseTreeNode>(left)),
	right(unique_ptr<parseTreeNode>(right)),
	op(mid) {}

pred parseTreeNode::toPredicate(){
	if(is_predicate)
		return predicate;
	else
		return op(left->toPredicate(), right->toPredicate());
}

pred boolop_or(pred a, pred b)
{
	return [a, b](boost::any x) { return a(x) || b(x); };
}

pred boolop_and(pred a, pred b)
{
	return [a, b](boost::any x) { return a(x) && b(x); };
}

pred boolop_xor(pred a, pred b)
{
	return [a, b](boost::any x) { return a(x) ^ b(x); };
}


pred parsePredicate(string str){
	int bracketerr = findBracketMatchFail(str);
	if(bracketerr != -1)
		throw "FAIL";
	auto tree = unique_ptr<parseTreeNode>(parsePred(str));
	return tree->toPredicate();
}

int findBracketMatchFail(string str)
{
	int count = 0;
	for(int pos = 0; pos != str.length(); pos++){
		char c = str.at(pos);
		if(c == '(')
			count++;
		else if (c == ')')
		{
			count--;
			if(count < 0)
				return pos;
		}
	}
	return count == 0 ? -1 : str.length();
}

//we know the brackets match.
pair<int, int> findFirstMatchingBrackets(string str){
	int count = 0;
	int start = -1, end = -1;
	for(int pos = 0; pos != str.length(); pos++){
		char c = str.at(pos);
		if(c == '('){
			if(count == 0)
				start = pos;
			count++;
		}
		else if (c == ')')
		{
			count--;
			if(count == 0){
				end = pos;
				return make_pair(start, end);
			}
		}
	}
	return make_pair(start, -1);
}

tuple<string, string, string> splitPredicate(string str){
	str = Parser::trim(str);
	//find matching right bracket to first left bracket.
	auto brackets = findFirstMatchingBrackets(str);
	if(brackets.first == 0){ //there are brackets, split out the first bracket set.
		string first = string(str.begin()+1, str.begin() + brackets.second); //but ignore the brackets
		auto opAndRemainder = splitOp(str.substr(brackets.second+1, str.length())); 
		return make_tuple(first, opAndRemainder.first, opAndRemainder.second);
	} 
	else //if(brackets.first == -1) //in this case, no brackets, split by first boolop
	{
		regex extractor("(.+?)((?:\\|\\|)|(?:\\&\\&))(.*)");
		smatch matches;
		regex_match(str, matches, extractor);
		regex hasBoolop ("(\\|\\|)|(\\&\\&)");
		if(regex_match(string(matches[1]), hasBoolop))
			throw expected("(", 0);
		return make_tuple(matches[1], matches[2], matches[3]);
	} 
}

pair<string, string> splitOp(string str){
	str = Parser::trim(str);
	regex extractor ("((?:\\|\\|)|(?:\\&\\&))(.*)");
	smatch matches;
	regex_match(str, matches, extractor);
	return make_pair(matches[1],matches[2]);
}

boolop parseOp(string str){
	str = Parser::trim(str);
	if(str == "||")
		return boolop_or;
	if(str == "&&")
		return boolop_and;
	else
		throw std::exception("impossibru");
}

pred parseConditional(string str){ //TODO
	return [](boost::any e)->bool{ return false; }; 
}

//Guarantee: brackets match
parseTreeNode* parsePred(string str){
	str = Parser::trim(str);
	//format 1: empty string
	if( str.length() == 0 )
		throw expected("predicate", 0);
	//Format 2: first and last brackets match
	if(str.at(0) == '(' && str.at(str.length() - 1) == ')') 
		return parsePred(string(str.begin()+1, str.end()-1));
	//Format 3: assume that there's a boolop inside.
	regex hasBoolop ("(\\|\\|)|(\\&\\&)");
	if(regex_match(str, hasBoolop))
	{
		//matches the || and && operators
		auto split = splitPredicate(str);
		string left = std::get<0>(split);
		string op = std::get<1>(split);
		string right = std::get<2>(split);
		return new parseTreeNode(parsePred(left), parseOp(op), parsePred(right));
	}
	//Otherwise: treat as single conditional.
	return new parseTreeNode(parseConditional(str));
}