#pragma once
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "SimpleAudioEngine.h"
using namespace CocosDenshion;

class GamePlay :public cocos2d::Layer
{
public:
	
	CREATE_FUNC(GamePlay);

	virtual bool init();

	void update(float t);

	void CreateSceneMeNuGame(Ref* pSender);

	void menuCloseCallback(Ref* pSender);

	CocosDenshion::SimpleAudioEngine* sound;

private:
	
	enum class GameState { Wait, Play, Die, Relive };

	GameState gameState;
#pragma region game controller

	void setGameStateWait();

	void setGameStatePlay();

	void setGameStateDie();

#pragma endregion

#pragma region  Characters

	cocos2d::Sprite* Characters;

	float Characters_SpeedY;

	float Characters_SpeedX;

	float Characters_AirTime;

	cocos2d::Animation* aniCharactersRun;

	void update_Characters(float t);

	void makeAPersuasiveDeath();

#pragma endregion Characters 

#pragma region  Monster

	cocos2d::Sprite* Monster;

	cocos2d::Animation* aniMonsterRun;

	cocos2d::Animation* aniMonsterAtTak;
	
	void update_Monster(float t);

#pragma endregion Monster 

#pragma region Impediment 

	std::vector<cocos2d::Sprite*> Impediment_Manager;

	void update_Impediment(float t);

	void destroy_Impediment(int index);

	cocos2d::Sprite* create_Impediment();

	float Impediment_DistanceMin;

	float Impediment_DistanceMax;

	void recalculate_Impediment_Spacing();

	bool checkCollision_Impediment();

#pragma endregion Impediment

#pragma region GOLD 

	cocos2d::Sprite* Gold;

	std::vector<cocos2d::Sprite*> Gold_Manager;

	void update_Gold(float t);

	void effect_Gold(int index);

	void destroy_Gold(int index);

	void create_Gold();

#pragma endregion GOLD 

#pragma region Score

	void init_Scores();

	void set_Score(int score);

	float fScore;

	int score;

	int high_Score;

	int fake_High_Score;

	cocos2d::LabelTTF* lbScore;

	cocos2d::LabelTTF* lbHighScore;

#pragma endregion Score

#pragma region Roads

	cocos2d::Sprite* Road_1;

	cocos2d::Sprite* Road_2;

	void update_Roads(float t);

#pragma endregion Roads

#pragma region Back Grounds

	cocos2d::Sprite* BG_1;

	cocos2d::Sprite* BG_2;

	void update_BG(float t);

#pragma endregion Back Grounds

#pragma region UIs

	cocos2d::LabelTTF* lbTapJump;

	cocos2d::ui::Button* btnMeNuGameDie;

	cocos2d::ui::Button* BtnCloseGame;

	cocos2d::ui::Button* BtnRelive;

	cocos2d::ui::Button* BtnMenuHome;

	cocos2d::Sprite* Tap_1;

	cocos2d::Sprite* Tap_2;

	cocos2d::Sprite* Hand;

#pragma endregion UIs

	bool touched_Flag;

	bool cancel_Touch;

};