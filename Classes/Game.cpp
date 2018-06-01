#include "Game.h"
#include "GameCommon.h"
#include "CustomSound.h"
#include "ui/CocosGUI.h"
#include "MenuGame.h"
#include "MenuScene.h"
USING_NS_CC; 

// Characters
#define GRAVITY -1000.0f
#define CHARACTERS_JUMPING_TOLERANCE 3
#define CHARACTERS_SPEED_Y_MIN -500.0f
#define CHARACTERS_SPEED_Y_MAX 460.0f
#define CHARACTERS_SPEED_X_INITIAL 350.0f
#define CHARACTERS_SPEED_X_INCREASING_RATE 5
#define CHARACTERS_X 250.0f
#define CHARCTERS_X_PLAYING 100.0f

// Impediment
#define IMPEDIMENT_NUMBER 5
#define IMPEDIMENT_INTIAL_X 1500
#define IMPEDIMENT_SPEED_RATIO 1.2

// ground 
#define GROUND_HEIGHT 0

// road
#define ROAD_SPEED_RATIO 1.2
#define ROAD_2_PROBABILITY 30

// storage
#define STORE_HIGH_SCORE "HIGH_SCORE"

// score (gold)
#define SCORE_TEXT "%d - GOLD"
#define HIGHSCORE_TEXT "GOLD MOST - %d"
#define SCORE_RATE 6
#define SCORE_INTIAL_HIGH 60
#define FAKE_HIGHSCORE 100

// sound
#define SOUND_BACKGROUND "BackGround.wav"
#define SOUND_JUMP "jump.wav"
#define SOUND_OVER "over.wav"
#define SOUND_NEW_HIGH "newhigh.wav"
#define SOUND_HIT_GROUND "hitground.wav"

// zorder
#define BACKGROUND_ZORDER 1
#define TREE_ZORDER 2
#define IMPEDIMENT_ZORDER 3
#define ROAD_ZORDER 3
#define CHARACTERS_ZORDER 3
#define GOLD_ZORDER 3
#define SCORE_ZORDER 4
#define MENUGAMEDIE_ZORDER 5

MenuGame menugame;


#pragma region global functions

bool check_Jumping_Characters_Collision(Rect CharactersRect, Rect ImpedimentRect)
{

#define CHARCTERS_RECTS 3

	///
	Rect CharactersRects[CHARCTERS_RECTS] = {
		Rect(10, 0, 14, 8),
		Rect(0, 8, 28, 35),
		Rect(28, 20, 12, 23)
	};

	bool rs = false;

	///
	for (int i = 0; i < CHARCTERS_RECTS; i++)
	{
		CharactersRects[i].origin += CharactersRect.origin;
		if (CharactersRects[i].intersectsRect(ImpedimentRect))
		{
			rs = true;
			break;
		}
	}

	///
	return rs;


#undef CHARCTERS_RECTS 2
}

#pragma endregion global functions


