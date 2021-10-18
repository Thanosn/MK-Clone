#pragma once
#include "Animator.h"
#include "Sprite.h"
#include "MovingPathAnimation.h"

class MovingPathAnimator :
	public Animator
{
	Sprite *sprite;
	MovingPathAnimation *anim;
	int index;
public:
	void Progress(timestamp_t currTime);

	void Start(Sprite* s, MovingPathAnimation* a, timestamp_t t);

	Sprite* GetSprite()const { return sprite; }
	MovingPathAnimation* GetAnimation() const { return anim; }

	int GetIndex() const { return index; }
	void SetIndex(int _index) { index = _index; }

	MovingPathAnimator();
	~MovingPathAnimator();
};

