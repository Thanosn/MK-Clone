#pragma once
#include "Animator.h"
#include "Sprite.h"
#include "FrameRangeAnimation.h"

class FrameRangeAnimator : public Animator {
	Sprite*			sprite;
	FrameRangeAnimation*	anim;
	frame_t			currFrame;

public:
	void Progress(timestamp_t currTime);
	void Start(Sprite* s, FrameRangeAnimation* a, timestamp_t t);

	Sprite* GetSprite()const { return sprite; }
	FrameRangeAnimation* GetAnimation() const { return anim; }

	FrameRangeAnimator(void);
	~FrameRangeAnimator();
};


