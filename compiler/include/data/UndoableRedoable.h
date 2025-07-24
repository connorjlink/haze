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
		std::size_t _current_state{ 0uz };

	public:
		template<typename Self>
		void undo(this Self& self)
		{
			if (self.can_undo())
			{
				self._current_state++;
				self.set_state(self._history[self._current_state]);
			}
		}

		template<typename Self>
		void redo(this Self& self)
		{
			if (self.can_redo())
			{
				self._current_state++;
				self.set_state(self._history[self._current_state]);
			}
		}

		template<typename Self>
		void save_state(this Self& self)
		{
			// truncate forward history to avoid branching states
			while (self.can_redo())
			{
				self._history.pop_front();
				//_history.erase(_history.begin() + _current_state + 1, _history.end());
			}

			if (self._history.size() == MaxHistory)
			{
				self._history.pop_back();
				//_history.erase(_history.begin());
				self._current_state--;
			}

			self._history.emplace_back(self.get_state());
			self._current_state++;
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
