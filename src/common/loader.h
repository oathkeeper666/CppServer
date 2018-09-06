#ifndef _LOADER_H_
#define _LOADER_H_

#include <string>

namespace common 
{
	class ILoader
	{
		virtual bool loadRes(const std::string & file) = 0;	
	};
}

#endif // _LOADER_H_