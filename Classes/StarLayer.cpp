
#include"StarLayer.h"

USING_NS_CC;


STARCOLOR  startypemap[NODEMAXROW][NODEMAXCOL] = {
	{ BLUE, ORARGE, ORARGE, RED, RED, ORARGE },
	{ GREEN, BLUE, GREEN, ORARGE, ORARGE, BLUE },
	{ GREEN, GREEN, ORARGE, GREEN, GREEN, RED, },
	{ BLUE, RED, ORARGE, RED, RED, GREEN },
	{ BLUE, GREEN, GREEN, ORARGE, BLUE, BLUE },
	{ RED, RED, BLUE, GREEN, ORARGE, ORARGE }
};
OFFSETTYPE offsetmodemap[2][OFFSETMAXROW][OFFSETMAXCOL] = {
	//横向
	{ { BORDER, BORDER, BORDER, BORDER, BORDER, BORDER, UNUSE },
	{ NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, UNUSE },
	{ NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, UNUSE },
	{ NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, UNUSE },
	{ NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, UNUSE },
	{ NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, UNUSE },
	{ CREATE, CREATE, CREATE, CREATE, CREATE, CREATE, UNUSE } },
	//纵向
	{ { BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ BORDER, NORMAL, NORMAL, NORMAL, NORMAL, NORMAL, BORDER },
	{ UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE, UNUSE } }
};

