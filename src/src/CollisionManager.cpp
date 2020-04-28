#include "stdafx.h"
#include "ObjectBase.h"


CollisionManager::CollisionManager()
	: _pPlayer(nullptr)
	, _pEnemyManager(nullptr)
	, _pWorldWeaponManager(nullptr)
	, _pMapObjectManager(nullptr)
{
	_debugFlag = false;
}

CollisionManager::~CollisionManager()
{
}

bool CollisionManager::init(
	Player* pPlayer,
	EnemyManager* pEnemyManager,
	MapObjectManager* pMapObjectManager,
	WeaponManager* pWorldWeapon,
	ItemManager* pItemManager
)
{
	_pPlayer = pPlayer;
	_pEnemyManager = pEnemyManager;
	_pMapObjectManager = pMapObjectManager;
	_pWorldWeaponManager = pWorldWeapon;
	_pItemManager = pItemManager;

	_damageSound = new Sound();
	if (_damageSound == nullptr) return false;
	_damageSound->loadFile(L".\\data\\Sound\\damage4.mp3");
	_damageSound->volumeDown(2000);

	

	return true;
}

void CollisionManager::update()
{
	// �v���C���[
	const auto matChar = _pPlayer->getMatrix();
	auto player = _pPlayer->getCollision();
	Vector3 p0  = player->_line._position[0].transform(matChar);
	Vector3 p1  = player->_line._position[1].transform(matChar);
	Capsule playerCapsule(p0, p1, player->_radius);

	// �v���C���[�ƕ�
	playerCollisionWall();
	// �G�l�~�[�ƕ�
	enemyCollisionWall();

	// �v���C���[�Ɨ����Ă��镐��
	playerCollisionWalrdWeapon(playerCapsule);

	// �v���C���[�̕���ƓG
	playerWeaponCollisionNomalEnemy();

	// �G�̕���ƃv���C���[
	nomalEnemyWeaponCollisionPlayer(playerCapsule);

	// �v���C���[�ƃL�[�A�C�e��
	playerCollisionKeyAitem(playerCapsule);

	// �{�X�̍U���ƃv���C���[
	bossEnemyAttackCollisionPlayer(playerCapsule);

	// �m�[�}���G�l�~�[�ƃm�[�}���G�l�~�[�̏Փ˔���
	nomalEnemyCollisionNomalEnemy();

	// �v���C���[�ƃm�[�}���G�l�~�[�̏Փ˔���
	playerCollisionNomalEnemy(playerCapsule);

#if 0
	

	

	// �v���C���[�Ɨ����Ă��镐��
	playerCollisionWalrdWeapon(playerCapsule);

	// �v���C���[�̕���ƓG
	playerWeaponCollisionNomalEnemy();

	// �G�̕���ƃv���C���[
	nomalEnemyWeaponCollisionPlayer(playerCapsule);

	// �{�X�̍U���ƃv���C���[
	bossEnemyAttackCollisionPlayer(playerCapsule);



	// �G�l�~�[�ƕ�
	enemyCollisionWall();

	// �v���C���[�Ɠ�
	playerCollisionTower(playerCapsule);

	// �v���C���[�Ə�
	playerCollisionFloor(playerCapsule);

	// �v���C���[�ƃL�[�A�C�e��
	playerCollisionKeyAitem(playerCapsule);




#endif


	//playerCollisionDumyWall2(playerCapsule);

#if 1
	// �v���C���[���G�̍��G�͈͓��ɂ��邩
	{
		//int count = 0;
		Sphere* sphere1 = new Sphere(_pPlayer->getPosition(), _pPlayer->getCollision()->_radius);
	//	sphere1->_position._y = 0.0f;
		for (auto* enemy : _pEnemyManager->getNomalEnemys()) {
			

			Sphere* sphere2 = new Sphere(enemy->getPosition(), enemy->getSearchRange());
		//	sphere2->_position._y = 0.0f;
			if (Collision::isHit(*sphere1, *sphere2)) {
				// �������Ă����痼���ɒʒm	
				// �K������
				enemy->isSearc(true);
				enemy->setPlayerPosition(_pPlayer->getPosition());
			}
			else {
				enemy->isSearc(false);
			}
			SAFE_DELETE(sphere2);
		}

		for (auto* enemy : _pEnemyManager->getBossEnemys()) {
			Sphere* sphere2 = new Sphere(enemy->getPosition(), enemy->getSearchRange());
		//	sphere2->_position._y = 0.0f;
			if (Collision::isHit(*sphere1, *sphere2)) {
				// �������Ă����痼���ɒʒm	
				// �K������
				enemy->isSearc(true);
				enemy->setPlayerPosition(_pPlayer->getPosition());
			}
			else {
				enemy->isSearc(false);
			}
			SAFE_DELETE(sphere2);
		}
		SAFE_DELETE(sphere1);
	}
#endif
}






