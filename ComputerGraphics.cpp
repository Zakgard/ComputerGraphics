#include "Game.h"
#include "Helpers.h"

int main()
{
	auto game = Game();
	game.Initialize(LABARATORY_MODE::Architecture);
	game.PrepareResources(10);
	game.Run();
	return 0;
}

