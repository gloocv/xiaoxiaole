#ifndef STARSPRITE_H
#define STARSPRITE_H
#include"cocos2d.h"
#include"StarLayer.h"

USING_NS_CC;
#define TOTLE_STAR (5)

class OffsetSprite;

typedef enum {
	STOP = 0,
	ACTIVE,
	CHAIN,
	REMOVE,
	MOVEDOWNMID,
	MOVEDOWNLEFT,
	MOVEDOWNRIGHT,
	MOVING
}STATUS;
typedef enum
{
	NORMALSTAR,//普通星
	HORIZONTAL_4X,//横向四消星
	VERTICAL_4X,//纵向四消星
	SUPER_33,//33,34,43,44消星
	SUPER_5X,//五消星
}STARTYPE;//ATTRIBUTE
typedef enum
{
	RED = 0,
	BLUE,
	GREEN,
	ORARGE,
	PURPLE,
}STARCOLOR;//STARTYPE

static const char *starPng[TOTLE_STAR] = {
	"red_heart.png",
	"blue_heart.png",
	"green_heart.png",
	"orange_heart.png",
	"purple_heart.png",
};
static const char *starPng_4h[TOTLE_STAR] = {
	"red_heart_4h.png",
	"blue_heart_4h.png",
	"green_heart_4h.png",
	"orange_heart_4h.png",
	"purple_heart_4h.png",
};
static const char *starPng_4v[TOTLE_STAR] = {
	"red_heart_4v.png",
	"blue_heart_4v.png",
	"green_heart_4v.png",
	"orange_heart_4v.png",
	"purple_heart_4v.png",
};
static const char *starPng_33[TOTLE_STAR] = {
	"red_heart_33.png",
	"blue_heart_33.png",
	"green_heart_33.png",
	"orange_heart_33.png",
	"purple_heart_33.png",
};
static const char *super_5x = "super_5x.png";


class StarSprite :public Sprite
{
public:
	StarSprite();
	static StarSprite* create(STARCOLOR);
	virtual void update(float) override;
	virtual bool init(STARCOLOR);
	STARTYPE m_startype;
	void setStarType(STARTYPE);
	STARTYPE getStarType();
	CC_SYNTHESIZE(STARCOLOR, m_color, MyColor);
	CC_SYNTHESIZE(STATUS, m_status, Status);
	CC_SYNTHESIZE(bool,m_moveable,MoveAble);
	CC_SYNTHESIZE(bool,m_colored,Colored);
	CC_SYNTHESIZE(int, m_row, ROW);
	CC_SYNTHESIZE(int, m_col, COL);
	CC_SYNTHESIZE(bool, m_nearIsRemove, NearIsRemove);

	StarSprite **star_map;
	OffsetSprite ** offset_map;
	bool getRowChain(StarSprite *star, std::list<StarSprite *> &chainList);//横向检测
	bool getColChain(StarSprite *star, std::list<StarSprite *> &chainList);
	void setliststatus(std::list<StarSprite *> &, std::list<StarSprite *> &);
	//CoveringSprite* m_Coveringsprite;//覆盖精灵.如冰块,云朵
	void stop();
	void chain();
	void active();
	void removeFromMap();
	void movedownMid();
	void movedownLeft();
	void movedownRight();
	StarSprite * getStarByDir(DIR);
	OffsetSprite * getOffsetByDir(DIR);
	void broadcastRemove();
	void callBackRemove();


};

#endif