//======================================================================================
//! ���
//======================================================================================
void CollisionManager::cleanup()
{
	SAFE_DELETE_CLEANUP(_damageSound);

}


//======================================================================================
//! �v���C���[�ƃm�[�}���G�l�~�[�̏Փ˔��� �ꎞ�I�ɋ��Ƌ��̔���
//======================================================================================
void CollisionManager::playerCollisionNomalEnemy(const Capsule& capsule)
{
	// Player vs Enemy
	for (auto& enemy : _pEnemyManager->getNomalEnemys()) {
		if(enemy->getState() == EnemyState::Dead) continue;
		const auto matChara1 = enemy->getMatrix();
		auto enem = enemy->getCollision();
		Vector3 p2 = enem->_line._position[0].transform(matChara1);
		Vector3 p3 = enem->_line._position[1].transform(matChara1);
		Capsule capsule0(p2, p3, enem->_radius);

		Vector3 extrusion0(Vector3::ZERO);
		Vector3 extrusion1(Vector3::ZERO);

		if (Collision::Collision2Capsule(capsule, capsule0, extrusion0, extrusion1)) {
			// �����o���ʉ��Z
			enemy->addExtrusion(extrusion1);
			_pPlayer->addExtrusion(extrusion0);
			// �ʒm
			enemy->setHitObjectType(_pPlayer->getObjectType());
			_pPlayer->setHitObjectType(enemy->getObjectType());
		}
	}
}


//======================================================================================
//! �m�[�}���G�l�~�[�ƃm�[�}���G�l�~�[�̏Փ˔���
//======================================================================================
void CollisionManager::nomalEnemyCollisionNomalEnemy()
{
	// Enemy vs Enemy
	auto itr1 = _pEnemyManager->getNomalEnemys().begin();
	while (itr1 != _pEnemyManager->getNomalEnemys().end()) {
		if ((*itr1)->getState() == EnemyState::Dead) {
			++itr1;
			continue;
		}
		Sphere* enemySphere1 = new Sphere((*itr1)->getPosition(), (*itr1)->getCollision()->_radius);
		auto itr2 = itr1;
		++itr2;

		while (itr2 != _pEnemyManager->getNomalEnemys().end()) {
			if ((*itr2)->getState() == EnemyState::Dead) {
				++itr2;
				continue;
			}
			Sphere* enemySphere2 = new Sphere((*itr2)->getPosition(), (*itr2)->getCollision()->_radius);
			// �����蔻�� & �������Ă����牟���o���ʂ�Ԃ�
			if (Collision::isHit(*enemySphere1, *enemySphere2)) {
				// �����o����
				Vector3 extrusion1(Vector3::ZERO);
				Vector3 extrusion2(Vector3::ZERO);

				Collision::adjustPosition(*enemySphere1, *enemySphere2, extrusion1, extrusion2);

				// �����o���ʉ��Z
				(*itr2)->addExtrusion(extrusion2);
				(*itr1)->addExtrusion(extrusion1);
				// �ʒm
				(*itr2)->setHitObjectType((*itr1)->getObjectType());
				(*itr1)->setHitObjectType((*itr2)->getObjectType());
			}
			SAFE_DELETE(enemySphere2);
			++itr2;
		}
		++itr1;
		SAFE_DELETE(enemySphere1);
	}
}

//======================================================================================
//! �v���C���[�Ɨ����Ă��镐��
//======================================================================================
void CollisionManager::playerCollisionWalrdWeapon(const Capsule& capsule)
{
	//�v���C���[�ƃ��[���h�ɗ����Ă��镐��
	for (auto warldWeapon : _pWorldWeaponManager->getWorldWeapons()) {

		const auto matWeapon = warldWeapon->getMatrix();
		auto weapon = warldWeapon->getCollision();
		Vector3 p2 = weapon->_line._position[0].transform(matWeapon);
		Vector3 p3 = weapon->_line._position[1].transform(matWeapon);
		Capsule capsule1(p2, p3, weapon->_radius + 3.0f);

		// �J�v�Z���ƃJ�v�Z���̓����蔻��
		if (Collision::Collision2Capsule(capsule, capsule1)) {
			// �������Ă����痼���ɒʒm	
			_pPlayer->addHitObjectType(warldWeapon->getObjectType());
		}
		// �v���C���[������
		if (_pPlayer->getWeapon()) {
			warldWeapon->setHaveWeapon(true);
		}
	}
}

