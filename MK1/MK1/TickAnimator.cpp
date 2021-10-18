#include "pch.h"
#include "TickAnimator.h"


void TickAnimator::Progress(timestamp_t currTime)
{
	if (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		anim->NotifyTicked();
		state = ANIMATOR_FINISHED;
		NotifyStopped();
		return;
	}
}

void TickAnimator::Start(TickAnimation * a, timestamp_t t)
{
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
}

TickAnimator::TickAnimator():
	anim(nullptr)
{
}


TickAnimator::~TickAnimator()
{
}
