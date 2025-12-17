#ifndef OPTIONSMENU_HPP_INCLUDED
#define OPTIONSMENU_HPP_INCLUDED

#include "define.h"
#include "CustomScene.h"
#include <Engine.h>
#include "OptionController.hpp"
#include "Components/SliderComponent.h"

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
        uiImage.btmBrush = new BitMapBrush("res/ArenaShooter/Option/PARAMETRE.png");
        uiImage.btmBrush->SetTransformMatrix({ 0, 0, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImage.SetActive(true);

        GameObject& Sensi = AddObject();
        ImageUI& uiImageS = *Sensi.AddComponent<ImageUI>();
        uiImageS.InitializeImage({ 680,680 }, {603,48}, 1.f);
        uiImageS.btmBrush = new BitMapBrush("res/ArenaShooter/Option/sensibilité_e.png");
        uiImageS.btmBrush->SetTransformMatrix({ 680, 680, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImageS.SetActive(true);
        
        GameObject& slider5 = AddObject();
        SliderComponent* s5 = slider5.AddComponent<SliderComponent>();
        s5->size = { 800, 30 };
        s5->min = 0.f;
        s5->max = 0.5f;
        s5->interval = 0.01f;
        s5->value = 0.f;
        s5->handleSize = { 40, 40 };
        s5->orientation = SliderComponent::LEFT_TO_RIGHT;
        s5->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
        s5->useFillImage = TRUE;
        s5->fillImageBrush = new BitMapBrush("res/ArenaShooter/Option/barre_a.png");
		s5->useBarImage = TRUE;
		s5->barImageBrush = new BitMapBrush("res/ArenaShooter/Option/barre_e.png");
		s5->useHandleImage = TRUE;
		s5->handleImageBrush = new BitMapBrush("res/ArenaShooter/Option/curseur.png");
        s5->SetPosition({ 1000, 750 });

        GameObject& Audio = AddObject();
        ImageUI& uiImageA = *Audio.AddComponent<ImageUI>();
        uiImageA.InitializeImage({ 680,580 }, { 603,48 }, 1.f);
        uiImageA.btmBrush = new BitMapBrush("res/ArenaShooter/Option/son_e.png");
        uiImageA.btmBrush->SetTransformMatrix({ 680, 580, 0 }, { 1.f , 1.f , 1.f }, 0);
        uiImageA.SetActive(true);

        GameObject& slider6 = AddObject();
        SliderComponent* s6 = slider6.AddComponent<SliderComponent>();
        s6->size = { 800, 30 };
        s6->min = 0.f;
        s6->max = 1.f;
        s6->interval = 0.2f;
        s6->value = 0.f;
        s6->handleSize = { 40, 40 };
        s6->orientation = SliderComponent::LEFT_TO_RIGHT;
        s6->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
        s6->useFillImage = TRUE;
        s6->fillImageBrush = new BitMapBrush("res/ArenaShooter/Option/barre_a.png");
        s6->useBarImage = TRUE;
        s6->barImageBrush = new BitMapBrush("res/ArenaShooter/Option/barre_e.png");
        s6->useHandleImage = TRUE;
        s6->handleImageBrush = new BitMapBrush("res/ArenaShooter/Option/curseur.png");
        s6->SetPosition({ 1000, 650 });

        BitMapBrush* buttonBrushA = new BitMapBrush{ "res/ArenaShooter/Option/avancer_e.png" };
        BitMapBrush* button2BrushA = new BitMapBrush{ "res/ArenaShooter/Option/avancer_a.png" };

        GameObject& avancerButton = AddObject();
        avancerButton.transform.LocalTranslate({ 650.f,400.f ,0.f });
        avancerButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonA = *avancerButton.AddComponent<UIButton>();

        buttonA.pBitMapBrush = buttonBrushA;
        buttonA.pHoverBitMapBrush = button2BrushA;

        BitMapBrush* buttonBrushG = new BitMapBrush{ "res/ArenaShooter/Option/gauche_e.png" };
        BitMapBrush* button2BrushG = new BitMapBrush{ "res/ArenaShooter/Option/gauche_a.png" };

        GameObject& gaucheButton = AddObject();
        gaucheButton.transform.LocalTranslate({ 865.f,400.f ,0.f });
        gaucheButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonG = *gaucheButton.AddComponent<UIButton>();

        buttonG.pBitMapBrush = buttonBrushG;
        buttonG.pHoverBitMapBrush = button2BrushG;

        BitMapBrush* buttonBrushS = new BitMapBrush{ "res/ArenaShooter/Option/sauter_e.png" };
        BitMapBrush* button2BrushS = new BitMapBrush{ "res/ArenaShooter/Option/sauter_a.png" };

        GameObject& sauterButton = AddObject();
        sauterButton.transform.LocalTranslate({ 1080.f,400.f ,0.f });
        sauterButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonS = *sauterButton.AddComponent<UIButton>();

        buttonS.pBitMapBrush = buttonBrushS;
        buttonS.pHoverBitMapBrush = button2BrushS;

        BitMapBrush* buttonBrushT = new BitMapBrush{ "res/ArenaShooter/Option/tirer_e.png" };
        BitMapBrush* button2BrushT = new BitMapBrush{ "res/ArenaShooter/Option/tirer_a.png" };

        GameObject& tirerButton = AddObject();
        tirerButton.transform.LocalTranslate({ 1295.f,400.f ,0.f });
        tirerButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonT = *tirerButton.AddComponent<UIButton>();

        buttonT.pBitMapBrush = buttonBrushT;
        buttonT.pHoverBitMapBrush = button2BrushT;

        BitMapBrush* buttonBrushR = new BitMapBrush{ "res/ArenaShooter/Option/reculer_e.png" };
        BitMapBrush* button2BrushR = new BitMapBrush{ "res/ArenaShooter/Option/reculer_a.png" };

        GameObject& reculerButton = AddObject();
        reculerButton.transform.LocalTranslate({ 650.f,500.f ,0.f });
        reculerButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonR = *reculerButton.AddComponent<UIButton>();

        buttonR.pBitMapBrush = buttonBrushR;
        buttonR.pHoverBitMapBrush = button2BrushR;

        BitMapBrush* buttonBrushD = new BitMapBrush{ "res/ArenaShooter/Option/droite_e.png" };
        BitMapBrush* button2BrushD = new BitMapBrush{ "res/ArenaShooter/Option/droite_a.png" };

        GameObject& droiteButton = AddObject();
        droiteButton.transform.LocalTranslate({ 865.f,500.f ,0.f });
        droiteButton.transform.LocalScale({ 216.f, 60.0f, 1.0f });
        UIButton& buttonD = *droiteButton.AddComponent<UIButton>();

        buttonD.pBitMapBrush = buttonBrushD;
        buttonD.pHoverBitMapBrush = button2BrushD;


        BitMapBrush* buttonBrush = new BitMapBrush{ "res/ArenaShooter/retour_au_menu_e.png" };
        BitMapBrush* button2Brush = new BitMapBrush{ "res/ArenaShooter/retour_au_menu_a.png" };

        GameObject& menu_controller = AddObject();
        menu_controller.AddScript<OptionController>();

        GameObject& backButton = AddObject();
        backButton.transform.LocalTranslate({ 750.f,850.f ,0.f});
        backButton.transform.LocalScale({ 480.f, 80.0f, 1.0f });
        UIButton& button = *backButton.AddComponent<UIButton>();


        button.AddListener(&(menu_controller.GetScript<OptionController>()->GoToMainMenu));

        button.pBitMapBrush = buttonBrush;
        button.pHoverBitMapBrush = button2Brush;
    }
};

#endif // !1
