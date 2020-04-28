#pragma once

class Sword : public WeaponBase
{
public:
	//! デフォルトコンストラクタ
	Sword();

	//! デストラクタ
	virtual	~Sword();

	//! 初期化
	bool init(const ObjectType objType, const Line& line);

	//! 更新
	void update(const Matrix& matParent);
	//! 描画
	void render() override;
	void render(int alpha);

	//! 解放
	void cleanup() override;



private:
	float _rot;
};