//======================================================================================
//! �v���C���[�̕���ƃm�[�}���G�l�~�[
//======================================================================================
void CollisionManager::playerWeaponCollisionNomalEnemy()
{
	if (_pPlayer->getWeapon()) {
		if (_pPlayer->getState3() == PlayerState::Attack) {
			// �v���C���[������
			const auto matWeapon = _pPlayer->getArmMatrix();
			auto weapon = _pPlayer->getWeaponInfo()->getCollision();
			Vector3 p2 = weapon->_line._position[0].transform(matWeapon);
			Vector3 p3 = weapon->_line._position[1].transform(matWeapon);
			Capsule weaponCapsule(p2, p3, weapon->_radius);
			
			// NomalEnemy
			for (auto& enemy : _pEnemyManager->getNomalEnemys()) {
				if (!enemy->getIsSearc()) continue;
				if(enemy->getState() == EnemyState::Dead) continue;
				const auto matEnemy = enemy->getMatrix();
				auto cap			= enemy->getCollision();
				Vector3 p0 = cap->_line._position[0].transform(matEnemy);
				Vector3 p1 = cap->_line._position[1].transform(matEnemy);
				Capsule capsule1(p0, p1, cap->_radius);

				if (Collision::Collision2Capsule(weaponCapsule, capsule1)) {
					_damageSound->playRoop();
					enemy->setHitObjectType(ObjectType::PlayerWeapon);
					return;
				}
			}

			// BossEnemy
			for (auto bossEnemy : _pEnemyManager->getBossEnemys()) {
				if (bossEnemy->getState() == EnemyState::Dead) continue;
				const auto matEnemy = bossEnemy->getMatrix();
				auto cap = bossEnemy->getCollision();
				Vector3 p0 = cap->_line._position[0].transform(matEnemy);
				Vector3 p1 = cap->_line._position[1].transform(matEnemy);
				Capsule capsule1(p0, p1, cap->_radius);

				if (Collision::Collision2Capsule(weaponCapsule, capsule1)) {
					bossEnemy->setHitObjectType(ObjectType::PlayerWeapon);
					return;
				}
			}
		}
	}
}

//======================================================================================
//! �G�̕���ƃv���C���[
//======================================================================================
void CollisionManager::nomalEnemyWeaponCollisionPlayer(const Capsule& capsule)
{
	for (auto enemy : _pEnemyManager->getNomalEnemys()) {
		if(!enemy->getIsSearc()) continue;
		if (enemy->getState() == EnemyState::Attack) {
			// �G�l�~�[����
			const auto matWeapon = enemy->getWeaponInfo()->getMatrix();
			auto weapon			 = enemy->getWeaponInfo()->getCollision();
			Vector3 p0			 = weapon->_line._position[0].transform(matWeapon);
			Vector3 p1			 = weapon->_line._position[1].transform(matWeapon);
			Capsule capsule0(p0, p1, weapon->_radius);

			if (Collision::Collision2Capsule(capsule0, capsule)) {
				_pPlayer->setHitObjectType(ObjectType::EnemyWeapon);
			}
		}
	}
}

//======================================================================================
//! �{�X�G�l�~�[�̍U���ƃv���C���[
//======================================================================================
void CollisionManager::bossEnemyAttackCollisionPlayer(const Capsule& capsule)
{
	for (auto& bossEnemy : _pEnemyManager->getBossEnemys()) {
		// ShortAttack
		if (bossEnemy->getState() == EnemyState::ShortAttack) {
			const auto matrix = bossEnemy->getMatrix();
			auto attackSphere = bossEnemy->shortAttackSphere();
			Vector3 p0 = attackSphere->_position;
			Sphere sphere1(p0, attackSphere->_radius);

			Vector3 p1 = capsule._line.getPosition(0.5f);
			Sphere sphere0(p1, 3.0f);

			Vector3 ex0 = Vector3::ZERO;
			Vector3 ex1 = Vector3::ZERO;

			if (Collision::adjustPosition(sphere0, sphere1, ex0, ex1, 1.0f, 0.0f)) {
				_pPlayer->setHitObjectType(ObjectType::BossShortAttack);
				_pPlayer->addPosition(ex0);
				break;
			}
		}

		// LongAttack
		if (bossEnemy->getState2() == EnemyState::LongAttack2) {
			auto playerSphere =  Sphere(capsule._line.getPosition(0.5f), capsule._radius);
			auto longAttack = bossEnemy->getLongAttack2();
	
			for(auto& magic : longAttack) { 
				if(magic->_isMagic) { 
					auto sphere =  Sphere(magic->_longAttack2MagicPosition, magic->_magicRadius);
					if(Collision::isHit(playerSphere, sphere)) {
						_pPlayer->addHitObjectType(ObjectType::BossLongAttackMagic);
					}
				}

				if(magic->_isAlive) {
					auto sphere =  Sphere(magic->_longAttack2Sphere._position, magic->_longAttack2Sphere._radius);
					if (Collision::isHit(playerSphere, sphere)) {
						_pPlayer->addHitObjectType(ObjectType::BossLongAttack);
					}
				}
			}
		}
	}
}


