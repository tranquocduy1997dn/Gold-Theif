#include "HelloWorldScene.h"
#include "Game.h"
USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
	scene->addChild(LayerColor::create(Color4B(247,247,247,255)));
	scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	this->addChild(GamePlay::create());
    return true;
}
