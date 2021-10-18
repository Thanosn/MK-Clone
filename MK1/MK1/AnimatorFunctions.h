#pragma once
#include "pch.h"

#include "AnimatorHolder.h"
#include "AnimationFilmHolder.h"
#include "FrameRangeAnimation.h"
#include "FrameRangeAnimator.h"
#include "MovingAnimation.h"
#include "MovingAnimator.h"
#include "MovingPathAnimation.h"
#include "MovingPathAnimator.h"

#include "CollisionChecker.h"
#include "Fighter.h"

#include "AnimatorFunctions.h"


namespace AnimatorFunctions {
	void makeAnimation(void * args);
	void FinishFatalitiedCallback(Animator* a, void* fighter);
	void FinishFatalityCallback(Animator * a, void* fighter);
	void FinishNormalCallback(Animator * a, void * args);
	void FinishRoundCallback(Animator * a, void * args);
	void FinishPunchCallback(Animator * a, void * args);
	void ForceHitted(void* fighter);
}