//======================================================================================
//! �v���C���[�ƕ�
//======================================================================================
void CollisionManager::playerCollisionWall()
{
#if 0
	auto playerCapsule = capsule;

	Vector3 nearestHitPosition = Vector3::ZERO;
	Vector3 nearestHitNormal = Vector3::ZERO;
	f32		nearestT;
	Vector3 front = _pPlayer->getDirection();

	Vector3 hitNormal = Vector3::ZERO;
	int hitCount = 0;

	_hitTriangle.clear();

	for (auto& triangle : _pMapObjectManager->getMazeWall()) {

		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.1f),
			front.normalize(),
			-1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		hitNormal += nearestHitNormal;
		hitCount++;
	}

	if (hitCount != 0) {
		//		hitNormal = hitNormal / hitCount;
		Vector3  extruded = Collision::wallShear(front, hitNormal, 2.0f);
		_pPlayer->addPosition(extruded);

		// �v���C���[
		const auto matChar = _pPlayer->getMatrix();
		auto player = _pPlayer->getCollision();
		playerCapsule._line._position[0] = player->_line._position[0].transform(matChar);
		playerCapsule._line._position[1] = player->_line._position[1].transform(matChar);
		hitCount  = 0;

		nearestHitPosition = Vector3::ZERO;
		nearestHitNormal = Vector3::ZERO;
		nearestT;
		front = _pPlayer->getDirection();

		hitNormal = Vector3::ZERO;
	}

	for (auto& triangle : _pMapObjectManager->getDumyWall()) {

		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.3f),
			front.normalize(),
			-1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		hitNormal += nearestHitNormal;
		hitCount++;
	}

	if (hitCount != 0) {
		//		hitNormal = hitNormal / hitCount;
		Vector3  extruded = Collision::wallShear(front, hitNormal, 2.0f);
		_pPlayer->addPosition(extruded);

		// �v���C���[
		const auto matChar = _pPlayer->getMatrix();
		auto player = _pPlayer->getCollision();
		playerCapsule._line._position[0] = player->_line._position[0].transform(matChar);
		playerCapsule._line._position[1] = player->_line._position[1].transform(matChar);
	}

#if 0
	while (_hitTriangle.size() > 0) {
		auto itr = _hitTriangle.begin();
		while (itr != _hitTriangle.end()) {
			if (!Collision::isHit(
				capsule,
				**itr,
				capsule._line.getPosition(0.1f),
				front.normalize(),
				-1.0f,
				&nearestHitPosition,
				&nearestHitNormal,
				&nearestT)
				)
			{
				//delete *itr;
				_hitTriangle.erase(itr);
				++itr;


			}
			else {
				// �������Ă�����
				// �i�s�x�N�g��
				// �����o���l
				Vector3  extruded = Collision::wallShear(front, nearestHitNormal, 2.0f);
				// ���������I�u�W�F�N�g��ʒm
				_pPlayer->setHitObjectType(ObjectType::Wall);
				//_pPlayer->addExtrusion(extruded);
				_pPlayer->addPosition(extruded);
				_pPlayer->setVelocity(0.0f);

				++itr;
			}
		}
	}
#else

	//while (_hitTriangle.size() > 0) {
	//	for (auto itr = _hitTriangle.begin(); itr != _hitTriangle.end(); ) {
	//		// �v���C���[
	//		const auto matChar = _pPlayer->getMatrix();
	//		auto player = _pPlayer->getCollision();
	//		Vector3 p0 = player->_line._position[0].transform(matChar);
	//		Vector3 p1 = player->_line._position[1].transform(matChar);
	//		Capsule playerCapsule(p0, p1, player->_radius);
	//
	//		
	//
	//		// �������Ă�����
	//		// �i�s�x�N�g��
	//		// �����o���l
	//		Vector3  extruded = Collision::wallShear(front, nearestHitNormal, 2.0f);
	//		// ���������I�u�W�F�N�g��ʒm
	//		_pPlayer->setHitObjectType(ObjectType::Wall);
	//		//_pPlayer->addExtrusion(extruded);
	//		_pPlayer->addPosition(extruded);
	//		_pPlayer->setVelocity(0.0f);
	//
	//
	//
	//		++itr;
	//	}
	//}
