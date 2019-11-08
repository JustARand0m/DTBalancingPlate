#include <iostream>

/**
 * Excption handling class.
 * This function is used for expetion handling, the constructor gets the error Message
 * which will than be accessable over the overloaded what() method
 */
class exceptionHandling: public std::exception {
	public:
		exceptionHandling(std::string _errorMsg): errorMsg(_errorMsg) {}
		const char* what() const throw() { return errorMsg.c_str(); }
		~exceptionHandling() throw () {} 
	private:
		std::string errorMsg;
};
