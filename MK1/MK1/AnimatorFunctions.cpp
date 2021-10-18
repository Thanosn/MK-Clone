#include "pch.h"
#include "AnimatorFunctions.h"
#include "AudioPlayer.h"

extern void ResetRound();
extern void GetFatalitied(mk::Fighter*);
extern void GameEnd();

namespace AnimatorFunctions {

	struct SimplerHandler : CollisionHandler
	{
		virtual void operator() (SpriteWithCollision* caller, SpriteWithCollision* arg) const {
			((Sprite*)caller)->DeCollision();
			((Sprite*)arg)->DeCollision();
		}
		virtual CollisionHandler* Clone(void) const { return new SimplerHandler; }
	};
	struct HitHandler : CollisionHandler
	{
		virtual void operator() (SpriteWithCollision* caller, SpriteWithCollision* arg) const {
			if (caller->GetState() == STATE::HITTING && arg->GetState() != STATE::TRANSPARENT) {
				((Sprite*)arg)->makeDamage(25);
				caller->SetState(STATE::IDLE);
				ForceHitted(((Sprite*)arg)->GetOwner());
			}
		}
		virtual CollisionHandler* Clone(void) const { return new HitHandler; }
	};
	struct Handler : CollisionHandler
	{
		virtual void operator() (SpriteWithCollision* caller, SpriteWithCollision* arg) const {

		}
		virtual CollisionHandler* Clone(void) const { return new Handler; }
	};


