#pragma once


namespace logic {

	class StateTransitions final {
	public:
		using Input = std::set<std::string>;
	private:
		using Inputs = std::map<std::string, std::list<Input>>;
		using Table = std::map<std::string, std::function<void(void)>>;
		Table			table;
		Inputs			inputs;
		std::string		state;

		const std::string	MakeKey(const Input& input) const;

		void				PerformDefaultTransition(void);

		void				FireTransitions(const std::set<std::string>& keys);

		void				InsertByRetainingMaximalEdges(std::set<std::string>& keys, const std::string& fired);
	public:
		StateTransitions&	SetTransition(
			const std::string&	from,
			const Input&		input,
			const  std::function<void(void)>& f
		);

		StateTransitions&	SetState(const std::string& newState);

		// all plausible transitions are fired
		StateTransitions&	PerformTransitions(const Input& input, bool useMaximalEdges);

		const std::string&	GetState(void) const;

		StateTransitions(StateTransitions&&) = delete;
		StateTransitions(const StateTransitions&) = delete;
		StateTransitions(void) = default;
	};

}