bool GamePlay::init()
{
	///
	if (!Layer::init())
		return false;
	// effect snow
	auto effect_1 = ParticleSnow::create();
	this->addChild(effect_1, ROAD_ZORDER);
	auto effect_2 = ParticleRain::create();
	this->addChild(effect_2, ROAD_ZORDER);
	// preload sounds
	sound = CocosDenshion::SimpleAudioEngine::getInstance();
	sound->preloadEffect(SOUND_JUMP);
	sound->preloadEffect(SOUND_NEW_HIGH);
	sound->preloadEffect(SOUND_OVER);
	sound->preloadBackgroundMusic(SOUND_BACKGROUND);
	if (menugame.checkmusic == 0)
	{
		sound->playBackgroundMusic(SOUND_BACKGROUND, true);
	}
	// hand
	Hand = Sprite::create("hand_1.png");
	Hand->setPosition(Vec2(DW / 2, DH / 2));
	this->addChild(Hand, SCORE_ZORDER);
	// label tap to jump
	lbTapJump = LabelTTF::create("TAP TO PLAY AND JUMP", "fonts/score.ttf", 50);
	lbTapJump->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	lbTapJump->setPosition(Vec2(DW / 2, DH / 2 + 25));
	lbTapJump->setFontFillColor(Color3B::RED);
	this->addChild(lbTapJump, SCORE_ZORDER);

	// Tap
	Tap_1 = Sprite::create("Tap_1.png");
	Tap_1->setPosition(Vec2(DW / 2 - Tap_1->getContentSize().width, DH / 2));
	this->addChild(Tap_1, SCORE_ZORDER);
	Tap_2 = Sprite::create("Tap_2.png");
	Tap_2->setPosition(Vec2(DW / 2 + Tap_2->getContentSize().width, DH / 2));
	this->addChild(Tap_2, SCORE_ZORDER);

	// road
	Road_1 = Sprite::create("road_1.png");
	Road_1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Road_1->setPosition(Vec2(0, GROUND_HEIGHT));
	Road_2 = Sprite::create("road_1.png");
	Road_2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Road_2->setPosition(Vec2(Road_1->getPositionX() + Road_1->getContentSize().width - 5, GROUND_HEIGHT));
	this->addChild(Road_1, ROAD_ZORDER);
	this->addChild(Road_2, ROAD_ZORDER);

	// Back Ground
	BG_1 = Sprite::create("BG_1.png");
	BG_1->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	BG_1->setPosition(Vec2(0, GROUND_HEIGHT + Road_1->getContentSize().height));
	BG_2 = Sprite::create("BG_1.png");
	BG_2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	BG_2->setPosition(Vec2(BG_1->getPositionX() + BG_1->getContentSize().width - 5, GROUND_HEIGHT + Road_1->getContentSize().height));
	this->addChild(BG_1, BACKGROUND_ZORDER);
	this->addChild(BG_2, BACKGROUND_ZORDER);

	// Character
	Characters = Sprite::create("Characters_0.png");
	Characters->setScale(0.35);
	Characters->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Characters->setPosition(Vec2(CHARACTERS_X, GROUND_HEIGHT + Road_2->getContentSize().height-7));
	this->addChild(Characters, CHARACTERS_ZORDER);

	Characters_SpeedY = 0;
	Characters_SpeedX = CHARACTERS_SPEED_X_INITIAL;

	Characters_AirTime = -((float)CHARACTERS_SPEED_Y_MAX*2.0f) / (float)GRAVITY;

	// animation dino running
	Vector<SpriteFrame*> aniCharactersFrames;
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_0.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_1.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_2.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_3.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_4.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_5.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_6.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_7.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_8.png", Rect(0, 0, 415, 507)));
	aniCharactersFrames.pushBack(SpriteFrame::create("Characters_9.png", Rect(0, 0, 415, 507)));
	aniCharactersRun = Animation::createWithSpriteFrames(aniCharactersFrames, 0.1f);
	aniCharactersRun->retain();
	//
	// Monster
	Monster = Sprite::create("WALK_000.png");
	Monster->setScale(0.5);
	Monster->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	Monster->setPosition(Vec2(CHARACTERS_X-150, GROUND_HEIGHT + Road_2->getContentSize().height-30));
	this->addChild(Monster, CHARACTERS_ZORDER);

	 // dino running
	Vector<SpriteFrame*> aniMonsterFrames;
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_000.png", Rect(0, 0, 820, 555))); 
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_001.png", Rect(0, 0, 820, 555)));
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_002.png", Rect(0, 0, 820, 555)));
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_003.png", Rect(0, 0, 820, 555)));
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_004.png", Rect(0, 0, 820, 555)));
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_005.png", Rect(0, 0, 820, 555)));
	aniMonsterFrames.pushBack(SpriteFrame::create("WALK_006.png", Rect(0, 0, 820, 555)));
	aniMonsterRun = Animation::createWithSpriteFrames(aniMonsterFrames, 0.1f);
	aniMonsterRun->retain();
	// animation dino running
	Vector<SpriteFrame*> aniMonsterFramesAT;
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_000.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_001.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_002.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_003.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_004.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_005.png", Rect(0, 0, 940, 817)));
	aniMonsterFramesAT.pushBack(SpriteFrame::create("ATTAK_006.png", Rect(0, 0, 940, 817)));
	aniMonsterAtTak = Animation::createWithSpriteFrames(aniMonsterFramesAT, 0.1f);
	aniMonsterAtTak->retain();
	//
	btnMeNuGameDie = cocos2d::ui::Button::create("GAME_OVER_MENU.png");
	btnMeNuGameDie->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	btnMeNuGameDie->setPosition(Vec2(DW / 2, DH / 2));
	btnMeNuGameDie->setScale(0);
	this->addChild(btnMeNuGameDie, MENUGAMEDIE_ZORDER);
	/*btnMeNuGameDie->setVisible(false);*/
	//
	BtnRelive = cocos2d::ui::Button::create("Btn_Revive01.png", "Btn_Revive02.png", "Btn_Revive03.png");
	BtnRelive->setScale(0);
	BtnRelive->setPosition(Vec2(DW / 2, btnMeNuGameDie->getPositionY() + 30));
	BtnRelive->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			setGameStateWait(); 
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(BtnRelive, MENUGAMEDIE_ZORDER);
	//
	//
	BtnMenuHome = cocos2d::ui::Button::create("btn_menu1.png", "btn_menu2.png", "btn_menu3.png");
	BtnMenuHome->setScale(0);
	BtnMenuHome->setPosition(Vec2(DW / 2, BtnRelive->getPositionY()));
	BtnMenuHome->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			CreateSceneMeNuGame(this);
			CCLOG("%s", "Button 1 clicked");
			break;
		default:
			break;
		}

	});
	this->addChild(BtnMenuHome, MENUGAMEDIE_ZORDER);
	//
	BtnCloseGame = cocos2d::ui::Button::create("Button_05 - Copy.png", "Button_06 - Copy.png", "Button_07 - Copy.png");
	BtnCloseGame->setScale(0);
	BtnCloseGame->setPosition(Vec2(DW / 2, /*BtnMenuHome->getPositionY() - BtnMenuHome->getContentSize().height + 70*/BtnRelive->getPositionY()));
	BtnCloseGame->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			menuCloseCallback(this);
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(BtnCloseGame, MENUGAMEDIE_ZORDER);
	// scores
	init_Scores();

	//// preload sounds
	//auto sound = CocosDenshion::SimpleAudioEngine::getInstance();
	//sound->preloadEffect(SOUND_JUMP);
	//sound->preloadEffect(SOUND_NEW_HIGH);
	//sound->preloadEffect(SOUND_OVER);
	//sound->playBackgroundMusic(SOUND_BACKGROUND, true);
	/*MenuGame menugame;
	if (menugame.checkmusic==true )
	{
		menugame.Pause_Music(this);
	}*/
	//auto _emitter = ParticleSystem::create("particle_texture.plist");
	///*_emitter->retain();*/
	//_emitter->setPosition(DW/2,DH/2);
	//addChild(_emitter, SCORE_ZORDER);
	// touch
	touched_Flag = false;
	cancel_Touch = false;
	auto touchListener = EventListenerTouchAllAtOnce::create();
	touchListener->onTouchesBegan = [=](const std::vector<Touch*>&, Event*)
	{
		this->touched_Flag = true;
	};
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(touchListener, this);

	///
	gameState = GameState::Wait;

	///
	Characters->runAction(RepeatForever::create(Animate::create(aniCharactersRun)));
	Monster->runAction(RepeatForever::create(Animate::create(aniMonsterRun)));

	this->scheduleUpdate();
	return true;
}

