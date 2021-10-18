#include "pch.h"
#include "CollisionChecker.h"


CollisionChecker* CollisionChecker::checker = 0;

void CollisionChecker::Register(Sprite* s1, Sprite* s2) {
	assert(s1);
	assert(s2);


	auto i = std::find_if(pairs.begin(), pairs.end(), [s1, s2](const Pair & m) -> bool { return(m.first == s1 && m.second == s2) || (m.first == s2 && m.second == s1); });

	if (i != pairs.end())
		return;

	pairs.push_back(std::make_pair(s1, s2));
}


void CollisionChecker::Cancel(Sprite* s1, Sprite* s2) {
	assert(false);
}

void CollisionChecker::Cancel(Sprite* s1) {
	if (!s1)return;
	auto i = pairs.begin();
	while ((i = std::find_if(pairs.begin(), pairs.end(), [s1](const Pair & m) -> bool { return m.first == s1 || m.second == s1; })) != pairs.end()) 
		pairs.erase(i);
}

void CollisionChecker::Check(void) const {
	std::for_each(
		pairs.begin(), pairs.end(), CheckFunctor()
	);
}

CollisionChecker * CollisionChecker::CollisionChecker::Get()
{
	if (!checker)
		checker = new CollisionChecker();
	return checker;
}
