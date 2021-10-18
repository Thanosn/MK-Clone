#include "pch.h"
#include "Fighter.h"
#include "AudioPlayer.h"


namespace mk {
	Fighter::Fighter(int _id, std::string _name) :
		id(_id),
		name(_name),
		tickAnim(new TickAnimation(FIGHTER_ACTION_DELAY_MSECS, _id)),
		inputController(),
		stateTransitions(),
		looksRight(false),
		currentHP(INITIAL_MAX_FIGHTER_HP)
	{
		currentState = mk::MOVING;
		nextAction = READY;
		tickAnimator = new TickAnimator();
	}


	Fighter::~Fighter()
	{
		if (sprite)sprite->Destroy();
		tickAnimator->Destroy();
		tickAnim->Destroy();
	}


	void Fighter::loadKeys(const std::string cfg_path)
	{
		auto cfg = al_load_config_file(cfg_path.c_str());
		const std::string moves[] = {
			"UP","DOWN","FORWARD",
			"BACK","LP",
			"HP","LK",
			"HK","BLK"
		};

		for (auto i : moves) {
			const char *key = al_get_config_value(cfg, ("INPUT " + std::to_string(id)).c_str(), i.c_str());
			keyBinds[i] = key;
		}
		al_destroy_config(cfg);
	}
	std::string Fighter::translateKey(std::string name)
	{
		return keyBinds[name];
	}
	void Fighter::initInputController()
	{
		std::list<std::string> keys;

		keys.push_back(translateKey("HP"));
		inputController.AddAction(keys, "HP");
		keys.clear();

		keys.push_back(translateKey("LP"));
		inputController.AddAction(keys, "LP");
		keys.clear();

		keys.push_back(translateKey("HK"));
		inputController.AddAction(keys, "HK");
		keys.clear();

		keys.push_back(translateKey("LK"));
		inputController.AddAction(keys, "LK");
		keys.clear();

		keys.push_back(translateKey("BLK"));
		inputController.AddAction(keys, "BLK");
		keys.clear();

		keys.push_back(translateKey("UP"));
		inputController.AddAction(keys, "UP");
		keys.clear();

		keys.push_back(translateKey("DOWN"));
		inputController.AddAction(keys, "DOWN");
		keys.clear();

		keys.push_back(translateKey("BACK"));
		inputController.AddAction(keys, "BACK");
		keys.clear();

		keys.push_back(translateKey("FORWARD"));
		inputController.AddAction(keys, "FORWARD");
		keys.clear();
	}

	void Fighter::setState(FIGHTERSTATES _state) {
		/*
		if (_state == FIGHTERSTATES::WALKING_FORWARD && !this->looksRight)
			_state = FIGHTERSTATES::WALKING_BACKWARD;
		else if (_state == FIGHTERSTATES::WALKING_BACKWARD && !this->looksRight)
			_state = FIGHTERSTATES::WALKING_FORWARD;
		*/

		nextAction = _state;
		stateTransitions.SetState(FighterStatesStrings(_state));

		//if (_state == FIGHTERSTATES::READY)AnimatorFunctions::makeAnimation(this);
	}