StarLayer::StarLayer()
{
	int arrysize = sizeof(StarSprite *)*NODEMAXROW*NODEMAXCOL;
	star_map = (StarSprite**)malloc(arrysize);
	memset((void*)star_map, 0, arrysize);
	arrysize = sizeof(OffsetSprite *)* 2 * OFFSETMAXROW*OFFSETMAXCOL;
	offset_map = (OffsetSprite**)malloc(arrysize);
	memset((void*)offset_map, 0, arrysize);
}
StarLayer::~StarLayer()
{
	if (!star_map)
	{
		free(star_map);
		star_map = nullptr;
	}
	if (!offset_map)
	{
		free(offset_map);
		offset_map = nullptr;
	}
}
Scene* StarLayer::createScene()
{
	auto scene = Scene::create();
	auto layer = StarLayer::create();
	layer->setPosition(0, 0);
	scene->addChild(layer);

	return scene;
}
bool StarLayer::init()
{
	if (!Layer::init())
		return false;
	reset_change();
	//场景布局
	auto visibleSize = Director::getInstance()->getVisibleSize();
	auto bground = Sprite::create("bgr_game_5.png");
	bground->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	this->addChild(bground, -1);
	MenuItemFont* startBtn = MenuItemFont::create("restart", CC_CALLBACK_1(StarLayer::restart, this));
	startBtn->setFontSize(60);
	startBtn->setPosition(320, 880);
	auto menu = Menu::create(startBtn, nullptr);
	menu->setPosition(0, 0);
	this->addChild(menu);
	//.....
	//载入关卡地图
	auto layer = LayerColor::create(Color4B(123, 123, 123, 80), 426, 426);
	layer->setPosition(Vec2(LAYER_W, LAYER_H));
	this->addChild(layer);
	//加载星星
	for (int row = 0; row < NODEMAXROW; row++)
	{
		for (int col = 0; col < NODEMAXCOL; col++)
		{
			auto istar = StarSprite::create(startypemap[row][col]);
			if (!istar)
			{
				//当为空缺时,此点需初始化
			}
			istar->setROW(row);
			istar->setCOL(col);
			auto iposition = Vec2(OFFSETWIDTH + STARWIDTH / 2 + OFFSETWIDTH*col + STARWIDTH*col, OFFSETWIDTH + STARHIGHT / 2 + OFFSETWIDTH*row + STARHIGHT*row);
			istar->setPosition(iposition);
			star_map[row*NODEMAXROW + col] = istar;
			istar->star_map = star_map;
			istar->offset_map = offset_map;
			layer->addChild(istar);
		}
	}
	for (int row = 0; row < OFFSETMAXROW; row++)
	{
		for (int col = 0; col < OFFSETMAXCOL; col++)
		{
			auto ioffset = OffsetSprite::create(HORIZONTAL, offsetmodemap[HORIZONTAL][row][col]);
			ioffset->setROW(row);
			ioffset->setCOL(col);
			auto iposition = Vec2(OFFSETWIDTH + OFFSETHIGHT / 2 + (OFFSETWIDTH + OFFSETHIGHT)*col, OFFSETWIDTH / 2 + (OFFSETWIDTH + OFFSETHIGHT)*row);
			ioffset->setPosition(iposition);
			offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + row*OFFSETMAXROW + col] = ioffset;
			ioffset->star_map = star_map;
			ioffset->offset_map = offset_map;
			layer->addChild(ioffset);

			ioffset = OffsetSprite::create(VERTICAL, offsetmodemap[VERTICAL][row][col]);
			ioffset->setROW(row);
			ioffset->setCOL(col);
			iposition = Vec2(OFFSETWIDTH / 2 + (OFFSETWIDTH + OFFSETHIGHT)*col, OFFSETWIDTH + OFFSETHIGHT / 2 + (OFFSETWIDTH + OFFSETHIGHT)*row);
			ioffset->setPosition(iposition);
			offset_map[VERTICAL*OFFSETMAXROW *OFFSETMAXCOL + row*OFFSETMAXROW + col] = ioffset;
			ioffset->star_map = star_map;
			ioffset->offset_map = offset_map;
			layer->addChild(ioffset);
		}
	}
	initoffsetDirFloor();
	///////////////////////////////////////////////////////////////////
	OffsetSprite * tempoffset = nullptr;
	for (int row = OFFSETMAXROW - 1; row >= 0; --row)
	{
		for (int col = 0; col < OFFSETMAXCOL; ++col)
		{
			tempoffset = offset_map[row*OFFSETMAXROW + col];
			log("[%d][%d] getFallStarDir:%d ,getStarFloor:%d  ", (row + 1), (col + 1), tempoffset->getFallStarDir(), tempoffset->getStarFloor());
		}
		log("\n");
	}
	////////////////////////////////////////////////////////////////////
	auto listen = EventListenerTouchOneByOne::create();
	listen->onTouchBegan = CC_CALLBACK_2(StarLayer::onTouchBegan, this);
	listen->onTouchMoved = CC_CALLBACK_2(StarLayer::onTouchMoved, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listen, this);
	enabletouch = true;
	scheduleUpdate();

	return true;
}
bool StarLayer::onTouchBegan(Touch *touch, Event *event)
{
	///////////////////////////////////////////////////////////////////
	OffsetSprite * tempoffset = nullptr;
	for (int row = OFFSETMAXROW - 1; row >= 0; --row)
	{
		for (int col = 0; col < OFFSETMAXCOL; ++col)
		{
			tempoffset = offset_map[row*OFFSETMAXROW + col];
			log("[%d][%d] getFallStarDir:%d ,getStarFloor:%d,m_downStatus:%d,m_leftStatus %d,m_rightStatus%d,",
				(row + 1), (col + 1), tempoffset->getFallStarDir(), tempoffset->getStarFloor(),
				tempoffset->getDownStatus(), tempoffset->getLeftStatus(), tempoffset->getRightStatus());
			if (tempoffset->getStarByDir(MIDDLE))
			{
				log("DOWNRIGHTStarStatus %d,   ", tempoffset->getStarByDir(MIDDLE)->getStatus());
			}
		}
		log("\n");
	}
	////////////////////////////////////////////////////////////////////
	if (!enabletouch){ successbegan = false; return false; }
	auto location = touch->getLocation();
	src_starsprite = spriteofPoint(location);
	if (!src_starsprite || !src_starsprite->getMoveAble())
	{
		successbegan = false;
		return false;
	}
	log("touchbegan!  src:%d (%d,%d)",
		src_starsprite->getMyColor(), src_starsprite->getROW(), src_starsprite->getCOL());
	successbegan = true;

	return true;

}
void StarLayer::onTouchMoved(Touch *touch, Event *event)
{
	static int i = 0;
	if (!successbegan || !enabletouch){ return; }
	log("(%i)", i++);
	// 获取 初始精灵 的行列  
	int row = src_starsprite->getROW();
	int col = src_starsprite->getCOL();
	// 获取移动到的 点 的位置  
	auto location = touch->getLocation();
	auto halfSpriteWidth = STARWIDTH / 2;
	auto halfSpriteHeight = STARHIGHT / 2;

	//do{}while()判断dirsprite位置
	do{
		auto  upRect = Rect(src_starsprite->getPositionX() + LAYER_W - halfSpriteWidth,
			src_starsprite->getPositionY() + LAYER_H + halfSpriteHeight,
			STARWIDTH,
			STARHIGHT);
		if (upRect.containsPoint(location)) {
			++row;
			if (row < NODEMAXROW) {
				dir_starsprite = star_map[row*NODEMAXROW + col];
				offsetsprite = offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + row*OFFSETMAXROW + col];
				break;
			}
			break;
		}
		auto  downRect = Rect(src_starsprite->getPositionX() + LAYER_W - halfSpriteWidth,
			src_starsprite->getPositionY() + LAYER_H - (halfSpriteHeight * 3),
			STARWIDTH,
			STARHIGHT);
		if (downRect.containsPoint(location)) {
			--row;
			if (row >= 0) {
				dir_starsprite = star_map[row*NODEMAXROW + col];
				offsetsprite = offset_map[HORIZONTAL*OFFSETMAXROW*OFFSETMAXCOL + (row + 1)*OFFSETMAXROW + col];
				break;
			}
			break;
		}
		auto  leftRect = Rect(src_starsprite->getPositionX() + LAYER_W - (halfSpriteWidth * 3),
			src_starsprite->getPositionY() + LAYER_H - halfSpriteHeight,
			STARWIDTH,
			STARHIGHT);
		if (leftRect.containsPoint(location)) {
			--col;
			if (col >= 0) {
				dir_starsprite = star_map[row*NODEMAXROW + col];
				offsetsprite = offset_map[VERTICAL*OFFSETMAXROW*OFFSETMAXCOL + row*OFFSETMAXROW + col + 1];
				break;
			}
			break;
		}
		auto  rightRect = Rect(src_starsprite->getPositionX() + LAYER_W + halfSpriteWidth,
			src_starsprite->getPositionY() + LAYER_H - halfSpriteHeight,
			STARWIDTH,
			STARHIGHT);
		if (rightRect.containsPoint(location)) {
			++col;
			if (col < NODEMAXCOL) {
				dir_starsprite = star_map[row*NODEMAXROW + col];
				offsetsprite = offset_map[VERTICAL*OFFSETMAXROW*OFFSETMAXCOL + row*OFFSETMAXROW + col];
				break;
			}
			break;
		}
	} while (false);

	//dirsprite点不存在,且不可移动,offset点不可穿越,半穿越;就继续获取move点
	if ((!dir_starsprite) ||
		!dir_starsprite->getMoveAble() ||
		offsetsprite->getType() == OFFSETTYPE::BORDER ||
		offsetsprite->getType() == OFFSETTYPE::CREATE ||
		offsetsprite->getType() == OFFSETTYPE::DELIVERY)
	{
		log("move");
		return;
	}
	//间隔为栅栏,实现半穿越返回动画
	else if (offsetsprite->getType() == OFFSETTYPE::LATICE)
	{
		log("LATICL!  src:%d (%d,%d),dir:%d (%d,%d)",
			src_starsprite->getMyColor(), src_starsprite->getROW(), src_starsprite->getCOL(),
			dir_starsprite->getMyColor(), dir_starsprite->getROW(), dir_starsprite->getCOL());
		dir_starsprite->setStatus(ACTIVE);
		auto i_action = MoveBy::create(CHANGETIME, Vec2(offsetsprite->getPositionX() - dir_starsprite->getPositionX(),
			offsetsprite->getPositionY() - dir_starsprite->getPositionY()));
		auto i_sequence = Sequence::createWithTwoActions(i_action, i_action->reverse());
		dir_starsprite->runAction(i_sequence);
		src_starsprite->setStatus(ACTIVE);
		i_action = MoveBy::create(CHANGETIME, Vec2(offsetsprite->getPositionX() - src_starsprite->getPositionX(),
			offsetsprite->getPositionY() - src_starsprite->getPositionY()));
		i_sequence = Sequence::createWithTwoActions(i_action, i_action->reverse());
		src_starsprite->runAction(i_sequence);
		dir_starsprite->stop();
		src_starsprite->stop();
	}
	//间隔为NORMAL,表示可穿越
	else if (offsetsprite->getType() == OFFSETTYPE::NORMAL)
	{
		log("onTouchMoved:NORMAL");

		if (src_starsprite->getStarType() == SUPER_5X || dir_starsprite->getStarType() == SUPER_5X)
		{//有一个是五消
			super5xOther();
		}
		else if (src_starsprite->getStarType() == NORMALSTAR || dir_starsprite->getStarType() == NORMALSTAR)
		{
			//普通碰非五消
			normalNormal();
		}
		else if (src_starsprite->getStarType() == SUPER_33 || dir_starsprite->getStarType() == SUPER_33)
		{
			super33Other();
		}
		else
		{
			super4xsuper4x();
		}
		/*else
		{
		log("error: else if (offsetsprite->getTYPE() == OFFSETTYPE::NORMAL) \n \
		src_starsprite->getARRTIBUTE() == %d || dir_starsprite->getARRTIBUTE() == %d,",src_starsprite->getARRTIBUTE() ,dir_starsprite->getARRTIBUTE());
		}*/
	}
	//触摸点与移动点确认有效,执行操作后清空触摸记录.开始下一轮触摸
	reset_change();
	successbegan = false;

}
void StarLayer::update(float)
{
	StarSprite *spr = NULL;
	enabletouch = true;
	for (int r = 0; r < NODEMAXROW; ++r)    {
		for (int c = 0; c < NODEMAXCOL; ++c)    {
			spr = star_map[r*NODEMAXROW + c];
			if (spr)
			{
				if (STOP != spr->getStatus())
				{
					enabletouch = false;
					return;
				}
			}
		}
	}
	//for (auto i = 0; i < NODEMAXROW*NODEMAXROW; ++i)
	//{

	//}
}
StarSprite * StarLayer::spriteofPoint(Vec2 point)
{
	StarSprite *spr = NULL;
	Rect rect = Rect(0, 0, 0, 0);

	for (int r = 0; r < NODEMAXROW; ++r)    {
		for (int c = 0; c < NODEMAXCOL; ++c)    {
			spr = star_map[r*NODEMAXROW + c];
			if (spr)   {
				rect.origin.x = spr->getPositionX() + LAYER_W - (STARWIDTH / 2);
				rect.origin.y = spr->getPositionY() + LAYER_H - (STARHIGHT / 2);
				rect.size.width = STARWIDTH;
				rect.size.height = STARHIGHT;

				if (rect.containsPoint(point)) {
					return spr;
				}
			}
		}
	}
	return NULL;
}
void StarLayer::reset_change()
{
	//刷新changetag
	src_starsprite = nullptr;
	dir_starsprite = nullptr;
	offsetsprite = nullptr;
}
void StarLayer::getRowChain(StarSprite *star, std::list<StarSprite *> &chainList)
{
	//横向检测
	chainList.push_back(star);// add first sushi

	int neighborCol = star->getCOL() - 1;
	while (neighborCol >= 0) {
		auto  neighborStar = star_map[star->getROW() * NODEMAXROW + neighborCol];
		if (neighborStar && (neighborStar->getMyColor() == star->getMyColor()) && SUPER_5X != neighborStar->getStarType()) {
			chainList.push_back(neighborStar);
			neighborCol--;
		}
		else {
			break;
		}
	}

	neighborCol = star->getCOL() + 1;
	while (neighborCol < NODEMAXCOL) {
		auto neighborStar = star_map[star->getROW() * NODEMAXROW + neighborCol];
		if (neighborStar && (neighborStar->getMyColor() == star->getMyColor()) && SUPER_5X != neighborStar->getStarType()) {
			chainList.push_back(neighborStar);
			neighborCol++;
		}
		else {
			break;
		}
	}
}
void StarLayer::getColChain(StarSprite *star, std::list<StarSprite *> &chainList)
{
	chainList.push_back(star);// add first sushi

	int neighborRow = star->getROW() - 1;
	while (neighborRow >= 0) {
		auto neighborStar = star_map[neighborRow * NODEMAXROW + star->getCOL()];
		if (neighborStar && (neighborStar->getMyColor() == star->getMyColor()) && SUPER_5X != neighborStar->getStarType()) {
			chainList.push_back(neighborStar);
			neighborRow--;
		}
		else {
			break;
		}
	}

	neighborRow = star->getROW() + 1;
	while (neighborRow < NODEMAXROW) {
		auto neighborStar = star_map[neighborRow * NODEMAXROW + star->getCOL()];
		if (neighborStar && (neighborStar->getMyColor() == star->getMyColor()) && SUPER_5X != neighborStar->getStarType()) {
			chainList.push_back(neighborStar);
			neighborRow++;
		}
		else {
			break;
		}
	}
}

