#include "stdafx.h"

#pragma once

Render* Render::_instance = new Render();

void drawRect(const Color& color, const Vector2& position0, const Vector2& position1)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//------------------------------
	// �g
	//------------------------------
	glBegin(GL_LINES);
	glColor3fv((GLfloat*)&color);
	// ��
	glVertex2f(position0._x, position0._y);
	glVertex2f(position1._x, position0._y);

	glVertex2f(position0._x, position1._y);
	glVertex2f(position1._x, position1._y);

	// �c
	glVertex2f(position0._x, position0._y);
	glVertex2f(position0._x, position1._y);

	glVertex2f(position1._x, position0._y);
	glVertex2f(position1._x, position1._y);

	glEnd();
}

//---------------------------------------------------------------------------
//!	���C���̕`��
//!	@param	[in]	p1	�n�_���W
//!	@param	[in]	p2	�I�_���W
//---------------------------------------------------------------------------
void	Render::drawLine(const Vector3& p1, const Vector3& p2, const Color& color)
{
	glColor4ubv((GLubyte*)&color);

	glBegin(GL_LINES);
	glVertex3fv((GLfloat*)&p1);
	glVertex3fv((GLfloat*)&p2);
	glEnd();
}


void Render::myCylinder(f32 radius, f32 height, int sides, const Color& color)
{
	f32 step = (PI * 2.0f) / (f32)sides;

	// ��
	//glNormal3d(0.0, 1.0, 0.0);
	//glBegin(GL_TRIANGLE_FAN);
	//for (i = 0; i < sides; i++) {
	//	double t = step * (double)i;
	//	glVertex3d(radius * sin(t), height, radius * cos(t));
	//}
	//glEnd();

	// ��
	//glNormal3d(0.0, -1.0, 0.0);
	//glBegin(GL_TRIANGLE_FAN);
	//for (i = sides; --i >= 0;) {
	//	double t = step * (double)i;
	//	glVertex3d(radius * sin(t), 0.0, radius * cos(t));
	//}
	//glEnd();

	// ����
	glBegin(GL_TRIANGLE_STRIP);

	glColor4ubv((GLubyte*)&color);
	for (int i = 0; i <= sides; i++) {
		f32 t = step * (f32)i;
		f32 x = sin(t);
		f32 z = cos(t);

		glNormal3f(x, 0.0, z);
		glVertex3f(radius * x, height, radius * z);
		glVertex3f(radius * x, 0.0   , radius * z);
	}
	glEnd();
}

void Render::draw(const Vector3& pos, float radius) {
	 // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //�o�b�t�@�̏���

	//��
	glPushMatrix();
	glColor3d(1.0, 0.0, 0.0);				//�F�̐ݒ�
	glTranslated(pos._x, pos._y, pos._z);	//���s�ړ��l�̐ݒ�
	glutSolidSphere(radius, 10, 10);		//�����F(���a, Z���܂��̕�����, Z���ɉ�����������)
	glPopMatrix();
}