	void makeAnimation(void* args) {
		mk::Fighter* f = (mk::Fighter*)args;

		if (f->getNextAction() == f->getCurrentState())
			return;

		if (f->getNextAction() == mk::JUMP_HITTING_PUNCH ||
			f->getNextAction() == mk::JUMP_HITTING_KICK) {
			if (f->getCurrentState() != mk::JUMP) {
				f->setState(mk::JUMP);
				makeAnimation(f);
				return;
			}
		}

		if (f->getNextAction() == mk::JUMP_MOVEMENT_HITTING_PUNCH ||
			f->getNextAction() == mk::JUMP_MOVEMENT_HITTING_KICK) {
			if (f->getCurrentState() != mk::JUMP_MOVEMENT_BACKWARD &&
				f->getCurrentState() != mk::JUMP_MOVEMENT_FORWARD) {
				/*
				if (f->getCurrentState() == mk::WALKING_BACKWARD) {
					f->setState(mk::JUMP_MOVEMENT_BACKWARD);
					makeAnimation(f);
				}
				if (f->getCurrentState() == mk::WALKING_FORWARD) {
					f->setState(mk::JUMP_MOVEMENT_FORWARD);
					makeAnimation(f);
				}
				*/
				f->setState(mk::JUMP);
				makeAnimation(f);
				return;
			}
		}



		f->setCurrentState(f->getNextAction());

		AnimationFilm* film;
		if (f->getNextAction() == mk::SKELETON_F)
			film = AnimationFilmHolder::Get()->GetFilm(std::string("") + "SCORPION" + " " + f->FighterStatesStrings(f->getNextAction()));
		else if (f->getNextAction() == mk::BLUE_DESTRUCTION_F)
			film = AnimationFilmHolder::Get()->GetFilm(std::string("") + "SUB0" + " " + f->FighterStatesStrings(f->getNextAction()));
		else
			film = AnimationFilmHolder::Get()->GetFilm(f->getName() + " " + f->FighterStatesStrings(f->getNextAction()));

		if (!f->GetLookRight())
			if (f->getNextAction() == mk::WALKING_FORWARD)
				film = AnimationFilmHolder::Get()->GetFilm(f->getName() + " " + f->FighterStatesStrings(mk::WALKING_BACKWARD));
			else if (f->getNextAction() == mk::WALKING_BACKWARD)
				film = AnimationFilmHolder::Get()->GetFilm(f->getName() + " " + f->FighterStatesStrings(mk::WALKING_FORWARD));

		if (!f->GetLookRight())
			if (f->getNextAction() == mk::JUMP_MOVEMENT_FORWARD)
				film = AnimationFilmHolder::Get()->GetFilm(f->getName() + " " + f->FighterStatesStrings(mk::JUMP_MOVEMENT_BACKWARD));
			else if (f->getNextAction() == mk::JUMP_MOVEMENT_BACKWARD)
				film = AnimationFilmHolder::Get()->GetFilm(f->getName() + " " + f->FighterStatesStrings(mk::JUMP_MOVEMENT_FORWARD));


		SpriteHolder::SpriteList fighter2_sprites;
		SpriteHolder::SpriteList fighter2_throwables;
		if (f->GetId() == 1) {
			SpriteHolder::Get()->GetSprites(TYPE::FIGHTER_0, &fighter2_sprites);
			SpriteHolder::Get()->GetSprites(TYPE::THROWABLE_0, &fighter2_throwables);
		}
		else {
			SpriteHolder::Get()->GetSprites(TYPE::FIGHTER_1, &fighter2_sprites);
			SpriteHolder::Get()->GetSprites(TYPE::THROWABLE_1, &fighter2_throwables);
		}

		f->setSprite(film, (TYPE)f->GetId());

		//f->getSprite()->AddColisionHandler(Handler());

		std::for_each(fighter2_sprites.begin(), fighter2_sprites.end(), [f](Sprite* s) {CollisionChecker::Get()->Register(f->getSprite(), s); });
		std::for_each(fighter2_throwables.begin(), fighter2_throwables.end(), [f](Sprite* s) {CollisionChecker::Get()->Register(f->getSprite(), s); });

		MovingAnimation *Manim = nullptr;
		MovingAnimator	*Manimator = nullptr;

		MovingPathAnimation *MPanim = nullptr;
		MovingPathAnimator	*MPanimator = nullptr;

		FrameRangeAnimation  *FRanim = nullptr;
		FrameRangeAnimator *FRanimator = nullptr;




		std::vector<PathEntry>* entries = new std::vector<PathEntry>;

		int index;
		int x = 0;
		int lim = 0;

		switch (f->getNextAction())
		{
		case mk::HP1:
		case mk::LP1:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 100, false, 2);
			f->getSprite()->AddColisionHandler(HitHandler());
			f->getSprite()->SetState(STATE::HITTING);
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishPunchCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::HP2:
		case mk::LP2:

		case mk::LK:
		case mk::HK:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 80, false, 2);
			f->getSprite()->AddColisionHandler(HitHandler());
			f->getSprite()->SetState(STATE::HITTING);
		case mk::READY:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, -20, 80, true, 2);
		case mk::BLOCK:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 100, false, 2);
		case mk::DUCK:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 100, false, 2);
		case mk::DUCK_BLOCK:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 10, true, 2);


		case mk::DUCK_HITTING_PUNCH:
		case mk::DUCK_HITTING_KICK:
			if (!FRanim)
				FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 100, false, 2);
		case mk::FROZEN:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 240, true, 2);
			//case mk::PULLING:
		case mk::DIZZY:
			if (!FRanim)
				FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 150, true, 2);
		case mk::WALKING_FORWARD:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, mk::x_speed, 0, 100, false, 2);
		case mk::WALKING_BACKWARD:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, -mk::x_speed, 0, 100, false, 2);
		case mk::MOVING:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 5, 0, 70, false, 2);

			f->getSprite()->AddColisionHandler(SimplerHandler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::JUMP:
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, 10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			entries->push_back(PathEntry(0, -10, 0, 50));
			if (!MPanim)MPanim = new MovingPathAnimation(entries, 2);
			MPanimator = new MovingPathAnimator();
			MPanimator->SetOnFinish(FinishNormalCallback, f);

			f->getSprite()->AddColisionHandler(SimplerHandler());
			AnimatorHolder::Create(MPanimator);
			MPanimator->Start(f->getSprite(), MPanim, GameTimer::GetGameTime());
			f->setAnimator(MPanimator);
			break;
		case mk::JUMP_HITTING_PUNCH:
			MPanimator = (MovingPathAnimator*)f->getAnimator();
			if (!MPanim)MPanim = (MovingPathAnimation*)MPanimator->GetAnimation()->Clone(2);
			MPanim->GetPath()->at(MPanimator->GetIndex() < MPanim->GetPath()->size() - 1 ? MPanimator->GetIndex() : 0).frame = 0;
			lim = 3;
		case mk::JUMP_HITTING_KICK:
			if (!lim)lim = 2;
			MPanimator = (MovingPathAnimator*)f->getAnimator();
			if (!MPanim)MPanim = (MovingPathAnimation*)MPanimator->GetAnimation()->Clone(2);


			for (int i = MPanimator->GetIndex() + 1; i < MPanim->GetPath()->size() - 2; i++) {
				MPanim->GetPath()->at(i).frame = lim - 1;
			}
			MPanim->GetPath()->at(MPanim->GetPath()->size() - 2).frame = lim - 2;
			MPanim->GetPath()->at(MPanim->GetPath()->size() - 1).frame = lim - 2;

			index = MPanimator->GetIndex();
			MPanimator = new MovingPathAnimator();
			MPanimator->SetIndex(index);
			MPanimator->SetOnFinish(FinishNormalCallback, f);

			f->getSprite()->AddColisionHandler(HitHandler());
			f->getSprite()->SetState(STATE::HITTING);
			AnimatorHolder::Create(MPanimator);
			MPanimator->Start(f->getSprite(), MPanim, GameTimer::GetGameTime());
			f->setAnimator(MPanimator);

			break;
		case mk::JUMP_MOVEMENT_FORWARD:
		case mk::JUMP_MOVEMENT_BACKWARD:
			x = 10;
			if (f->getNextAction() == mk::JUMP_MOVEMENT_BACKWARD)
				x = -10;



			entries->push_back(PathEntry(x, 20, 0, 50));
			entries->push_back(PathEntry(x, 20, 1, 50));
			entries->push_back(PathEntry(x, 20, 2, 50));
			entries->push_back(PathEntry(x, 20, 3, 50));
			entries->push_back(PathEntry(x, 20, 4, 50));
			entries->push_back(PathEntry(x, 20, 5, 50));
			entries->push_back(PathEntry(x, 20, 6, 50));
			entries->push_back(PathEntry(x, 20, 7, 50));
			entries->push_back(PathEntry(x, 20, 1, 50));
			entries->push_back(PathEntry(x, 20, 2, 50));
			entries->push_back(PathEntry(x, -20, 3, 50));
			entries->push_back(PathEntry(x, -20, 4, 50));
			entries->push_back(PathEntry(x, -20, 5, 50));
			entries->push_back(PathEntry(x, -20, 6, 50));
			entries->push_back(PathEntry(x, -20, 7, 50));
			entries->push_back(PathEntry(x, -20, 1, 50));
			entries->push_back(PathEntry(x, -20, 2, 50));
			entries->push_back(PathEntry(x, -20, 3, 50));
			entries->push_back(PathEntry(x, -20, 4, 50));
			entries->push_back(PathEntry(x, -20, 5, 50));
			entries->push_back(PathEntry(x, -20, 0, 50));
			if (!MPanim)MPanim = new MovingPathAnimation(entries, 2);
			MPanimator = new MovingPathAnimator();
			MPanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(MPanimator);
			MPanimator->Start(f->getSprite(), MPanim, GameTimer::GetGameTime());
			f->setAnimator(MPanimator);
			f->getSprite()->AddColisionHandler(SimplerHandler());

			break;
		case mk::JUMP_MOVEMENT_HITTING_PUNCH:
			MPanimator = (MovingPathAnimator*)f->getAnimator();
			if (!MPanim)MPanim = (MovingPathAnimation*)MPanimator->GetAnimation()->Clone(2);
			MPanim->GetPath()->at(MPanimator->GetIndex() < MPanim->GetPath()->size() - 1 ? MPanimator->GetIndex() : 0).frame = 0;
			lim = 3;
		case mk::JUMP_MOVEMENT_HITTING_KICK:
			if (!lim)lim = 2;
			MPanimator = (MovingPathAnimator*)f->getAnimator();
			if (!MPanim)MPanim = (MovingPathAnimation*)MPanimator->GetAnimation()->Clone(2);

			for (int i = MPanimator->GetIndex(); i < MPanim->GetPath()->size() - 2; i++) {
				MPanim->GetPath()->at(i).frame = lim - 1;
			}

			MPanim->GetPath()->at(MPanim->GetPath()->size() - 2).frame = lim - 2;
			MPanim->GetPath()->at(MPanim->GetPath()->size() - 1).frame = lim - 2;

			index = MPanimator->GetIndex();
			MPanimator = new MovingPathAnimator();
			MPanimator->SetIndex(index);
			MPanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(MPanimator);
			MPanimator->Start(f->getSprite(), MPanim, GameTimer::GetGameTime());
			f->setAnimator(MPanimator);
			f->getSprite()->AddColisionHandler(HitHandler());
			f->getSprite()->SetState(STATE::HITTING);

			break;
		case mk::HITTING:
			break;
		case mk::GRABBED:
			x = 5;
			entries->push_back(PathEntry(x, 5, 0, 50));
			entries->push_back(PathEntry(x, 5, 1, 50));
			entries->push_back(PathEntry(x, 5, 2, 50));
			entries->push_back(PathEntry(x, 5, 3, 50));
			entries->push_back(PathEntry(x, 5, 4, 50));
			entries->push_back(PathEntry(x, 5, 5, 50));
			entries->push_back(PathEntry(x, 5, 6, 50));
			entries->push_back(PathEntry(x, 5, 7, 50));
			entries->push_back(PathEntry(x, 5, 1, 50));
			entries->push_back(PathEntry(x, 5, 2, 50));
			entries->push_back(PathEntry(x, -5, 3, 50));
			entries->push_back(PathEntry(x, -5, 4, 50));
			entries->push_back(PathEntry(x, -5, 5, 50));
			entries->push_back(PathEntry(x, -5, 6, 50));
			entries->push_back(PathEntry(x, -5, 7, 50));
			entries->push_back(PathEntry(x, -5, 1, 50));
			entries->push_back(PathEntry(x, -5, 2, 50));
			entries->push_back(PathEntry(x, -5, 3, 50));
			entries->push_back(PathEntry(x, -5, 4, 50));
			entries->push_back(PathEntry(x, -5, 5, 50));
			entries->push_back(PathEntry(x, -5, 0, 50));
			if (!MPanim)MPanim = new MovingPathAnimation(entries, 2);
			MPanimator = new MovingPathAnimator();
			MPanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(MPanimator);
			MPanimator->Start(f->getSprite(), MPanim, GameTimer::GetGameTime());
			f->setAnimator(MPanimator);
			break;
		case mk::PUNCHED:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, 0, 120, false, 2);

			f->getSprite()->AddColisionHandler(SimplerHandler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::FALL_DOWN:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, -20, 200, false, 2);

			f->getSprite()->AddColisionHandler(Handler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishRoundCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::FATALITY_SUB:
		case mk::FATALITY_SCORPION:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, -25, 200, false, 2);

			f->getSprite()->AddColisionHandler(Handler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishFatalityCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::WINNING_POS:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, -20, 220, false, 2);

			f->getSprite()->AddColisionHandler(Handler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishNormalCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::SKELETON_F:
		case mk::BLUE_DESTRUCTION_F:
			if (!FRanim)FRanim = new FrameRangeAnimation(0, film->GetTotalFrames() - 1, 0, -20, 200, false, 2);

			f->getSprite()->AddColisionHandler(Handler());
			FRanimator = new FrameRangeAnimator();
			FRanimator->SetOnFinish(FinishFatalitiedCallback, f);

			AnimatorHolder::Create(FRanimator);
			FRanimator->Start(f->getSprite(), FRanim, GameTimer::GetGameTime());
			f->setAnimator(FRanimator);
			break;
		case mk::EWOOD:
			break;
		default:
			break;
		}



	}


	void FinishFatalitiedCallback(Animator* a, void* fighter) {
		GameEnd();
	}

	void FinishFatalityCallback(Animator* a, void* fighter) {
		mk::Fighter* f = (mk::Fighter*)fighter;

		GetFatalitied(f);

		f->setState(mk::WINNING_POS);
		makeAnimation(f);
	}

	void FinishRoundCallback(Animator* a, void *fighter) {

		//mk::Fighter* f = (mk::Fighter*)fighter;
		//f->setAnimator(nullptr);
		ResetRound();
	}

	void FinishNormalCallback(Animator* a, void *fighter) {


		mk::Fighter* f = (mk::Fighter*)fighter;
		f->setCurrentState(mk::READY);
		makeAnimation(f);
		f->setState(mk::READY);
		//f->setAnimator(nullptr);
		f->startTickAnimator();
	}

	void FinishPunchCallback(Animator* a, void* fighter) {
		mk::Fighter* f = (mk::Fighter*)fighter;
		if (f->getNextAction() != mk::HP2 && f->getNextAction() != mk::LP2)
			FinishNormalCallback(a, f);
		else
			makeAnimation(f);
		//f->setAnimator(nullptr);
		f->startTickAnimator();
	}

	void ForceHitted(void* fighter) {
		mk::Fighter* f = (mk::Fighter*)fighter;

		f->setState(mk::PUNCHED);
		makeAnimation(f);
	}
}
