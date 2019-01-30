#include <windows.h>
#include "utils.h"

namespace ansiCToUnicode
{
	std::wstring ansiCToUnicode(const std::string& z_c)
	{
		std::string aux = z_c;
		std::wstring stemp = std::wstring(aux.begin(), aux.end());
		std::wstring sw = stemp.c_str();

		return sw;
	}
}

namespace Message
{
	void MessageBOX(const std::string& title, const std::string& message)
	{
#ifdef _VSTUDIO
		MessageBox(GetActiveWindow(), ansiCToUnicode::ansiCToUnicode(title).c_str(),
			ansiCToUnicode::ansiCToUnicode(message).c_str(), MB_OK);
#else   //Eclipse Mars2 5.11
		MessageBox(GetActiveWindow(), message.c_str(), title.c_str(), MB_OK);
#endif
	}
}
