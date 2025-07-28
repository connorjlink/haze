#include <Video.h>

void write_console(Rect parent, Point* cursor, uint8_t color, const char* text)
{
	const size_t bottom = parent.size.y - 2;

	if (cursor->y == bottom)
	{
		scroll_rect(parent, color);
		//cursor->y--;
	}

	else
	{
		cursor->y++;
	}

	render_text(parent, *cursor, color, text);
}