void GamePlay::update(float t)
{
	t = 1.0f / 60.0f;
#pragma region evolve state label

	///
	if (cancel_Touch)
	{
		touched_Flag = false;
		cancel_Touch = false;
	};

	// evolve state label
	switch (gameState)
	{
	case GamePlay::GameState::Wait:

		///
		if (touched_Flag)
			setGameStatePlay();

		///
		break;

	case GamePlay::GameState::Play:

		///
		if (checkCollision_Impediment())
		{
			makeAPersuasiveDeath();
			setGameStateDie();
		};

		///
		break;

	case GamePlay::GameState::Die:
		///
		/*if (touched_Flag)
			setGameStateWait();*/
		Monster->setPositionX(Monster->getPositionX() + t*0.9*Characters_SpeedX);
		if (Monster->getPositionX() > Characters->getPositionX()-10)
		{
			Monster->setPositionX(Characters->getPositionX() - 10);
		}
		///
		break;

	case GamePlay::GameState::Relive:

		///	remove
		for (int i = Impediment_Manager.size() - 1; i >= 0; i--)
			destroy_Impediment(i);

		for (int i = Gold_Manager.size() - 1; i >= 0; i--)
			destroy_Gold(i);
		// change state
		
		gameState = GameState::Wait;
		if (Monster->getPositionX()>CHARACTERS_X - 100)
		{
			Monster->setPositionX(CHARACTERS_X - 100);
		}
		break;

	default:
		break;
	}
#pragma endregion

#pragma region evolve state 
	// evolve state 
	switch (gameState)
	{
	case GamePlay::GameState::Wait:
		// roads
		update_Roads(t);
		// Back Ground
		update_BG(t);
		break;
	case GamePlay::GameState::Play:
		// Back Ground
		update_BG(t);
		// roads
		update_Roads(t);
		//
		update_Monster(t);
		// Characters
		update_Characters(t);

		// Impediment		
		update_Impediment(t);
		// Gold
		update_Gold(t);

		///
		break;
	case GamePlay::GameState::Die:
		///
		break;
	case GamePlay::GameState::Relive:

		///
		break;
	default:
		break;
	}
#pragma endregion evolve state
	///
	touched_Flag = false;
	return;
}

