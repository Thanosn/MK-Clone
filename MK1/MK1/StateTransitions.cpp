#include "pch.h"
#include "StateTransitions.h"


namespace logic {
	const std::string StateTransitions::MakeKey(const Input& input) const {
		std::string result;
		for (auto& s : input)
			result += "." + s;
		return result;
	}

	void StateTransitions::PerformDefaultTransition(void) {
		auto j(table.find(state + ".*"));
		if (j != table.end())
			j->second();
	}

	void StateTransitions::FireTransitions(const std::set<std::string>& keys) {

		// due to lexicographic ordering a fired input '<state-i>.a.b' precedes
		// an extended input '<state-i>.a.b.c', and fires before it as required

		for (auto& key : keys) {
			auto i(table.find(key));
			assert(i != table.end());
			i->second();
			if (state == mk::Fighter::FighterStatesStrings(mk::JUMP) ||
				state == mk::Fighter::FighterStatesStrings(mk::JUMP_MOVEMENT_BACKWARD) ||
				state == mk::Fighter::FighterStatesStrings(mk::JUMP_MOVEMENT_FORWARD))
				break;
		}
	}

	void StateTransitions::InsertByRetainingMaximalEdges(std::set<std::string>& keys, const std::string& fired) {
		for (auto& key : keys) {
			if (key.find(fired) != std::string::npos)	// substring of existing key, ignore fired
				return;
			if (fired.find(key) != std::string::npos)	// superstring of existing key, ignore key
			{
				keys.erase(key); break;
			}
		}
		keys.insert(fired);
	}

	StateTransitions& StateTransitions::SetTransition(
		const std::string&	from,
		const Input&		input,
		const  std::function<void(void)>& f  //TODO Fix it into a tamplate?
	) {
		if (!input.empty()) {
			table[from + MakeKey(input)] = f;
			inputs[from].push_back(input);
		}
		else
			table[from + ".*"] = f;
		return *this;
	}

	StateTransitions& StateTransitions::SetState(const std::string& newState)
	{
		state = newState;
		return*this;
	}

	// all plausible transitions are fired
	StateTransitions& StateTransitions::PerformTransitions(const Input& input, bool useMaximalEdges) {

		auto	i(inputs.find(state));
		assert(i != inputs.end());
		auto&	edges(i->second);

		std::set<std::string> fired;

		for (auto& edge : edges)
			if (std::includes(edge.begin(), edge.end(), input.begin(), input.end())) {	// is subset
				auto j(table.find(state + MakeKey(edge)));
				assert(j != table.end());
				auto& key(j->first);
				if (useMaximalEdges)
					InsertByRetainingMaximalEdges(fired, key);
				else
					fired.insert(key);
			}

		if (fired.empty())
			PerformDefaultTransition();
		else
			FireTransitions(fired);
		return *this;
	}

	const std::string& StateTransitions::GetState(void) const { return state; }

}
