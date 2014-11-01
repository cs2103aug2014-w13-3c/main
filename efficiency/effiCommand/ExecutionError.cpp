#include "ExecutionError.h"

executionError::executionError(executionErrType t): type(t){}

executionErrType executionError::getErrType(){
	return type;
}