//---------------------------------------------------------------------------
//! ���̕`��
//!	@param	[in]	position	���S���W
//!	@param	[in]	radius		���a
//---------------------------------------------------------------------------
void	Render::drawSphere(const Vector3& position, f32 radius, const Color& color, const int alpha)
{
	Color col(color._r, color._g, color._b, alpha);
	glColor4ubv((GLubyte*)&col);

	int	divCount = 16;
	f32	invDivCount = 1.0f / divCount;

	// XY���ʂ̉~
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(cosf(angle), sinf(angle), 0.0f) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	// XZ���ʂ̉~
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	// YZ���ʂ̉~
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(0.0f, cosf(angle), sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	//glBegin(GL_LINES);
	//glVertex3f(position._x - radius, position._y, position._z);
	//glVertex3f(position._x + radius, position._y, position._z);
	//glVertex3f(position._x, position._y - radius, position._z);
	//glVertex3f(position._x, position._y + radius, position._z);
	//glVertex3f(position._x, position._y, position._z - radius);
	//glVertex3f(position._x, position._y, position._z + radius);
	//glEnd();
}

void	Render::drawSphereFan(const Vector3& position, f32 radius, const Color& color, const int alpha)
{
	Color col(color._r, color._g, color._b, alpha);
	glColor4ubv((GLubyte*)&col);

	int	divCount = 16;
	f32	invDivCount = 1.0f / divCount;

	// XY���ʂ̉~
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(cosf(angle), sinf(angle), 0.0f) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	// XZ���ʂ̉~
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	// YZ���ʂ̉~
	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = position + Vector3(0.0f, cosf(angle), sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();

	//glBegin(GL_LINES);
	//glVertex3f(position._x - radius, position._y, position._z);
	//glVertex3f(position._x + radius, position._y, position._z);
	//glVertex3f(position._x, position._y - radius, position._z);
	//glVertex3f(position._x, position._y + radius, position._z);
	//glVertex3f(position._x, position._y, position._z - radius);
	//glVertex3f(position._x, position._y, position._z + radius);
	//glEnd();
}

void Render::drawSphere(const Sphere& s, const Color& color, const int alpha)
{
	drawSphere(s._position, s._radius, color, alpha);
}

void Render::drawSphereFan(const Sphere& s, const Color& color, const int alpha)
{
	drawSphereFan(s._position, s._radius, color, alpha);
}

#if 0
// �|���S�����̕`��̃x�[�X�I�Ȃ���
//---------------------------------------------------------------------------
//! ���̕`��
//!	@param	[in]	position	���S���W
//!	@param	[in]	radius		���a
//!	@param	[in]	divCount    ������
//---------------------------------------------------------------------------
void drawSphere(const Vector3& position, f32 radius, const Color& color, s32 divCount)
{
	if (divCount < 4) return;

	const f32 invDivCount = 1.0f / divCount;
	const f32 towPI       = PI * 2.0f;

	Matrix rotY;

	for (int i = 0; i < divCount; ++i) {
		f32 radius = i * divCount * towPI;

		rotY = Matrix::rotateY(radius);

	}



	// 
	glColor4ubv((GLubyte*)&color);
	glBegin(GL_TRIANGLES);
	glEnd();
}
#endif




void Render::drawPlaneCircle(const Vector3& position, f32 radius, const Color& color, float alpha)
{
	glPushMatrix();
	glTranslatef(position._x, position._y, position._z);
	Color c(color._r, color._g, color._b, alpha);
	glColor4ubv((GLubyte*)&c);

	int	divCount = 16;
	f32	invDivCount = 1.0f / divCount;

	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();
	glPopMatrix();
}

void Render::drawPlaneCircleTriangleFan(const Vector3& position, f32 radius, const Color& color, float alpha)
{
	glPushMatrix();
	glTranslatef(position._x, position._y, position._z);
	Color c(color._r, color._g, color._b, alpha);
	glColor4ubv((GLubyte*)&c);

	int	divCount = 16;
	f32	invDivCount = 1.0f / divCount;

	glBegin(GL_TRIANGLE_FAN);
	for (int i = 0; i < divCount; ++i) {
		f32	angle = ((f32)i * invDivCount) * (2.0f * PI);

		Vector3	p = Vector3(cosf(angle), 0.0f, sinf(angle)) * radius;

		glVertex3fv((GLfloat*)&p);
	}
	glEnd();
	glPopMatrix();
}

//---------------------------------------------------------------------------
// �J�v�Z���`��
//---------------------------------------------------------------------------
void	Render::drawCapsule(const Capsule* capsule, const Color& color, float divCount)
{
	const f32 invDivCount = 1.0f / divCount;
	const f32 TOW_PI      = 2.0f * PI;

	const Vector3 p0     = capsule->_line._position[0];
	const Vector3 p1     = capsule->_line._position[1];
	const f32     radius = capsule->_radius;

	Vector3 side = Vector3::ZERO;
	Vector3	point;
	f32	angle;
	glColor3f(color._r, color._b, color._g);
	// ��
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		angle = ((f32)i * invDivCount) * TOW_PI;

		point._x = p0._x + cos(angle) * radius;
		point._y = p0._y;
		point._z = p0._z + sin(angle) * radius;

		glVertex3fv((GLfloat*)&point);
	}
	glEnd();

	// ��
	glBegin(GL_LINE_LOOP);
	for (int i = 0; i < divCount; ++i) {
		angle = ((f32)i * invDivCount) * TOW_PI;

		point._x = p1._x + cos(angle) * radius;
		point._y = p1._y;
		point._z = p1._z + sin(angle) * radius;

		glVertex3fv((GLfloat*)&point);
	}
	glEnd();

	// ����
	glBegin(GL_LINES);
	for (int i = 0; i < divCount; ++i) {
		angle = ((f32)i * invDivCount) * TOW_PI;

		side._x = cos(angle) * radius;
		side._z = sin(angle) * radius;

		point = p0 + side;
		glVertex3fv((GLfloat*)&point);

		point = p1 + side;
		glVertex3fv((GLfloat*)&point);
	}
	glEnd();
	
}

//---------------------------------------------------------------------------
// ���̕`��
//---------------------------------------------------------------------------
void Render::drawArrow(const Vector3& position, const Vector3& dir, const Color& color)
{
	glColor3f(color._r, color._b, color._g);

	Vector3	s = position;
	Vector3	e = position + dir;

	Vector3	right = Vector3::cross(dir, Vector3(0, 1, 0)).normalize();
	Vector3	up = Vector3::cross(right, dir).normalize();

	Vector3	v[4];

	Vector3	bottom = (e - s) * 0.85f + s;	// 85%
	right *= 0.1f;
	up *= 0.1f;

	v[0] = bottom + right + up;
	v[1] = bottom + right - up;
	v[2] = bottom - right - up;
	v[3] = bottom - right + up;

	glBegin(GL_LINES);
	// ���S�̐�
	glVertex3fv((GLfloat*)&s);
	glVertex3fv((GLfloat*)&e);

	// �l�p�`�̓y�䕔��		
	glVertex3fv((GLfloat*)&v[0]);
	glVertex3fv((GLfloat*)&v[1]);

	glVertex3fv((GLfloat*)&v[1]);
	glVertex3fv((GLfloat*)&v[2]);

	glVertex3fv((GLfloat*)&v[2]);
	glVertex3fv((GLfloat*)&v[3]);

	glVertex3fv((GLfloat*)&v[3]);
	glVertex3fv((GLfloat*)&v[0]);

	//		// �l�p�̓_�����[�����Ԑ�
	//		glVertex3fv((GLfloat*)&v[0]);
	//		glVertex3fv((GLfloat*)&e);
	//	
	//		glVertex3fv((GLfloat*)&v[1]);
	//		glVertex3fv((GLfloat*)&e);
	//	
	//		glVertex3fv((GLfloat*)&v[2]);
	//		glVertex3fv((GLfloat*)&e);
	//	
	//		glVertex3fv((GLfloat*)&v[3]);
	//		glVertex3fv((GLfloat*)&e);
	//	
	glEnd();

	glBegin(GL_TRIANGLES);

	glVertex3fv((GLfloat*)&v[0]);
	glVertex3fv((GLfloat*)&v[1]);
	glVertex3fv((GLfloat*)&e);

	glVertex3fv((GLfloat*)&v[1]);
	glVertex3fv((GLfloat*)&v[2]);
	glVertex3fv((GLfloat*)&e);

	glVertex3fv((GLfloat*)&v[2]);
	glVertex3fv((GLfloat*)&v[3]);
	glVertex3fv((GLfloat*)&e);

	glVertex3fv((GLfloat*)&v[3]);
	glVertex3fv((GLfloat*)&v[0]);
	glVertex3fv((GLfloat*)&e);

	glEnd();
}

void Render::drawRectangle( const rRectangle* rectangle, const Color& color)
{
	glBegin(GL_TRIANGLE_FAN);
	glColor3f(color._r, color._b, color._g);

	glVertex3fv((GLfloat*)&rectangle->_vertexPosition[0]);
	glVertex3fv((GLfloat*)&rectangle->_vertexPosition[1]);
	glVertex3fv((GLfloat*)&rectangle->_vertexPosition[2]);
	glVertex3fv((GLfloat*)&rectangle->_vertexPosition[3]);

	//glVertex3fv((GLfloat*)&surface->_polygonPosition[3]);

	glEnd();
}

void	Render::drawAABB(const Vector3& min, const Vector3& max, const Color& color)
{
	Vector3	v[8];
	v[0] = Vector3(min._x, min._y, min._z);
	v[1] = Vector3(max._x, min._y, min._z);
	v[2] = Vector3(max._x, min._y, max._z);
	v[3] = Vector3(min._x, min._y, max._z);

	v[4] = Vector3(min._x, max._y, min._z);
	v[5] = Vector3(max._x, max._y, min._z);
	v[6] = Vector3(max._x, max._y, max._z);
	v[7] = Vector3(min._x, max._y, max._z);

	glBegin(GL_TRIANGLE_FAN);
	Color c(color._r, color._g, color._b, color._a);
	glColor4ubv((GLubyte*)&c);		// Yellow

													// ���
	glVertex3fv((GLfloat*)&v[0]);
	glVertex3fv((GLfloat*)&v[1]);

	glVertex3fv((GLfloat*)&v[1]);
	glVertex3fv((GLfloat*)&v[2]);

	glVertex3fv((GLfloat*)&v[2]);
	glVertex3fv((GLfloat*)&v[3]);

	glVertex3fv((GLfloat*)&v[3]);
	glVertex3fv((GLfloat*)&v[0]);

	// �V��
	glVertex3fv((GLfloat*)&v[4]);
	glVertex3fv((GLfloat*)&v[5]);

	glVertex3fv((GLfloat*)&v[5]);
	glVertex3fv((GLfloat*)&v[6]);

	glVertex3fv((GLfloat*)&v[6]);
	glVertex3fv((GLfloat*)&v[7]);

	glVertex3fv((GLfloat*)&v[7]);
	glVertex3fv((GLfloat*)&v[4]);

	// ��
	glVertex3fv((GLfloat*)&v[0]);
	glVertex3fv((GLfloat*)&v[4]);

	glVertex3fv((GLfloat*)&v[1]);
	glVertex3fv((GLfloat*)&v[5]);

	glVertex3fv((GLfloat*)&v[2]);
	glVertex3fv((GLfloat*)&v[6]);

	glVertex3fv((GLfloat*)&v[3]);
	glVertex3fv((GLfloat*)&v[7]);

	glEnd();
}





void	Render::drawModel(const Vector3& v1, const Vector3& v2, const Vector3& v3)
{
	glBegin(GL_TRIANGLES);

	// ���
	glColor3ub(255, 255, 255);
	glVertex3f(v1._x, v1._y, v1._z);	// -1,0,-1
	glVertex3f(v2._x, v2._y, v2._z);	// -1,0, 1
	glVertex3f(v3._x, v3._y, v3._z);	//  1,0,-1

										//glVertex3f(+1, 0, -1);
										//glVertex3f(-1, 0, +1);
										//glVertex3f(+1, 0, +1);

										// �E����
										//glColor3ub(255,255,0);
										//glVertex3f(0, 1, 0);
										//glVertex3f(+1, 0, +1);
										//glVertex3f(+1, 0, -1);
										//
										//// ������
										//glColor3ub(255,0,0);
										//glVertex3f(0, 1, 0);
										//glVertex3f(-1, 0, -1);
										//glVertex3f(-1, 0, +1);
										//
										//// ���̑���
										//glColor3ub(0,255,0);
										//glVertex3f(0, 1, 0);
										//glVertex3f(+1, 0, -1);
										//glVertex3f(-1, 0, -1);
										//
										//// ��O�̑���
										//glColor3ub(0, 0, 255);
										//glVertex3f(0, 1, 0);
										//glVertex3f(-1, 0, +1);
										//glVertex3f(+1, 0, +1);

	glEnd();
}



void Render::drawTriangle(const Vector3* trianglePosition)
{
	glBegin(GL_TRIANGLES);

	glColor3ub(255, 255, 0);

	for (int i = 0; i < 3; ++i) {
		const auto& p = trianglePosition[i];
		glVertex3f(p._x, p._y, p._z);
	}
	glEnd();
}

void Render::drawBox(const Vector3* rectPosition)
{
	glBegin(GL_TRIANGLE_FAN);

	glColor3ub(255, 0, 0);

	for (int i = 0; i < 8; ++i) {
		const auto& p = rectPosition[i];
		glVertex3f(p._x, p._y, p._z);
	}

	glEnd();
}


//-------------------------------------------------------------
// �n�ʂ̃O���b�h��\��
//-------------------------------------------------------------
void Render::drowGroundGrid()
{
	// ���S���̕`��
	glBegin(GL_LINES);

	// X��
	glColor3ub(255, 64, 64);
	glVertex3f(-160, 0, 0);
	glVertex3f(+160, 0, 0);

	// Y��
	glColor3ub(64, 255, 64);
	glVertex3f(0, -160, 0);
	glVertex3f(0, +160, 0);

	// Z��
	glColor3ub(160, 160, 255);
	glVertex3f(0, 0, -160);
	glVertex3f(0, 0, +160);

	glColor3ub(192, 192, 192);
	for (int x = -160; x <= +160; ++x) {
		glVertex3f((GLfloat)x, 0.0f, -160.0f);
		glVertex3f((GLfloat)x, 0.0f, +160.0f);
	}

	for (int z = -160; z <= +160; ++z) {
		glVertex3f(-160.0f, 0.0f, (GLfloat)z);
		glVertex3f(+160.0f, 0.0f, (GLfloat)z);
	}
	glEnd();
}





//===========================================================================
//	������`��
//===========================================================================
void DrawString(const std::string str, const Vector3& pos, int w, int h)
{
	glDisable(GL_LIGHTING);
	// ���s���e�ɂ���
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// ��ʏ�Ƀe�L�X�g�`��
	glRasterPos2f(pos._x, pos._y);
	int size = (int)str.size();
	for (int i = 0; i < size; ++i) {
		char ic = str[i];

		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
	}



	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void Render::drawParticle(const Vector3* position, const Color& color)
{
	//glEnable(GL_ALPHA_TEST);
	////-------------------------------------------------------------
	//// �e�N�X�`���̕`��e�X�g
	////-------------------------------------------------------------
	//
	//Texture::set(texture);
	//// ����������ON
	//glEnable(GL_BLEND);
	//// �ʏ�u�����h
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//
	//glBegin(GL_TRIANGLE_FAN);
	//Color c(color._r, color._g, color._b, color._a);
	//glColor4ubv((GLubyte*)&c);


	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv((GLfloat*)&position[1]);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv((GLfloat*)&position[0]);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv((GLfloat*)&position[2]);
	glEnd();


	//Texture::set(nullptr);
	//glDisable(GL_BLEND);
}

void Render::drawParticleRect(const Vector3& position, const Vector3& differencePosition, const Color& color)
{
	Vector3 pos[4];
	for(int i = 0; i < 4; i++)  pos[i]._y = differencePosition._y;
	pos[0]._x = position._x - differencePosition._x;	// ����
	pos[0]._z = position._z - differencePosition._z;

	pos[1]._x = position._x + differencePosition._x;	// �E��
	pos[1]._z = position._z - differencePosition._z;

	pos[2]._x = position._x + differencePosition._x;	// �E��
	pos[2]._z = position._z + differencePosition._z;

	pos[3]._x = position._x - differencePosition._x;	// ����
	pos[3]._z = position._z + differencePosition._z;

	glEnable(GL_ALPHA_TEST);
	//-------------------------------------------------------------
	// �e�N�X�`���̕`��e�X�g
	//-------------------------------------------------------------

	RenderState::setBlendMode(true);

	glBegin(GL_TRIANGLE_FAN);
	glColor4ubv((GLubyte*)&color);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv((GLfloat*)&pos[0]);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv((GLfloat*)&pos[1]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv((GLfloat*)&pos[2]);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv((GLfloat*)&pos[3]);
	glEnd();

	RenderState::setBlendMode(false);
}

bool Render::drawScreenTexture(const Texture* texture, const Color& color, int alpha, const Vector3& position0, const Vector3& position1)
{
	int a = alpha;
	if(a < 0) a = 0;
	if(a > 255) a = 255;
	
	Color c(color._r, color._g, color._b, a);

	//-------------------------------------------------------------
	// 2D�`��
	//-------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);

	// �s�N�Z�����W���X�N���[�����W�ɕϊ�����
	s32		screenW = 1280;
	s32		screenH = 720;

	Matrix	matPixelFit = Matrix::scale(Vector3(2.0f / screenW, -2.0f / screenH, 1.0f)) *
						  Matrix::translate(Vector3(-1.0f, +1.0f, 0.0f)); // ���_(����)�����S�ɂ���̂ŁA�X�N���[���̍���Ɉړ�
	glLoadMatrixf((GLfloat*)&matPixelFit);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// �e�N�X�`���̕��A����
	s32		textureW = texture->getWidth();
	s32		textureH = texture->getHeight();
	f32		x1 = position0._x;
	f32		y1 = position0._y;
	f32		x2 = position1._x;
	f32		y2 = position1._y;

	glColor4ubv((GLubyte*)&c);

	// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Texture::set(texture);
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(0.0f, 0.0f);	// ����
	glVertex3f(x1, y1, 0.0f);
	
	glTexCoord2f(1.0f, 0.0f);	// �E��
	glVertex3f(x2, y1, 0.0f);	

	glTexCoord2f(0.0f, 1.0f);	// ����
	glVertex3f(x1, y2, 0.0f);

	glTexCoord2f(1.0f, 1.0f);	// �E��
	glVertex3f(x2, y2, 0.0f);

	glEnd();
	Texture::set(nullptr);
	// ����������OFF
	glDisable(GL_BLEND);

	return true;
}


void Render::begin2D(s32 width, s32 height)
{
	//-------------------------------------------------------------
	// 2D�`��
	//-------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);

	Matrix	matPixelFit =
		Matrix::scale(Vector3(2.0f / width, -2.0f / height, 1.0f)) *
		Matrix::translate(Vector3(-1.0f, +1.0f, 0.0f)); // ���_(����)�����S�ɂ���̂ŁA�X�N���[���̍���Ɉړ�
	
	glLoadMatrixf((GLfloat*)&matPixelFit);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);				// �y�o�b�t�@��L���ɂ���
}

void Render::drawScreenTexture(
	const Texture& texture, 
	const Color& color, 
	const Vector2& position0,
	const Vector2& position1, 
	const float& tu0, const float& tv0, const float& tu1, const float& tv1)
{
	/*
	//-------------------------------------------------------------
	// 2D�`��
	//-------------------------------------------------------------
	glMatrixMode(GL_PROJECTION);

	// �s�N�Z�����W���X�N���[�����W�ɕϊ�����
	s32		screenW = 1280;
	s32		screenH = 720;

	Matrix	matPixelFit = Matrix::scale(Vector3(2.0f / screenW, -2.0f / screenH, 1.0f)) *
		Matrix::translate(Vector3(-1.0f, +1.0f, 0.0f)); // ���_(����)�����S�ɂ���̂ŁA�X�N���[���̍���Ɉړ�
	glLoadMatrixf((GLfloat*)&matPixelFit);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	*/
	// �e�N�X�`���̕��A����
	s32		textureW = texture.getWidth();
	s32		textureH = texture.getHeight();
	f32		x1 = position0._x;
	f32		y1 = position0._y;
	f32		x2 = position1._x;
	f32		y2 = position1._y;

	glColor4ubv((GLubyte*)&color);

	// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Texture::set(&texture);
	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(tu0, tv0);	// ����
	glVertex3f(x1, y1, 0.0f);

	glTexCoord2f(tu1, tv0);	// �E��
	glVertex3f(x2, y1, 0.0f);

	glTexCoord2f(tu0, tv1);	// ����
	glVertex3f(x1, y2, 0.0f);

	glTexCoord2f(tu1, tv1);	// �E��
	glVertex3f(x2, y2, 0.0f);

	glEnd();
	Texture::set(nullptr);
	// ����������OFF
	glDisable(GL_BLEND);
}

void Render::drawScreenTextureUV(
	const Texture& texture,
	const Color& color,
	const Vector2& position0,
	const Vector2& position1,
	const Vector2& uv1,
	const Vector2& uv2
)
{
	// �e�N�X�`���̕��A����
	s32		textureW = texture.getWidth();
	s32		textureH = texture.getHeight();
	f32		x1 = position0._x;
	f32		y1 = position0._y;
	f32		x2 = position1._x;
	f32		y2 = position1._y;

	glColor4ubv((GLubyte*)&color);

	RenderState::setBlendMode(true);

	Texture::set(&texture);

	glBegin(GL_TRIANGLE_STRIP);

	glTexCoord2f(uv1._x, uv1._y);	// ����
	glVertex3f(x1, y1, 0.0f);

	glTexCoord2f(uv2._x, uv1._y);	// �E��
	glVertex3f(x2, y1, 0.0f);

	glTexCoord2f(uv1._x, uv2._y);	// ����
	glVertex3f(x1, y2, 0.0f);

	glTexCoord2f(uv2._x, uv2._y);	// �E��
	glVertex3f(x2, y2, 0.0f);

	glEnd();
	Texture::set(nullptr);
	// ����������OFF
	RenderState::setBlendMode(false);
}


void Render::drawScreenTexture(const Texture& texture, const Color& color, const Vector2& position, float scale = 0.0f)
{
	glDepthMask(false);		// Z�������ݖ���
	// �e�N�X�`���̕��A����
	Vector2 center = position;
	Vector2 right = Vector2(1280.0f, 0.0f) * scale;
	Vector2 up    = Vector2(0.0f, 720.0f) * scale;

	Vector2 v[4];
	v[0] = center - right + up;
	v[1] = center + right + up;
	v[2] = center - right - up;
	v[3] = center + right - up;

	glColor4ubv((GLubyte*)&color);

	// ����������ON
	glEnable(GL_BLEND);
	// �ʏ�u�����h
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Texture::set(&texture);
	glBegin(GL_TRIANGLE_STRIP);
	glColor4ubv((GLubyte*)&color);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[0]);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[1]);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[2]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[3]);
	glEnd();
	Texture::set(nullptr);
	// ����������OFF

	glDepthMask(true);	// Z�������݂����Ƃɖ߂�

	glDisable(GL_BLEND);
}

