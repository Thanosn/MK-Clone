#pragma once
#include "Animator.h"
#include "TickAnimation.h"
#include "Sprite.h"



class TickAnimator : public Animator
{
private:
	TickAnimation* anim;
public:
	void Progress(timestamp_t currTime);

	void Start(TickAnimation* a, timestamp_t t);

	Sprite* GetSprite()const { return nullptr; }
	TickAnimation* GetAnimation() const { return anim; }

	TickAnimator();
	~TickAnimator();
};

