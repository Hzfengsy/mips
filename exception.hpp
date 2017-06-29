//
//  exception.hpp
//  mips
//
//  Created by 冯思远 on 2017/6/25.
//  Copyright © 2017年 冯思远. All rights reserved.
//

#ifndef exception_hpp
#define exception_hpp

#include <cstddef>
#include <cstring>
#include <string>

	
class exception
{
protected:
	const std::string variant = "";
	std::string detail = "";
public:
	exception() {}
	exception(const exception &ec) : variant(ec.variant), detail(ec.detail){}
	virtual std::string what() { return variant + " " + detail; }
};
	
/**
 * TODO
 * Please complete them.
 */
class index_out_of_bound : public exception {
	/* __________________________ */
};

class runtime_error : public exception {
	/* __________________________ */
};
class invalid_iterator : public exception {
	/* __________________________ */
};

class container_is_empty : public exception {
	/* __________________________ */
};

class invalid_identifier : public exception{
	/* __________________________ */
};

class invalid_program : public exception{
	/* __________________________ */
};


#endif /* exception.hpp */
