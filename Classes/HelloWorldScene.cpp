#include "HelloWorldScene.h"
#include "cocostudio/CocoStudio.h"
#include "ui/CocosGUI.h"
#include"SimpleAudioEngine.h"
#include"StarLayer.h"

USING_NS_CC;
using namespace CocosDenshion;

using namespace cocostudio::timeline;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
	
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bg = Sprite::create("starScene.png");
	bg->setPosition(visibleSize.width / 2, visibleSize.height/2);
	this->addChild(bg, -1);
	auto startBtn = MenuItemImage::create("btn_play.png", "btn_play_pressed.png", CC_CALLBACK_1(HelloWorld::menucallback,this));
	startBtn->setPosition(visibleSize.width / 2, 260);
	auto menu = Menu::create(startBtn, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu);
	//SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic("sounds/MainMenuMusic.mp3");
	//SimpleAudioEngine::sharedEngine()->playBackgroundMusic("sounds/MainMenuMusic.mp3", true);
    return true;
}
void HelloWorld::menucallback(cocos2d::Ref * sender)
{
	auto testmap = StarLayer::createScene();
	Director::getInstance()->replaceScene(testmap);
}