#endif

	

//	auto itr = _hitTriangle.begin();
//	while (itr != _hitTriangle.end()) {
//		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
//		if (!Collision::isHit(
//			capsule,
//			(itr),
//			capsule._line.getPosition(0.1f),
//			front,
//			-1.0f,
//			&nearestHitPosition,
//			&nearestHitNormal,
//			&nearestT)
//			)
//		{
//			_hitTriangle.erase(itr);
//			++itr;
//			continue;
//		}
//		// �������Ă�����
//		// �i�s�x�N�g��
//		Vector3 front = _pPlayer->getDirection();
//		// �����o���l
//		Vector3  extruded = Collision::wallShear(front, nearestHitNormal, 2.0f);
//		// ���������I�u�W�F�N�g��ʒm
//		_pPlayer->setHitObjectType(ObjectType::Wall);
//		//_pPlayer->addExtrusion(extruded);
//		_pPlayer->setPosition(_pPlayer->getPosition() + extruded);
//		_pPlayer->setVelocity(0.0f);
//	}

	hitNormal = Vector3::ZERO;
	hitCount = 0;

	for (auto& triangle : _pMapObjectManager->getMazeWall2()) {
		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.1f),
			front,
			-1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		hitNormal += nearestHitNormal;
		hitCount++;
	}

	if (hitCount != 0) {
		Vector3  extruded = Collision::wallShear(front, hitNormal, 2.0f);
		_pPlayer->addPosition(extruded);

		// �v���C���[
		const auto matChar = _pPlayer->getMatrix();
		auto player = _pPlayer->getCollision();
		playerCapsule._line._position[0] = player->_line._position[0].transform(matChar);
		playerCapsule._line._position[1] = player->_line._position[1].transform(matChar);
	}

#endif

	Vector3 newPosition  = _pPlayer->getNewPosition();
	Vector3 lastPosition = _pPlayer->getLastPosition();
	
	Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getMazeWall());

	Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getMazeWall2(), 5.0f);
	Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getDumyWall());



	
	// �n�ʂ̔���
	for (auto& triangle : _pMapObjectManager->getSlope()) {

		// ���ʏ�̍��������߂�
		Vector3	p = newPosition;
		p._y = Collision::getHeight(*triangle, p._x, p._z) + 0.0001f;

		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (Collision::isInside(*triangle, p) == false) continue;

		if (newPosition._y + 10.0f > p._y && p._y > newPosition._y - 5.0f && p._y > 0.1f)
		{
			_pPlayer->setVelocity(0.0f);
			newPosition = p;
			break;
		}
	}

	float v = _pPlayer->getVelocity();
	Collision::adjustPositionTower(newPosition, lastPosition, _pMapObjectManager->getTower(), v);
	_pPlayer->setVelocity(v);

	_pPlayer->setPosition(newPosition);
}

//======================================================================================
//! �v���C���[�ƕ�
//======================================================================================
void CollisionManager::playerCollisionDumyWall2(const Capsule& capsule)
{
	auto playerCapsule = capsule;

	Vector3 nearestHitPosition = Vector3::ZERO;
	Vector3 nearestHitNormal = Vector3::ZERO;
	f32		nearestT;
	Vector3 front = _pPlayer->getDirection();

	Vector3 hitNormal = Vector3::ZERO;
	int hitCount = 0;

	_hitTriangle.clear();

	for (auto& triangle : _pMapObjectManager->getDumyWall2()) {
		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.1f),
			front,
			-1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		int a = 0;
	}
}

