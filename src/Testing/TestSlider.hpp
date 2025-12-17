#ifndef TEST_SLIDER_HPP_INCLUDED
#define TEST_SLIDER_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
#include "Structs.h"

#include "Components/SliderComponent.h"
using namespace gce;


BEGIN_TEST(Slider)

GameManager::Create();
Scene& scene = Scene::Create();

GameObject& slider = GameObject::Create(scene);
slider.AddComponent<SliderComponent>();
slider.GetComponent<SliderComponent>()->size = { 700, 70 };
slider.GetComponent<SliderComponent>()->max = 2.f;
slider.GetComponent<SliderComponent>()->interval = 0.1f;
slider.GetComponent<SliderComponent>()->showHandle = true;
slider.GetComponent<SliderComponent>()->handleSize = { 100, 100 };
slider.GetComponent<SliderComponent>()->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
slider.GetComponent<SliderComponent>()->SetPosition({ 400, 100 });
slider.GetComponent<SliderComponent>()->SetBaseColor(Color::Blue);
slider.AddScript<HandleSelection>();

GameObject& slider1 = GameObject::Create(scene);
SliderComponent* s1 = slider1.AddComponent<SliderComponent>();
s1->size = { 700, 70 };
s1->max = 100.f;
s1->interval = 25.f;
s1->handleSize = { 20, 100 };
s1->sliderShape = SliderComponent::RECTANGLE;
s1->SetPosition({ 400, 200 });
s1->SetBaseColor(Color::Green);
slider1.AddScript<HandleSelection>();

GameObject& slider2 = GameObject::Create(scene);
SliderComponent* s2 = slider2.AddComponent<SliderComponent>();
s2->size = { 700, 70 };
s2->min = -100.f;
s2->max = 0.f;
s2->interval = 1.f;
s2->value = -100.f;
s2->handleSize = { 70, 70 };
s2->orientation = SliderComponent::RIGHT_TO_LEFT;
s2->sliderShape = SliderComponent::ROUND;
s2->SetPosition({ 400, 300 });
s2->SetBaseColor(Color::Red);
slider2.AddScript<HandleSelection>();

GameObject& slider3 = GameObject::Create(scene);
SliderComponent* s3 = slider3.AddComponent<SliderComponent>();
s3->size = { 70, 500 };
s3->min = -100.f;
s3->max = 100.f;
s3->interval = 20.f;
s3->value = -100.f;
s3->handleSize = { 70, 150 };
s3->orientation = SliderComponent::UP_TO_DOWN;
s3->sliderShape = SliderComponent::ROUND;
s3->SetPosition({ 800, 300 });
s3->SetBaseColor(Color::Yellow);
slider3.AddScript<HandleSelection>();


GameObject& slider4 = GameObject::Create(scene);
SliderComponent* s4 = slider4.AddComponent<SliderComponent>();
s4->size = { 70, 500 };
s4->min = 0.f;
s4->max = 1.f;
s4->interval = 0.2f;
s4->value = 0.f;
s4->handleSize = { 70, 150 };
s4->orientation = SliderComponent::DOWN_TO_UP;
s4->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
s4->SetPosition({ 900, 300 });
s4->SetBaseColor(Color(157, 0, 255));
slider4.AddScript<HandleSelection>();

GameObject& slider5 = GameObject::Create(scene);
SliderComponent* s5 = slider5.AddComponent<SliderComponent>();
s5->size = { 300, 300 };
s5->min = 0.f;
s5->max = 1.f;
s5->interval = 0.2f;
s5->value = 0.f;
s5->handleSize = { 70, 70 };
s5->orientation = SliderComponent::LEFT_TO_RIGHT;
s5->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
s5->useFillImage = TRUE;
s5->fillImageBrush = new BitMapBrush("res/ArenaShooter/VilleretAuxence.jpg");
s5->SetPosition({ 200, 500 });
slider5.AddScript<HandleSelection>();

GameObject& slider6 = GameObject::Create(scene);
SliderComponent* s6 = slider6.AddComponent<SliderComponent>();
s6->size = { 300, 300 };
s6->min = 0.f;
s6->max = 1.f;
s6->interval = 0.2f;
s6->value = 0.f;
s6->handleSize = { 70, 70 };
s6->orientation = SliderComponent::RIGHT_TO_LEFT;
s6->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
s6->useBarImage = TRUE;
s6->barImageBrush = new BitMapBrush("res/ArenaShooter/VilleretAuxence.jpg");
s6->SetPosition({ 600, 500 });
slider6.AddScript<HandleSelection>();

GameObject& slider7 = GameObject::Create(scene);
SliderComponent* s7 = slider7.AddComponent<SliderComponent>();
s7->size = { 70, 500 };
s7->min = 0.f;
s7->max = 1.f;
s7->interval = 0.2f;
s7->value = 0.f;
s7->handleSize = { 70, 150 };
s7->orientation = SliderComponent::DOWN_TO_UP;
s7->sliderShape = SliderComponent::ROUNDED_RECTANGLE;
s7->useHandleImage = TRUE;
s7->handleImageBrush = new BitMapBrush("res/ArenaShooter/VilleretAuxence.jpg");
s7->SetPosition({ 1000, 300 });
slider7.AddScript<HandleSelection>();

WindowParam params;
GameManager::Run(params);

GameManager::Destroy();

END_TEST_MAIN_FUNCTION(Slider)

DECLARE_SCRIPT(HandleSelection, ScriptFlag::Update)

void Update()
{

}
END_SCRIPT
END_TEST(Slider)

#endif