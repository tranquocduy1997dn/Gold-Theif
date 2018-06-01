#include "MenuGame.h"
#include "HelloWorldScene.h"
using namespace CocosDenshion;
USING_NS_CC;
int MenuGame::checkmusic; // check music
GamePlay game;

bool MenuGame::init()
{
	if (!Layer::init())
		return false;
	auto bg = Sprite::create("BG_1.png");
	bg->setScaleY(1.2);
	bg->setPosition(Vec2(DW / 2, DH / 2));
	this->addChild(bg);
	checkmusic = 0;
	// background menu
	BackGround_Menu = cocos2d::ui::Button::create("Windows_08.png");
	BackGround_Menu->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
	BackGround_Menu->setPosition(Vec2(DW / 2, DH / 2));
	this->addChild(BackGround_Menu);

	// play game
	Menu_PlayGame = cocos2d::ui::Button::create("Btn_Play01.png", "Btn_Play02.png", "Btn_Play03.png");
	Menu_PlayGame->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
	Menu_PlayGame->setScale(0.8);
	Menu_PlayGame->setPosition(Vec2(DW / 2, BackGround_Menu->getPositionY() + Menu_PlayGame->getContentSize().height / 2 + 30));
	Menu_PlayGame->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			MenuGame::CreateScenePlayGame(this);
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(Menu_PlayGame, 2);
	// pause music
	Menu_PauseMusic = cocos2d::ui::Button::create("btn_PauseMusic1.png", "btn_PauseMusic2.png", "btn_PauseMusic3.png");
	Menu_PauseMusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Menu_PauseMusic->setScale(0.8);
	Menu_PauseMusic->setPosition(Vec2(DW / 2, Menu_PlayGame->getPositionY() - 10));
	Menu_PauseMusic->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			MenuGame::Pause_Music(this);
			checkmusic = 1;
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(Menu_PauseMusic, 2);
	// resume music
	Menu_ResumeMusic = cocos2d::ui::Button::create("btn_ResumeMusic1.png", "btn_ResumeMusic2.png", "btn_ResumeMusic3.png");
	Menu_ResumeMusic->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Menu_ResumeMusic->setScale(0.8);
	Menu_ResumeMusic->setPosition(Vec2(DW / 2, Menu_PauseMusic->getPositionY() - Menu_PauseMusic->getContentSize().height + 30));
	Menu_ResumeMusic->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			MenuGame::Resume_Mucsic(this);
			checkmusic = 0;
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(Menu_ResumeMusic, 2);
	// exit game
	Menu_closeItem = cocos2d::ui::Button::create("Button_05 - Copy.png", "Button_06 - Copy.png", "Button_07 - Copy.png");
	Menu_closeItem->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);
	Menu_closeItem->setScale(0.8);
	Menu_closeItem->setPosition(Vec2(DW / 2, Menu_ResumeMusic->getPositionY() - Menu_ResumeMusic->getContentSize().height + 30));
	Menu_closeItem->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			game.menuCloseCallback(this);
			checkmusic = 0;
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(Menu_closeItem, 2);
	// LogIn Facebook
	Menu_LoginFacebook = cocos2d::ui::Button::create("btn_facebook1.png", "btn_facebook2.png", "btn_facebook3.png");
	Menu_LoginFacebook->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
	Menu_LoginFacebook->setScale(0.6);
	Menu_LoginFacebook->setPosition(Vec2(0,0));
	Menu_LoginFacebook->addTouchEventListener([&](Ref* sender,
		cocos2d::ui::Widget::TouchEventType type)
	{

		switch (type)
		{
		case ui::Widget::TouchEventType::BEGAN:

			break;

		case ui::Widget::TouchEventType::ENDED:
			
			CCLOG("%s", "Button 1 clicked");
			break;

		default:
			break;
		}

	});
	this->addChild(Menu_LoginFacebook, 2);
	return true;
}

void MenuGame::CreateScenePlayGame(Ref* pSender)
{
	auto sque = HelloWorld::createScene();
	Director::getInstance()->replaceScene(sque);
}

void MenuGame::Pause_Music(Ref* pSender)
{
	auto audio = SimpleAudioEngine::getInstance();
	// pause background music.
	audio->pauseBackgroundMusic();
	// pause all sound effects.
	audio->stopAllEffects();
}

void MenuGame::Resume_Mucsic(Ref* pSender)
{
	auto audio = SimpleAudioEngine::getInstance();
	// resume background music.
	audio->resumeBackgroundMusic();
	// resume all sound effects.
	audio->resumeAllEffects();
}