void GamePlay::update_Characters(float t)
{
	bool jump = false;
	bool hitGround = false;
	switch (gameState)
	{
	case GamePlay::GameState::Wait:
		break;
	case GamePlay::GameState::Play:
		///
		Characters->setPositionX(Characters->getPositionX() - t*0.1*Characters_SpeedX);
		if (Characters->getPositionX() <= CHARCTERS_X_PLAYING)
		{
			Characters->setPositionX(CHARCTERS_X_PLAYING);
		}
		break;
	case GamePlay::GameState::Die:
		/*animaMonsterAtTak();*/
		break;
	case GamePlay::GameState::Relive:

		break;
	default:
		break;
	}
	///
	if (touched_Flag)
	if (Characters->getPositionY() <= (GROUND_HEIGHT + Road_2->getContentSize().height + CHARACTERS_JUMPING_TOLERANCE))
	{
		Characters_SpeedY = CHARACTERS_SPEED_Y_MAX;
		Characters->stopAllActions();
		Characters->setTexture("Characters_jump.png");
		Characters->setTextureRect(Rect(0, 0, 415, 507));

		///
		jump = true;
	}
	else
	{
		log("Failed to jump 1. Delta: %f", Characters->getPositionY() - (GROUND_HEIGHT + Road_2->getContentSize().height));
	}


	///
	Characters->setPositionY(Characters->getPositionY() + Characters_SpeedY*t);

	///
	if (Characters->getPositionY() < GROUND_HEIGHT + Road_2->getContentSize().height)
	{
		Characters->setPositionY(GROUND_HEIGHT + Road_2->getContentSize().height);
		Characters_SpeedY = 0;
		if (Characters->numberOfRunningActions() == 0)
			Characters->runAction(RepeatForever::create(Animate::create(aniCharactersRun)));

		///
		hitGround = true;
	}
	else
	if (Characters->getPositionY() > GROUND_HEIGHT + Road_2->getContentSize().height)
		Characters_SpeedY += GRAVITY*t;



	// for more responsive: doudble-check tap
	if (touched_Flag)
	if (Characters->getPositionY() <= (GROUND_HEIGHT + Road_2->getContentSize().height + CHARACTERS_JUMPING_TOLERANCE))
	{
		Characters_SpeedY = CHARACTERS_SPEED_Y_MAX;
		Characters->stopAllActions();
		Characters->setTexture("Characters_jump.png");
		Characters->setTextureRect(Rect(0, 0, 415, 507));

		///
		jump = true;

	}
	else
	{
		log("Failed to jump 2. Delta: %f", Characters->getPositionY() - (GROUND_HEIGHT + Road_2->getContentSize().height));
	}
	
	
	// sound
	if (jump&&menugame.checkmusic == 0)
	{
			CustomSound::playEff(SOUND_JUMP);
	}
	else if (hitGround&&menugame.checkmusic==0)
	{
		CustomSound::playEff(SOUND_HIT_GROUND);
	}
	// increase X speed
	Characters_SpeedX += t*CHARACTERS_SPEED_X_INCREASING_RATE;
}

