
#include"StarSprite.h"
#include"StarLayer.h"
#include"SimpleAudioEngine.h"
USING_NS_CC;
using namespace CocosDenshion;

StarSprite::StarSprite()
:m_status(STATUS::STOP)
, m_row(-1)
, m_col(-1)
, m_startype(STARTYPE::NORMALSTAR)
, m_nearIsRemove(false)
, m_moveable(true)
, m_colored(true)
{}

StarSprite* StarSprite::create(STARCOLOR starcolor)
{
	auto star = new StarSprite();
	if (star&&star->init(starcolor))
	{
		star->autorelease();
		return star;
	}
	else
	{
		delete star;
		star = NULL;
		return NULL;
	}
}
bool StarSprite::init(STARCOLOR starcolor)
{
	if (!Sprite::init()){
		return false;
	}
	this->setMyColor(starcolor);
	this->initWithFile(starPng[starcolor]);
	scheduleUpdate();
	return true;
}
void StarSprite::update(float dt)
{
	if (CHAIN == m_status)
	{
		chain();
	}
	//判断star状态,执行动作,
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void StarSprite::broadcastRemove()
{
	if (getStarByDir(DOWNMIDDLE))
	{
		getStarByDir(DOWNMIDDLE)->setNearIsRemove(true);
	}
	if (getStarByDir(UPMIDDLE))
	{
		getStarByDir(UPMIDDLE)->setNearIsRemove(true);
	}
	if (getStarByDir(RIGHT))
	{
		getStarByDir(RIGHT)->setNearIsRemove(true);
	}
	if (getStarByDir(LEFT))
	{
		getStarByDir(LEFT)->setNearIsRemove(true);
	}

}
bool StarSprite::getColChain(StarSprite *star, std::list<StarSprite *> &chainList)
{
	log("getColChain");
	chainList.push_back(star);// add first sushi

	int neighborRow = star->getROW() - 1;
	while (neighborRow >= 0) {
		auto neighborStar = star_map[neighborRow * NODEMAXROW + star->getCOL()];
		auto nweghborOffset = offset_map[neighborRow * OFFSETMAXROW + star->getCOL()];
		if (neighborStar)
		{
			auto instatus = neighborStar->getStatus();
			if (!m_colored || SUPER_5X == neighborStar->getStarType())
			{
				break;
			}
			else if (MOVEDOWNMID == instatus || MOVEDOWNLEFT == instatus || MOVEDOWNRIGHT == instatus )
			{
				if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
				{
					return false;
				}
				break;
			}
			else if (neighborStar->getMyColor() == star->getMyColor()) {
				chainList.push_back(neighborStar);
				neighborRow--;
			}
			else {
				break;
			}
		}
		else
		{
			if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
			{//若要填补,需移动检测点,所以弹出,若检测不足可实现向下填补
				break;
			}
			break;
		}


	}

	neighborRow = star->getROW() + 1;
	while (neighborRow < NODEMAXROW) {
		auto neighborStar = star_map[neighborRow * NODEMAXROW + star->getCOL()];
		auto nweghborOffset = offset_map[neighborRow * OFFSETMAXROW + star->getCOL()];
		if (neighborStar)
		{
			auto instatus = neighborStar->getStatus();
			if (!m_colored || SUPER_5X == neighborStar->getStarType())
			{
				break;
			}
			else if (MOVEDOWNMID == instatus || MOVEDOWNLEFT == instatus || MOVEDOWNRIGHT == instatus )
			{
				if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
				{
					return false;
				}
				break;
			}
			else if ((neighborStar->getMyColor() == star->getMyColor())) {
				chainList.push_back(neighborStar);
				neighborRow++;
			}
			else {
				break;
			}
		}
		else
		{
			if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
			{
				return false;
			}
			break;
		}
	}
	return true;
}
bool StarSprite::getRowChain(StarSprite *star, std::list<StarSprite *> &chainList)
{
	log("getRowChain");
	chainList.push_back(star);// add first sushi

	int neighborCol = star->getCOL() - 1;
	while (neighborCol >= 0) {
		auto  neighborStar = star_map[star->getROW() * NODEMAXROW + neighborCol];
		auto nweghborOffset = offset_map[star->getROW() * OFFSETMAXROW + neighborCol];
		if (neighborStar)
		{
			auto instatus = neighborStar->getStatus();
			if (!m_colored || SUPER_5X == neighborStar->getStarType())
			{
				break;
			}
			else if (MOVEDOWNMID == instatus || MOVEDOWNLEFT == instatus || MOVEDOWNRIGHT == instatus )
			{
				if (neighborStar->getOffsetByDir(MIDDLE) && NOWAY != neighborStar->getOffsetByDir(MIDDLE)->getFallStarDir())
				{
					return false;
				}
				break;
			}
			else if (neighborStar->getMyColor() == star->getMyColor()) {
				chainList.push_back(neighborStar);
				neighborCol--;
			}
			else {
				break;
			}
		}
		else
		{
			if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
			{
				return false;
			}
			break;
		}
	}

	neighborCol = star->getCOL() + 1;
	while (neighborCol < NODEMAXCOL) {
		auto neighborStar = star_map[star->getROW() * NODEMAXROW + neighborCol];
		auto nweghborOffset = offset_map[star->getROW() * OFFSETMAXROW + neighborCol];
		if (neighborStar)
		{
			auto instatus = neighborStar->getStatus();
			if (!m_colored || SUPER_5X == neighborStar->getStarType())
			{
				break;
			}
			else if (MOVEDOWNMID == instatus || MOVEDOWNLEFT == instatus || MOVEDOWNRIGHT == instatus )
			{
				if (neighborStar->getOffsetByDir(MIDDLE) && NOWAY != neighborStar->getOffsetByDir(MIDDLE)->getFallStarDir())
				{
					return false;
				}
				break;
			}
			else if (neighborStar->getMyColor() == star->getMyColor()) {
				chainList.push_back(neighborStar);
				neighborCol++;
			}
			else {
				break;
			}
		}
		else
		{
			if (nweghborOffset && NOWAY != nweghborOffset->getFallStarDir())
			{
				return false;
			}
			break;
		}
	}
	return true;
}
void StarSprite::setliststatus(std::list<StarSprite *> &rowChainList, std::list<StarSprite *> &colChainList)
{
	log("setliststatus");
	auto row = rowChainList.size();
	auto col = colChainList.size();
	StarSprite *tempstar;
	if ((3 == row && 3 > col) || (3 > row && 3 == col))
	{
		if (3 == row)
		{
			for (auto star = rowChainList.begin(); star != rowChainList.end(); ++star){
				(*star)->removeFromMap();
			}
		}
		if (3 == col)
		{
			for (auto star = begin(colChainList); star != end(colChainList); ++star){
				(*star)->removeFromMap();
			}
		}
	}
	else if (5 > row && 5 > col && 3 <= row && 3 <= col)
	{
		tempstar = rowChainList.front();
		if (NORMALSTAR == tempstar->getStarType())
		{//自身为普通star,自身变为特殊star
			tempstar->setStarType(SUPER_33);
			tempstar->stop();
			rowChainList.pop_front();
			colChainList.pop_front();
		}
		else
		{//自身为特殊star,长的边尾巴变为特殊star
			auto tempchainlist = row >= col ? rowChainList : colChainList;
			tempstar = tempchainlist.back();
			tempstar->setStarType(SUPER_33);
			tempstar->stop();
			tempchainlist.pop_back();
		}
		for (auto star = begin(rowChainList); star != end(rowChainList); ++star)
		{
			(*star)->removeFromMap();
		}
		for (auto star = begin(colChainList); star != end(colChainList); ++star)
		{
			(*star)->removeFromMap();
		}
	}
	else if (4 == row && 3 > col || 3 > row && 4 == col)
	{
		std::list<StarSprite*> tempelist;
		STARTYPE tempAttribute;
		if (4 == row)
		{
			tempelist = rowChainList;
			tempAttribute = VERTICAL_4X;
		}
		else
		{
			tempelist = colChainList;
			tempAttribute = HORIZONTAL_4X;
		}
		tempstar = tempelist.front();
		if (tempstar->getStarType() == NORMALSTAR)
		{//自身为普通star,自身变为特殊star
			tempstar->setStarType(tempAttribute);
			tempstar->stop();
			tempelist.pop_front();
		}
		else
		{//自身为特殊star,长的边尾巴变为特殊star
			tempstar = tempelist.back();
			tempstar->setStarType(tempAttribute);
			tempstar->stop();
			tempelist.pop_back();
		}
		for (auto star = begin(tempelist); star != end(tempelist); ++star)
		{
			(*star)->removeFromMap();
		}
	}
	else if (5 <= row || 5 <= col)
	{
		auto templist = row >= col ? rowChainList : colChainList;
		auto shortchainlist = row < col ? rowChainList : colChainList;
		tempstar = templist.front();
		if (tempstar->getStarType() == NORMALSTAR)
		{
			tempstar->setStarType(SUPER_5X);
			tempstar->stop();
			templist.pop_front();
			shortchainlist.pop_front();
		}
		else
		{
			tempstar = templist.back();
			tempstar->setStarType(SUPER_5X);
			tempstar->stop();
			templist.pop_back();
		}
		if (shortchainlist.size() >= 2)
		{
			for (auto star = shortchainlist.begin(); star != shortchainlist.end(); star++)
			{
				(*star)->removeFromMap();
			}
		}
		for (auto star = templist.begin(); star != templist.end(); star++)
		{
			(*star)->removeFromMap();
		}
	}
	else
	{
		this->stop();
	}
}
void StarSprite::stop()
{
	m_status = STOP;
	auto midoffset = offset_map[HORIZONTAL + m_row*OFFSETMAXROW + m_col];
	midoffset->setStatus(UNACTIVATION);
	if (midoffset->getOffsetByDir(UPMIDDLE))
	{
		midoffset->getOffsetByDir(UPMIDDLE)->setDownStatus(UNACTIVATION);
	}
	if (midoffset->getOffsetByDir(UPRIGHT))
	{
		midoffset->getOffsetByDir(UPRIGHT)->setLeftStatus(UNACTIVATION);
	}
	if (midoffset->getOffsetByDir(UPLEFT))
	{
		midoffset->getOffsetByDir(UPLEFT)->setRightStatus(UNACTIVATION);
	}
	return;
}
void StarSprite::chain()
{
	m_status = CHAIN;
	auto midoffset = offset_map[HORIZONTAL + m_row*OFFSETMAXROW + m_col];
	midoffset->setStatus(UNACTIVATION);
	if (midoffset->getOffsetByDir(UPMIDDLE))
	{
		midoffset->getOffsetByDir(UPMIDDLE)->setDownStatus(UNACTIVATION);
	}
	if (midoffset->getOffsetByDir(UPRIGHT))
	{
		midoffset->getOffsetByDir(UPRIGHT)->setLeftStatus(UNACTIVATION);
	}
	if (midoffset->getOffsetByDir(UPLEFT))
	{
		midoffset->getOffsetByDir(UPLEFT)->setRightStatus(UNACTIVATION);
	}
	std::list<StarSprite *> rowChainList;
	std::list<StarSprite *> colChainList;
	if (!getRowChain(this, rowChainList) || !getColChain(this, colChainList))
	{//检测失败表示可检测位置有未补全star
		return;
	}
	else
	{
		setliststatus(rowChainList, colChainList);
	}
}
void StarSprite::active()
{
	m_status = ACTIVE;
	return;
}
void StarSprite::removeFromMap()
{
	if (REMOVE == m_status)return;

	m_status = REMOVE;
	if (this->getStarType() == NORMALSTAR)
	{

	}
	else if (this->getStarType() == HORIZONTAL_4X)
	{
		for (auto i_col = 0; i_col < NODEMAXCOL; ++i_col)
		{

			if (star_map[m_row*NODEMAXROW + i_col])
			{
				star_map[m_row*NODEMAXROW + i_col]->removeFromMap();
			}
		}
	}
	else if (this->getStarType() == VERTICAL_4X)
	{
		for (auto i_row = 0; i_row < NODEMAXCOL; ++i_row)
		{
			if (star_map[i_row*NODEMAXROW + m_col])
			{
				star_map[i_row*NODEMAXROW + m_col]->removeFromMap();
			}
		}
	}
#define SUPER_33SETP (2)//33消除步长
	else if (this->getStarType() == SUPER_33)
	{
		for (auto i_row = -SUPER_33SETP; i_row <= SUPER_33SETP; ++i_row)
		for (auto i_col = -SUPER_33SETP; i_col <= SUPER_33SETP; ++i_col)
		{
			if ((m_row + i_row) < 0 || (m_row + i_row) >= NODEMAXROW ||
				(m_col + i_col) < 0 || (m_col + i_col) >= NODEMAXCOL)
			{
				continue;
			}
			if ((abs(i_row) + abs(i_col)) <= SUPER_33SETP)
			{
				if (star_map[(m_row + i_row)*NODEMAXROW + m_col + i_col])
				{
					star_map[(m_row + i_row)*NODEMAXROW + m_col + i_col]->removeFromMap();
				}
			}
		}
	}
	else if (this->getStarType() == SUPER_5X)
	{
		//此处添加旋转特效
		/*ccBezierConfig bezier;
		bezier.endPosition = this->getPosition();
		this->runAction(RotateBy::create(1.0, 1080));*/
		for (auto i = 0; i < NODEMAXROW*NODEMAXCOL; ++i)
		{
			if (star_map[i] && star_map[i]->getMyColor() == m_color)
			{
				star_map[i]->removeFromMap();
				/*bezier.controlPoint_1 = star_map[i]->getPosition();
				bezier.controlPoint_2 = Vec2(200, -s.height / 2);
				auto bezier_act = BezierTo::create(0.9, bezier);
				star_map[m_row*NODEMAXROW + m_col] = nullptr;
				offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + (m_row + 1)*OFFSETMAXROW + m_col]->setSTATUS(ACTIVATION);
				broadcastRemove();*/
			}
		}
	}
	else
	{
		log("error:removeFromMap() \nthis->getARRTIBUTE() == %d", this->getStarType());
	}
	//remove from star_map,active the up offset
	this->runAction(Sequence::createWithTwoActions(Hide::create(), ScaleTo::create(0, 0.1)));
	this->setTexture("flash.png");
	this->runAction(Sequence::create(Show::create(), ScaleTo::create(0.2, 1.8), CallFunc::create(CC_CALLBACK_0(StarSprite::callBackRemove, this)), nullptr));
	return;
}
void StarSprite::movedownMid()
{
	//log("movedown");
	m_status = MOVEDOWNMID;
	auto i_row = m_row;
	while (offset_map[HORIZONTAL + i_row*OFFSETMAXROW + m_col]->getType() == NORMAL && star_map[(i_row - 1) *NODEMAXROW + m_col] == nullptr)
	{
		--i_row;
	};
	auto posoffset = offset_map[HORIZONTAL + i_row*OFFSETMAXROW + m_col]->getPosition();
	this->runAction(Sequence::createWithTwoActions(MoveTo::create(DOWNTIME*(m_row - i_row), Vec2(posoffset.x, posoffset.y + STARHIGHT / 2 + OFFSETWIDTH / 2)),
		CallFunc::create(CC_CALLBACK_0(StarSprite::active, this))));
	offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + m_row*OFFSETMAXROW + m_col]->setStatus(ACTIVATION);
	star_map[m_row*NODEMAXROW + m_col] = nullptr;
	star_map[i_row*NODEMAXROW + m_col] = this;
	this->setROW(i_row);
}
void StarSprite::movedownRight()
{
	//log("movedownright");
	m_status = MOVEDOWNRIGHT;
	if ((m_row - 1) > 0 && (m_col + 1) < NODEMAXROW && star_map[(m_row - 1) *NODEMAXROW + m_col + 1] == nullptr)
	{
		this->runAction(Sequence::createWithTwoActions(MoveBy::create(DOWNTIME, Vec2(STARWIDTH + OFFSETWIDTH, -STARHIGHT - OFFSETWIDTH)),
			CallFunc::create(CC_CALLBACK_0(StarSprite::active, this))));
		offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + m_row*OFFSETMAXROW + m_col]->setStatus(ACTIVATION);
		star_map[m_row*NODEMAXROW + m_col] = nullptr;
		star_map[(m_row - 1)*NODEMAXROW + m_col + 1] = this;
		this->setROW(m_row - 1);
		this->setCOL(m_col + 1);
		return;
	}
	this->active();
}
void StarSprite::movedownLeft()
{
	//log("movedownleft");
	m_status = MOVEDOWNLEFT;
	if ((m_row - 1) > 0 && (m_col - 1) > 0 && star_map[(m_row - 1) *NODEMAXROW + m_col - 1] == nullptr)
	{
		this->runAction(Sequence::createWithTwoActions(MoveBy::create(DOWNTIME, Vec2(-STARWIDTH - OFFSETWIDTH, -STARHIGHT - OFFSETWIDTH)),
			CallFunc::create(CC_CALLBACK_0(StarSprite::active, this))));
		offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + m_row*OFFSETMAXROW + m_col]->setStatus(ACTIVATION);
		star_map[m_row*NODEMAXROW + m_col] = nullptr;
		star_map[(m_row - 1)*NODEMAXROW + m_col - 1] = this;
		this->setROW(m_row - 1);
		this->setCOL(m_col - 1);
		return;
	}
	this->active();
}

