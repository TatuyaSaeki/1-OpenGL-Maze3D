#pragma once

class Sword : public WeaponBase
{
public:
	//! �f�t�H���g�R���X�g���N�^
	Sword();

	//! �f�X�g���N�^
	virtual	~Sword();

	//! ������
	bool init(const ObjectType objType, const Line& line);

	//! �X�V
	void update(const Matrix& matParent);
	//! �`��
	void render() override;
	void render(int alpha);

	//! ���
	void cleanup() override;



private:
	float _rot;
};