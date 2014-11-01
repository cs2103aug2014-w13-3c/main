#include <exception>

enum executionErrType { NOTHING_TO_UNDO, NAME_ALREADY_EXISTS, CANNOT_FIND_TARGET };
class executionError: public std::exception
{
protected:
	executionErrType type;
public:
	executionError(executionErrType);
	executionErrType getErrType();
};