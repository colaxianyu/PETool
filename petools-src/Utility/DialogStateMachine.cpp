module;

module DialogStateMachine;

namespace petools {

	void DialogStateMachine::add_transition_rule(DialogState from,
		DialogState to,
		TransitionCallback callback) noexcept
	{
		TransitionRule rule{ to, callback };
		transition_rules_[from].push_back(rule);
	}

	bool DialogStateMachine::transition_to(DialogState new_state) noexcept {
		auto it = transition_rules_.find(current_state_);
		if (it == transition_rules_.end()) {
			return false;
		}

		auto rule_it = std::find_if(it->second.begin(), it->second.end(),
			[new_state](const auto& rule) -> bool {return rule.to_state == new_state; }
		);

		if (rule_it == it->second.end()) {
			return false;
		}

		previous_state_ = current_state_;
		current_state_ = new_state;

		if (rule_it->callback) {
			rule_it->callback();
		}

		return true;
	}

} // namespace petools