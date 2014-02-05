#ifndef ERROR_HH
#define ERROR_HH

//To make sure Core is properly configured
#include "Config.hh"

//To include mandatory, generci classes
#include "Core/Error/ErrorCategory.hh"
#include "Core/Error/ErrorCondition.hh"

//including system specific errors.
//These will include ErrorCode.hh
//If a header doesnt match the targetted build platform, it will be empty
#include "Error/Win32Error.hh"
#include "Error/LinuxError.hh"



#endif // ERROR_HH




