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
		std::deque<State> _history;
		std::size_t _current_state{ -1 };

	public:
		template<typename Self>
		void undo(this Self& self)
		{
			if (can_undo())
			{
				_current_state++;
				self.set_state(_history[_current_state]);
			}
		}

		template<typename Self>
		void redo(this Self& self)
		{
			if (can_redo())
			{
				_current_state++;
				self.set_state(_history[_current_state]);
			}
		}

		template<typename Self>
		void save_state(this Self& self)
		{
			// truncate forward history to avoid branching states
			while (can_redo())
			{
				_history.pop_front();
				//_history.erase(_history.begin() + _current_state + 1, _history.end());
			}

			if (_history.size() == MaxHistory)
			{
				_history.pop_back();
				//_history.erase(_history.begin());
				_current_state--;
			}

			_history.emplace_back(self.get_state());
			++_current_state;
		}

		bool can_undo(void) const
		{
			return _current_state > 0;
		}

		bool can_redo(void) const
		{
			return _current_state + 1 < _history.size();
		}

	protected:
		// enforce inherited-only construction
		UndoableRedoable(void) = default;
	};
}

#endif
