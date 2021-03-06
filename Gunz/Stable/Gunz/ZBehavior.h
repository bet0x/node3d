#ifndef _ZBEHAVIOR_H
#define _ZBEHAVIOR_H


#include "ZStateMachine.h"

/// AI가 하는 일
enum ZBEHAVIOR_STATE
{
	ZBEHAVIOR_STATE_IDLE			=0,				///< 가만히 있는다.
	ZBEHAVIOR_STATE_PATROL,							///< 순찰
	ZBEHAVIOR_STATE_ATTACK,							///< 공격
	ZBEHAVIOR_STATE_RETREAT,						///< 도망
	ZBEHAVIOR_STATE_SCRIPT,							///< 스크립트
	ZBEHAVIOR_STATE_END
};

enum ZBEHAVIOR_INPUT 
{
	ZBEHAVIOR_INPUT_NONE = 0,
	ZBEHAVIOR_INPUT_ATTACKED,						///< 공격당함

	ZBEHAVIOR_INPUT_END
};

// 공격 특성
enum ZOFFENSETYPE
{
	ZOFFENSETYPE_MELEE		= 1,					// 근거리 공격형
	ZOFFENSETYPE_RANGE		= 2,					// 원거리 공격형
};


class ZBrain;

/// Behavior State 추상 클래스
class ZBehaviorState : public ZState
{
protected:
	ZBrain*		m_pBrain;

	virtual void OnEnter() {}
	virtual void OnExit() {}
	virtual void OnRun(float fDelta) {}
public:
	ZBehaviorState(ZBrain* pBrain, int nStateID) : ZState(nStateID), m_pBrain(pBrain) { }
	virtual ~ZBehaviorState() { }
	void Run(float fDelta)	{ OnRun(fDelta); }
	void Enter()			{ OnEnter(); }
	void Exit()				{ OnExit(); }
};



class ZBehavior
{
private:
	ZStateMachine		m_FSM;
	ZBehaviorState*		m_pCurrState;
	ZBrain*				m_pBrain;
	ZOFFENSETYPE		m_nOffecseType;
	bool				m_bFriendly;
	void ChangeBehavior(ZBEHAVIOR_STATE nState);

public:
	ZBehavior();
	virtual ~ZBehavior();
	void Init(ZBrain* pBrain);
	void Run(float fDelta);
	bool Input(ZBEHAVIOR_INPUT nInput);
	void ForceState(ZBEHAVIOR_STATE nState);
	ZOFFENSETYPE GetOffenseType()						{ return m_nOffecseType;	}
	void SetOffenseType( ZOFFENSETYPE nType)			{ m_nOffecseType = nType;	}
	bool IsFriendly()									{ return m_bFriendly;		}
	void SetFriendly( bool bFriendly)					{ m_bFriendly = bFriendly;	}
};




#endif