	void Fighter::initStateTransitions()
	{
		void (mk::Fighter::*tmp)(FIGHTERSTATES _state);
		tmp = &Fighter::setState;

		using Input = logic::StateTransitions::Input;


		// READY
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "-" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::READY); }).
			SetTransition(FighterStatesStrings(READY), Input{ "UP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "DOWN" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "FORWARD" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::WALKING_FORWARD); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "BACK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::WALKING_BACKWARD); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(READY), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::BLOCK); });

		//WALKING
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "UP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_FORWARD); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "DOWN" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::BLOCK); });

		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "UP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_BACKWARD); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "DOWN" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HP1); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::BLOCK); });

		// BLOCK
		stateTransitions.
			SetTransition(FighterStatesStrings(BLOCK), Input{ "UP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(BLOCK), Input{ "DOWN" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK); });


		// DUCK
		stateTransitions.
			SetTransition(FighterStatesStrings(DUCK), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(DUCK), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(DUCK), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(DUCK), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(DUCK), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::DUCK_BLOCK); });

		// JUMP
		/*stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "FORWARD" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVING); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "BACK" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVING); });*/
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_HITTING_KICK); });

		// JUMP



		// HITTING
		/*stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "UP" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "DOWN" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "FORWARD" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "BACK" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "LP" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "HP" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "LK" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "HK" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HITTING), Input{ "BLK" },  [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP); });*/



			// EWOOD
		stateTransitions.
			SetTransition(FighterStatesStrings(EWOOD), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::BLOCK); });


		//PUNCHED
		stateTransitions.
			SetTransition(FighterStatesStrings(PUNCHED), Input{ "BLK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::BLOCK); });


		//JUMP_MOVEMENT
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_FORWARD), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_FORWARD), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_FORWARD), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_FORWARD), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_PUNCH); });

		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_BACKWARD), Input{ "LK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_BACKWARD), Input{ "HK" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_KICK); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_BACKWARD), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_PUNCH); });
		stateTransitions.
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_BACKWARD), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::JUMP_MOVEMENT_HITTING_PUNCH); });

		//HP1
		stateTransitions.
			SetTransition(FighterStatesStrings(HP1), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LP2); });
		stateTransitions.
			SetTransition(FighterStatesStrings(HP1), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HP2); });



		//LP1
		stateTransitions.
			SetTransition(FighterStatesStrings(LP1), Input{ "LP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::LP2); });
		stateTransitions.
			SetTransition(FighterStatesStrings(LP1), Input{ "HP" }, [this](void) { (this->setState)(mk::FIGHTERSTATES::HP2); });

		stateTransitions.
			SetTransition(FighterStatesStrings(WALKING_FORWARD), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(WALKING_BACKWARD), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(BLOCK), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(MOVING), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(DUCK), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(DUCK_BLOCK), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(DUCK_HITTING_PUNCH), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(DUCK_HITTING_KICK), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(JUMP), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_FORWARD), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_BACKWARD), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(HP1), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(HP2), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(LP1), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(LP2), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(LK), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(HK), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(HITTING), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(DUCK_HITTING_PUNCH), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(DUCK_HITTING_KICK), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(JUMP_HITTING_PUNCH), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(JUMP_HITTING_KICK), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_HITTING_PUNCH), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(JUMP_MOVEMENT_HITTING_KICK), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(FROZEN), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(GRABBED), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(PULLING), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(PULLED), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(FALL_DOWN), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(WINNING_POS), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(DIZZY), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(FATALITY_SUB), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(FATALITY_SCORPION), Input{ "-" }, [](void) {}).

			SetTransition(FighterStatesStrings(PUNCHED), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(SKELETON_F), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(BLUE_DESTRUCTION_F), Input{ "-" }, [](void) {}).
			SetTransition(FighterStatesStrings(EWOOD), Input{ "-" }, [](void) {});

		stateTransitions.
			SetState(FighterStatesStrings(READY));


	}

	const std::string Fighter::FighterStatesStrings(const FIGHTERSTATES st) {
		switch (st)
		{
		case mk::READY:							return "Fighting Stance";
		case mk::WALKING_FORWARD:				return "WALKING_FORWARD";
		case mk::WALKING_BACKWARD:				return "WALKING_BACKWARD";
		case mk::BLOCK:							return "Block";
		case mk::MOVING:						return "MOVING";
		case mk::DUCK:							return "Full Ducking";
		case mk::DUCK_BLOCK:					return "Block";
		case mk::JUMP:							return "Jump";
		case mk::JUMP_MOVEMENT_FORWARD:			return "Barrel";
		case mk::JUMP_MOVEMENT_BACKWARD:		return "Reverse Barrel";
		case mk::HP1:							return "Right High Punch";
		case mk::HP2:							return "Left High Punch";
		case mk::LP1:							return "Right Punch";
		case mk::LP2:							return "Left Punch";
		case mk::LK:							return "Kicking 1";
		case mk::HK:							return "Kicking 2";
		case mk::HITTING:						return "HITTING";
		case mk::DUCK_HITTING_PUNCH:			return "Uppercut";
		case mk::DUCK_HITTING_KICK:				return "Down Body High Kicking";
		case mk::JUMP_HITTING_PUNCH:			return "Jumping Punch";
		case mk::JUMP_HITTING_KICK:				return "Jumping High Kicking";
		case mk::JUMP_MOVEMENT_HITTING_PUNCH:	return "Jumping Punch";
		case mk::JUMP_MOVEMENT_HITTING_KICK:	return "Jumping Low Kicking";
		case mk::FROZEN:						return "Feeling Blue";
		case mk::GRABBED:						return "Getting Up";
		case mk::PULLED:						return "Jesus Christ";
		case mk::PULLING:						return "Move To Throw Spear";
		case mk::FALL_DOWN:						return "Being Hit And Lie Back Down";
		case mk::WINNING_POS:					return "Victory Pose";
		case mk::DIZZY:							return "Dizzy";
		case mk::FATALITY_SUB:					return "Uppercut";
		case mk::FATALITY_SCORPION:				return "Fatality Move";
		case mk::PUNCHED:						return "Being Hit On Chest";
		case mk::EWOOD:							return "EWOOD";

		case mk::SKELETON_F:					return"Fatality Skeleton";
		case mk::BLUE_DESTRUCTION_F:			return"Fatality Freeze Destruction";

		default:
			assert(false);
			return "";
		}
	}

	void Fighter::performTransitions()
	{
		if (!inputController.GetLogical().empty()) {
			startTickAnimator();
		}
		stateTransitions.PerformTransitions(inputController.GetLogical(), true);
	}

	Sprite* Fighter::getSprite() const { return sprite; }

	void Fighter::setSprite(int x, int y, std::string filmName, TYPE collisionType) {
		if (sprite)
			sprite->Destroy();
		auto film = AnimationFilmHolder::Get()->GetFilm(name + " " + filmName);
		sprite = new Sprite(x, y, film, collisionType, this);
		//sprite->SetLookRight(looksRight);
	}

	void Fighter::setSprite(AnimationFilm* film, TYPE collisionType) {
		if (sprite)
			sprite->Destroy();
		assert(film);
		sprite = new Sprite(sprite->GetX(), sprite->GetY(), film, collisionType, this);
		//sprite->SetLookRight(looksRight);
	}

	Animator* Fighter::getAnimator() const { return currentAnimator; }

	void Fighter::setAnimator(Animator *a) {
		if (currentAnimator) {
			AnimatorHolder::Disable(currentAnimator);
			DELETE_ANIMATOR(currentAnimator);
		}
		currentAnimator = a;
	}

	std::string Fighter::getName() const { return name; }

	int Fighter::GetId()const { return id; }

	FIGHTERSTATES Fighter::getNextAction() const { return nextAction; }

	FIGHTERSTATES Fighter::getCurrentState() const { return currentState; }

	void Fighter::setCurrentState(FIGHTERSTATES _state) { currentState = _state; }



	void Fighter::damage(int dmg)
	{
		std::cout << "OUCHI said " << name << std::endl;
		if (currentHP > 26)
		{
			AudioPlayer::Get()->Play("../../sounds/punch.ogg", false);
		}
		currentHP -= dmg;
	}
	void Fighter::ResetCurrentHP()
	{
		currentHP = INITIAL_MAX_FIGHTER_HP;
	}
	bool Fighter::isDead()const {
		return currentHP <= 0;
	}

	void Fighter::calculateLook(Fighter * f)
	{
		looksRight = this->sprite->GetX() < f->getSprite()->GetX();
		sprite->SetLookRight(looksRight);
		f->getSprite()->SetLookRight(!looksRight);
	}

	void Fighter::AddWin()
	{
		numOfWins++;
	}

	int Fighter::GetWins() const
	{
		return numOfWins;
	}

	void destroyTick(Animator* a, void *args) {
		AnimatorHolder::Disable(a);
		//DELETE_ANIMATOR(((TickAnimator*) a));
	}
	void Fighter::startTickAnimator()
	{
		if (tickAnimator->GetState() == ANIMATOR_RUNNING)return;
		tickAnim->SetOnTick(AnimatorFunctions::makeAnimation, this);


		tickAnimator->SetOnFinish(destroyTick);
		tickAnimator->Start(tickAnim, GameTimer::GetGameTime());
		AnimatorHolder::Create(tickAnimator);
	}
}
