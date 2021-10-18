#include "pch.h"
#include "LatelyDestroyable.h"


void LatelyDestroyable::Delete::operator()(LatelyDestroyable* o) const
{
	o->inDestruction = true;
	delete o;
}

void LatelyDestroyable::Destroy(void)
{
	if (alive) {
		alive = false;
		DestructionManager::Register(this);
	}
}

std::list<LatelyDestroyable*> DestructionManager::dead = std::list<LatelyDestroyable*>();

void DestructionManager::Register(LatelyDestroyable* o) {
	assert(!o->IsAlive());
	dead.push_back(o);
}

void DestructionManager::Commit(void) {
	std::for_each(
		dead.begin(),
		dead.end(),
		LatelyDestroyable::Delete()
	);
	dead.clear();
}