//======================================================================================
//! �G�l�~�[�ƕ�
//======================================================================================
void CollisionManager::enemyCollisionWall()
{
#if 0
	for (auto& enemy : _pEnemyManager->getNomalEnemys()) {
		if(enemy->getState() == EnemyState::Dead) continue;
		const auto matChara = enemy->getMatrix();
		auto enemyCollision = enemy->getCollision();
		Vector3 p0 = enemyCollision->_line._position[0].transform(matChara);
		Vector3 p1 = enemyCollision->_line._position[1].transform(matChara);
		Capsule capsule(p0, p1, enemyCollision->_radius);
		Vector3 nearestHitPosition	= Vector3::ZERO;
		Vector3 nearestHitNormal	= Vector3::ZERO;
		f32		nearestT = 0.0f;

		Vector3 dir = enemy->getDir().normalize();

		for (auto& triangle : _pMapObjectManager->getMazeWall()) {
			// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
			if (!Collision::isHit(
				capsule,
				*triangle,
				capsule._line.getPosition(0.1f),
				dir,
				-1.0f,
				&nearestHitPosition,
				&nearestHitNormal,
				&nearestT)
				) continue;

			// �i�s�x�N�g��
			Vector3 front = enemy->getDir();
			// �����o���l
			Vector3  extruded = Collision::wallShear(front, nearestHitNormal, 2.0f);
			// ���������I�u�W�F�N�g��ʒm
			enemy->setHitObjectType(ObjectType::Wall);
			enemy->addExtrusion(extruded);
			enemy->setVelocity(0.0f);
		}

		nearestHitPosition = Vector3::ZERO;
		nearestHitNormal = Vector3::ZERO;
		nearestT = 0.0f;

		for (auto& triangle : _pMapObjectManager->getMazeWall2()) {
			// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
			if (!Collision::isHit(
				capsule,
				*triangle,
				capsule._line.getPosition(0.1f),
				dir,
				-1.0f,
				&nearestHitPosition,
				&nearestHitNormal,
				&nearestT)
				) continue;

			// �i�s�x�N�g��
			Vector3 front = enemy->getDir();
			// �����o���l
			Vector3  extruded = Collision::wallShear(front, nearestHitNormal, 2.0f);
			// ���������I�u�W�F�N�g��ʒm
			enemy->setHitObjectType(ObjectType::Wall);
			enemy->addExtrusion(extruded);
			enemy->setVelocity(0.0f);
		}
	}
#endif

	for (auto& enemy : _pEnemyManager->getNomalEnemys()) {
		if (!enemy->getFlag()) continue;
		if (enemy->getState() == EnemyState::Dead) continue;

		Vector3 newPosition = enemy->getNewPosition();
		Vector3 lastPosition = enemy->getLastPosition();

		Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getMazeWall());
		Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getMazeWall2());
		Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getTower());
		Collision::adjustPosition(newPosition, lastPosition, _pMapObjectManager->getDumyWall());

		// �n�ʂ̔���
		for (auto& triangle : _pMapObjectManager->getSlope()) {

			// ���ʏ�̍��������߂�
			Vector3	p = newPosition;
			p._y = Collision::getHeight(*triangle, p._x, p._z) + 0.0001f;

			// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
			if (Collision::isInside(*triangle, p) == false) continue;

			if (newPosition._y + 10.0f > p._y && p._y > newPosition._y - 5.0f && p._y > 0.1f)
			{
				enemy->setVelocity(0.0f);
				newPosition = p;
				break;
			}
		}

		enemy->setPosition(newPosition);
	}
}

//======================================================================================
//! �v���C���[�Ə�
//======================================================================================
void CollisionManager::playerCollisionFloor(const Capsule& capsule)
{
#if 0
	float distance = Collision::distance(capsule._line._position[0], nearestHitPosition, 0.0f, 0.0f);

	Vector3 reject;
	// �ǂ̖@���x�N�g��
	Vector3 normal = Vector3::cross(
		triangle._position[1] - triangle._position[0],
		triangle._position[2] - triangle._position[0]
	).normalize();

	reject = normal * (capsule._radius - distance);

	//_pPlayer->setHitObjectType(ObjectType::Wall);
	_pPlayer->addExtrusion(capsule._line._position[0] + reject);
#endif
	auto playerCapsule = capsule;
	// �v���C���[
	const auto matChar = _pPlayer->getMatrix();
	auto player = _pPlayer->getCollision();
	playerCapsule._line._position[0] = player->_line._position[0].transform(matChar);
	playerCapsule._line._position[1] = player->_line._position[1].transform(matChar);

	Vector3 collisionPosition = capsule._line.getPosition(0.1f);
	Vector3 nearestHitPosition = Vector3::ZERO;
	Vector3 nearestHitNormal = Vector3::ZERO;
	f32		nearestT;
	Vector3 front = _pPlayer->getDirection();

	Vector3 hitNormal = Vector3::ZERO;
	int hitCount = 0;

	//collisionPosition = capsule._line.getPosition(0.01f);
	for (auto& triangle : _pMapObjectManager->getSlope()) {


		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.1f),
			front.normalize(),
			1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		hitNormal += nearestHitNormal;
		hitCount++;
	}

	if (hitCount != 0) {
		Vector3  extruded = Collision::wallShear(front, hitNormal, 2.0f);
		_pPlayer->addPosition(extruded);
		_pPlayer->setVelocity(0.0f);
	}
}

