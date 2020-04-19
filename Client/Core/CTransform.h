#ifndef __CTRANSFORM_H__
#define __CTRANSFORM_H__

class CTransform
{
public:
	CTransform() {}
	~CTransform() {}

	CVector3 Position;
	CVector3 Rotation;
	CVector3 Velocity;
	//Quaternion;

};

#endif