#ifndef STARLAYER_H
#define STARLAYER_H
#include"cocos2d.h"

typedef enum Dir
{
	MIDDLE = 0,
	RIGHT,
	LEFT,
	UPMIDDLE,
	UPRIGHT,
	UPLEFT,
	DOWNMIDDLE,
	DOWNRIGHT,
	DOWNLEFT,
	NOWAY,
	CREATESTAR
}DIR;
#include"StarSprite.h"
#include"OffsetSprite.h"

USING_NS_CC;

#define NODEMAXROW (6)
#define NODEMAXCOL (6)
#define OFFSETMAXROW (NODEMAXROW+1)
#define OFFSETMAXCOL (NODEMAXCOL+1)
#define OFFSETWIDTH (6)
#define OFFSETHIGHT (64)
#define STARWIDTH (64)
#define STARHIGHT (64)
//map01
#define LAYER_W (107)
#define LAYER_H (214)
#define CHANGETIME (0.15)
#define DOWNTIME (0.2)



class StarLayer :public Layer
{
public:
	StarLayer();
	~StarLayer();
	static Scene* createScene();
	CREATE_FUNC(StarLayer);
	virtual bool init()override;

	StarSprite **star_map;
	OffsetSprite** offset_map;//ºáÏò×ÝÏò[2][row][col]

	bool enabletouch;
	bool successbegan;
	StarSprite * src_starsprite;
	StarSprite * dir_starsprite;
	OffsetSprite *offsetsprite;
	bool  onTouchBegan(Touch *touch, Event *event);
	void  onTouchMoved(Touch *touch, Event *event);
	StarSprite * spriteofPoint(Vec2 point);
	void reset_change();
	void normalNormal();
	void super5xOther();
	void super33Other();
	void super4xsuper4x();
	void getRowChain(StarSprite *star, std::list<StarSprite *> &chainList);//ºáÏò¼ì²â
	void getColChain(StarSprite *star, std::list<StarSprite *> &chainList);//×ÝÏò¼ì²â

	void initoffsetDirFloor();

	virtual void update(float)override;
	void restart(cocos2d::Object * pSender);
};


#endif