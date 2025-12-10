#ifndef GAMEMENU_HPP_INCLUDED
#define GAMEMENU_HPP_INCLUDED

#include "define.h"
#include "Engine.h"
#include "SceneManager.h"
#include "CustomScene.h"
#include "MapLoader.hpp"
#include "Player.hpp"
#include "PlayerController.hpp"
#include "Kamikaze.hpp"

using namespace gce;

void InitMenuGame(CustomScene* menu, WindowParam* windowParam, D12PipelineObject* pso)
{
    MapLoader::LoadMap(RES_PATH"res/Maps/blockout.json", menu, pso);

    GameObject& player = menu->AddObject();
    player.transform.SetWorldPosition({ 0,10,0 });
    player.transform.SetWorldScale({ 1.f, 1.f, 1.f });
    player.AddComponent<BoxCollider>();
    player.AddComponent<PhysicComponent>()->SetMass(80.0f);
    player.GetComponent<PhysicComponent>()->SetBounciness(0.0f);
    player.SetName("Player");

    player.AddScript<Player>();
    player.AddScript<PlayerController>();


    GameObject& kamikaze = menu->AddObject();
    MeshRenderer& mesh = *kamikaze.AddComponent<MeshRenderer>();
    mesh.pGeometry = SHAPES.CUBE;
    kamikaze.transform.SetWorldPosition({ 5.f,8.f,0.f });
    kamikaze.transform.SetWorldScale({ 1.f,1.f,1.f });
    kamikaze.AddScript<Kamikaze>();
    kamikaze.AddComponent<BoxCollider>();

}

#endif // !MAINMENU_HPP_INCLUDED
