#include "pch.h"
#include "Animator.h"

void Animator::NotifyStopped(void) {
	if (onFinish)
		(*onFinish)(this, finishClosure);
}
animatorstate_t Animator::GetState() const
{
	return state;
}
void Animator::Stop(void) {
	if (!HasFinished()) {
		state = ANIMATOR_STOPPED;
		NotifyStopped();
	}
}
void Animator::TimeShift(timestamp_t offset)
{
	std::cout << offset << std::endl;
	lastTime += offset;
}

bool Animator::HasFinished(void) const
{
	return state != ANIMATOR_RUNNING;
}

void Animator::SetOnFinish(FinishCallback f, void* c)
{
	onFinish = f,
		finishClosure = c;
}

Animator::Animator(void) : 
	LatelyDestroyable(),
	lastTime(0), state(ANIMATOR_FINISHED),
	onFinish((FinishCallback)0), finishClosure((void*)0) {}

Animator::~Animator() {
	AnimatorHolder::Disable(this); 
}
