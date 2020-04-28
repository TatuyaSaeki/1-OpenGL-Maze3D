#include "stdafx.h"


namespace /* 外部非公開 */
{
	float clamp(float _x, float _min, float _max)
	{
		return std::fmin(std::fmax(_x, _min), _max);
	}

	// 点と線分の距離の2乗
	float getSqDistancePoint2Segment(const Vector3& point, const Line& segment)
	{
		// 線分の始点から終点へのベクトル
		Vector3 lineDir = segment._position[1] - segment._position[0];
		// 線分の始点からポイントへのベクトル
		Vector3 linePointDir = point - segment._position[1];

		if (Vector3::dot(lineDir, linePointDir) < FLT_EPSILON) {
			// ２ベクトルの内積が負なら、線分の始点が最近傍
			return Vector3::dot(linePointDir, linePointDir);
		}

		// 点から線分の終点へのベクトル
		linePointDir = segment._position[1] - point;
		if (Vector3::dot(lineDir, linePointDir) < FLT_EPSILON) {
			// ２ベクトルの内積が負なら、線分の終点が最近傍
			return Vector3::dot(linePointDir, linePointDir);
		}

		// 上記のどちらにも該当しない場合、線分上に落とした射影が最近傍
		// (本来ならサインで求めるが、外積の大きさ/線分のベクトルの大きさで求まる)
		Vector3 CP = Vector3::cross(lineDir, linePointDir);

		return Vector3::dot(CP, CP) / Vector3::dot(lineDir, lineDir);
	}

	float getSqDistanceSegment2Segment(const Line& segment1, const Line& segment2)
	{
		Vector3 line1[] = { segment1._position[0], segment1._position[1] };
		Vector3 line2[] = { segment2._position[0], segment2._position[1] };
		// 線分1の方向ベクトル
		Vector3 segment1Dir = line1[1] - line1[0];
		// 線分2の方向ベクトル
		Vector3 segment2Dir = line2[1] - line2[0];

		// 線分2の始点から線分1の始点へのベクトル
		Vector3 segmentDir = line1[0] - line2[0];

		float segment1Sq = Vector3::dot(segment1Dir, segment1Dir);
		float segment2Sq = Vector3::dot(segment2Dir, segment2Dir);

		// いづれかの線分の長さが0かどうかチェック
		if (segment1Sq <= FLT_EPSILON && segment2Sq <= FLT_EPSILON) {
			// 両方長さ0
			Vector3 v = line1[0] - line2[1];
			return Vector3::dot(v, v);
		}

		if (segment1Sq <= FLT_EPSILON) {// 線分0が長さ0
			return getSqDistancePoint2Segment(line1[0], Line(line2[0], line2[1]));
		}

		if (segment2Sq <= FLT_EPSILON) {// 線分1が長さ0
			return getSqDistancePoint2Segment(line2[0], Line(line1[0], line1[1]));
		}

		float b = Vector3::dot(segment1Dir, segment2Dir);
		float f = Vector3::dot(segment2Dir, segmentDir);
		float c = Vector3::dot(segment1Dir, segmentDir);

		float s = 0.0f;
		float t = 0.0f;
		// 線分が平行でない場合、直線0上の直線1に対する最近接点を計算、そして
		// 線分0上にクランプ。そうでない場合は任意のsを選択
		float denom = segment1Sq * segment2Sq - b * b;
		if (denom != 0.0f) {
			s = clamp((b - c) / segment1Sq, 0, 1);
		}
		else {
			s = 0.0f;
		}

		// 直線1上の最接近点を計算
		float tnom = b * s + f;

		if (tnom < 0.0f) {
			t = 0.0f;
			s = clamp((-c / segment1Sq), 0, 1);
		}
		else if (tnom > segment2Sq) {
			t = 1.0f;
			s = clamp((b - segment2Sq) / segment1Sq, 0, 1);
		}
		else {
			t = tnom / segment2Sq;
		}

		Vector3 c0 = (segment1Dir * s) + line1[0];
		Vector3 c1 = (segment2Dir * t) + line2[0];
		Vector3 v = c0 - c1;

		return Vector3::dot(v, v);
	}

	float getSqDistanceSegment2Segment(const Line& segment1, const Line& segment2, const Matrix& matrix1, const Matrix& matrix2)
	{
		Line l0 = segment1;
		Line l1 = segment2;

		l0._position[0] = l0._position[0].transform(matrix1);
		l0._position[1] = l0._position[1].transform(matrix1);

		l1._position[0] = l1._position[0].transform(matrix2);
		l1._position[1] = l1._position[1].transform(matrix2);

		return getSqDistanceSegment2Segment(l0, l1);
	}
}





bool	Collision::_debugFlag = false;
//Vector3	Collision::_kouten    = Vector3::ZERO;

//---------------------------------------------------------------------------
// 距離を求める
//---------------------------------------------------------------------------
f32 Collision::distance(const Vector3& p0, const Vector3& p1, f32 r0, f32 r1)
{
	const f32 length = (p1- p0).lengthSq();
	if (length == 0.0f) return 0.0f;

	const f32 L = sqrtf(length);
	const f32 R = r0 + r1;
	return L - R;
}