void GamePlay::update_Monster(float t)
{
	switch (gameState)
	{
	case GamePlay::GameState::Wait:
		break;
	case GamePlay::GameState::Play:
		///
		Monster->setPositionX(Monster->getPositionX() - t*0.1*Characters_SpeedX);
		if (Monster->getPositionX() < -Monster->getContentSize().width/2 + 330)
		{
			Monster->setPositionX(-Monster->getContentSize().width / 2 + 330);
		}
		break;
	case GamePlay::GameState::Die:
		break;
	case GamePlay::GameState::Relive:
		
		break;
	default:
		break;
	}
}
/*==================================================================Impediment==================================================================*/

Sprite* GamePlay::create_Impediment()
{
	float positionX;
	// determine X
	if (Impediment_Manager.size() == 0)
	{
		positionX = IMPEDIMENT_INTIAL_X;
	}
	else
	{
		auto sp = Impediment_Manager[Impediment_Manager.size() - 1];
		positionX = RandomHelper::random_real<float>(Impediment_DistanceMin, Impediment_DistanceMax) + sp->getPositionX() + sp->getContentSize().width;
	};

	auto image = CCString::createWithFormat("Impediment_%d.png", RandomHelper::random_int(1, 2))->getCString();
	Sprite* Impediment = Sprite::create(image);
	Impediment->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Impediment->setPosition(Vec2(positionX, GROUND_HEIGHT + Road_1->getContentSize().height));
	this->addChild(Impediment, IMPEDIMENT_ZORDER);
	Impediment_Manager.push_back(Impediment);

	///
	return Impediment;
}

void GamePlay::destroy_Impediment(int index)
{
	Impediment_Manager[index]->removeFromParent();
	Impediment_Manager.erase(Impediment_Manager.begin() + index);
}

void GamePlay::update_Impediment(float t)
{

	switch (gameState)
	{
	case GamePlay::GameState::Wait:
		break;
	case GamePlay::GameState::Play:

		// translate
		for (int i = 0; i < IMPEDIMENT_NUMBER; i++)
		{
			Sprite* sp = Impediment_Manager[i];
			sp->setPositionX(sp->getPositionX() - t*IMPEDIMENT_SPEED_RATIO*Characters_SpeedX);
		}

		// remove / remove
		if (Impediment_Manager[0]->getPositionX() < (-Impediment_Manager[0]->getContentSize().width))
		{
			recalculate_Impediment_Spacing();
			destroy_Impediment(0);
			create_Impediment();
		}

		///
		break;
	case GamePlay::GameState::Die:
		break;
	case GamePlay::GameState::Relive:
		break;
	default:
		break;
	}
}

bool GamePlay::checkCollision_Impediment()
{
	bool rs = false;
	auto rtCharacters = Characters->getBoundingBox();

	///
	if (Characters->getNumberOfRunningActions() != 0)
	{
		/// check in case of running
		for (int i = 0; i < Impediment_Manager.size(); i++)
		if (Impediment_Manager[i]->getBoundingBox().intersectsRect(rtCharacters))
		{
			rs = true;
			break;
		};
	}
	else
	{
		/// check in case of jumping
		for (int i = 0; i < Impediment_Manager.size(); i++)
		if (check_Jumping_Characters_Collision(rtCharacters, Impediment_Manager[i]->getBoundingBox()))
		{
			rs = true;
			break;
		};
	}

	///
	return rs;
}
//=====================================================================================================================================================//