//======================================================================================
//! �v���C���[�ƃ^���[
//======================================================================================
void CollisionManager::playerCollisionTower(const Capsule& capsule)
{
	auto playerCapsule = capsule;
	// �v���C���[
	const auto matChar = _pPlayer->getMatrix();
	auto player = _pPlayer->getCollision();
	playerCapsule._line._position[0] = player->_line._position[0].transform(matChar);
	playerCapsule._line._position[1] = player->_line._position[1].transform(matChar);

	Vector3 collisionPosition = capsule._line.getPosition(0.1f);
	Vector3 nearestHitPosition = Vector3::ZERO;
	Vector3 nearestHitNormal = Vector3::ZERO;
	f32		nearestT;
	Vector3 front = _pPlayer->getDirection();

	Vector3 hitNormal = Vector3::ZERO;
	int hitCount = 0;

	for (auto& triangle : _pMapObjectManager->getTower()) {
		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		// �O�p�`�̒��Ɋ܂܂�邩�e�X�g
		if (!Collision::isHit(
			playerCapsule,
			*triangle,
			playerCapsule._line.getPosition(0.5f),
			front.normalize(),
			1.0f,
			&nearestHitPosition,
			&nearestHitNormal,
			&nearestT)
			) continue;

		hitNormal += nearestHitNormal;
		hitCount++;
	}

	if (hitCount != 0) {
		Vector3  extruded = Collision::wallShear(front, hitNormal, 2.0f);
		_pPlayer->addPosition(extruded);
		_pPlayer->setVelocity(0.0f);
	}
}


//======================================================================================
//! �v���C���[�ƃA�C�e��
//======================================================================================
void CollisionManager::playerCollisionKeyAitem(const Capsule& capsule)
{
	Capsule c = capsule;
	//c._line._position[0] = c._line.getPosition(0.5f);
	// �v���C���[�ƃL�[�A�C�e��
	int count = 0;
	for (auto& keyItem : _pItemManager->getKeyItems()) {
		const auto matrix = keyItem->getMatrix();
		auto item = keyItem->getCollision();
		Sphere sphere(item->_position.transform(matrix), item->_radius);
		if (!keyItem->isActive()) {
			if (Collision::isHit(c, sphere)) {
				// �ʒm
				_pPlayer->setHitObjectType(keyItem->getObjectType());
				if (_pPlayer->getState2() == PlayerState::StartUp) {
					keyItem->hitObjectType(_pPlayer->getObjectType());
					count++;
				}
			}
		}
		else {
			count++;
		}
	}

	auto* playerSphere = new Sphere(c._line.getPosition(0.5f), c._radius);
	for (auto& keyItem : _pItemManager->getKeyItems()) {
		Vector3 extrusion0 = Vector3::ZERO;
		Vector3 extrusion1 = Vector3::ZERO;
		const auto matrix = keyItem->getMatrix();
		auto item = keyItem->getDumyCollision();
		Sphere sphere(item->_position.transform(matrix), item->_radius);
		if (Collision::adjustPosition(*playerSphere, sphere, extrusion0, extrusion1, 0.0f, 1.0f)) {
			extrusion0._y = 0.0f;
			_pPlayer->addPosition(extrusion0);
		}
	}
	SAFE_DELETE(playerSphere);


	const auto& lastKeyItem = _pItemManager->getLastKeyItem();
	if (lastKeyItem != nullptr) {
		if (lastKeyItem->isActive()) {
			auto* playerSphere = new Sphere(c._line.getPosition(0.8f), c._radius);
			Vector3 extrusion0 = Vector3::ZERO;
			Vector3 extrusion1 = Vector3::ZERO;
			const auto lastKeyItem = _pItemManager->getLastKeyItem();
			const auto matrix = lastKeyItem->getMatrix();
			auto item = lastKeyItem->getDumyCollision();

			Sphere sphere(item->_position.transform(matrix), item->_radius);
			if (Collision::adjustPosition(*playerSphere, sphere, extrusion0, extrusion1, 0.0f, 1.0f)) {
				extrusion0._y = 0.0f;
				_pPlayer->addPosition(extrusion0);
			}
			SAFE_DELETE(playerSphere);
		}
	}


	if (_pItemManager->getLastKeyItem() != nullptr) {
		if (!_pItemManager->getLastKeyItem()->isHit() &&
			count >= _pItemManager->getKeyItems().size()) {

			const auto lastKeyItem = _pItemManager->getLastKeyItem();
			const auto matrix = lastKeyItem->getMatrix();
			auto item = lastKeyItem->getCollision();

			Sphere sphere(item->_position.transform(matrix), item->_radius);
			if (Collision::isHit(c, sphere)) {
				// �ʒm
				_pPlayer->setHitObjectType(lastKeyItem->getObjectType());
				if (_pPlayer->getState2() == PlayerState::StartUp) {
					lastKeyItem->hitObjectType(_pPlayer->getObjectType());
				}
			}
		}
	}


	// �v���C���[�ƃh���b�v�A�C�e��
	for (auto dropItem : _pItemManager->getDropItem()) {
		const auto matrix = dropItem->getMatrix();
		auto item = dropItem->getCollision();
		Sphere sphere(item->_position.transform(matrix), item->_radius + 2.0f);

		if (Collision::isHit(capsule, sphere)) {
			// �ʒm
			_pPlayer->setHitObjectType(dropItem->getObjectType());
			dropItem->setActive(false);
		}
	}


	if (_pItemManager->getBossKeyItem() != nullptr) {
		auto* playerSphere = new Sphere(c._line.getPosition(0.8f), c._radius);
		Vector3 extrusion0 = Vector3::ZERO;
		Vector3 extrusion1 = Vector3::ZERO;
		const auto lastKeyItem = _pItemManager->getBossKeyItem();
		const auto matrix = lastKeyItem->getMatrix();
		auto item = lastKeyItem->getDumyCollision();

		Sphere sphere(item->_position.transform(matrix), item->_radius);
		if (Collision::adjustPosition(*playerSphere, sphere, extrusion0, extrusion1, 0.0f, 1.0f)) {
			extrusion0._y = 0.0f;
			_pPlayer->addPosition(extrusion0);
		}
		SAFE_DELETE(playerSphere);

		if (!_pItemManager->getBossKeyItem()->isActive()) {
			const auto bossKeyItem = _pItemManager->getBossKeyItem();
			const auto matrix = bossKeyItem->getMatrix();
			auto item = bossKeyItem->getCollision();

			Sphere sphere(item->_position.transform(matrix), item->_radius);
			if (Collision::isHit(c, sphere)) {
				// �ʒm
				auto a  = bossKeyItem->getObjectType();
				_pPlayer->setHitObjectType(bossKeyItem->getObjectType());
				if (_pPlayer->getState2() == PlayerState::StartUp) {
					bossKeyItem->hitObjectType(_pPlayer->getObjectType());
				}
			}
		}
	}		
}




