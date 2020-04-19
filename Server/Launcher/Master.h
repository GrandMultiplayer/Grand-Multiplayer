#ifndef __MASTER_H__
#define __MASTER_H__
#include <string>

static class Master
{
public:
	Master() {}
	~Master() {}

	static bool IsLocalIPAddress(std::string ipaddr);
	static void UpdateMaster();
};
#endif