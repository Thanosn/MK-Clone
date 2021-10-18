#pragma once


namespace input {

	using key_combination = std::list<std::string>;

	extern bool test_key(const std::string& keyCode);

	// check for key combinations
	inline bool test_keys(const key_combination& keys) {
		for (auto& key : keys)
			if (!test_key(key))
				return false;
		return true;
	}

	class InputController final {
	public:
		using Logical = std::set<std::string>;
	private:
		using Actions = std::list<std::pair<input::key_combination, std::string>>;
		using All = std::list<InputController*>;
		Actions actions;
		Logical logical;
		static All all;		// TODO: define it in a cpp file!	
		void SetLogical(const std::string& id);
		
		key_combination keys; // button list
	
	public:
		void AddAction(const input::key_combination& keys, const std::string& logical);
		void Handle(void);
		const Logical& GetLogical(void) const;
		static void HandleAll(void);
		InputController(void);
		~InputController();


		void AddToKeysList();
	};

} // input