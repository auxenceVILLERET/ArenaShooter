#include <Windows.h>
#include <Render.h>
#include <Engine.h>
#include <Maths/Vector2.hpp>
#include "Console.h"
#include "Engine/Components/Camera.h"

using namespace gce;

DECLARE_SCRIPT( goScript, ScriptFlag::Update )

void Update() override
{
    if (GetKeyDown(Keyboard::Q)) m_pOwner->GetComponent<PhysicComponent>()->SetVelocity({ -5.f, 0.f, 0.f });
    if (GetKeyDown(Keyboard::D)) m_pOwner->GetComponent<PhysicComponent>()->SetVelocity({ 5.f, 0.f, 0.f });
    if (GetKeyDown(Keyboard::Z)) m_pOwner->GetComponent<PhysicComponent>()->SetVelocity({ 0.f, 0.f, 5.f });
    if (GetKeyDown(Keyboard::S)) m_pOwner->GetComponent<PhysicComponent>()->SetVelocity({ 0.f, 0.f, -5.f });
    Console::Log(m_pOwner->transform.GetWorldPosition().y);
    Console::Log("//////");
    //Console::Log(m_pOwner->GetComponent<PhysicComponent>()->GetVelocity().y);
    Console::Log("------");
}

END_SCRIPT


    int32 APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
    {
        GameManager::Create();

        Scene& scene = Scene::Create();

        WindowParam windowParam;

        Console::Init();

        Geometry* const pSphere = GeometryFactory::CreateSphereGeo(0.5f, 60, 60);
        Geometry* const pCube = GeometryFactory::CreateCubeGeo();
        D12PipelineObject* const pDefaultPSO = new D12PipelineObject(
            SHADERS.VERTEX,
            SHADERS.PIXEL_TEXTURE,
            SHADERS.HULL,
            SHADERS.DOMAIN_,
            SHADERS.ROOT_SIGNATURE_TEXTURE
        );
        Texture t1("res/Textures/aaa.png");

        GameObject& cameraObject = GameObject::Create(scene);
        Camera& camera = *cameraObject.AddComponent<Camera>();
        camera.SetType(PERSPECTIVE);
        camera.perspective.fov = XM_PIDIV4;
        camera.perspective.nearPlane = 0.001f;
        camera.perspective.farPlane = 500.0f;
        camera.perspective.up = { 0.0f, 1.0f, 0.0f };
        cameraObject.transform.SetWorldPosition({0.0f,3.0f,-10.0f});
        cameraObject.transform.LocalRotate({ 0.20f, 0.0f, 0.0f });
        
        GameObject& groundb = GameObject::Create(scene);
        groundb.transform.LocalScale({ 400.f,400.f,400.f });
        groundb.transform.SetLocalPosition({ 9.f, -200.f, 10.f });
        MeshRenderer& meshRendererb = *groundb.AddComponent<MeshRenderer>();
        meshRendererb.pGeometry = pSphere;
        meshRendererb.pPso = pDefaultPSO;
        SphereCollider& colb = *groundb.AddComponent<SphereCollider>();


        GameObject& ground4 = GameObject::Create(scene);
        ground4.transform.SetLocalPosition({ 7.f, 4.f, 10.f });
        MeshRenderer& meshRenderer4 = *ground4.AddComponent<MeshRenderer>();
        meshRenderer4.pGeometry = pSphere;
        meshRenderer4.pPso = pDefaultPSO;
        SphereCollider& col4 = *ground4.AddComponent<SphereCollider>();


        GameObject& second = GameObject::Create(scene);
        second.transform.SetLocalPosition({ 0.5f, 2.3f, 10.f });
        second.transform.SetLocalScale({ 5.f, 0.2f, 5.f });
        //second.transform.WorldRotate({ 0.f,0.f, 0.2f });
        MeshRenderer& meshRenderer2 = *second.AddComponent<MeshRenderer>();
        meshRenderer2.pGeometry = pCube;
        meshRenderer2.pPso = pDefaultPSO;
        BoxCollider& col2 = *second.AddComponent<BoxCollider>();

        GameObject& second2 = GameObject::Create(scene);
        second2.transform.SetLocalPosition({ -2.f, 2.3f, 10.f });
        second2.transform.SetLocalScale({ 0.5f, 5.f, 5.f });
        //second.transform.WorldRotate({ 0.f,0.f, 0.2f });
        MeshRenderer& meshRenderer22 = *second2.AddComponent<MeshRenderer>();
        meshRenderer22.pGeometry = pCube;
        meshRenderer22.pPso = pDefaultPSO;
        BoxCollider& col22 = *second2.AddComponent<BoxCollider>();


    //-----------------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------------


        GameObject& go = GameObject::Create(scene);
        go.AddScript<goScript>();
        go.transform.SetLocalPosition({ -1.f, 7.f, 10.f });
		//go.transform.SetLocalScale({ 0.25f, 1.f, 0.25f });
        MeshRenderer& mR = *go.AddComponent<MeshRenderer>();
        mR.pGeometry = pCube;
        mR.pPso = pDefaultPSO;
        BoxCollider& sC = *go.AddComponent<BoxCollider>();
        PhysicComponent& pC = *go.AddComponent<PhysicComponent>();
        pC.SetBounciness(0.f);
        pC.SetAirDragCoefficient(0.f);
        pC.SetDynamicDragCoefficient(0.03f);
        //pC.SetBounciness(0.2f);
        /*pC.SetDynamicDragCoefficient(0.f);
        pC.SetStaticDragCoefficient(0.f);*/
        /*Force newForce2;
        newForce2.direction = { 1.f,1.f,0.f };
        newForce2.relativeApplicationPoint = { 0.f,0.5f,0.f };
        newForce2.norm = 1000.f;
        pC.AddForceWithPosition(newForce2);*/

        GameObject& go2 = GameObject::Create(scene);
        go2.transform.SetLocalPosition({ 2.f, 7.f, 10.f });
        MeshRenderer& mR2 = *go2.AddComponent<MeshRenderer>();
        mR2.pGeometry = pCube;
        mR2.pPso = pDefaultPSO;
        BoxCollider& sC2 = *go2.AddComponent<BoxCollider>();
        PhysicComponent& pC2 = *go2.AddComponent<PhysicComponent>();
        pC2.SetBounciness(0.f);
        pC2.SetAirDragCoefficient(0.f);
        pC2.SetDynamicDragCoefficient(0.03f);

        GameManager::Run(windowParam);

        delete pCube;
        delete pDefaultPSO;

        GameManager::Destroy();
        return 0;
    }
