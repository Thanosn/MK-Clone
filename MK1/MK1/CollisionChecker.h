#pragma once

#include "Sprite.h"

class CollisionChecker {
	typedef std::pair<Sprite*, Sprite*> Pair;
	std::list<Pair> pairs;

	struct CheckFunctor : public std::unary_function<Pair, void> {
		void operator()(const Pair& p) const { 
			p.first->CollisionCheck(p.second);
		}
	};

	static CollisionChecker* checker;
public:

	static CollisionChecker* Get();

	void Register(Sprite* s1, Sprite* s2);
	void Cancel(Sprite* s1, Sprite* s2);
	void Cancel(Sprite* s1);
	void Check(void) const;
};
