#pragma once
#include "Animation.h"


class TickAnimation :
	public Animation
{
public:
	typedef void(*TickCallback)(void* tick);
private:
	delay_t		delay;
	TickCallback	onTick;
	void*			tick;
public:

	void NotifyTicked(void);
	void SetOnTick(TickCallback t, void* c);

	delay_t   		GetDelay(void) const;
	void	     	SetDelay(delay_t v);

	Animation*		Clone(animid_t newId) const;


public:
	TickAnimation(delay_t, animid_t);
	~TickAnimation() {}
};

