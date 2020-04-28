#pragma once

#include <array>

class SceneBase
{
public:
	//! デフォルトコンストラクタ
	SceneBase();
	//! デストラクタ
	virtual ~SceneBase() {};

	//! 初期化
	virtual bool init();
	//! 更新
	virtual void update(){};
	//! 描画
	virtual void render();
	//! 解放
	virtual void cleanup();

	//! シーンが終わったかどうか
	virtual bool sceneEnd() { return _sceneEnd; }

	virtual void setBlackoutAlpha(int alpha)
	{
		_alpha = alpha;
	}
	
protected:
	enum class OBJECTIVE_NUMBER : unsigned int
	{
		WEAPON_PICKUP = 0,
		FOUR_OF_GIMMICKS,
		CENTER_GIMMICK,
		DEFEAT_BOSS,

		MAX_OBJECTIVE
	};

	enum class MENU_SELECTION : int
	{
		GAME_START = 0,
		BACK_TO_TITLE,
		CONTINUE,
		QUIT_GAME,



		MAX_OBJECTIVE
	};

	bool _sceneEnd;
	bool _isOption;

	Texture* _blackoutTexture;
	bool _isBlackOut;
	int  _alpha;

	virtual void menuSelection(
		const Vector2& position0,
		const Vector2& position1,
		const MENU_SELECTION& menu
	);

	void menuRender(
		const MENU_SELECTION& menu,
		const Color& color
	);

	bool blackoutInit(char* const fileName, int alpha);
	void setBlackOutAlpha(int alpha);



	OBJECTIVE_NUMBER _objectiveNum;
	Texture* _objectives[4];
	Texture* _menuSelection[4];

	std::array<std::array<Vector2, 2>, 4> _menuSelectionPosition;
	std::array<std::array<Vector2, 2>, 4> _menuRenderPosition; 
	std::array<std::array<Vector2, 2>, 4> _menuTextureTv;

	Texture* _menuSelection0;
	Texture* _tabMenu;
};