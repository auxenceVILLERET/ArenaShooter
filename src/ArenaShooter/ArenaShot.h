#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED

#include <Engine.h>
#include <Render.h>


#include "PlayerController.hpp"

using namespace gce;

struct Game
{
    Game(); ;
    ~Game() {};
    void Init();
    int RUN();
    static Game* Create();

private:

    inline static Game* game_Instance = nullptr;

    Scene* m_Scene;
    WindowParam windowParam;
    D12PipelineObject* pPso;

};

#endif

