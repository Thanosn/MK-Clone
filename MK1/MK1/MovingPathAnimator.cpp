#include "pch.h"
#include "MovingPathAnimator.h"


void MovingPathAnimator::Progress(timestamp_t currTime)
{
	while (currTime > lastTime && currTime - lastTime >= anim->GetPath()->at(index).delay) {
		if (index == anim->GetPath()->size()-1) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}

		sprite->Move(anim->GetPath()->at(index).dx, anim->GetPath()->at(index).dy);
		sprite->SetFrame(anim->GetPath()->at(index).frame);
		lastTime += anim->GetPath()->at(index).delay;

		if (index == anim->GetPath()->size()-1)
			index = 0;
		else
			++index;
	}
}



void MovingPathAnimator::Start(Sprite* s, MovingPathAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
	sprite->SetFrame(anim->GetPath()->at(index).frame);
}

MovingPathAnimator::MovingPathAnimator(void) :
	sprite((Sprite*)0),
	anim((MovingPathAnimation*)0), index(0)
{}

MovingPathAnimator::~MovingPathAnimator() {
	anim->Destroy();
}

