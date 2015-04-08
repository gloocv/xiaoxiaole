
#include"OffsetSprite.h"

USING_NS_CC;

OffsetSprite::OffsetSprite()
:m_leftStatus(OFFSETSTATUS::UNACTIVATION)
, m_rightStatus(OFFSETSTATUS::UNACTIVATION)
, m_downStatus(OFFSETSTATUS::UNACTIVATION)
, m_Status(OFFSETSTATUS::UNACTIVATION)
, m_row(-1)
, m_col(-1)
, m_fallstardir(DIR::NOWAY)
, m_starfloor(0)
{}

OffsetSprite* OffsetSprite::create(HORVER horver, OFFSETTYPE type)
{
	auto offset = new OffsetSprite();

	if (offset&&offset->init(horver, type))
	{
		offset->autorelease();
		return offset;
	}
	else
	{
		delete offset;
		offset = NULL;
		return NULL;
	}
}
bool OffsetSprite::init(HORVER horver, OFFSETTYPE type)
{
	if (!Sprite::init()){
		return false;
	}
	this->setType(type);
	this->setHorver(horver);
	this->initWithFile(offsetPng[horver][type]);
	scheduleUpdate();
	return true;
}
void OffsetSprite::update(float dt) 
{
	if (UNACTIVATION == m_Status)
	{
		return;
	}
	else 
	{	
		auto i_row = m_row;
		auto i_col = m_col;
		auto star = getStarByDir(MIDDLE);
		if (NOWAY == m_fallstardir && !star)
		{
			//m_starfloor = 0;//由于resetOffsetDir里做递归,此处被注释
			m_Status = UNACTIVATION;
			resetOffsetDir();
		}
		else if (star)
		{
			if (m_downStatus || m_leftStatus || m_rightStatus)
			{//如果上面存在star,且下方有需要star的,则什么都不做,等待下方获取或者设置flase
				return;
			}
			else
			{
				if (ACTIVE == star->getStatus())
				{
					if (SUPER_5X == star->getStarType())
					{
						star->stop();
						return;
					}
					star->chain();
					return;
				}
				return; 
			}
		}
		else if (!star)
		{//有填充路径
			fallStarFromWay();
			return;
		}
		else
		{
			log("error:OffsetSprite::update");
		}
	} 
}
void OffsetSprite::fallStarFromWay()
{
	auto falledoffset = getOffsetByDir(m_fallstardir);
	if (CREATE == falledoffset->getType())
	{
		int randcolor = CCRANDOM_0_1() * 5;
		auto newstar = StarSprite::create((STARCOLOR)randcolor);
		newstar->setPosition(this->getPositionX(), this->getPositionY() + (STARHIGHT / 2 + OFFSETWIDTH / 2)*1.5);
		this->getParent()->addChild(newstar);
		newstar->setROW(m_row);
		newstar->setCOL(m_col);
		star_map[m_row*NODEMAXROW + m_col] = newstar;
		newstar->star_map = star_map;
		newstar->offset_map = offset_map;
		newstar->setStatus(MOVEDOWNMID);
		newstar->runAction(Sequence::createWithTwoActions(MoveTo::create(DOWNTIME*0.5, Vec2(this->getPositionX(), this->getPositionY() + STARHIGHT / 2 + OFFSETWIDTH / 2))
			, CallFunc::create(CC_CALLBACK_0(StarSprite::active,newstar))));
	}
	else if (NORMAL == falledoffset->getType())
	{
		switch (m_fallstardir)
		{
		case UPMIDDLE:
			if (!getStarByDir(UPMIDDLE))
			{
				getOffsetByDir(UPMIDDLE)->setDownStatus(ACTIVATION);
				return;
			}
			if (ACTIVE == getStarByDir(UPMIDDLE)->getStatus() || STOP == getStarByDir(UPMIDDLE)->getStatus())
			{
				getStarByDir(UPMIDDLE)->movedownMid();
				return;
			}
			else
			{
				getOffsetByDir(UPMIDDLE)->setDownStatus(ACTIVATION);
				return;
			}
			break;
		case UPRIGHT:
			if (!getStarByDir(UPRIGHT))
			{
				getOffsetByDir(UPRIGHT)->setLeftStatus(ACTIVATION);
				return;
			}
			if ((ACTIVE == getStarByDir(UPRIGHT)->getStatus() || STOP == getStarByDir(UPRIGHT)->getStatus())&&
				UNACTIVATION == getOffsetByDir(UPRIGHT)->getDownStatus())
			{
				getStarByDir(UPRIGHT)->movedownLeft();
				return;
			}
			else
			{
				getOffsetByDir(UPRIGHT)->setLeftStatus(ACTIVATION);
				return;
			}
			break;
		case UPLEFT:
			if (!getStarByDir(UPLEFT))
			{
				getOffsetByDir(UPLEFT)->setRightStatus(ACTIVATION);
				return;
			}
			if ((ACTIVE == getStarByDir(UPLEFT)->getStatus() || STOP == getStarByDir(UPLEFT)->getStatus())&&
				UNACTIVATION == getOffsetByDir(UPLEFT)->getDownStatus())
			{
				getStarByDir(UPLEFT)->movedownRight();
				return;
			}
			else
			{
				getOffsetByDir(UPLEFT)->setRightStatus(ACTIVATION);
				return;
			}
			break;
		case NOWAY:
			log("error:getOffsetFromWay (NOWAY)");
			break;
		default:
			log("error:getOffsetFromWay (default)");
			break;
		}
	}
	else if (DELIVERY == falledoffset->getType())
	{
	}
	else
	{
		log("error:fallStarFromWay");
	}
	return;
}
void OffsetSprite::resetOffsetDir()
{
	auto formerFallDir = m_fallstardir;
	auto formerStarFloor = m_starfloor;

	auto upOffset = getOffsetByDir(UPMIDDLE);
	if (CREATE==m_type)
	{
		m_fallstardir = CREATESTAR;
		m_starfloor = 1000;
	}
	else if (!upOffset)
	{
		getupRorLdir();
	}
	else if (CREATE == upOffset->getType())
	{
		m_fallstardir = UPMIDDLE;
		m_starfloor = upOffset->getStarFloor() + 1;
	}
	else if (NORMAL == upOffset->getType())
	{
		auto upStar = getStarByDir(UPMIDDLE);
		if (!upStar)
		{
			if (NOWAY == upOffset->getFallStarDir())
			{
				getupRorLdir();
			}
			else
			{
				m_fallstardir = UPMIDDLE;
				m_starfloor = upOffset->getStarFloor() + 1;
			}
		}
		else if (upStar)
		{
			if (!upStar->getMoveAble())
			{
				getupRorLdir();
			}
			else 
			{
				m_fallstardir = UPMIDDLE;
				m_starfloor = upOffset->getStarFloor() + 1;
			}
		}
	}
	else if (DELIVERY == upOffset->getType())
	{
		//传送点类型
	}
	else 
	{//从up左右寻找最佳路径
		getupRorLdir();
	}

	if (formerFallDir != m_fallstardir || formerStarFloor != m_starfloor)
	{
		if (getOffsetByDir(DOWNMIDDLE))
		{
			getOffsetByDir(DOWNMIDDLE)->resetOffsetDir();
		}
		if (getOffsetByDir(DOWNRIGHT))
		{
			getOffsetByDir(DOWNRIGHT)->resetOffsetDir();
		}
		if (getOffsetByDir(DOWNLEFT))
		{
			getOffsetByDir(DOWNLEFT)->resetOffsetDir();
		}
	}
}
void OffsetSprite::getupRorLdir()
{
	auto upRightOffset = getOffsetByDir(UPRIGHT);
	auto upLeftOffset = getOffsetByDir(UPLEFT);
	auto upRightStarFloor = 0;
	auto upLeftStarFloor = 0;
	if (upRightOffset)
	{
		if (NORMAL == upRightOffset->getType())
		{
			auto upStar = upRightOffset->getStarByDir(MIDDLE);
			if (!upStar)
			{
				if (NOWAY == upRightOffset->getFallStarDir())
				{
						upRightStarFloor = 0;
				}
				else
				{
					upRightStarFloor = upRightOffset->getStarFloor();
				}
			}
			else if (upStar)
			{
 				if (!upStar->getMoveAble())
				{
					upRightStarFloor = 0;
				}
				else
				{
					upRightStarFloor = upRightOffset->getStarFloor();
				}
			}
		}
		else if (DELIVERY == upRightOffset->getType())
		{
			//传送点类型
		}
	}
	if (upLeftOffset)
	{
		if (NORMAL == upLeftOffset->getType())
		{
			auto upStar = upLeftOffset->getStarByDir(MIDDLE);
			if (!upStar)
			{
				if (NOWAY == upLeftOffset->getFallStarDir())
				{
						upLeftStarFloor = 0;
				}
				else
				{
					upLeftStarFloor = upLeftOffset->getStarFloor();
				}
			}
			else if (upStar)
			{
				if (!upStar->getMoveAble())
				{
					upLeftStarFloor = 0;

				}
				else
				{
					upLeftStarFloor = upLeftOffset->getStarFloor();
				}
			}
		}
		else if (DELIVERY == upLeftOffset->getType())
		{
			//传送点类型
		}
	}
	if (0 == upRightStarFloor && 0 == upLeftStarFloor)
	{
		m_fallstardir = NOWAY;
		if (getStarByDir(MIDDLE))
		{
			m_starfloor = 1;
		}
		else
		{
			m_starfloor = 0;
		}
	}
	else
	{
		if (upRightStarFloor > upLeftStarFloor)
		{
			m_fallstardir = UPRIGHT,
				m_starfloor = upRightStarFloor+1;
		}
		else
		{
			m_fallstardir = UPLEFT,
				m_starfloor = upLeftStarFloor+1;
		}
	}
}
StarSprite * OffsetSprite::getStarByDir(DIR dir)
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
OffsetSprite * OffsetSprite::getOffsetByDir(DIR dir)
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