//---------------------------------------------------------------------------
// 距離を求める
//---------------------------------------------------------------------------
f32 Collision::distance(const Sphere& s0, const Sphere& s1)
{
	return distance(s0._position, s1._position, s0._radius, s1._radius);
}

//---------------------------------------------------------------------------
// 距離を求める
//---------------------------------------------------------------------------
//f32 Collision::distance(const Line& line0, const Vector3& point0)
//{
//	Vector3 lineDir		= line0._position[1] - line0._position[0];
//	Vector3 lineDirP 	= point0			 - line0._position[0]; 
//
//	float area			= Vector3::cross(lineDir, lineDirP).length();
//	float lineDistance  = lineDir.length();
//
//	return area / lineDistance;
//}

//---------------------------------------------------------------------------
// 距離を求める
//---------------------------------------------------------------------------
f32 Collision::lineDistancePoint(const Line& line0, const Vector3& point0)
{
	Vector3 dir		= line0._position[1] - line0._position[0];
	float lenSqV	= line0._position[0].lengthSq();
	float t			= 0.0f;

	if (lenSqV > 0.0f) {
		t = Vector3::dot(line0._position[0], point0 - line0._position[0]) / lenSqV;
	}

	Vector3 h = line0._position[0] + (dir * t);
	return (h - point0).length();
}

// 点と線分の最短距離
// p : 点
// seg : 線分
// h : 最短距離となる端点（戻り値）
// t : 端点位置（ t < 0: 始点の外, 0 <= t <= 1: 線分内, t > 1: 終点の外 ） 
// 戻り値: 最短距離
f32 Collision::segmentShortDistance(const Line& line0, const Vector3& point0) 
{
	// 垂線の長さ、垂線の足の座標及びtを算出
	float len = lineDistancePoint(line0, point0);

	Vector3 lineDir = line0._position[1] - line0._position[0];
	Vector3 SP		= point0			 - line0._position[0];
	Vector3 EP		= point0			 - line0._position[1];

	if(Vector3::dot(lineDir, SP) <= 0.0f) {
		// 始点側の外側
		return (line0._position[0] - point0).length();
	}

	if (Vector3::dot(lineDir, EP) >= 0.0f) {
		// 終点側の外側
		return (line0._position[1] - point0).length();
	}
	
	return len;
}

//---------------------------------------------------------------------------
// 線分と点の最寄点を求める
//---------------------------------------------------------------------------
Vector3 Collision::nearestDotLineSegment(const Line& line0, const Vector3& point0)
{
	Vector3 lineDir = line0._position[1] - line0._position[0];
	Vector3 dir		= point0			 - line0._position[0];

	float dot		= Vector3::dot(lineDir, dir);
	float t			= dot / Vector3::dot(lineDir, lineDir);

	if      (t <= 0) return line0._position[0];
	if		(t >= 1) return line0._position[1];
					 return line0._position[0] + lineDir * t;
}

//---------------------------------------------------------------------------
//! 球 vs 球
//---------------------------------------------------------------------------
void Collision::adjustPosition(Sphere* s0, Sphere* s1, f32 mass0, f32 mass1)
{
	f32 d = distance(*s0, *s1);
	if (d > 0.0f) return;

	Vector3 hitNormal = (s1->_position - s0->_position).normalize();
	d *= -1.0f;

	s1->_position += hitNormal * d * 0.5f;
	s0->_position -= hitNormal * d * 0.5f;
}

//---------------------------------------------------------------------------
//! 球 vs 球
//---------------------------------------------------------------------------
bool Collision::adjustPosition(
	const Sphere& s0,
	const Sphere& s1,
	Vector3&      extrusion0,
	Vector3&      extrusion1,
	f32           mass0,
	f32           mass1
) {
	f32 d = distance(s0, s1);
	if (d > 0.0f) return false;	// 衝突なし

	Vector3 hitNormal = (s1._position - s0._position).normalize();
	d *= -1.0f;

	f32 s1factor = mass0 / (mass0 + mass1);
	f32 s2factor = mass1 / (mass0 + mass1);

	extrusion1 += hitNormal * d * 0.5f * s1factor;
	extrusion0 -= hitNormal * d * 0.5f * s2factor;
	return true;
}



//---------------------------------------------------------------------------
//! カプセル vs カプセル
//! @note 特殊
//---------------------------------------------------------------------------
void Collision::adjustPosition(
	const Capsule& c1,
	const Capsule& c2,
	Vector3&       extrusion1,
	Vector3&       extrusion2,
	f32            mass1,
	f32            mass2
) {

	// カプセルの中心位置
	const f32 CENTER_T = 0.5f;
	Vector3   p0 = c1._line.getPosition(CENTER_T);
	Vector3   p1 = c2._line.getPosition(CENTER_T);

	// 球の形状にする
	Sphere s0(c1._line.getPosition(CENTER_T), c1._radius);
	Sphere s1(c2._line.getPosition(CENTER_T), c2._radius);

	//　高さを無視する
	s0._position._y = 0.0f;
	s1._position._y = 0.0f;


	// 衝突していない場合は何もしない
	f32 d = distance(s0, s1);
	if (d > 0.0f) return;

	//	衝突の可能性があるか
	f32 y = abs(p0._y - p1._y);
	f32 h0 = c1._line._position[1]._y - p0._y;
	f32 h1 = c2._line._position[1]._y - p1._y;

	if (y < (h0 + h1)) {

		// 衝突しているので、押し出し計算を行う
		Vector3 hitNormal = (s1._position - s0._position).normalize();
		d *= -1.0f;
		d *= 0.5f;

		extrusion2 += hitNormal * d;
		extrusion1 -= hitNormal * d;
	}
}



