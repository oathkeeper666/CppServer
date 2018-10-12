#ifndef _RESOURCE_H_
#define _RESOURCE_H_

namespace gateway {
	class Resource
	{
	public:
		Resource();
		~Resource();

		bool load();

	private:
		
	};
}

#define RESOURCE_INS bb::Singleton<gateway::Resource>::instance()

#endif // _RESOURCE_H_