#include "pch.h"
#include "MovingAnimator.h"


void MovingAnimator::Progress(timestamp_t currTime) {
	//TODO: ASSERt something

	while (currTime > lastTime && currTime - lastTime >= anim->GetDelay()) {
		sprite->Move(anim->GetDx(), anim->GetDy());
		if (!anim->GetContinuous()) {
			state = ANIMATOR_FINISHED;
			NotifyStopped();
			return;
		}
		else
			lastTime += anim->GetDelay();
	}
}

void MovingAnimator::Start(Sprite* s, MovingAnimation* a, timestamp_t t) {
	sprite = s;
	anim = a;
	lastTime = t;
	state = ANIMATOR_RUNNING;
}

MovingAnimator::MovingAnimator(void) :
	sprite((Sprite*)0), anim((MovingAnimation*)0) {}

MovingAnimator::~MovingAnimator() {}