//---------------------------------------------------------------------------
//	球 vs 球
//	@retval true	衝突あり
//	@retval false	衝突なし
//---------------------------------------------------------------------------
bool Collision::isHit(const Sphere& s0, const Sphere& s1)
{
	return distance(s0, s1) < 0.0f;
}


//---------------------------------------------------------------------------
//! カプセル vs 球
//---------------------------------------------------------------------------
bool Collision::isHit(const Capsule& capsule0, const Sphere& sphere0)
{
	Line line;
	line._position[0] = capsule0._line._position[0];
	line._position[1] = capsule0._line._position[1];

	float d = segmentShortDistance(line, sphere0._position);
	
	float r = capsule0._radius + sphere0._radius;
	

	if(d <= r)	return true;

	return false;
}

//---------------------------------------------------------------------------
//	カプセル vs ポリゴン
//	@retval true	衝突あり
//	@retval false	衝突なし
//---------------------------------------------------------------------------
bool Collision::isHit(
	const Capsule&		character,
	const Triangle&		triangle,
	const Vector3&		collisionPosition,
	const Vector3&		dir,
	const float&		magnifications,
	Vector3*			nearestHitPosition,
	Vector3*			nearestHitNormal,
	f32*                nearestT
) {
	//const Vector3 lineCenter = character._line.getPosition(0.0f);

	// 壁の法線ベクトル
	Vector3 normal = Vector3::cross(
		triangle._position[1] - triangle._position[0],
		triangle._position[2] - triangle._position[0]
	);
	normal = normal.normalize();// *magnifications;
	////normal = Vector3(0,0, -1);

	// キャラクターからポリゴンへ向かうベクトル
	// Plane使いなさい
	//Vector3 characterRay(Vector3::ZERO);
	//characterRay._x = dir._x * character._radius;
	Vector3 characterRay = normal * character._radius;
	float dot1 = 0.0f;
	float dot2 = 0.0f;
			   
	float dot3 = 0.0f;
	float dot4 = 0.0f;

	{
		// ポリゴンから始点、終点のベクトル
		Vector3 PA = collisionPosition - triangle._position[0];
		Vector3 PB = collisionPosition + characterRay - triangle._position[0];

		float dotPA = Vector3::dot(PA, normal);
		float dotPB = Vector3::dot(PB, normal);

		if (dotPA == 0.0 && dotPB == 0.0) {
			//両端が平面上にあり、交点を計算できない。
			return false;
		}


		if ((dotPA >= 0.0 && dotPB <= 0.0) ||
			(dotPA <= 0.0 && dotPB >= 0.0))
		{
			//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB

			// 交点 名前適当
			float t = abs(dotPA) / (abs(dotPA) + abs(dotPB));
			Vector3 kouten;
			kouten = collisionPosition + (characterRay * t);

			// ポリゴンに点があるか
			Vector3 polygonVec[3];
			polygonVec[0] = triangle._position[1] - triangle._position[0];
			polygonVec[1] = triangle._position[2] - triangle._position[1];
			polygonVec[2] = triangle._position[0] - triangle._position[2];

			// 交点へのベクトル
			Vector3 naibunVec[3];
			naibunVec[0] = kouten - triangle._position[0];
			naibunVec[1] = kouten - triangle._position[1];
			naibunVec[2] = kouten - triangle._position[2];

			Vector3 nomal[3];
			nomal[0] = Vector3::cross(polygonVec[0], naibunVec[0]);
			nomal[1] = Vector3::cross(polygonVec[1], naibunVec[1]);
			nomal[2] = Vector3::cross(polygonVec[2], naibunVec[2]);


			for (int i = 0; i < 3; i++) {
				nomal[i] = nomal[i].normalize();
			}

			dot1 = Vector3::dot(nomal[0], nomal[1]);
			dot2 = Vector3::dot(nomal[0], nomal[2]);


			if (dot1 > 0 && dot2 > 0) {

				if (nearestHitNormal != nullptr) {
					*nearestHitNormal = normal;
				}
				return true;
			}
		}
	}

#if 0
	{
		characterRay = Vector3::ZERO;
		characterRay._z = dir._z * character._radius;


		// ポリゴンから始点、終点のベクトル
		Vector3 PA = collisionPosition - triangle._position[0];
		Vector3 PB = collisionPosition + characterRay - triangle._position[0];

		float dotPA = Vector3::dot(PA, normal);
		float dotPB = Vector3::dot(PB, normal);

		if (dotPA == 0.0 && dotPB == 0.0) {
			//両端が平面上にあり、交点を計算できない。
			return false;
		}


		if ((dotPA >= 0.0 && dotPB <= 0.0) ||
			(dotPA <= 0.0 && dotPB >= 0.0))
		{
			//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB

			// 交点 名前適当
			float t = abs(dotPA) / (abs(dotPA) + abs(dotPB));
			Vector3 kouten;
			kouten = collisionPosition + (characterRay * t);

			// ポリゴンに点があるか
			Vector3 polygonVec[3];
			polygonVec[0] = triangle._position[1] - triangle._position[0];
			polygonVec[1] = triangle._position[2] - triangle._position[1];
			polygonVec[2] = triangle._position[0] - triangle._position[2];

			// 交点へのベクトル
			Vector3 naibunVec[3];
			naibunVec[0] = kouten - triangle._position[0];
			naibunVec[1] = kouten - triangle._position[1];
			naibunVec[2] = kouten - triangle._position[2];

			Vector3 nomal[3];
			nomal[0] = Vector3::cross(polygonVec[0], naibunVec[0]);
			nomal[1] = Vector3::cross(polygonVec[1], naibunVec[1]);
			nomal[2] = Vector3::cross(polygonVec[2], naibunVec[2]);


			for (int i = 0; i < 3; i++) {
				nomal[i] = nomal[i].normalize();
			}

			dot3 = Vector3::dot(nomal[0], nomal[1]);
			dot4 = Vector3::dot(nomal[0], nomal[2]);
		}
	}

	if (dot1 > 0 && dot2 > 0 || dot3 > 0 && dot4 > 0) {

		if (nearestHitNormal != nullptr) {
			*nearestHitNormal = normal;
		}
		return true;
	}
#endif
	//交差していない
	return false;
}

