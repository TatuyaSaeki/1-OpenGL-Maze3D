#pragma once

class ItemManager;

class EnemyManager
{
public:
	//! デフォルトコンストラクタ
	EnemyManager();
	//! デストラクタ
	~EnemyManager();

	//! 初期化
	bool init();
	//! 更新
	void update();

	void update(ItemManager* itemManager);
	void lateUpdate();

	//! 描画
	void render();
	//! 解放
	void cleanup();

	//! 敵をvectorに追加
	void addNomalEnemy( NomalEnemy* nomalEnemy );
	void addBossEnemy(BossEnemy* bossEnemy);

	std::list<NomalEnemy*>& getNomalEnemys() { return _nomalEnemy; };
	std::vector<BossEnemy*>& getBossEnemys() { return _bossEnemy; };

	bool bossEnemyIsDead()
	{
		return _bossEnemy[0]->isDead();
	}

	EnemyState getDeadState()
	{
		return _bossEnemy[0]->getState();
	}

private:
	// 敵全体を保持する配列
	std::list<NomalEnemy*> _nomalEnemy;
	std::vector<BossEnemy*> _bossEnemy;
};