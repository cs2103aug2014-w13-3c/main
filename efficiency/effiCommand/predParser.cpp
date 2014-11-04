#include "predParser.h"
#include <vector>
#include <boost/algorithm/string.hpp>
#include <regex>
#include <map>
#include "boost/date_time/gregorian/gregorian.hpp"

using namespace std;
using namespace boost::gregorian;

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

std::tuple<string, string, string> splitPredicate(string str){
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
		return make_tuple(string(matches[1]), string(matches[2]), string(matches[3]));
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
	if(regex_search(str, hasBoolop))
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

#include <controller.h>
//This part needs to know things about the controller.

//Assume [string] operator -> tuple(string, integer, type(string)).
	//Check the corresponding fields.

//Much copy pasta after here.
//integer operator.
pred operator_gt(long compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "STRING")
			return false;
		//Do comparison
		if(int_value > compvalue)
			return true;
		else
			return false;
	};
}

pred operator_gte(long compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "STRING")
			return false;
		//Do comparison
		if(int_value >= compvalue)
			return true;
		else
			return false;
	};
}


//integer operator.
pred operator_lt(long compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "STRING")
			return false;
		//Do comparison
		if(int_value < compvalue)
			return true;
		else
			return false;
	};
}

pred operator_lte(long compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "STRING")
			return false;
		//Do comparison
		if(int_value <= compvalue)
			return true;
		else
			return false;
	};
}

//integer operator.
pred operator_eq(long compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "STRING")
			return false;
		//Do comparison
		if(int_value == compvalue)
			return true;
		else
			return false;
	};
}

//string operator
pred operator_eq(string compvalue){
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "INTEGER")
			return false;
		//Do comparison
		if(boost::iequals(str_value, compvalue))
			return true;
		else
			return false;
	};
}

pred operator_has(string compvalue){
	compvalue = to_lower_copy(compvalue);
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "INTEGER")
			return false;
		//Do comparison
		if(to_lower_copy(str_value).find(compvalue)!=-1)
			return true;
		else
			return false;
	};
}

pred operator_nothas(string compvalue){
	compvalue = to_lower_copy(compvalue);
	return [compvalue](boost::any e){
		auto values = any_cast<std::tuple<string, long, string >>(e);
		string str_value = std::get<0>(values);
		int int_value = std::get<1>(values);
		string type = std::get<2>(values);
		//Check presence
		if(type == "NONE" || type == "INTEGER")
			return false;
		//Do comparison
		if(!(to_lower_copy(str_value).find(compvalue)!=-1))
			return true;
		else
			return false;
	};
}

//TODO: test the hell out of it.
std::tuple<string, string, string> splitConditional(string str)
{
	regex extractor("(.*?)(:|!:|>|<|=|>=|<=|~|!~)(.*)");
	smatch matches;
	regex_match(str, matches, extractor);
	regex symbols("(:|!:|>|<|=|>=|<=|~|!~)");
	if(matches[0].length() == 0)
		throw expected("operator", 0);
	string s = string(matches[3]);
	/*if(regex_search(string(matches[3]), symbols))
		throw expected("!operator",matches[1].length()+matches[2].length());*/ //unexpected
	if(matches[1].length() == 0)
		throw expected("field", 0);
	return make_tuple(Parser::trim(matches[1]), matches[2], Parser::trim(matches[3]));
}