//---------------------------------------------------------------------------
//	カプセル vs ポリゴン(床)
//	@retval true	衝突あり
//	@retval false	衝突なし
//---------------------------------------------------------------------------
bool Collision::isHit(
	const Capsule&		character,
	const Triangle&		triangle,
	const float&		magnifications,
	Vector3*			nearestHitPosition,
	Vector3*			nearestHitNormal,
	f32*                nearestT) 
{
	//const Vector3 lineCenter = character._line.getPosition(0.0f);

	// 壁の法線ベクトル
	Vector3 normal = Vector3::cross(
		triangle._position[1] - triangle._position[0],
		triangle._position[2] - triangle._position[0]
	);
	normal = normal.normalize() * magnifications;
	////normal = Vector3(0,0, -1);

	Line line = Line(character._line._position[0], character._line._position[1]);
	// キャラクターカプセル線分
	// Plane使いなさい
	Vector3 characterRay = line._position[1] - line._position[0];

	// ポリゴンから始点、終点のベクトル
	Vector3 PA = line._position[0] - triangle._position[0];
	Vector3 PB = line._position[1] - triangle._position[0];

	float dotPA = Vector3::dot(PA, normal);
	float dotPB = Vector3::dot(PB, normal);

	if (dotPA == 0.0 && dotPB == 0.0) {
		//両端が平面上にあり、交点を計算できない。
		return false;
	}


	if ((dotPA >= 0.0 && dotPB <= 0.0) ||
		(dotPA <= 0.0 && dotPB >= 0.0))
	{
		//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB

		// 交点 名前適当
		float t = abs(dotPA) / (abs(dotPA) + abs(dotPB));
		Vector3 kouten;
		kouten =  line._position[0] + (characterRay * t);

		// ポリゴンに点があるか
		Vector3 polygonVec[3];
		polygonVec[0] = triangle._position[1] - triangle._position[0];
		polygonVec[1] = triangle._position[2] - triangle._position[1];
		polygonVec[2] = triangle._position[0] - triangle._position[2];

		// 交点へのベクトル
		Vector3 naibunVec[3];
		naibunVec[0] = kouten - triangle._position[0];
		naibunVec[1] = kouten - triangle._position[1];
		naibunVec[2] = kouten - triangle._position[2];

		Vector3 nomal[3];
		nomal[0] = Vector3::cross(polygonVec[0], naibunVec[0]);
		nomal[1] = Vector3::cross(polygonVec[1], naibunVec[1]);
		nomal[2] = Vector3::cross(polygonVec[2], naibunVec[2]);


		for (int i = 0; i < 3; i++) {
			nomal[i] = nomal[i].normalize();
		}

		float dot1 = Vector3::dot(nomal[0], nomal[1]);
		float dot2 = Vector3::dot(nomal[0], nomal[2]);


		if (dot1 > 0 && dot2 > 0) {

			if (nearestHitPosition != nullptr) {
				*nearestHitPosition = kouten;
			}

			if (nearestHitNormal != nullptr) {
				*nearestHitNormal = normal;
			}

			if (nearestT != nullptr) {
				*nearestT = t;
			}
			return true;
		}
	}

	//交差していない
	return false;
}

