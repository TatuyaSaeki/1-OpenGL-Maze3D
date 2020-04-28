#pragma once

class BossKeyItem : public KeyItem
{
public:
	BossKeyItem();
	virtual ~BossKeyItem();

	bool init()	   override;
	void update()  override;
	void render()  override;
	void cleanup() override;

	void setAlpha(float alpha)
	{
		_alpha = alpha;
	}

private:
	float _alpha;
};