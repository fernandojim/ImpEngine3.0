#ifndef _UTILS_H_
#define _UTILS_H_

#include <iostream>

namespace ansiCToUnicode
{
	std::wstring ansiCToUnicode(const std::string& z_c);
}

namespace Message
{
	void MessageBOX(const std::string& title, const std::string& message);
}

#endif