//---------------------------------------------------------------------------
//	球 vs ポリゴン
//	@retval true	衝突あり
//	@retval false	衝突なし
//---------------------------------------------------------------------------
bool Collision::isHit(
	const Sphere&		character,
	const Triangle&		triangle,
	Vector3*			nearestHitPosition,
	Vector3*			nearestHitNormal,
	f32*                nearestT
) {
	//const Vector3 lineCenter = character._line.getPosition(0.5f);
	//const Vector3 lineCenter = character._line.getPosition(0.0f);
	const Vector3 center = character._position;

	// 壁の法線ベクトル
	Vector3 normal = Vector3::cross(
		triangle._position[1] - triangle._position[0],
		triangle._position[2] - triangle._position[0]
	);
	normal = normal.normalize() * -1;

	// キャラクターからポリゴンへ向かうベクトル
	// Plane使いなさい
	Vector3 characterRay = (normal * -1) * (character._radius + 1.5f);

	// ポリゴンから始点、終点のベクトル
	Vector3 PA = (center)-triangle._position[0];
	Vector3 PB = (center + characterRay) - triangle._position[0];

	float dotPA = Vector3::dot(PA, normal);
	float dotPB = Vector3::dot(PB, normal);

	if (dotPA == 0.0 && dotPB == 0.0) {
		//両端が平面上にあり、交点を計算できない。
		return false;
	}


	if ((dotPA >= 0.0 && dotPB <= 0.0) ||
		(dotPA <= 0.0 && dotPB >= 0.0))
	{
		//交点とAの距離 : 交点とBの距離 = dot_PA : dot_PB

		// 交点 名前適当
		float t = abs(dotPA) / (abs(dotPA) + abs(dotPB));
		Vector3 kouten;
		kouten = center + (characterRay * t);

		// ポリゴンに点があるか
		Vector3 polygonVec[3];
		polygonVec[0] = triangle._position[1] - triangle._position[0];
		polygonVec[1] = triangle._position[2] - triangle._position[1];
		polygonVec[2] = triangle._position[0] - triangle._position[2];

		// 交点へのベクトル
		Vector3 naibunVec[3];
		naibunVec[0] = kouten - triangle._position[0];
		naibunVec[1] = kouten - triangle._position[1];
		naibunVec[2] = kouten - triangle._position[2];

		Vector3 cross[3];
		cross[0] = Vector3::cross(polygonVec[0], naibunVec[0]);
		cross[1] = Vector3::cross(polygonVec[1], naibunVec[1]);
		cross[2] = Vector3::cross(polygonVec[2], naibunVec[2]);


		for (int i = 0; i < 3; i++) {
			cross[i] = cross[i].normalize();
		}

		float dot1 = Vector3::dot(cross[0], cross[1]);
		float dot2 = Vector3::dot(cross[0], cross[2]);


		if (dot1 > 0 && dot2 > 0) {

			if (nearestHitPosition != nullptr) {
				*nearestHitPosition = kouten;
			}

			if (nearestHitNormal != nullptr) {
				*nearestHitNormal = normal;
			}

			if (nearestT != nullptr) {
				*nearestT = t;
			}
			return true;
		}
	}

	//交差していない
	return false;
}

//---------------------------------------------------------------------------
//	壁ずりベクトル計算
//	@retval 
//---------------------------------------------------------------------------
Vector3 Collision::wallShear(const Vector3& direction, const Vector3& normal, f32 ratio)
{
	f32 t = Vector3::dot(direction, normal);

	// ratio = 1.0f // 壁ずり等
	// ratio = 2.0f // 反射
	t *= ratio;

	//Vector3 w;
	//w._x = (normal._x * t) - direction._x;
	//w._y = (normal._y * t) - direction._y;
	//w._z = (normal._z * t) - direction._z;

	Vector3 w;
	w._x = direction._x - t * normal._x;
	w._y = direction._y - t * normal._y;
	w._z = direction._z - t * normal._z;

	return w;
}


//---------------------------------------------------------------------------
//	カプセル vs カプセル
//	@retval true	衝突あり
//	@retval false	衝突なし
//---------------------------------------------------------------------------
bool Collision::Collision2Capsule(const Capsule& capsule0, const Capsule& capsule1)
{
	float distanceSq = getSqDistanceSegment2Segment(capsule0._line, capsule1._line);

	float radiusSum = capsule0._radius + capsule1._radius;
	float radiusSq = radiusSum * radiusSum;

	if (distanceSq > radiusSq) return false;

	return true;
}

bool Collision::Collision2Capsule(const Capsule& capsule0, const Capsule& capsule1, Vector3& extruded0, Vector3& extruded1)
{
	float distanceSq = getSqDistanceSegment2Segment(capsule0._line, capsule1._line);
	float distance   = sqrtf(distanceSq);

	float radius	 = capsule0._radius + capsule1._radius;

	if(distance - radius > 0.0f) return false;

	Vector3 hitNormal = (capsule0._line.getPosition(0.5f) - capsule1._line.getPosition(0.5f)).normalize();
	distance *= -1.0f;

	extruded1 += hitNormal * distance * 0.5f;
	extruded0 -= hitNormal * distance * 0.5f;
	extruded1._y = 0.0f;
	extruded0._y = 0.0f;

	return true;
}


