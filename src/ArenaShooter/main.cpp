#include <windows.h>
#include "main.h"
#include "ArenaShot.h"

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR cmdLine, int cmdShow)
{
    Game* game = Game::Create();
    game->Init();
    game->RUN();

    return 0;
}