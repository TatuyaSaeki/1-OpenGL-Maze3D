#pragma once

class ItemManager;

class EnemyManager
{
public:
	//! �f�t�H���g�R���X�g���N�^
	EnemyManager();
	//! �f�X�g���N�^
	~EnemyManager();

	//! ������
	bool init();
	//! �X�V
	void update();

	void update(ItemManager* itemManager);
	void lateUpdate();

	//! �`��
	void render();
	//! ���
	void cleanup();

	//! �G��vector�ɒǉ�
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
	// �G�S�̂�ێ�����z��
	std::list<NomalEnemy*> _nomalEnemy;
	std::vector<BossEnemy*> _bossEnemy;
};