//==================================================================GOLD===============================================================================//
void GamePlay::create_Gold()
{
	float positionX;
	float positionY;
	//// X
	if (Gold_Manager.size() == 0)
	{
		positionX = IMPEDIMENT_INTIAL_X;
	}
	else
	{
		positionX = RandomHelper::random_real<float>(600, 735);
	};
	//  Y
	positionY = RandomHelper::random_real<float>(Road_1->getContentSize().height + 10, CHARACTERS_SPEED_Y_MAX - 55);
	Gold = Sprite::create("Gold_2.png");
	Gold->setScale(0.1);
	Gold->setPosition(Vec2(positionX, positionY));
	this->addChild(Gold, IMPEDIMENT_ZORDER);
	Gold_Manager.push_back(Gold);
}

void GamePlay::destroy_Gold(int index)
{
	Gold_Manager[index]->removeFromParent();
	Gold_Manager.erase(Gold_Manager.begin() + index);
}
void GamePlay::effect_Gold(int index)
{
	auto effect_gold = ParticleSystemQuad::create("particle_texture (3).plist");
	effect_gold->retain();
	effect_gold->setPosition(Gold_Manager[index]->getPositionX(), Gold_Manager[index]->getPositionY());
	this->addChild(effect_gold, IMPEDIMENT_ZORDER);
}
void GamePlay::update_Gold(float t)
{
	auto rtCharacters = Characters->getBoundingBox();
	switch (gameState)
	{
	case GamePlay::GameState::Wait:
		break;
	case GamePlay::GameState::Play:
		// translate
		for (int i = 0; i < Gold_Manager.size(); i++)
		{
			Sprite* sp = Gold_Manager[i];
			sp->setPositionX(sp->getPositionX() - t*IMPEDIMENT_SPEED_RATIO*Characters_SpeedX);
			// remove / add
			if (Gold_Manager[i]->getPositionX() < (-Gold_Manager[i]->getContentSize().width))
			{
				destroy_Gold(i);
				create_Gold();
			}
			//// remove/ set score
			if (Gold_Manager[i]->getBoundingBox().intersectsRect(rtCharacters))
			{
				effect_Gold(i);
				destroy_Gold(i);
				create_Gold();
				if (menugame.checkmusic == 0)
				{
					CustomSound::playEff(SOUND_NEW_HIGH);
				}
				fScore += 1;
				set_Score((int)fScore);
			}
		}
		///
		break;
	case GamePlay::GameState::Die:
		break;
	case GamePlay::GameState::Relive:
		break;
	default:
		break;
	}
}


/*============================================================SCORE=========================================================================================*/
void GamePlay::init_Scores()
{
	float leftMargin = 15;
	float topMargin = leftMargin + 60;

	///
	score = 0;
	fScore = 0;
	high_Score = CCUserDefault::getInstance()->getIntegerForKey(STORE_HIGH_SCORE, SCORE_INTIAL_HIGH);

	// high score icon
	auto sp = Sprite::create("highscoreicon.png");
	sp->setScale(0.4);
	sp->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	sp->setPosition(Vec2(leftMargin, DH - leftMargin));
	this->addChild(sp, SCORE_ZORDER);

	// score
	sp = Sprite::create("Gold_2.png");
	sp->setScale(0.1);
	sp->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	sp->setPosition(Vec2(DW - leftMargin, DH - leftMargin));
	this->addChild(sp, SCORE_ZORDER);

	// high score label
	lbHighScore = LabelTTF::create(CCString::createWithFormat(HIGHSCORE_TEXT, high_Score)->getCString(), "fonts/score.ttf", 30);
	lbHighScore->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
	lbHighScore->setPosition(Vec2(leftMargin, DH - topMargin));
	lbHighScore->setFontFillColor(Color3B::RED);
	this->addChild(lbHighScore, SCORE_ZORDER);

	// score label
	lbScore = LabelTTF::create(CCString::createWithFormat(SCORE_TEXT, score)->getCString(), "fonts/score.ttf", 30);
	lbScore->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);
	lbScore->setPosition(Vec2(DW - leftMargin, DH - topMargin));
	lbScore->setFontFillColor(Color3B::RED);
	this->addChild(lbScore, SCORE_ZORDER);
}

