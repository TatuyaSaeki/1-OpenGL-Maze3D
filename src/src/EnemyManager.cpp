#include "stdafx.h"

//===========================================================================
//! �f�t�H���g�R���X�g���N�^
//===========================================================================
EnemyManager::EnemyManager()
{
}

//===========================================================================
//! �f�X�g���N�^
//===========================================================================
EnemyManager::~EnemyManager()
{

}

//===========================================================================
//! ������
//===========================================================================
bool EnemyManager::init()
{

	return true;
}

//===========================================================================
//! �X�V
//===========================================================================
void EnemyManager::update()
{
	auto itr = _nomalEnemy.begin();
	while (itr != _nomalEnemy.end()) {
		if ((*itr)->isDead()) {
			(*itr)->cleanup();
			delete *itr;
			itr = _nomalEnemy.erase(itr);
		}
		else {
			(*itr)->update();
			++itr;
		}
	}

	//removeif;

	//auto removelist = _normal.remove_if([](Enamy * obj) {
	//	return obj.isdeade();
	//});
	//remov
	//
	//for (auto& enemy : _nomalEnemy ) {
	//
	//}



	auto itr1 = _bossEnemy.begin();
	while (itr1 != _bossEnemy.end()) {
		if ((*itr1)->isDead()) {
			(*itr1)->cleanup();
			delete *itr1;
			itr1 = _bossEnemy.erase(itr1);
		}
		else {
			(*itr1)->update();
			++itr1;
		}
	}
}

//===========================================================================
//! �X�V
//===========================================================================
void EnemyManager::update(ItemManager* itemManager)
{
#if 1
	auto itr = _nomalEnemy.begin();
	while( itr != _nomalEnemy.end() ){
		if( (*itr)->isDead() ){
			// �h���b�v�A�C�e��
			DropItem* dropItem = new DropItem(1.0f);
			dropItem->init();
			dropItem->setPosition((*itr)->getDeathPosition());
			dropItem->setColor(Color(0, 255, 0));
			itemManager->addDropItem(dropItem);
			//SAFE_DELETE(dropItem);
	
			// ���
			(*itr)->cleanup();
			delete *itr;
			itr = _nomalEnemy.erase(itr);
		}else{
			(*itr)->update();
			++itr;
		}
	}
#endif

	auto itr1 = _bossEnemy.begin();
	while (itr1 != _bossEnemy.end()) {
		if ((*itr1)->isDead()) {
				
			auto itr = _nomalEnemy.begin();
			while(itr != _nomalEnemy.end()) { 
				(*itr)->state(EnemyState::Dead);
				++itr;
			}
		
				Vector3 position(Vector3(0.0f, 0.0f, 0.0f));
				auto* keyItem = new BossKeyItem();
				if (keyItem == nullptr) return;
			if (!keyItem->init())	return;
			keyItem->setPosition(position);
			itemManager->addBossKeyItem(keyItem);
		

			(*itr1)->cleanup();
			delete *itr1;
			itr1 = _bossEnemy.erase(itr1);
		}
		else {
			(*itr1)->update();
			++itr1;
		}
	}
}

void EnemyManager::lateUpdate()
{
	for(auto& bossEnemy : _bossEnemy) {
		bossEnemy->lateUpdate();
	}
}

//===========================================================================
//! �`��
//===========================================================================
void EnemyManager::render()
{
	for (auto nomalEnemy : _nomalEnemy) {
		nomalEnemy->render();
	}

	for (auto bossEnemy : _bossEnemy) {
		bossEnemy->render();
	}
}

//===========================================================================
//! ���
//===========================================================================
void EnemyManager::cleanup()
{
	for( auto& nomalEnemy : _nomalEnemy){
		SAFE_DELETE_CLEANUP(nomalEnemy);
	}

	for (auto& bossEnemy : _bossEnemy) {
		SAFE_DELETE_CLEANUP(bossEnemy);
	}
}

//===========================================================================
//! �G��vector�ɒǉ�
//===========================================================================
void EnemyManager::addNomalEnemy( NomalEnemy* nomalEnemy )
{
	_nomalEnemy.push_back(nomalEnemy);
}

//===========================================================================
//! �G��vector�ɒǉ�
//===========================================================================
void EnemyManager::addBossEnemy(BossEnemy* bossEnemy)
{
	_bossEnemy.push_back(bossEnemy);
}