#ifndef HAZE_UNDOABLEREDOABLE_H
#define HAZE_UNDOABLEREDOABLE_H

// Haze UndoableRedoable.h
// (c) Connor J. Link. All Rights Reserved.

namespace hz
{
	template<typename State, size_t MaxHistory = 5>
	class UndoableRedoable
	{
	private:
		// front = redoable states, back = undoable states
		std::deque<State> history;
		std::size_t current_state{ 0uz };

	public:
		template<typename Self>
		void undo(this Self& self)
		{
			if (self.can_undo())
			{
				self.current_state++;
				self.set_state(self.history[self.current_state]);
			}
		}

		template<typename Self>
		void redo(this Self& self)
		{
			if (self.can_redo())
			{
				self.current_state++;
				self.set_state(self.history[self.current_state]);
			}
		}

		template<typename Self>
		void save_state(this Self& self)
		{
			// truncate forward history to avoid branching states
			while (self.can_redo())
			{
				self.history.pop_front();
				//history.erase(history.begin() + current_state + 1, history.end());
			}

			if (self.history.size() == MaxHistory)
			{
				self.history.pop_back();
				//history.erase(history.begin());
				self.current_state--;
			}

			self.history.emplace_back(self.get_state());
			self.current_state++;
		}

		bool can_undo(void) const
		{
			return current_state > 0;
		}

		bool can_redo(void) const
		{
			return current_state + 1 < history.size();
		}

	protected:
		// enforce inherited-only construction
		UndoableRedoable(void) = default;
	};
}

#endif