void GamePlay::set_Score(int score)
{
	if (score == this->score)
		return;

	this->score = score;

	// high
	if (score > high_Score)
	{
		high_Score = score;
		lbHighScore->setString(CCString::createWithFormat(HIGHSCORE_TEXT, high_Score)->getCString());

		// fake high
		if (high_Score > fake_High_Score)
		{
			if (menugame.checkmusic==0)
			{
				CustomSound::playEff(SOUND_NEW_HIGH);
			}
			fake_High_Score += FAKE_HIGHSCORE;
		}
	}

	// score
	lbScore->setString(CCString::createWithFormat(SCORE_TEXT, score)->getCString());

	///
	return;
}
/*============================================================SCORE=========================================================================================*/


/*=============================================================ROAD==========================================================================================*/

void GamePlay::update_Roads(float t)
{
	// translate
	Road_1->setPositionX(Road_1->getPositionX() - t*Characters_SpeedX*ROAD_SPEED_RATIO);
	Road_2->setPositionX(Road_2->getPositionX() - t*Characters_SpeedX*ROAD_SPEED_RATIO);

	// swap
	if (Road_1->getPositionX() < -800)
	{
		Road_1->removeFromParent();
		Road_1 = Road_2;
		Road_2 = Sprite::create(CCString::createWithFormat("road_%d.png", RandomHelper::random_int(1, 100) <= ROAD_2_PROBABILITY ? 2 : 1)->getCString());
		Road_2->setPosition(Vec2(Road_1->getPositionX() + Road_1->getContentSize().width - 5, GROUND_HEIGHT));
		Road_2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		this->addChild(Road_2, ROAD_ZORDER);
	}
}
/*=============================================================ROAD==========================================================================================*/

/*=============================================================BACK GROUND==========================================================================================*/

void GamePlay::update_BG(float t)
{
	// translate
	BG_1->setPositionX(BG_1->getPositionX() - t*Characters_SpeedX*0.1);
	BG_2->setPositionX(BG_2->getPositionX() - t*Characters_SpeedX*0.1);

	// swap
	if (BG_1->getPositionX() < 0-BG_1->getContentSize().width)
	{
		BG_1->removeFromParent();
		BG_1 = BG_2;
		BG_2 = Sprite::create(CCString::createWithFormat("BG_%d.png", RandomHelper::random_int(1, 2) <= ROAD_2_PROBABILITY ? 2 : 1)->getCString());
		BG_2->setPosition(Vec2(BG_1->getPositionX() + BG_1->getContentSize().width - 5, GROUND_HEIGHT+ Road_1->getContentSize().height));
		BG_2->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
		this->addChild(BG_2, 1);
	}
}
/*=============================================================BACK GROUND==========================================================================================*/

void GamePlay::setGameStatePlay()
{
	// create Impediment, Impediment distances
	recalculate_Impediment_Spacing();
	//---
	for (int i = 0; i < IMPEDIMENT_NUMBER; i++)
	{
		create_Impediment();
	}
	//create Gold
	for (int i = 0; i < 5; i++)
	{
		create_Gold();
	}
	// scores
	set_Score(0);
	fScore = 0;
	fake_High_Score = high_Score;
	///
	lbTapJump->setVisible(false);
	// hand
	Hand->setVisible(false);
	Tap_1->setVisible(false);
	Tap_2->setVisible(false);

	gameState = GameState::Play;
}