#if 0
//---------------------------------------------------------------------------
//! 三角形の中に含まれるかどうか
//---------------------------------------------------------------------------
bool Collision::isInside(const Triangle& triangle, const Vector3& point)
{
	const int nextIndex[3] = { 1, 2, 0 };
	Vector3 triangleDir[3];
	Vector3 pointDir[3];

	for (int i = 0; i < 3; ++i) {
		triangleDir[i] = triangle._position[nextIndex[i]] - triangle._position[i];
		pointDir[i] = point - triangle._position[nextIndex[i]];
	}

	Vector3 nomal[3];
	for (int i = 0; i < 3; ++i) {
		nomal[i] = Vector3::cross(triangleDir[i], pointDir[i]).normalize();
	}

	float dot0 = Vector3::dot(nomal[0], nomal[1]);
	float dot1 = Vector3::dot(nomal[0], nomal[2]);

	if (dot0 > 0.00001f && dot1 > 0.00001f) {
		return true;
	}
	return false;
}
#endif


//---------------------------------------------------------------------------
//! 三角形の中に含まれるかどうか
//---------------------------------------------------------------------------
bool	Collision::isInside(const Triangle& triangle, const Vector3& p)
{
	const u32 nextIndex[3] = { 1,2,0 };

	for (u32 i = 0; i < 3; ++i) {
		Vector3 edge = triangle._position[nextIndex[i]] - triangle._position[i];
		Vector3 to = p - triangle._position[nextIndex[i]];

		Vector3 vcross = Vector3::cross(edge, to);

		if (Vector3::dot(vcross, triangle._normal) < -0.00001f) return false;
	}
	return true;
}


//---------------------------------------------------------------------------
//! 位置補正
//! @param	[in]	newPosition		移動後の新しい座標
//! @param	[in]	lastPosition	移動前の以前の座標
//! @retval true	衝突補正あり
//! @retval false	衝突補正なし
//---------------------------------------------------------------------------
bool Collision::adjustPosition(const Triangle& triangle, Vector3& newPosition)
{
	// 平面上の高さを求める
	Vector3 p = newPosition;
	p._y = getHeight(triangle, p._x, p._z) + 0.0001f;

	// 三角形の中に含まれるかテスト
	if (Collision::isInside(triangle, p)) {
		newPosition = p;
		return true;
	}

	return false;
}




























#if 0

//! 球 vs 球
//! @retval true	衝突あり
//! @retval false	衝突なし
bool isHit(const Sphere* s1, const Sphere* s2)
{
#if 1
	if (distance(s1, s2) > 0.0f) {
		return false;
	}

#else
	Vector3 v = s1._position - s2._position;
	f32		lengthSq = v.length();		// 中心同士の "距離の2乗"


#endif
	return true;
}
bool isHitRect(const Vector3& v1, const Box* box1, const Vector3& v2, const Box* box2)
{
	if (v2._x + box2->_min._x < (v1._x + box1->_max._x) &&
		v1._x + box1->_min._x < (v2._x + box2->_max._x) &&
		v2._y + box2->_min._y < (v1._y + box1->_max._y) &&
		v1._y + box1->_min._y < (v2._y + box2->_max._y) &&
		v2._z + box2->_min._z < (v1._z + box1->_max._z) &&
		v1._z + box1->_min._z < (v2._z + box2->_max._z))
	{

		//衝突
		return true;
	}
	return false;
}

//======================================================================================
// BOX 押し出し　座標まとめて
//======================================================================================
Vector3 boxDistance(const Vector3& v1, const Box* box1, const Vector3& v2, const Box* box2)
{
	Vector3 v = v1 - v2;
	Vector3 w1 = (box1->_max - box1->_min) * 0.5f;
	Vector3 w2 = (box2->_max - box2->_min) * 0.5f;

	v._x = std::abs(v._x);
	v._y = std::abs(v._y);
	v._z = std::abs(v._z);

	return (v)-(w1 + w2);
}

void boxAdjustPosition(Vector3& v1, const Box* box1, Vector3& v2, const Box* box2, f32 mass1, f32 mass2)
{
	Vector3 v = boxDistance(v1, box1, v2, box2);

	if (v._x > 0.0f) return;	// 衝突なし
	if (v._y > 0.0f) return;	// 衝突なし
	if (v._z > 0.0f) return;	// 衝突なし

	Vector3 hit = v.normalize();

	v *= -1.0f;

	v2 += hit * v * 0.5f;
	v1 -= hit * v * 0.5f;
}

//======================================================================================
// BOX 押し出し　座標個別
//======================================================================================
float boxDistanceX(const float& v1, const Box* box1, const float& v2, const Box* box2)
{
	float x = v1 - v2;
	float w1 = (box1->_max._x - box1->_min._x) * 0.5f;
	float w2 = (box2->_max._x - box2->_min._x) * 0.5f;

	return std::abs(x) - (w1 + w2);
}