void Render::drawRect2D(
	const Vector3& position,
	const Vector2& size,
	const Color&   color
) {
	f32	x1 = position._x;
	f32	y1 = position._y;
	f32	x2 = x1 + size._x;
	f32	y2 = y1 + size._y;

	f32 z = position._z;

	RenderState::setBlendMode(true);

	glBegin(GL_TRIANGLE_STRIP);
	{
		glColor4ubv((GLubyte*)&color);

		glVertex3f(x1, y1, z);
		glVertex3f(x2, y1, z);
		glVertex3f(x1, y2, z);
		glVertex3f(x2, y2, z);
	}
	glEnd();

	RenderState::setBlendMode(false);
}

//----
void Render::drawTexture2D(
	const Texture& texture,
	const Vector2& position,
	const Vector2& size,
	const Color&   color
) {
	f32 x1 = position._x;
	f32 y1 = position._y;
	f32 x2 = x1 + size._x;
	f32 y2 = y1 + size._y;

	float z = -10;

	RenderState::setBlendMode(true);

	Texture::set(&texture);

	glBegin(GL_TRIANGLE_STRIP);
	{
		glColor4ubv((GLubyte*)&color);


		//glColor4ub(255,0,0,255);
		glTexCoord2f(0.0f, 1.0f);	// ����
		glVertex2f(x1, y1);

		//glColor4ub(0, 255, 0, 255);
		glTexCoord2f(1.0f, 1.0f);	// �E��
		glVertex2f(x2, y1);

		//glColor4ub(0, 0, 255, 255);
		glTexCoord2f(0.0f, 0.0f);	// ����
		glVertex2f(x1, y2);

		//glColor4ub(255, 255, 255, 255);
		glTexCoord2f(1.0f, 0.0f);	// �E��
		glVertex2f(x2, y2);

	}
	glEnd();

	Texture::set(nullptr);
	RenderState::setBlendMode(false);
}



