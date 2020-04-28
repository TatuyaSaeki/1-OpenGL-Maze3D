#pragma once

class KeyInformation
{
public:
	KeyInformation();
	~KeyInformation();

	void update();
	void render();
	void cleanup();

	void setPlaeyr( Player& player);
	void setMapObject(MapObjectManager& mapObj);

private:
	Player*			  _pPlayer;
	MapObjectManager* _pMapObjectManager;

	enum class KEY_INFO : int
	{
		NONE		   = 0,
		PICK_UP_WEAPON,
		START_UP_GIMMICK,
		AVOIDANCE,
		ATTACK,
		MOVE_LOOKAT,

		MENU,

		MAX,
	};

	KEY_INFO _keyInformation;
	Texture* _keyInformationTex;
	Texture* _keyInformationTex2;

	bool moveLookAt;

	//void keyInformation(KEY_INFO keyInfo, const Vector2& uv);
};