#pragma once

//Å@àÍéû
#define PI						3.14159265f
#define DEG_TO_RAD(degree)		((degree)*(PI/180.0f))
#define RAD_TO_DEG(radian)		((radian)*(180.0f/PI))
#define WORLD_GRAVITY			0.03f

class Render
{
public:
	Render() {};
	~Render() {
		SAFE_DELETE(_instance);
	};

	//---------------------------------------------------------------------------
	//!	ÉâÉCÉìÇÃï`âÊ
	//!	@param	[in]	p1	énì_ç¿ïW
	//!	@param	[in]	p2	èIì_ç¿ïW
	//---------------------------------------------------------------------------
	void	drawLine(const Vector3& p1, const Vector3& p2, const Color& color);
	//---------------------------------------------------------------------------
	//! ãÖÇÃï`âÊ
	//!	@param	[in]	position	íÜêSç¿ïW
	//!	@param	[in]	radius		îºåa
	//---------------------------------------------------------------------------

	void myCylinder(f32 radius, f32 height, int sides, const Color& color);
	// â~ï`âÊ
	void drawSphere(const Vector3& position, f32 radius, const Color& color, const int alpha = 255);
	void	drawSphereFan(const Vector3& position, f32 radius, const Color& color, const int alpha);
	// â~ï`âÊ
	void drawSphere(const Sphere& s, const Color& color = Color(0, 0, 0), const int alpha = 255);
	void drawSphereFan(const Sphere& s, const Color& color = Color(0, 0, 0), const int alpha = 255);


#if 0
	// É|ÉäÉSÉìãÖÇÃï`âÊÇÃÉxÅ[ÉXìIÇ»Ç‡ÇÃ
	void drawSphere(const Vector3& position, f32 radius, const Color& color, s32 divCount = 16);
#endif

	void drawPlaneCircle(const Vector3& position, f32 radius, const Color& color, float alpha = 255);
	void drawPlaneCircleTriangleFan(const Vector3& position, f32 radius, const Color& color, float alpha = 255);

	void	drawCapsule(const Capsule* capsule, const Color& color, float divCount = 16);
	void draw(const Vector3& pos, float radius);
	// ñÓàÛÇÃï`âÊ
	void drawArrow(const Vector3& position, const Vector3& dir, const Color& color);

	void drawRectangle(const rRectangle* rectangle, const Color& color);

	// AABBï`âÊ
	void drawAABB(const Vector3& min, const Vector3& max, const Color& color);

	void drawModel(const Vector3& v1, const Vector3& v2, const Vector3& v3);
	// éOäpå`ï`âÊ
	void drawTriangle(const Vector3* trianglePosition);
	void drawBox(const Vector3* rectPosition);
	void drowGroundGrid();

	bool drawScreenTexture(const Texture* texture, const Color& color, int alpha, const Vector3& position0 = Vector3::ZERO, const Vector3& position1 = Vector3(1280.0f, 720.0f, 0.0f));



	void drawParticle(const Vector3* position, const Color& color = Color(255, 255, 255, 255));
	void drawParticleRect(const Vector3& position, const Vector3& differencePosition, const Color& color);

	void begin2D(s32 width, s32 height);
	void drawScreenTexture(
		const Texture& texture,
		const Color&   color,
		const Vector2& position0 = Vector2(0.0f, 0.0f),
		const Vector2& position1 = Vector2(1280.0f, 720.0f),
		const float& tu0 = 0.0f,
		const float& tv0 = 0.0f,
		const float& tu1 = 1.0f,
		const float& tv1 = 1.0f
	);

	void drawScreenTextureUV(
		const Texture& texture,
		const Color&   color,
		const Vector2& position0 = Vector2(0.0f, 0.0f),
		const Vector2& position1 = Vector2(1280.0f, 720.0f),
		const Vector2& uv1		 = Vector2(0.0f, 0.0f),
		const Vector2& uv2		 = Vector2(1.0f, 1.0f)
	);

	void drawRect2D(
		const Vector3& position,
		const Vector2& size,
		const Color&   color
	);

	void drawTexture2D(
		const Texture& texture,
		const Vector2& position,
		const Vector2& size,
		const Color&   color
	);

	//void drawParticleBillboard(const Vector3& position, const Texture* texture, const Color& color = Color(255, 255, 255, 255), f32 scale = 1.0f, f32 angle = 0.0f);
	void drawParticle2D(const Vector2& position, const Texture* texture, const Color& color, f32 scale = 1.0f, f32 angle = 0.0f);
	void drawScreenTexture(const Texture& texture, const Color& color, const Vector2& position, float scale);
	void drawParticleBillboard(const Vector3& position, const Color& color = Color(255, 255, 255, 255), const Vector3& differencePosition = Vector3::ZERO, f32  angle = 0.0f, f32 scale = 1.0f);
	void drawParticleBillboard(const Vector3& position, const Vector3& size, const Color& color = Color(255, 255, 255, 255));

	void drawArrorMatrix(const Matrix& m, const float scale);

	static Render* getInstance()
	{
		return _instance;
	}


private:
	static Render* _instance;
};

class RenderState
{
public:
	static void	setBlendMode(bool enabled);
	static void setDelthTest(bool enabled);
};

//===========================================================================
//	ï∂éöóÒï`âÊ
//===========================================================================
void DrawString(const std::string str, const Vector3& pos, int w = 1296, int h = 759);

void drawRect(const Color& color, const Vector2& position0, const Vector2& position);