float boxDistanceY(const float& v1, const Box* box1, const float& v2, const Box* box2)
{
	float y = v1 - v2;
	float w1 = (box1->_max._y - box1->_min._y) * 0.5f;
	float w2 = (box2->_max._y - box2->_min._y) * 0.5f;

	return std::abs(y) - (w1 + w2);
}

float boxDistanceZ(const float& v1, const Box* box1, const float& v2, const Box* box2)
{
	float z = v1 - v2;
	float w1 = (box1->_max._z - box1->_min._z) * 0.5f;
	float w2 = (box2->_max._z - box2->_min._z) * 0.5f;

	return std::abs(z) - (w1 + w2);
}

void boxAdjustPositionX(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1, f32 mass2)
{
	float x = boxDistanceX(v1, box1, v2, box2);

	if (x > 0.0f) return;	// 衝突なし

	if (v1 < v2) {
		x *= -1.0f;
	}

	v2 += x * 0.5f;
	v1 -= x * 0.5f;
}

void boxAdjustPositionY(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1, f32 mass2)
{
	float y = boxDistanceX(v1, box1, v2, box2);

	if (y > 0.0f) return;	// 衝突なし

	if (v1 < v2) {
		y *= -1.0f;
	}

	v2 += y * 0.5f;
	v1 -= y * 0.5f;
}

void boxAdjustPositionZ(float& v1, const Box* box1, float& v2, const Box* box2, f32 mass1, f32 mass2)
{
	float z = boxDistanceX(v1, box1, v2, box2);

	if (z > 0.0f) return;	// 衝突なし

	if (v1 < v2) {
		z *= -1.0f;
	}

	v2 += z * 0.5f;
	v1 -= z * 0.5f;
}

#endif


#if 0
bool Collision::BoxVsLineSegment(Box* box, Line* line, const Matrix& m1, const Matrix& m2)
{
	Vector3 min = box->_min.transform(m1);
	Vector3 max = box->_max.transform(m1);

	Line left(Vector3(min._x, max._y, 0.0f), Vector3(min._x, min._y, 0.0f));
	Line right(Vector3(max._x, min._y, 0.0f), Vector3(max._x, max._y, 0.0f));
	Line top(Vector3(min._x, min._y, 0.0f), Vector3(max._x, min._y, 0.0f));
	Line bottom(Vector3(min._x, max._y, 0.0f), Vector3(max._x, max._y, 0.0f));

	Line lines[] = { left, right, top, bottom };

	for (int i = 0; i < 4; i++) {
		if (LineSegmentVsLineSegment(&lines[i], line)) {
			return true;
		}
	}

	return false;
}


//============================================================
// 線分と線分
//============================================================
bool Collision::LineSegmentVsLineSegment(Line* line1, Line* line2)
{
	// 法線ベクトル用
	Vector3 lineV1 = line2->_linePosition[1] - line2->_linePosition[0];
	Vector3 lineV2 = lineV1 * -1;

	Vector3 cross = Vector3::cross(lineV1, lineV2);
	cross = cross.normalize();

	float d = -(line2->_linePosition[0]._x * cross._x) +
		(line2->_linePosition[0]._y * cross._y);

	float t = -(cross._x * line1->_position._x + cross._y * line1->_position._y + d) /
		(cross._x * 1.0f + cross._y * 1.0f);

	float dot = (line2->_linePosition[0]._x * line2->_linePosition[1]._x) +
		(line2->_linePosition[0]._y * line2->_linePosition[1]._y);

	Vector3 c;
	c._x = line1->_position._x + 1.0f * t;
	c._y = line1->_position._y + 1.0f * t;
	c._z = 0.0f;

	Vector3 ac = c - line2->_linePosition[0];
	Vector3 bc = c - line2->_linePosition[1];

	float dott = Vector3::dot(ac, bc);

	if (dot < 0) return true;

	//Vector3::cross()
	return false;
}
#endif



//---------------------------------------------------------------------------
//! 線分 vs 平面
//---------------------------------------------------------------------------
bool	Collision::isHit(const Line& lineSegment, const Plane& plane, f32* pT)
{
	// S・
	// | ＼		|------求める点
	// |   ＼	|
	// |     ＼ |
	//=========◎===========
	// |  	     ＼
	// |           ＼
	// |             ＼
	// |---------------・E

	Vector3 S = lineSegment._position[0];
	Vector3 E = lineSegment._position[1];

	// 平面の方程式で高さを計算
	f32		A = Vector3::dot(plane._normal, S) + plane._distance;
	f32		B = Vector3::dot(plane._normal, E) + plane._distance;

	if (A < 0) return false; // 始点が裏にあったら
	if (B > 0) return false; // 終点が表にあったら

							 // ここに来るのは交差する可能性がある状態の場合
	B = abs(B); // Bは負の値のため絶対値を計算

				// 比率からtを求める
	f32 t = A / (A + B);

	if (pT) *pT = t; // 戻り値

	return true;
}

