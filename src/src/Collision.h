//=================================================================================
//! @file	Collision.h
//! @brief	�Փ˔��菈��
//! @auther	saeki
//=================================================================================
#pragma once

class Collision
{
public:
	static void setDebugFlag(bool flag)
	{
		_debugFlag = flag;
	}

	static bool isDebugMode(void)
	{
		return _debugFlag;
	}

	// ���������߂�
	static f32 distance(const Vector3& p0, const Vector3& p1, f32 r0, f32 r1);

	// ���������߂�
	// ���Ƌ�
	static f32 distance(const Sphere& s0, const Sphere& s1);

	// ���������߂�
	// ���Ɠ_�̍ŒZ����
	static f32 lineDistancePoint(const Line& line0, const Vector3& point0);
	// �����Ɠ_�̍ŒZ����
	static f32 segmentShortDistance(const Line& line0, const Vector3& point0);

	//! �� vs ��	�֐����ŉ����o��
	static void adjustPosition(
		Sphere* s0,
		Sphere* s1,
		f32     mass1 = 1.0f,
		f32     mass2 = 1.0f
	);

	//! �� vs ��	�֐��O�ŉ����o��
	static bool adjustPosition(
		const Sphere& s0,
		const Sphere& s1,
		Vector3&      extrusion0,
		Vector3&      extrusion1,
		f32           mass0 = 1.0f,
		f32           mass1 = 1.0f
	);

	//! �J�v�Z�� vs �J�v�Z��
	//! @note ����
	static void adjustPosition(
		const Capsule& c1,
		const Capsule& c2,
		Vector3&       extrusion1,
		Vector3&       extrusion2,
		f32            mass1 = 1.0f,
		f32            mass2 = 1.0f
	);

	

	//! �J�v�Z�� vs �|���S��
	//! @retval true	�Փ˂���
	//! @retval false	�Փ˂Ȃ�
	static bool isHit(
		const Sphere& capsule,
		const Triangle& triangle,
		Vector3* nearestHitPosition = nullptr,
		Vector3* nearestHitNormal = nullptr,
		f32* nearestT = nullptr
	);

	//! �J�v�Z�� vs �|���S��(��)
	//! @retval true	�Փ˂���
	//! @retval false	�Փ˂Ȃ�
	static bool isHit(
		const Capsule&  capsule,
		const Triangle& triangle,
		const Vector3&		collisionPosition,
		const Vector3&  dir,
		const float& magnifications = 1.0f,
		Vector3* nearestHitPosition = nullptr,
		Vector3* nearestHitNormal = nullptr,
		f32* nearestT = nullptr
	);

	//	�J�v�Z�� vs �|���S��(��)
	static	bool Collision::isHit(
		const Capsule&		character,
		const Triangle&		triangle,
		const float&		magnifications = 1.0f,
		Vector3*			nearestHitPosition = nullptr,
		Vector3*			nearestHitNormal = nullptr,
		f32*                nearestT = nullptr
	);

	//! �� vs ��
	//! @retval true	�Փ˂���
	//! @retval false	�Փ˂Ȃ�
	static bool isHit(const Sphere& s0, const Sphere& s1);

	

	//! �J�v�Z�� vs ��
	static bool isHit(
		const Capsule& capsule0,
		const Sphere& sphere0
	);


	//! �ǂ���x�N�g��
	static Vector3 wallShear(const Vector3& direction, const Vector3& normal, f32 ratio = 2.0f);

	//static bool isHit(
	//	const Capsule& capsule,
	//	const rRectangle& rectangle,
	//	Vector3* pNearestPosition = nullptr,
	//	Vector3* pHitNormal = nullptr,
	//	f32* pNearestT = nullptr
	//);


	static bool Collision2Capsule(
		const Capsule& capsule0,
		const Capsule& capsule1
	);

	static bool Collision2Capsule(
		const Capsule& capsule0,
		const Capsule& capsule1,
		Vector3& extruded0,
		Vector3& extruded1
	);

	
	//static bool BoxVsLineSegment(Box* box, Line* l, const Matrix& m1, const Matrix& m2);
	//static bool LineSegmentVsLineSegment(Line* line1, Line* line2);




	//bool isHitRect(const Vector3& v1, const Box* box1, const Vector3& v2, const Box* box2);

	//Vector3 boxDistance(const Vector3& v1, const Box* box1, const Vector3& v2, const Box* box2);
	//void	boxAdjustPosition(Vector3& v1, const Box* box1, Vector3& v2, const Box* box2, f32 mass1 = 1, f32 mass2 = 1);

	// void boxAdjustPositionX(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1 = 1, f32 mass2 = 1);
	// void boxAdjustPositionY(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1 = 1, f32 mass2 = 1);
	// void boxAdjustPositionZ(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1 = 1, f32 mass2 = 1);

	//bool isHit(const Capsule& capsule, const Triangle& triangle);
	//bool isHit(const Sphere& sphere, const Triangle& triangle);


	
	//! �O�p�`�̒��Ɋ܂܂�邩�ǂ���
	//static bool	isInside(const Triangle& triangle, const Vector3& p);
	static bool isInside(const Triangle& triangle, const Vector3& point);



	//---------------------------------------------------------------------------
	//! �ʒu�␳
	//! @param	[in]	newPosition		�ړ���̐V�������W
	//! @param	[in]	lastPosition	�ړ��O�̈ȑO�̍��W
	//! @retval true	�Փ˕␳����
	//! @retval false	�Փ˕␳�Ȃ�
	//---------------------------------------------------------------------------
	static bool Collision::adjustPosition(const Triangle& triangle, Vector3& newPosition);







	//---------------------------------------------------------------------------
	//! ���� vs ����
	//---------------------------------------------------------------------------
	static bool	isHit(const Line& lineSegment, const Plane& plane, f32* pT);
	//---------------------------------------------------------------------------
	//! �� vs ��
	//!	@return �I�u�W�F�N�g�Ԃ̋��� (���̒l�Ȃ�߂荞�񂾗�)
	//---------------------------------------------------------------------------
	static f32		distance1(const Sphere& s1, const Sphere& s2);
	//---------------------------------------------------------------------------
	//! ���ʏ��Y���W���擾
	//---------------------------------------------------------------------------
	static f32		getHeight(const Plane& plane, f32 x, f32 z);
	//---------------------------------------------------------------------------
	//! �O�p�`�̒��Ɋ܂܂�邩�ǂ���
	//---------------------------------------------------------------------------
	static bool	isInside1(const Triangle& triangle, const Vector3& p);

	static void	adjustPosition(Sphere& s1, Sphere& s2, f32 mass1, f32 mass2);


	//---------------------------------------------------------------------------
	//! �ʒu�␳
	//!	@param	[in]	newPosition		�ړ���̐V�������W
	//!	@param	[in]	lastPosition	�ړ��O�̈ȑO�̍��W
	//!	@retval	true	�Փ˕␳����
	//!	@retval	false	�Փ˂Ȃ�
	//---------------------------------------------------------------------------
	static bool adjustPosition(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, const float radius = 2.0f);
	static bool adjustPositionTower(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, float& v, const float radius = 2.0f);
private:
	Vector3 nearestDotLineSegment(const Line& line0, const Vector3& point0);


	static bool		_debugFlag;

	

};
