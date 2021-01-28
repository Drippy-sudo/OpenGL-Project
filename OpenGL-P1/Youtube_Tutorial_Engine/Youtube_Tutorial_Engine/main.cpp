#include "Game.h"

int main()
{
	Game game("Youtube_Tut", 
		1920, 1080,
		4, 4, 
		false);
	
	// Main loop
	while (!game.GetWindowShouldClose())
	{
		// Update input
		game.Update();
		game.Render();
	}

	return 0;
}
