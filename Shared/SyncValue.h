#ifndef __SYNCVALUE_H__
#define __SYNCVALUE_H__
#include <RakNetTypes.h>
#include <string>
#include "Common.h"

class SyncFloat
{
private:
	char * varName;
	float var;
public:
	SyncFloat() : var(0.0f), varName("NULL") {}

	SyncFloat(float _v, char * _varName) : var(_v), varName(_varName) {}

	~SyncFloat() {}

	SyncFloat(const SyncFloat & e)
	{
		varName = e.varName;
		var = e.var;
	}

	SyncFloat & operator=(const SyncFloat &e)
	{
		varName = e.varName;
		var = e.var;

		return (*this);
	}

	void Set(float _v, RakNet::RakNetGUID sender, int ent, int type);

	//Only to be used when not being synced to others (ie reviving a player) where the values have already been synced/set
	void Set(float _v);

	float Get() { return var; }

	void Sync(RakNet::RakNetGUID user, int ent, int type);
};

class  SyncInt
{
private:
	char * varName;
	int var;
public:
	SyncInt();
	SyncInt(int _v, char * _varName);

	~SyncInt() {}

	SyncInt(const SyncInt & e)
	{
		varName = e.varName;
		var = e.var;
	}

	SyncInt & operator=(const SyncInt &e)
	{
		varName = e.varName;
		var = e.var;

		return (*this);
	}

	void Set(int _v, RakNet::RakNetGUID sender, int ent, int type);

	//Only to be used when not being synced to others (ie reviving a player) where the values have already been synced/set
	void Set(int _v);

	int Get() { return var; }

	void Sync(RakNet::RakNetGUID user, int ent, int type);
};

class SyncBool
{
private:
	char * varName;
	bool var;
public:
	SyncBool() : var(NULL), varName(false) {}

	SyncBool(bool _v, char * _varName) : var(_v), varName(_varName) {}

	~SyncBool() {}

	SyncBool(const SyncBool & e)
	{
		varName = e.varName;
		var = e.var;
	}

	SyncBool & operator=(const SyncBool &e)
	{
		varName = e.varName;
		var = e.var;

		return (*this);
	}

	void Set(bool _v, RakNet::RakNetGUID sender, int ent, int type);

	//Only to be used when not being synced to others (ie reviving a player) where the values have already been synced/set
	void Set(bool _v);

	bool Get() { return var; }

	void Sync(RakNet::RakNetGUID user, int ent, int type);
};

class SyncString
{
private:
	char * varName;
	std::string var;
public:
	SyncString() : var(NULL), varName("NULL") {}

	SyncString(std::string _v, char * _varName) : var(_v), varName(_varName) {}

	~SyncString() {}

	SyncString(const SyncString & e)
	{
		varName = e.varName;
		var = e.var;
	}

	SyncString & operator=(const SyncString &e)
	{
		varName = e.varName;
		var = e.var;

		return (*this);
	}

	void Set(std::string _v, RakNet::RakNetGUID sender, int ent, int type);

	//Only to be used when not being synced to others (ie reviving a player) where the values have already been synced/set
	void Set(std::string _v);

	std::string Get() { return var; }

	void Sync(RakNet::RakNetGUID user, int ent, int type);
};

#endif // !__SYNCVALUE_H__