void CollisionManager::debugDisp()
{
	//Vector3 lines[2] =
	//{
	//	Vector3(_pPlayer->getStartLine().transform(_pPlayer->getMatrix())),
	//	Vector3(_pPlayer->getEndLine().transform(_pPlayer->getMatrix())),
	//};

	glColor3ub(255, 255, 0);

	Vector3 stringPosition(Vector3(20.0f, 10.0f, 0.0f));

	std::vector<std::string> text;
	text.push_back("x = ");
	text.push_back("y = ");
	text.push_back("z = ");

	DebugStringValue(text, _pPlayer->getPosition(), stringPosition, Vector3(100.0f, 0.0f, 0.0f));

	stringPosition._y += 30;
	for (auto& item : _pItemManager->getKeyItems()){
		stringPosition._y += 20;
		DebugStringValue(text, item->getPosition(), stringPosition, Vector3(100.0f, 0.0f, 0.0f));
	}
}


//======================================================================================
//! ������`��(���l����)
//======================================================================================
void DebugStringValue(const std::vector<std::string> text, const Vector3& num, const Vector3& pos, const Vector3& space)
{
	Vector3 p = pos;
	char buff[3][256];

	sprintf_s(buff[0], "%0.1f", num._x);
	sprintf_s(buff[1], "%0.1f", num._y);
	sprintf_s(buff[2], "%0.1f", num._z);

	for (int i = 0; i < (signed)text.size(); i++) {
		std::string textt(text[i]);
		textt += buff[i];
		DrawString(textt, p);
		p += space;
	}
}

//======================================================================================
//! ������`��(���l����)
//======================================================================================
void DebugString(const std::vector<std::string> text, const Vector3& pos, const Vector3& space)
{
	Vector3 p = pos;

	for (int i = 0; i < (signed)text.size(); i++) {
		std::string textt(text[i]);
		DrawString(textt, p);
		p += space;
	}
}

//======================================================================================
//! 
//======================================================================================
void CollisionManager::debugKoutenCircle()
{
	//if (Collision::isDebugMode() ) {
	//	drawSphere(Collision::getKouten(), 0.5f, Color(255, 255, 0));
	//}
}


void render_string(float x, float y, std::string const& str)
{
	float z = -1.0f;
	glRasterPos3f(x, y, z);

	auto size = str.begin();
	auto sizeEnd = str.end();
	while (size != sizeEnd)
	{
		char c;
		c = *size;
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, c);
		++size;
	}
}