void StarSprite::setStarType(STARTYPE startype)
{
	m_startype = startype;
	switch (startype)
	{
	case NORMALSTAR:
		this->setTexture(starPng[this->getMyColor()]);
		break;
	case HORIZONTAL_4X:
		this->setTexture(starPng_4h[this->getMyColor()]);
		break;
	case VERTICAL_4X:
		this->setTexture(starPng_4v[this->getMyColor()]);
		break;
	case SUPER_33:
		this->setTexture(starPng_33[this->getMyColor()]);
		break;
	case SUPER_5X:
		this->setTexture(super_5x);
		break;
	default:
		break;
	}
}
STARTYPE StarSprite::getStarType()
{
	return m_startype;
}
void StarSprite::callBackRemove()
{
	star_map[m_row*NODEMAXROW + m_col] = nullptr;
	offset_map[HORIZONTAL*OFFSETMAXROW *OFFSETMAXCOL + m_row*OFFSETMAXROW + m_col]->setStatus(ACTIVATION);
	broadcastRemove();
	this->removeFromParent();
}
StarSprite * StarSprite::getStarByDir(DIR dir)
{
	int i_row;
	int i_col;
	switch (dir)
	{
	case MIDDLE:
		i_row = m_row;
		i_col = m_col;
		break;
	case RIGHT:
		i_row = m_row;
		i_col = m_col + 1;
		break;
	case LEFT:
		i_row = m_row;
		i_col = m_col - 1;
		break;
	case UPMIDDLE:
		i_row = m_row + 1;
		i_col = m_col;
		break;
	case UPRIGHT:
		i_row = m_row + 1;
		i_col = m_col + 1;
		break;
	case UPLEFT:
		i_row = m_row + 1;
		i_col = m_col - 1;
		break;
	case DOWNMIDDLE:
		i_row = m_row - 1;
		i_col = m_col;
		break;
	case DOWNRIGHT:
		i_row = m_row - 1;
		i_col = m_col - 1;
		break;
	case DOWNLEFT:
		i_row = m_row - 1;
		i_col = m_col + 1;
		break;
	default:
		log("error");
		return nullptr;
	}
	if (0 <= i_row && i_row < NODEMAXROW && 0 <= i_col && i_col < NODEMAXCOL)
	{
		return star_map[i_row*NODEMAXROW + i_col];
	}
	else
	{
		return nullptr;
	}
}
OffsetSprite * StarSprite::getOffsetByDir(DIR dir)
{
	int i_row;
	int i_col;
	switch (dir)
	{
	case MIDDLE:
		i_row = m_row;
		i_col = m_col;
		break;
	case RIGHT:
		i_row = m_row;
		i_col = m_col + 1;
		break;
	case LEFT:
		i_row = m_row;
		i_col = m_col - 1;
		break;
	case UPMIDDLE:
		i_row = m_row + 1;
		i_col = m_col;
		break;
	case UPRIGHT:
		i_row = m_row + 1;
		i_col = m_col + 1;
		break;
	case UPLEFT:
		i_row = m_row + 1;
		i_col = m_col - 1;
		break;
	case DOWNMIDDLE:
		i_row = m_row - 1;
		i_col = m_col;
		break;
	case DOWNRIGHT:
		i_row = m_row - 1;
		i_col = m_col - 1;
		break;
	case DOWNLEFT:
		i_row = m_row - 1;
		i_col = m_col + 1;
		break;
	default:
		log("error");
		return nullptr;
	}
	if (0 <= i_row && i_row < OFFSETMAXROW && 0 <= i_col && i_col < OFFSETMAXCOL)
	{
		return offset_map[i_row*OFFSETMAXROW + i_col];
	}
	else
	{
		return nullptr;
	}
}


