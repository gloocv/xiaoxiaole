#ifndef OFFSETSPRITE_H
#define OFFSETSPRITE_H
#include"cocos2d.h"
#include"StarLayer.h"

USING_NS_CC;
#define TOTLE_OFFSET (6)
#define HORVER_OFFSET (2)
#define NOWAYNUM (-999)
class StarSprite;


typedef enum
{
	NORMAL = 0,
	BORDER,//边界
	LATICE,//栅栏
	CREATE,//产生点
	DELIVERY,//穿越点
	UNUSE//占位无用
}OFFSETTYPE;
typedef enum
{
	UNACTIVATION = 0,
	ACTIVATION,
}OFFSETSTATUS;
typedef enum
{
	HORIZONTAL = 0,//水平
	VERTICAL
}HORVER; 

static const char *offsetPng[HORVER_OFFSET][TOTLE_OFFSET] = {
	{ "unuse.png", "unuse.png", "offsetH.png", "createH.png", "deliveryH.png", "unuse.png" },
	{ "unuse.png", "unuse.png", "offsetV.png", "createV.png", "deliveryV.png", "unuse.png" }
};
class OffsetSprite :public Sprite
{
public:
	OffsetSprite();
	static OffsetSprite * create(HORVER, OFFSETTYPE);
	virtual bool OffsetSprite::init(HORVER horver, OFFSETTYPE type);
	virtual void update(float);
	CC_SYNTHESIZE(OFFSETSTATUS, m_leftStatus, LeftStatus);
	CC_SYNTHESIZE(OFFSETSTATUS, m_rightStatus, RightStatus);
	CC_SYNTHESIZE(OFFSETSTATUS, m_downStatus, DownStatus);
	CC_SYNTHESIZE(OFFSETSTATUS, m_Status, Status);
	CC_SYNTHESIZE(OFFSETTYPE, m_type, Type);
	CC_SYNTHESIZE(HORVER, m_horver, Horver);
	CC_SYNTHESIZE(int, m_row, ROW);
	CC_SYNTHESIZE(int, m_col, COL);
	CC_SYNTHESIZE(DIR, m_fallstardir, FallStarDir);
	CC_SYNTHESIZE(int, m_starfloor, StarFloor);
	StarSprite **star_map;
	OffsetSprite ** offset_map;

	OffsetSprite* getOffsetFromWay();
	void fallStarFromWay();
	void resetOffsetDir();
	void getupRorLdir();
	StarSprite * getStarByDir(DIR);
	OffsetSprite * getOffsetByDir(DIR);

};


#endif