#ifndef __HOOKTYPES__
#define __HOOKTYPES__

#include <windows.h>

typedef DWORD Void;
typedef DWORD Any;
typedef DWORD uint;
typedef DWORD Hash;
typedef int Entity;
typedef int Player;
typedef int FireId;
typedef int Ped;
typedef int Vehicle;
typedef int Cam;
typedef int CarGenerator;
typedef int Group;
typedef int Train;
typedef int Pickup;
typedef int Object;
typedef int Weapon;
typedef int Interior;
typedef int Blip;
typedef int Texture;
typedef int TextureDict;
typedef int CoverPoint;
typedef int Camera;
typedef int TaskSequence;
typedef int ColourIndex;
typedef int Sphere;
typedef int ScrHandle;

#define PI 3.141592653589793238462643383279502884L

struct Vector2
{
	float x, y;

	Vector2()
		: x(0.f)
		, y(0.f)
	{ }

	Vector2(float x, float y)
		: x(x)
		, y(y)
	{ }
};

struct Vector3
{
public:
	alignas(8) float x;
	alignas(8) float y;
	alignas(8) float z;

public:
	Vector3()
		: x(0.f)
		, y(0.f)
		, z(0.f)
	{ }

	Vector3(float x, float y, float z)
		: x(x)
		, y(y)
		, z(z)
	{ }

	Vector3(float arr[3]) 
		: x(arr[0])
		, y(arr[1])
		, z(arr[2])
	{ }
};

struct Vector4
{
	float x;
	float y;
	float z;
	float w;

	Vector4()
		: x(0.f)
		, y(0.f)
		, z(0.f)
		, w(0.f)
	{ }

	Vector4(float x, float y, float z, float w)
		: x(x)
		, y(y)
		, z(z)
		, w(w)
	{ }
};

struct Blip_t
{
public:
	__int32 iID; //0x0000    
	__int8 iID2; //0x0004    
	char _0x0005[3];
	BYTE N000010FB; //0x0008 (80 = moves with player, some values will turn icon into map cursor and break it)   
	char _0x0009[7];
	Vector3 coords;
	char _0x001C[6];
	BYTE bFocused; //0x0022   (Focused? 0100 0000)   
	char _0x0023[5];
	char* szMessage; //0x0028 If not null, contains the string of whatever the blip says when selected.   
	char _0x0030[16];
	int iIcon; //0x0040   
	char _0x0044[4];
	DWORD dwColor; //0x0048 (Sometimes works?)   
	char _0x004C[4];
	float fScale; //0x0050    
	__int16 iRotation; //0x0054 Heading   
	BYTE bInfoIDType; //0x0056 GET_BLIP_INFO_ID_TYPE   
	BYTE bZIndex; //0x0057    
	BYTE bDisplay; //0x0058  Also Visibility 0010   
	BYTE bAlpha; //0x0059   
};//Size=0x005A

struct BlipList
{
public:
	Blip_t* m_Blips[1500]; //0x0000    

};//Size=0x2F18 

typedef struct
{
	int shapeFirst, shapeSecond, shapeThird;
	int skinFirst, skinSecond, skinThird;
	float shapeMix, skinMix, thirdMix;
} headBlendData;

#endif