void StarLayer::normalNormal()
{
	//完成逻辑交换
	auto posOfSrc = src_starsprite->getPosition();
	auto posOfDest = dir_starsprite->getPosition();
	auto dir_row = dir_starsprite->getROW();
	auto dir_col = dir_starsprite->getCOL();

	star_map[src_starsprite->getROW()*NODEMAXROW + src_starsprite->getCOL()] = dir_starsprite;
	star_map[dir_starsprite->getROW()*NODEMAXROW + dir_starsprite->getCOL()] = src_starsprite;
	dir_starsprite->setROW(src_starsprite->getROW());
	dir_starsprite->setCOL(src_starsprite->getCOL());
	src_starsprite->setROW(dir_row);
	src_starsprite->setCOL(dir_col);

	//执行chain检测
	std::list<StarSprite *> colChainListOfSrc;
	getColChain(src_starsprite, colChainListOfSrc);

	std::list<StarSprite *> rowChainListOfSrc;
	getRowChain(src_starsprite, rowChainListOfSrc);

	std::list<StarSprite *> colChainListOfDir;
	getColChain(dir_starsprite, colChainListOfDir);

	std::list<StarSprite *> rowChainListOfDir;
	getRowChain(dir_starsprite, rowChainListOfDir);
	//可交换
	if (colChainListOfSrc.size() >= 3 ||
		rowChainListOfSrc.size() >= 3 ||
		colChainListOfDir.size() >= 3 ||
		rowChainListOfDir.size() >= 3
		)
	{
		dir_starsprite->setStatus(ACTIVE);
		src_starsprite->setStatus(ACTIVE);

		dir_starsprite->runAction(Sequence::createWithTwoActions(MoveTo::create(CHANGETIME, posOfSrc),
			CallFunc::create(CC_CALLBACK_0(StarSprite::chain, dir_starsprite))));
		src_starsprite->runAction(Sequence::createWithTwoActions(MoveTo::create(CHANGETIME, posOfDest),
			CallFunc::create(CC_CALLBACK_0(StarSprite::chain, src_starsprite))));
	}
	else//不可交换
	{
		dir_row = dir_starsprite->getROW();
		dir_col = dir_starsprite->getCOL();
		star_map[src_starsprite->getROW()*NODEMAXROW + src_starsprite->getCOL()] = dir_starsprite;
		star_map[dir_starsprite->getROW()*NODEMAXROW + dir_starsprite->getCOL()] = src_starsprite;
		dir_starsprite->setROW(src_starsprite->getROW());
		dir_starsprite->setCOL(src_starsprite->getCOL());
		src_starsprite->setROW(dir_row);
		src_starsprite->setCOL(dir_col);

		dir_starsprite->setStatus(ACTIVE);
		src_starsprite->setStatus(ACTIVE);
		
		dir_starsprite->runAction(Sequence::create(MoveTo::create(CHANGETIME, posOfSrc), MoveTo::create(CHANGETIME, posOfDest),
			CallFunc::create(CC_CALLBACK_0(StarSprite::stop, dir_starsprite)), nullptr));
		src_starsprite->runAction(Sequence::create(MoveTo::create(CHANGETIME, posOfDest), MoveTo::create(CHANGETIME, posOfSrc),
			CallFunc::create(CC_CALLBACK_0(StarSprite::stop, src_starsprite)), nullptr));
	}
}
void StarLayer::super5xOther()
{
	if (src_starsprite->getStarType() == SUPER_5X && dir_starsprite->getStarType() == SUPER_5X)
	{
		for (auto i = 0; i < NODEMAXROW*NODEMAXCOL; ++i)
		{
			if (star_map[i])
			{
				star_map[i]->removeFromMap();
			}
		}
	}
	auto super5x = src_starsprite->getStarType() == SUPER_5X ? src_starsprite : dir_starsprite;
	auto otherstar = src_starsprite->getStarType() == SUPER_5X ? dir_starsprite : src_starsprite;
	super5x->setMyColor(otherstar->getMyColor());
	if (otherstar->getStarType() == HORIZONTAL_4X || otherstar->getStarType() == VERTICAL_4X)
	{
		for (auto i = 0; i < NODEMAXROW*NODEMAXCOL; ++i)
		{
			if (star_map[i] && star_map[i]->getMyColor() == otherstar->getMyColor())
			{
				int randHorVer = CCRANDOM_0_1() * 2 + 1;//随机为HOR或者VER
				star_map[i]->setStarType((STARTYPE)randHorVer);
				star_map[i]->removeFromMap();
			}
		}
	}
	else if (otherstar->getStarType() == SUPER_33)
	{
		for (auto i = 0; i < NODEMAXROW*NODEMAXCOL; ++i)
		{
			if (star_map[i] && star_map[i]->getMyColor() == otherstar->getMyColor())
			{
				star_map[i]->setStarType(SUPER_33);
				star_map[i]->removeFromMap();
			}
		}
	}
	else if (otherstar->getStarType() == NORMALSTAR)
	{
		super5x->removeFromMap();
	}
	else
	{
		log("error:super5xOther() \n\
					src_starsprite->getARRTIBUTE() == %d && dir_starsprite->getARRTIBUTE() == %d", src_starsprite->getStarType(), dir_starsprite->getStarType());
	}

}
#define SUPER_33_33SETP (3)
void StarLayer::super33Other()
{
	auto row = src_starsprite->getROW();
	auto col = src_starsprite->getCOL();
	if (src_starsprite->getStarType() == SUPER_33 && dir_starsprite->getStarType() == SUPER_33)
	{
		for (auto i_row = -SUPER_33_33SETP; i_row <= SUPER_33_33SETP; ++i_row)
		for (auto i_col = -SUPER_33_33SETP; i_col <= SUPER_33_33SETP; ++i_col)
		{
			if (row + i_row < 0 || row + i_row >= NODEMAXROW ||
				col + i_col < 0 || col + i_col >= NODEMAXCOL)
			{
				continue;
			}
			if ((abs(i_row) + abs(i_col)) <= SUPER_33_33SETP)
			{
				if (star_map[(row + i_row)*NODEMAXROW + col + i_col])
				{
					star_map[(row + i_row)*NODEMAXROW + col + i_col]->removeFromMap();
				}
			}
		}
	}
	auto super3x = src_starsprite->getStarType() == SUPER_33 ? src_starsprite : dir_starsprite;
	auto otherstar = src_starsprite->getStarType() == SUPER_33 ? dir_starsprite : src_starsprite;
	if (otherstar->getStarType() == HORIZONTAL_4X)
	{
		auto i_row = super3x->getROW();
		for (auto i = i_row - 2; i <= i_row + 2; ++i)
		{
			if (i < 0 || i >= NODEMAXROW)
			{
				continue;
			}
			for (auto j = 0; j < NODEMAXCOL; ++j)
			{
				if (star_map[i*NODEMAXROW + j])
				{
					star_map[i*NODEMAXROW + j]->removeFromMap();
				}
			}
		}
	}
	else if (otherstar->getStarType() == VERTICAL_4X)
	{
		auto i_col = super3x->getCOL();
		for (auto i = i_col - 2; i <= i_col + 2; ++i)
		{
			if (i < 0 || i >= NODEMAXCOL)
			{
				continue;
			}
			for (auto j = 0; j < NODEMAXROW; ++j)
			{
				if (star_map[j*NODEMAXROW + i])
				{
					star_map[j*NODEMAXROW + i]->removeFromMap();
				}
			}
		}
	}
	else
	{
		log("error:super33Other() \n\
					src_starsprite->getARRTIBUTE() == %d && dir_starsprite->getARRTIBUTE() == %d", src_starsprite->getStarType(), dir_starsprite->getStarType());
	}
}
void StarLayer::super4xsuper4x()
{
	src_starsprite->removeFromMap();
	dir_starsprite->removeFromMap();
}
void StarLayer::restart(cocos2d::Object * pSender)
{
	Director::getInstance()->replaceScene(TransitionFade::create(0.7f, StarLayer::createScene()));
}
void StarLayer::initoffsetDirFloor()
{
	OffsetSprite * tempoffset = nullptr;
	for (int row = OFFSETMAXROW - 1; row >= 0; --row)
	{
		for (int col = 0; col < OFFSETMAXCOL; ++col)
		{
			tempoffset = offset_map[row*OFFSETMAXROW + col];
			auto upOffset = tempoffset->getOffsetByDir(UPMIDDLE);
			if (CREATE == tempoffset->getType())
			{
				tempoffset->setFallStarDir(CREATESTAR);
				tempoffset->setStarFloor(1000);
			}
			else if (!upOffset)
			{
				tempoffset->getupRorLdir();
			}
			else if (CREATE == upOffset->getType())
			{
				tempoffset->setFallStarDir(UPMIDDLE);
				tempoffset->setStarFloor(upOffset->getStarFloor() + 1);
			}
			else if (NORMAL == upOffset->getType())
			{
				auto upStar = tempoffset->getStarByDir(UPMIDDLE);
				if (!upStar)
				{
					if (NOWAY == upOffset->getFallStarDir())
					{
						tempoffset->getupRorLdir();
					}
					else
					{
						tempoffset->setFallStarDir(UPMIDDLE);
						tempoffset->setStarFloor(upOffset->getStarFloor() + 1);
					}
				}
				else if (upStar)
				{
					if (!upStar->getMoveAble())
					{
						tempoffset->getupRorLdir();
					}
					else
					{
						tempoffset->setFallStarDir(UPMIDDLE);
						tempoffset->setStarFloor(upOffset->getStarFloor() + 1);
					}
				}
			}
			else if (DELIVERY == upOffset->getType())
			{
				//传送点类型
			}
			else
			{//从up左右寻找最佳路径
				tempoffset->getupRorLdir();
			}
		}
	}
}