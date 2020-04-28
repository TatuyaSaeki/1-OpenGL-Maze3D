//=================================================================================
//! @file	Collision.h
//! @brief	衝突判定処理
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

	// 距離を求める
	static f32 distance(const Vector3& p0, const Vector3& p1, f32 r0, f32 r1);

	// 距離を求める
	// 球と球
	static f32 distance(const Sphere& s0, const Sphere& s1);

	// 距離を求める
	// 線と点の最短距離
	static f32 lineDistancePoint(const Line& line0, const Vector3& point0);
	// 線分と点の最短距離
	static f32 segmentShortDistance(const Line& line0, const Vector3& point0);

	//! 球 vs 球	関数内で押し出し
	static void adjustPosition(
		Sphere* s0,
		Sphere* s1,
		f32     mass1 = 1.0f,
		f32     mass2 = 1.0f
	);

	//! 球 vs 球	関数外で押し出し
	static bool adjustPosition(
		const Sphere& s0,
		const Sphere& s1,
		Vector3&      extrusion0,
		Vector3&      extrusion1,
		f32           mass0 = 1.0f,
		f32           mass1 = 1.0f
	);

	//! カプセル vs カプセル
	//! @note 特殊
	static void adjustPosition(
		const Capsule& c1,
		const Capsule& c2,
		Vector3&       extrusion1,
		Vector3&       extrusion2,
		f32            mass1 = 1.0f,
		f32            mass2 = 1.0f
	);

	

	//! カプセル vs ポリゴン
	//! @retval true	衝突あり
	//! @retval false	衝突なし
	static bool isHit(
		const Sphere& capsule,
		const Triangle& triangle,
		Vector3* nearestHitPosition = nullptr,
		Vector3* nearestHitNormal = nullptr,
		f32* nearestT = nullptr
	);

	//! カプセル vs ポリゴン(壁)
	//! @retval true	衝突あり
	//! @retval false	衝突なし
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

	//	カプセル vs ポリゴン(床)
	static	bool Collision::isHit(
		const Capsule&		character,
		const Triangle&		triangle,
		const float&		magnifications = 1.0f,
		Vector3*			nearestHitPosition = nullptr,
		Vector3*			nearestHitNormal = nullptr,
		f32*                nearestT = nullptr
	);

	//! 球 vs 球
	//! @retval true	衝突あり
	//! @retval false	衝突なし
	static bool isHit(const Sphere& s0, const Sphere& s1);

	

	//! カプセル vs 球
	static bool isHit(
		const Capsule& capsule0,
		const Sphere& sphere0
	);


	//! 壁ずりベクトル
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


	
	//! 三角形の中に含まれるかどうか
	//static bool	isInside(const Triangle& triangle, const Vector3& p);
	static bool isInside(const Triangle& triangle, const Vector3& point);



	//---------------------------------------------------------------------------
	//! 位置補正
	//! @param	[in]	newPosition		移動後の新しい座標
	//! @param	[in]	lastPosition	移動前の以前の座標
	//! @retval true	衝突補正あり
	//! @retval false	衝突補正なし
	//---------------------------------------------------------------------------
	static bool Collision::adjustPosition(const Triangle& triangle, Vector3& newPosition);







	//---------------------------------------------------------------------------
	//! 線分 vs 平面
	//---------------------------------------------------------------------------
	static bool	isHit(const Line& lineSegment, const Plane& plane, f32* pT);
	//---------------------------------------------------------------------------
	//! 球 vs 球
	//!	@return オブジェクト間の距離 (負の値ならめり込んだ量)
	//---------------------------------------------------------------------------
	static f32		distance1(const Sphere& s1, const Sphere& s2);
	//---------------------------------------------------------------------------
	//! 平面上のY座標を取得
	//---------------------------------------------------------------------------
	static f32		getHeight(const Plane& plane, f32 x, f32 z);
	//---------------------------------------------------------------------------
	//! 三角形の中に含まれるかどうか
	//---------------------------------------------------------------------------
	static bool	isInside1(const Triangle& triangle, const Vector3& p);

	static void	adjustPosition(Sphere& s1, Sphere& s2, f32 mass1, f32 mass2);


	//---------------------------------------------------------------------------
	//! 位置補正
	//!	@param	[in]	newPosition		移動後の新しい座標
	//!	@param	[in]	lastPosition	移動前の以前の座標
	//!	@retval	true	衝突補正あり
	//!	@retval	false	衝突なし
	//---------------------------------------------------------------------------
	static bool adjustPosition(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, const float radius = 2.0f);
	static bool adjustPositionTower(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, float& v, const float radius = 2.0f);
private:
	Vector3 nearestDotLineSegment(const Line& line0, const Vector3& point0);


	static bool		_debugFlag;

	

};
