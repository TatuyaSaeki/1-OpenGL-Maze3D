#pragma once

class DummyKeyItem : public KeyItem
{
public:
	DummyKeyItem();
	virtual ~DummyKeyItem();

	virtual bool init()		override;
	virtual void update()	override;
	virtual void render()	override;
	virtual void cleanup()	override;

private:
};