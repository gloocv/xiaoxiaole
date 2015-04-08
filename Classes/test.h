
#include"cocos2d.h"

USING_NS_CC;

class TestLayer :public Layer
{
public:
	static Scene *createScene();
	CREATE_FUNC(TestLayer);
	virtual bool onTouchBegan(Touch *touch, Event *unused) override;
	virtual void onTouchMoved(Touch *touch, Event *unused) override;
};