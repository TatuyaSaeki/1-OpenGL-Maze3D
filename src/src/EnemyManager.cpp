#include "stdafx.h"

//===========================================================================
//! デフォルトコンストラクタ
//===========================================================================
EnemyManager::EnemyManager()
{
}

//===========================================================================
//! デストラクタ
//===========================================================================
EnemyManager::~EnemyManager()
{

}

//===========================================================================
//! 初期化
//===========================================================================
bool EnemyManager::init()
{

	return true;
}

//===========================================================================
//! 更新
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
//! 更新
//===========================================================================
void EnemyManager::update(ItemManager* itemManager)
{
#if 1
	auto itr = _nomalEnemy.begin();
	while( itr != _nomalEnemy.end() ){
		if( (*itr)->isDead() ){
			// ドロップアイテム
			DropItem* dropItem = new DropItem(1.0f);
			dropItem->init();
			dropItem->setPosition((*itr)->getDeathPosition());
			dropItem->setColor(Color(0, 255, 0));
			itemManager->addDropItem(dropItem);
			//SAFE_DELETE(dropItem);
	
			// 解放
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
//! 描画
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
//! 解放
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
//! 敵をvectorに追加
//===========================================================================
void EnemyManager::addNomalEnemy( NomalEnemy* nomalEnemy )
{
	_nomalEnemy.push_back(nomalEnemy);
}

//===========================================================================
//! 敵をvectorに追加
//===========================================================================
void EnemyManager::addBossEnemy(BossEnemy* bossEnemy)
{
	_bossEnemy.push_back(bossEnemy);
}