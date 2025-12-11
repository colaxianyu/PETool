module;

export module DialogStateMachine;

import STL;

using std::uint8_t;
using std::vector;
using std::unordered_map;
using std::function;

namespace petools {

	export enum class DialogState : uint8_t {
		Constructed,
		Initializing,
		Active,
		Suspended,
		Closing,
		Destroyed
	};

	export class DialogStateMachine {
	public:
		using TransitionCallback = function<void()>;

		struct TransitionRule {
			DialogState to_state;
			TransitionCallback callback;
		};

		void add_transition_rule(DialogState from, DialogState to, TransitionCallback callback = nullptr) noexcept;
		bool transition_to(DialogState new_state) noexcept;
		[[nodiscard]] DialogState get_current_state() const noexcept { return current_state_; }
		[[nodiscard]] DialogState get_previous_state() const noexcept { return previous_state_; }

	private:
		unordered_map<DialogState, vector<TransitionRule>> transition_rules_;
		DialogState current_state_ = DialogState::Constructed;
		DialogState previous_state_ = DialogState::Constructed;
	};

} // namespace petools