const map<string, map<string, string>> makeOpTable()
{
	map<string, map<string, string>> OpTable;
	OpTable.insert(make_pair("start",map<string, string>()));
	OpTable["start"].insert(make_pair("=", "PTIME"));
	OpTable["start"].insert(make_pair(">", "PTIME"));
	OpTable["start"].insert(make_pair("<", "PTIME"));
	OpTable["start"].insert(make_pair(">=", "PTIME"));
	OpTable["start"].insert(make_pair("<=", "PTIME"));
	OpTable["start"].insert(make_pair("=", "PTIME"));
	OpTable["start"].insert(make_pair("~", "PTIME"));
	OpTable["start"].insert(make_pair("!~", "PTIME"));
	OpTable.insert(make_pair("end",map<string, string>()));
	OpTable["end"].insert(make_pair("=", "PTIME"));
	OpTable["end"].insert(make_pair(">", "PTIME"));
	OpTable["end"].insert(make_pair("<", "PTIME"));
	OpTable["end"].insert(make_pair(">=", "PTIME"));
	OpTable["end"].insert(make_pair("<=", "PTIME"));
	OpTable["end"].insert(make_pair("=", "PTIME"));
	OpTable["end"].insert(make_pair("~", "PTIME"));
	OpTable["end"].insert(make_pair("!~", "PTIME"));
	OpTable.insert(make_pair("name",map<string, string>()));
	OpTable["name"].insert(make_pair(":", "STRING"));
	OpTable["name"].insert(make_pair("!:", "STRING"));
	OpTable["name"].insert(make_pair("=", "STRING"));
	OpTable.insert(make_pair("tags",map<string, string>()));
	OpTable["tags"].insert(make_pair(":", "STRING"));
	OpTable["tags"].insert(make_pair("!:", "STRING"));
	OpTable["tags"].insert(make_pair("=", "STRING"));
	OpTable.insert(make_pair("content",map<string, string>()));
	OpTable["content"].insert(make_pair(":", "STRING"));
	OpTable["content"].insert(make_pair("!:", "STRING"));
	OpTable["content"].insert(make_pair("=", "STRING"));
	OpTable.insert(make_pair("priority",map<string, string>()));
	OpTable["priority"].insert(make_pair("=", "INTEGER"));
	OpTable["priority"].insert(make_pair(">", "INTEGER"));
	OpTable["priority"].insert(make_pair("<", "INTEGER"));
	OpTable["priority"].insert(make_pair(">=", "INTEGER"));
	OpTable["priority"].insert(make_pair("<=", "INTEGER"));
	OpTable["priority"].insert(make_pair("=", "INTEGER"));
	return OpTable;
}

const map<string, std::function<pred (boost::any e)>> makeOplookup()
{
	map<string, std::function<pred (boost::any e)>> oplookup;
	oplookup.insert(make_pair("=_STRING" , 
		[](boost::any e)->pred{ return operator_eq(any_cast<string>(e)); }));
	oplookup.insert(make_pair("=_INTEGER" , 
		[](boost::any e)->pred{ return operator_eq(any_cast<long>(e)); }));
	oplookup.insert(make_pair(">_INTEGER" , 
		[](boost::any e)->pred{ return operator_gt(any_cast<long>(e)); }));
	oplookup.insert(make_pair("<_INTEGER" , 
		[](boost::any e)->pred{ return operator_lt(any_cast<long>(e)); }));
	oplookup.insert(make_pair("<=_INTEGER" , 
		[](boost::any e)->pred{ return operator_lte(any_cast<long>(e)); }));
	oplookup.insert(make_pair(">=_INTEGER" , 
		[](boost::any e)->pred{ return operator_gte(any_cast<long>(e)); }));
	oplookup.insert(make_pair(":_STRING" , 
		[](boost::any e)->pred{ return operator_has(any_cast<string>(e)); }));
	oplookup.insert(make_pair("!:_STRING" , 
		[](boost::any e)->pred{ return operator_nothas(any_cast<string>(e)); }));
	return oplookup;
}

//Based on the field and op_str, decide the operator to use 
// and cast comp_str to the appropriate type.
pred decide_op(string field, string op_str, string comp_str){
	auto OpTable = makeOpTable();
	auto oplookup = makeOplookup();
	if(OpTable.find(field) == OpTable.end())
		throw expected("!"+field, 0);
	auto validOps = OpTable.find(field)->second;
	if(validOps.find(op_str) == validOps.end())
		throw expected("!"+op_str, field.length()+1);
	string optype = validOps.find(op_str)->second;
	string op = op_str+"_"+optype;
	if(optype == "STRING")
		return oplookup[op](comp_str);
	if(optype == "INTEGER")
		return oplookup[op](atol(comp_str.c_str())); 
	if(optype == "PTIME")
	{
		try{//copy pasted from Controller.cpp.
			ptime t = Parser::parseDate(comp_str);
			ptime myEpoch(date(1970,Jan,1));
			time_duration myTimeFromEpoch = t-myEpoch;
			auto op = oplookup[op_str+"_INTEGER"];
			long ticks= myTimeFromEpoch.ticks();
			return op(ticks);
		}
		catch(std::exception &e)
		{
			throw expected("formatted date time", field.length() + op_str.length() + 1);
		}
	}
	else
		throw "IMPOSSIBRU";
}

//<comparator>:= >= || <= || = || :(has) || != || !: (does not have) || ~ (present) || !~ (not present)
pred parseConditional(string str){
	to_lower(str);
	auto split = splitConditional(str);
	string field = std::get<0>(split);
	string op_str = std::get<1>(split);
	string comp_str = std::get<2>(split);
	pred op = decide_op(field, op_str, comp_str);
	return [op, field](boost::any &e)->bool{  
		std::tuple<string, long, string> val = any_cast<Controller::CEvent>(e)[field];
		return op(boost::any(val));
	};
}