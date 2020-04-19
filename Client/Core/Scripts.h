#ifndef __SCRIPTS_H__
#define __SCRIPTS_H__

class Scripts
{
public:
	static void StopAll();
	static void Pulse();

	typedef struct {
		char *name;
	} ScriptInfo;
};
#endif