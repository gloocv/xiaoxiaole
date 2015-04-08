#ifndef CONTROLNODE_H
#define CONTROLNODE_H
#include"cocos2d.h"

USING_NS_CC;

typedef enum
{
	TOUCHMOVE=0,
	REMOVE,
	STOP,
	MOVEDOWN,
}CONTROLMODE;

class ControlNode :public Node
{
public:
	ControlNode();
	~ControlNode();
	CREATE_FUNC(ControlNode);
	virtual void update(float) override;
	CC_SYNTHESIZE(CONTROLMODE, m_mode, MODE);

};






#endif