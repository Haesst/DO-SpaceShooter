
#include "Engine/Engine.h"
#include "Game/Game.h"

int main(int argc, char* args[])
{
	Engine engine;

	engine.Init();

	Game game;

	game.Init(&engine);

	game.Run();

	game.Quit();
	engine.Quit();

	return 0;
}