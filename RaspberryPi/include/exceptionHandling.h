#include <iostream>

class exceptionHandling: public std::exception {
	public:
		exceptionHandling(std::string _errorMsg) : errorMsg(_errorMsg) {}
		const char* what() const throw() { return errorMsg.c_str(); }
		~exceptionHandling() throw () {} 
	private:
		std::string errorMsg;
};