//---------------------------------------------------------------------------
//! 球 vs 球
//!	@return オブジェクト間の距離 (負の値ならめり込んだ量)
//---------------------------------------------------------------------------
f32		Collision::distance1(const Sphere& s1, const Sphere& s2)
{
	Vector3	v = s1._position - s2._position;
	f32		length = v.length();		// 中心同士の距離

	return length - s1._radius - s2._radius;
}

//---------------------------------------------------------------------------
//! 平面上のY座標を取得
//---------------------------------------------------------------------------
f32		Collision::getHeight(const Plane& plane, f32 x, f32 z)
{
	// Ax + By + Cz + D = 0
	//      By          = -(Ax + Cz + D)
	//       y          = -(Ax + Cz + D) / B
	return -(plane._a * x + plane._c * z + plane._d) / plane._b;
}

//---------------------------------------------------------------------------
//! 三角形の中に含まれるかどうか
//---------------------------------------------------------------------------
bool	Collision::isInside1(const Triangle& triangle, const Vector3& p)
{
	const u32 nextIndex[3] = { 1,2,0 };

	for (u32 i = 0; i<3; ++i) {
		Vector3	edge = triangle._position[nextIndex[i]] - triangle._position[i];
		Vector3	to = p - triangle._position[nextIndex[i]];

		Vector3	vcross = Vector3::cross(edge, to);

		if (Vector3::dot(vcross, triangle._normal) < -0.00001f) return false;
	}

	return true;
}

// !球 vs 球
void	Collision::adjustPosition(Sphere& s1, Sphere& s2, f32 mass1, f32 mass2)
{
	f32	d = distance1(s1, s2);
	if (d > 0.0f) return;		// 衝突なし

								// 衝突あり
	Vector3	hitNormal = (s2._position - s1._position).normalize();
	d *= -1.0f;

	f32	s1factor = mass1 / (mass1 + mass2);
	f32	s2factor = mass2 / (mass1 + mass2);


	s2._position += hitNormal * d * s1factor;
	s1._position -= hitNormal * d * s2factor;
}


//---------------------------------------------------------------------------
//! 位置補正
//!	@param	[in]	newPosition		移動後の新しい座標
//!	@param	[in]	lastPosition	移動前の以前の座標
//!	@retval	true	衝突補正あり
//!	@retval	false	衝突なし
//---------------------------------------------------------------------------
bool Collision::adjustPosition(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, const float radius)
{
	// 壁の判定
	Line	ls(lastPosition, newPosition);

	for (auto& triangle : wall) {
		f32 t;

#if 0
		Plane	offsetPlane = *triangle;
		offsetPlane._distance -= radius;	// 平面を半径分浮かせる

											// 平面で衝突点を計算
		if (isHit(ls, offsetPlane, &t) == false) continue;

		// 三角形の中に衝突点があるかチェック
		Vector3	hitPosition = ls.getPosition(t);
		if (isInside(*triangle, hitPosition) == false) continue;
#else
		// 移動先が三角形の中にあるかチェック
		if( isInside(*triangle, newPosition) == false ) continue;
#endif
		
		// 半径分以上めり込んだら押し出し
		f32 depth = Vector3::dot(triangle->_normal, newPosition) + triangle->_distance;

		if (depth >  radius) continue;	// 離れていた場合
		if (depth < -radius) continue;	// めり込みすぎている場合

		f32 push = radius - depth;	// 押し出し量 = 半径分上に浮いた場所からの距離
		push += 0.01f;				// 少し多めに押し出す

		newPosition += triangle->_normal * push;
	}

#if 0
	// 地面の判定
	for (auto& triangle : _groundTriangles) {

		// 平面上の高さを求める
		Vector3	p = newPosition;
		p._y = getHeight(triangle, p._x, p._z) + 0.0001f;

		// 三角形の中に含まれるかテスト
		if (isInside1(triangle, p) == false) continue;

		newPosition = p;
	}
#endif
	return false;
}

bool Collision::adjustPositionTower(Vector3& newPosition, const Vector3& lastPosition, std::vector<Triangle*>& wall, float& v, const float radius)
{
	// 壁の判定
	Line	ls(lastPosition, newPosition);

	for (auto& triangle : wall) {
		f32 t;

#if 0
		Plane	offsetPlane = *triangle;
		offsetPlane._distance -= radius;	// 平面を半径分浮かせる

											// 平面で衝突点を計算
		if (isHit(ls, offsetPlane, &t) == false) continue;

		// 三角形の中に衝突点があるかチェック
		Vector3	hitPosition = ls.getPosition(t);
		if (isInside(*triangle, hitPosition) == false) continue;
#else
		// 移動先が三角形の中にあるかチェック
		if (isInside(*triangle, newPosition) == false) continue;
#endif

		// 半径分以上めり込んだら押し出し
		f32 depth = Vector3::dot(triangle->_normal, newPosition) + triangle->_distance;

		if (depth >  radius) continue;	// 離れていた場合
		if (depth < -radius) continue;	// めり込みすぎている場合

		f32 push = radius - depth;	// 押し出し量 = 半径分上に浮いた場所からの距離
		push += 0.01f;				// 少し多めに押し出す

		newPosition += triangle->_normal * push;
		v = 0.0f;
	}
	return false;
}