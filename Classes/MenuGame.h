#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "GameCommon.h"
#include "Game.h"
class MenuGame: public cocos2d::Layer
{
public:

	CREATE_FUNC(MenuGame);

	void CreateScenePlayGame(Ref* pSender);

	void LoginFacebook(Ref* pSender);

	static int checkmusic;

	void Pause_Music(Ref* pSender);

	void Resume_Mucsic(Ref* pSender);

	virtual bool init();

private:
	
#pragma region Game

	cocos2d::ui::Button* Menu_ResumeMusic;

	cocos2d::ui::Button* Menu_PauseMusic;

	cocos2d::ui::Button* BackGround_Menu;

	cocos2d::ui::Button* Menu_closeItem;

	cocos2d::ui::Button* Menu_PlayGame;

	cocos2d::ui::Button* Menu_LoginFacebook;

#pragma endregion

};