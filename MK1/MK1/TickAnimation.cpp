#include "pch.h"
#include "TickAnimation.h"



delay_t TickAnimation::GetDelay(void) const
{
	return delay;
}

void TickAnimation::SetDelay(delay_t v)
{
	delay = v;
}

Animation * TickAnimation::Clone(animid_t newId) const
{
	auto* tmp = new TickAnimation(delay, newId);
	tmp->SetOnTick(onTick, tick);
	return tmp;
}

TickAnimation::TickAnimation(delay_t _delay, animid_t _id)
	:delay(_delay), Animation(_id), onTick((TickCallback)0), tick((void*)0)
{}

void TickAnimation::NotifyTicked(void) {
	if (onTick)
		(*onTick)(tick);
}

void TickAnimation::SetOnTick(TickCallback t, void* c)
{
	onTick = t,
	tick = c;
}

