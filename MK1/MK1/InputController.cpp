#include "pch.h"
#include "InputController.h"

namespace input {

	InputController::All InputController::all = InputController::All();

	void InputController::SetLogical(const std::string& id) { logical.insert(id); }

	void InputController::AddAction(const input::key_combination& keys, const std::string& logical)
	{
		actions.push_back(std::make_pair(keys, logical));
	}

	//clear logical and then set a new logical
	void InputController::Handle(void) {
		logical.clear();
		for (auto& i : actions)
			if (input::test_keys(i.first))
				SetLogical(i.second);
		if (logical.empty())SetLogical("-");
	}
	const InputController::Logical& InputController::GetLogical(void) const
	{
		return logical;
	}
	// for each player we call the Handle
	void InputController::HandleAll(void) {
		for (auto* handler : all)
			handler->Handle();
	}

	void InputController::AddToKeysList()
	{
		//standard moves
		keys.push_back("high_punch");
		keys.push_back("low_punch");
		keys.push_back("high_kick");
		keys.push_back("low_kick");
		keys.push_back("block");
		keys.push_back("jump");
		keys.push_back("duck");
		keys.push_back("left");
		keys.push_back("right");

		//special moves for all

		keys.push_back("back+low_kick"); // sweep kick
		keys.push_back("back+high_kick"); //round house
		keys.push_back("(pushed)right+low_kick"); //throw opponent (must be close to enemy) maybe right should be forward
	
		//sub-zero

		keys.push_back("down+right+low_punch"); // freeze ball
		keys.push_back("left+low_kick+high_kick"); //slide
		keys.push_back("right+down+right+high_punch"); // fatality

		//scorpion

		keys.push_back("left+left+low_punch"); // hand spear
		keys.push_back("down+back+low_punch"); //teleport punch
		keys.push_back("jump+jump"); // fatality jump == up
	}

	InputController::InputController(void) { all.push_back(this); }
	InputController::~InputController() { all.remove(this); }
	

}