void Render::drawParticle2D(const Vector2& position, const Texture* texture, const Color& color, f32 scale, f32 angle)
{
	Texture::set(texture);

	Vector2	right  = Vector2(1.0f, 0.0f);
	Vector2	up = Vector2(0.0f, 1.0f);
	right *= scale;
	up	  *= scale;
	
	Vector2	center = position;

	Vector2	v[4];
	v[0] = center - right + up;
	v[1] = center + right + up;
	v[2] = center - right - up;
	v[3] = center + right - up;

	glBegin(GL_TRIANGLE_STRIP);
	glColor4ubv((GLubyte*)&color);

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[0]);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[1]);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[2]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[3]);
	glEnd();

	Texture::set(nullptr);
}

#if 0
void Render::drawParticleBillboard(const Vector3& position, const Color& color, const Vector3& differencePosition, f32  angle, f32 scale)
{
	//-------------------------------------------------------------
	// �r���{�[�h�̕`��e�X�g
	//-------------------------------------------------------------
	//glEnable(GL_ALPHA_TEST);
	glDepthMask(false);

	Camera*	camera = Camera::getActiveCamera();
	Matrix&	matWorld = camera->getMatWorld();

	Vector3	right  = matWorld.axisX()  * scale;
	Vector3	up     = matWorld.axisY()  * scale;
	Vector3 behind = matWorld.axisZ();

	right = right.transformNormal(Matrix::rotateAxis(behind, angle));
	up = up.transformNormal(Matrix::rotateAxis(behind, angle));

	Vector3	center = position;

	up._y -= differencePosition._y;

	Vector3	v[4];
	v[0] = center - right + up;
	v[1] = center + right + up;
	v[2] = center - right - up;
	v[3] = center + right - up;

	glBegin(GL_TRIANGLE_STRIP);
	glColor4ubv((GLubyte*)&color);		// White

	glTexCoord2f(0.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[0]);

	glTexCoord2f(1.0f, 0.0f);
	glVertex3fv((GLfloat*)&v[1]);

	glTexCoord2f(0.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[2]);

	glTexCoord2f(1.0f, 1.0f);
	glVertex3fv((GLfloat*)&v[3]);
	glEnd();

	glDepthMask(true);	// Z�������݂����Ƃɖ߂�

	RenderState::setBlendMode(false);
}
#endif

