//===========================================================================
//!
//!	@file	Game.h
//!	@brief	ゲームメインループ
//!
//===========================================================================
#pragma once

class GameManager
{
public:
	GameManager(void);

	virtual ~GameManager(void);

	bool	init(void);

	void	update(void);

	void	render(void);

	void	cleanup(void);

	void	quitGame(void);

	bool	isQuitGame(void) const;

	void menuSelection(
		const Vector2& position0,
		const Vector2& position1,

		const Vector2& position2,
		const Vector2& position3
	);

	void menuRender(
		const Texture& texture,
		const Color& color,
		const Vector2& position0,
		const Vector2& position1,
		const Vector2& tu0 = Vector2(0,0),
		const Vector2& tu = Vector2(1,1)
	);

private:
	bool			_isQuitGame;

	bool			_isOption;
	Texture*		_pTexture;
	bool			_isBlackOut;

	std::array<std::array<Vector2, 2>, 2> _menuSelectionPosition;
	std::array<std::array<Vector2, 2>, 2> _menuRenderPosition;
	std::array<std::array<Vector2, 2>, 2> _menuTextureTv;

	Texture* _menu;

	Texture* _menuSelection0;
};


//bool pushKeyState(int key);
//===========================================================================
//	END OF FILE
//===========================================================================
