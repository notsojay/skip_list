#ifndef __RUNTIME_EXCEPTION_HPP
#define __RUNTIME_EXCEPTION_HPP

#include <string>

class RuntimeException
{
private:
	std::string errorMsg;
public:
	RuntimeException(const std::string & err){ errorMsg = err; }
	std::string getMessage() const { return errorMsg; }
};

inline std::ostream & operator << (std::ostream & out, const RuntimeException & e)
{
	return out << e.getMessage(); 
}

#endif
