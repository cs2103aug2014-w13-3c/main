//@author A0086591U
#include "ExecutionError.h"

executionError::executionError(executionErrType t): type(t){}

executionErrType executionError::getErrType(){
	return type;
}