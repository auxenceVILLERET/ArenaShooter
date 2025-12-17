#ifndef OPTIONSMENU_HPP_INCLUDED
#define OPTIONSMENU_HPP_INCLUDED

#include "define.h"
#include "CustomScene.h"
#include <Engine.h>
#include "OptionController.hpp"

using namespace gce;

class OptionMenu : public CustomScene
{
public:
    OptionMenu(Scene* pScene) : CustomScene(pScene) {}
    
    void Init() override
    {
        CustomScene::Init();

        GameObject& Background = AddObject();
        ImageUI& uiImage = *Background.AddComponent<ImageUI>();
        Vector2f32 size = { 1920, 1080 };
        uiImage.InitializeImage({ 0,0 }, size, 1.f);
        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/Option/paramettre.png");
        uiImage.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage.SetActive(true);

        GameObject& ligne = AddObject();
        ImageUI& uiImageL = *ligne.AddComponent<ImageUI>();
        uiImageL.InitializeImage({ 0,0 }, {1920,134}, 1.f);
        uiImageL.btmBrush = new BitMapBrush("res/ArenaShooter/Option/Ligne.png");
        uiImageL.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImageL.SetActive(true);
        

        BitMapBrush* buttonBrush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg" };
        BitMapBrush* button2Brush = new BitMapBrush{ "res/ArenaShooter/VilleretAuxence.jpg" };

        GameObject& menu_controller = AddObject();
        menu_controller.AddScript<OptionController>();

        GameObject& playButton = AddObject();
        playButton.transform.LocalTranslate({ GameManager::GetWindowParam().width / 2.f, 200.f, 0.0f });
        playButton.transform.LocalScale({ 216.0f, 69.0f, 1.0f });
        UIButton& button = *playButton.AddComponent<UIButton>();


        button.AddListener(&(menu_controller.GetScript<OptionController>()->GoToMainMenu));

        button.pBitMapBrush = buttonBrush;
        button.pHoverBitMapBrush = button2Brush;
    }
};

#endif // !1
