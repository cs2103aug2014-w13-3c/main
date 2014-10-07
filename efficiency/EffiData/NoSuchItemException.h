#include <exception>
#include <string>

class NoSuchItemException: public std::exception{
protected:
	std::string id;
public:
	NoSuchItemException(std::string id):id(id) {}
};