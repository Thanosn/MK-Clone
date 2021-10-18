#pragma once

#include "InputController.h"
#include "StateTransitions.h"

#include "Sprite.h"
#include "TickAnimation.h"
#include "TickAnimator.h"

#include "AnimatorHolder.h"
#include "AnimationFilmHolder.h"

#include "AnimatorFunctions.h"


namespace mk {
	const int x_speed = 25;
	enum FIGHTERSTATES {
		READY,
		WALKING_FORWARD,
		WALKING_BACKWARD,
		BLOCK,

		MOVING,

		DUCK,
		DUCK_BLOCK,

		JUMP,
		JUMP_MOVEMENT_FORWARD,
		JUMP_MOVEMENT_BACKWARD,

		HP1,
		HP2,
		LP1,
		LP2,
		LK,
		HK,

		HITTING,

		DUCK_HITTING_PUNCH,
		DUCK_HITTING_KICK,

		JUMP_HITTING_PUNCH,
		JUMP_HITTING_KICK,
		JUMP_MOVEMENT_HITTING_PUNCH,
		JUMP_MOVEMENT_HITTING_KICK,

		FROZEN,
		GRABBED,
		PULLING,

		FALL_DOWN,
		WINNING_POS,

		PULLED,
		DIZZY,

		FATALITY_SUB,
		FATALITY_SCORPION,

		PUNCHED,
		EWOOD,

		SKELETON_F,
		BLUE_DESTRUCTION_F,

	};


#define INITIAL_MAX_FIGHTER_HP 100

	class Fighter final {
	private:
		Sprite*						sprite = nullptr;
		FIGHTERSTATES				nextAction;
		FIGHTERSTATES				currentState;
		TickAnimator*				tickAnimator = nullptr;	// deferred firing actions; always dynamic
		TickAnimation*				tickAnim = nullptr;

		Animator*					currentAnimator = nullptr;

		std::string					name;
		input::InputController		inputController;
		logic::StateTransitions		stateTransitions;

		bool						looksRight;
		const int					id;
		std::map<std::string, std::string> keyBinds;

		int							currentHP;

		FIGHTERSTATES				tempState;

		int							numOfWins;
	public:
		// TODO: 
		static const std::string FighterStatesStrings(const FIGHTERSTATES st);
		Sprite* getSprite() const;
		void setSprite(int x, int y, std::string filmName, TYPE collisionType);
		void setSprite(AnimationFilm* film, TYPE collisionType);
		Animator* getAnimator() const;
		void setAnimator(Animator *a);

		std::string getName() const;
		int GetId() const;

		FIGHTERSTATES getNextAction() const;

		FIGHTERSTATES getCurrentState() const;
		void setCurrentState(FIGHTERSTATES _state);

		void setState(FIGHTERSTATES);

		void loadKeys(const std::string cfg_path);
		std::string translateKey(std::string name);
		void initInputController();
		void initStateTransitions();
		void performTransitions();

		void damage(int dmg);
		int getCurrentHP()const { return currentHP; }
		void ResetCurrentHP();
		bool isDead()const;

		void calculateLook(Fighter*);
		bool GetLookRight()const { return looksRight; }

		void AddWin();
		int GetWins()const;

		void startTickAnimator();

		Fighter(int id,std::string name);
		~Fighter();
	};

}