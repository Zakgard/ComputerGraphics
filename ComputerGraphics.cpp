#include "Game.h"
#include "Helpers.h"

int main()
{
	auto game = Game();
	game.Initialize(LABARATORY_MODE::Pong);
	game.PrepareResources(1);
	game.Run();
	return 0;
}

