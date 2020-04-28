//クラス型の列挙体
enum class SCENE_TYPE : unsigned int
{
	EMPTY,

	TITLE,
	GAME,
	BOSS_BATTLE,
	MENU,
	GAME_OVER,
	GAME_OVER_BOSS_BATTLE,
	GAME_CLEAR,

	SCENE_MAX
};

//シーンのマネジャー
class SceneManager
{
public:
	static SceneManager& getInstance(void) 
	{
		return _instance; 
	}

	bool init(void)
	{
		_pActiveScene = nullptr;
		_activeSceneType = SCENE_TYPE::EMPTY;
		_nextSceneType = SCENE_TYPE::TITLE;
		//_nextSceneType = SCENE_TYPE::GAME;
		//_nextSceneType = SCENE_TYPE::BOSS_BATTLE;
		//_nextSceneType = SCENE_TYPE::GAME_OVER;
	    //_nextSceneType = SCENE_TYPE::GAME_CLEAR;
		_gameOverCount = 0;
		_bossHitPoint = 70;

		return true;
	}

	void update(void)
	{
		if (_activeSceneType != _nextSceneType) {
			_activeSceneType = _nextSceneType;

			auto pNextScene = createScene();
			if (pNextScene != nullptr) {	
				if (pNextScene->init()) {
					if (_pActiveScene != nullptr) {
						SAFE_DELETE_CLEANUP(_pActiveScene);
					}
					_pActiveScene = pNextScene;
				}
				else {
					SAFE_DELETE_CLEANUP(pNextScene);
				}
			}
		}

		if (_pActiveScene != nullptr) {
			_pActiveScene->update();
		}
	}

	void render(void)
	{
		if (_pActiveScene != nullptr) {
			_pActiveScene->render();
		}
	}

	void cleanup(void)
	{
		if (_pActiveScene != nullptr) {
			SAFE_DELETE_CLEANUP(_pActiveScene);
		}

		_activeSceneType = SCENE_TYPE::EMPTY;
		_nextSceneType = _activeSceneType;
	}

	void jump(SCENE_TYPE type)
	{
		_nextSceneType = type;
	}

	void quitGame()
	{
		_isQuitGame = true;
	}

	bool isQuitGame(void) const
	{
		return _isQuitGame;
	}

	void setPlayerHitPoint(int playerHitPoint)
	{
		_playerHitPoint = playerHitPoint;
		_playerHitPoint = 50;
	}

	void setBossHitPoint(int bossHitPoint)
	{
		_bossHitPoint = bossHitPoint;
		_playerHitPoint = 50;
	}

	SCENE_TYPE isActiveScene()
	{
		return _activeSceneType;
	}

private:
	SceneManager(void) { _isQuitGame = false; }
	virtual ~SceneManager(void) {}

	SceneBase * createScene()
	{
		SceneBase* pNextScene = nullptr;
		switch (_nextSceneType)
		{
		case SCENE_TYPE::TITLE:
			return new SceneTitle();
		case SCENE_TYPE::GAME:
			return new SceneGame();
		case SCENE_TYPE::BOSS_BATTLE:
			_playerHitPoint = 50;
			return new SceneBossBattle(_playerHitPoint + (_gameOverCount * 10), _bossHitPoint - (_gameOverCount * 10));
		case SCENE_TYPE::GAME_OVER:
			return new SceneResult();
		case SCENE_TYPE::GAME_OVER_BOSS_BATTLE:
			_gameOverCount++;
			return new SceneBossBattleResult();
		case SCENE_TYPE::GAME_CLEAR:
			return new SceneGameClear();
		}

		return nullptr;
	}

protected:
	SceneBase * _pActiveScene;
	SCENE_TYPE  _activeSceneType;
	SCENE_TYPE  _nextSceneType;
	bool _isQuitGame;

private:
	static SceneManager _instance;
	int _playerHitPoint;
	int _bossHitPoint;
	int _gameOverCount;
};

inline SceneManager& scene()
{
	return SceneManager::getInstance();
}