#if 1
void Render::drawParticleBillboard(const Vector3& position, const Vector3& size, const Color& color)
{
	//-------------------------------------------------------------
	// �r���{�[�h�̕`��e�X�g
	//-------------------------------------------------------------
	//glEnable(GL_ALPHA_TEST);
#if 0
	glDepthMask(false);

	Camera*	camera = Camera::getActiveCamera();
	Matrix&	matWorld = camera->getMatWorld();

	Vector3	right = matWorld.axisX()  * scale;
	Vector3	up = matWorld.axisY()  * scale;
	Vector3 behind = matWorld.axisZ();

	right = right.transformNormal(Matrix::rotateAxis(behind, angle));
	up = up.transformNormal(Matrix::rotateAxis(behind, angle));
#endif

	Vector3	left  = position;
	Vector3 right = Vector3(size._x, 0.0f   , 0.0f);
	Vector3 up	  = Vector3(0.0f   , size._y, 0.0f);

	Vector3	v[4];
	v[0] = left;
	v[1] = left + right;
	v[2] = left + right - up;
	v[3] = left - up;

	glBegin(GL_TRIANGLE_STRIP);
	glColor4ubv((GLubyte*)&color);		// White

	glVertex3fv((GLfloat*)&v[0]);

	glVertex3fv((GLfloat*)&v[1]);

	glVertex3fv((GLfloat*)&v[3]);

	glVertex3fv((GLfloat*)&v[2]);
	glEnd();

	glDepthMask(true);	// Z�������݂����Ƃɖ߂�

	RenderState::setBlendMode(false);
}

#endif


void Render::drawArrorMatrix(const Matrix& m, const float scale)
{
	Vector3 position = m.translate();
	Vector3 x = m.axisX().normalize() * scale;
	Vector3 y = m.axisY().normalize() * scale;
	Vector3 z = m.axisZ().normalize() * scale;

	drawArrow(position, x, Color(255, 0, 0,   0));
	drawArrow(position, y, Color(0,   0, 255, 0));
	drawArrow(position, z, Color(0, 255, 0,   0));
}

void RenderState::setBlendMode(bool enabled)
{
	if (enabled) {
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	else {
		glDisable(GL_BLEND);
	}
}

void RenderState::setDelthTest(bool enabled)
{
	if (enabled) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}