void GamePlay::setGameStateDie()
{
	// save high score
	CCUserDefault::getInstance()->setIntegerForKey(STORE_HIGH_SCORE, high_Score);
	
	Monster->stopAllActions();
	Monster->setPositionY(GROUND_HEIGHT + Road_2->getContentSize().height - 30);
	Monster->runAction(RepeatForever::create(Animate::create(aniMonsterAtTak)));
	
	///
	Characters->stopAllActions();
	Characters->setTexture("Characters_Dead.png");
	Characters->setTextureRect(Rect(0, 0, 485, 520));

	// Characters speed
	Characters_SpeedX = CHARACTERS_SPEED_X_INITIAL;

	///
	lbTapJump->setString("TAP TO RELIVE");
	lbTapJump->setVisible(true);

	btnMeNuGameDie->stopAllActions();
	btnMeNuGameDie->runAction(MoveTo::create(2, Vec2(DW / 2, DH / 2)));
	btnMeNuGameDie->runAction(ScaleTo::create(2,1));
	

	BtnRelive->stopAllActions();
	BtnRelive->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	BtnRelive->runAction(MoveTo::create(2, Vec2(DW / 2, btnMeNuGameDie->getPositionY()+30)));
	BtnRelive->runAction(ScaleTo::create(2, 0.6));
	
	BtnMenuHome->stopAllActions();
	BtnMenuHome->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	BtnMenuHome->runAction(MoveTo::create(2, Vec2(DW / 2, BtnRelive->getPositionY())));
	BtnMenuHome->runAction(ScaleTo::create(2, 0.6));
	//
	BtnCloseGame->stopAllActions();
	BtnCloseGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	BtnCloseGame->runAction(MoveTo::create(2, Vec2(DW / 2, BtnMenuHome->getPositionY()-BtnMenuHome->getContentSize().height+70)));
	BtnCloseGame->runAction(ScaleTo::create(2, 0.6));
	/*closeItemDie->setVisible(true);*/
	// hand
	Hand->setVisible(true);
	// Tap
	Tap_1->setVisible(true);
	Tap_2->setVisible(true);
	// play sound
	if (menugame.checkmusic==0)
	{
		CustomSound::playEff(SOUND_OVER);
	}
	// change state
	gameState = GameState::Die;
}

void GamePlay::setGameStateWait()
{
	Monster->stopAllActions();
	Monster->runAction(RepeatForever::create(Animate::create(aniMonsterRun)));
	Monster->setPositionX(CHARACTERS_X - 150);
	//
	Characters->setPositionX(CHARACTERS_X);
	//
	btnMeNuGameDie->stopAllActions();
	
	btnMeNuGameDie->setScale(0);

	BtnRelive->stopAllActions();
	
	BtnRelive->setScale(0);

	BtnMenuHome->stopAllActions();
	BtnMenuHome->setScale(0);

	BtnCloseGame->stopAllActions();
	BtnCloseGame->setPositionY(BtnMenuHome->getPositionY());
	BtnCloseGame->setScale(0);

	// clear Impediment
	for (int i = 0; i < Impediment_Manager.size(); i++)
	{
		Impediment_Manager[i]->removeFromParent();
	};
	Impediment_Manager.clear();
	// clear Gold
	for (int i = 0; i < Gold_Manager.size(); i++)
	{
		Gold_Manager[i]->removeFromParent();
	};
	Gold_Manager.clear();
	///
	Characters->setPositionY(GROUND_HEIGHT + Road_2->getContentSize().height);
	Characters->runAction(RepeatForever::create(Animate::create(aniCharactersRun)));
	/// Set UI
	lbTapJump->setString("TAP TO PLAY AGAIN");
	lbTapJump->setVisible(true);
	//
	// change state
	gameState = GameState::Wait;
}

void GamePlay::makeAPersuasiveDeath()
{
	Characters->setPositionY(Characters->getPositionY() - 5);
	if (Characters->getPositionY() < GROUND_HEIGHT)
	{
		Characters->setPositionY(GROUND_HEIGHT);
	}
}

void GamePlay::recalculate_Impediment_Spacing()
{
	Impediment_DistanceMin = 1500;
	Impediment_DistanceMax = 4000;
	return;
}
void GamePlay::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);


}
void GamePlay::CreateSceneMeNuGame(Ref* pSender)
{
	auto sque = MenuScene::createScene();
	Director::getInstance()->replaceScene(sque);
}