#ifndef TEST_RAYCAST_H
#define TEST_RAYCAST_H


#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;



void RotateTest(GameObject* pGameObjet) {

    float32 const deltaTime = GameManager::DeltaTime();
    pGameObjet->transform.WorldRotate({ 0.0f, 1.0f * deltaTime, 0.0f });
}

// Script to rotate the sphere
DECLARE_SCRIPT(RotateScript, ScriptFlag::Update)

void Update() override
{
    RotateTest(m_pOwner);
}

END_SCRIPT

DECLARE_SCRIPT(RaycastScriptWindow, ScriptFlag::Update)

GameObject* playerSphere = nullptr;
GameObject* cameraObject = nullptr;
GameObject* targetSphere = nullptr;
GameObject* targetSphere2 = nullptr;
GameObject* targetCube = nullptr;
GameObject* targetCube2 = nullptr;

Texture* laserTexture = nullptr;
Texture* brickTexture = nullptr;
Texture* metalTexture = nullptr;

void Update() override
{
    if (!playerSphere || !cameraObject) return;

    static float32 timer = 0.f;
    float32 delay = 0.05f;
    timer += GameManager::DeltaTime();

    if (timer > delay)
    {
        timer = 0.f;
        Ray ray;
        ray.origin = playerSphere->transform.GetLocalPosition();
        ray.direction = playerSphere->transform.GetLocalForward();
        float32 maxDistance = 100.f;

        RaycastHit hitInfo;
        float32 distance = maxDistance;
        Vector3f32 hitPoint = ray.origin + ray.direction * distance;

        auto* window = GameManager::GetWindow();
        Vector2i32 screenSize(window->GetWidth(), window->GetHeight());
        wchar_t buffer[128];
        swprintf(buffer, 128, L"[V] W and H : %.2f, %.2f\n", screenSize.x, screenSize.y);
        OutputDebugString(buffer);
        if (PhysicSystem::RaycastFromCursor(*cameraObject, screenSize, hitInfo, maxDistance))
        {
            if (hitInfo.pGameObject && hitInfo.pGameObject->HasComponent<MeshRenderer>())
            {
                hitInfo.pGameObject->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = laserTexture->GetTextureID();
            }

            distance = hitInfo.distance;
            hitPoint = hitInfo.point;

            swprintf(buffer, 128, L"[V] Hit at: %.2f, %.2f, %.2f | Dist: %.2f\n", hitPoint.x, hitPoint.y, hitPoint.z, distance);
            OutputDebugString(buffer);

            swprintf(buffer, 128, L"[V] Hit GameObject Name : %hs\n", hitInfo.pGameObject->GetName());
            OutputDebugString(buffer);
        }
        else
        {
            targetSphere->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = brickTexture->GetTextureID();
            targetSphere2->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = brickTexture->GetTextureID();
            targetCube->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = metalTexture->GetTextureID();
            targetCube2->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = metalTexture->GetTextureID();

            OutputDebugString(L"[x] No hit.\n");
        }
    }
}
END_SCRIPT


DECLARE_SCRIPT(RaycastScript, ScriptFlag::Update)

GameObject* PlayerSphere = nullptr;
GameObject* targetSphere = nullptr;
GameObject* targetSphere2 = nullptr;
GameObject* targetCube = nullptr;
GameObject* targetCube2 = nullptr;

Texture* laserTexture = nullptr;
Texture* brickTexture = nullptr;
Texture* metalTexture = nullptr;

void Update() override
{
    static float32 timer = 0.f;
    float32 delay = 0.02f;
    timer += GameManager::DeltaTime();
    Ray ray;
    float32 maxDistance = 100.f;

    if (timer > delay)
    {
        timer = 0.f;

        ray.origin = PlayerSphere->transform.GetWorldPosition();
        ray.direction = PlayerSphere->transform.GetLocalForward();

        RaycastHit hitInfo;
        float32 distance = maxDistance;
        Vector3f32 hitPoint = ray.origin + ray.direction * distance;

        auto* window = GameManager::GetWindow();
        Vector2f32 screenSize(window->GetWidth(), window->GetHeight());
        wchar_t buffer[128];
        swprintf(buffer, 128, L"[V] W and H : %.2f, %.2f\n", screenSize.x, screenSize.y);
        OutputDebugString(buffer);
        if (PhysicSystem::IntersectRay(ray, hitInfo, maxDistance))
        {
            if (hitInfo.pGameObject && hitInfo.pGameObject->HasComponent<MeshRenderer>())
            {
                hitInfo.pGameObject->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = laserTexture->GetTextureID();
            }

            distance = hitInfo.distance;
            hitPoint = hitInfo.point;

            wchar_t buffer[128];
            swprintf(buffer, 128, L"[V] Hit at: %.2f, %.2f, %.2f | Dist: %.2f\n", hitPoint.x, hitPoint.y, hitPoint.z, distance);
            OutputDebugString(buffer);

            swprintf(buffer, 128, L"[V] Hit GameObject Name : %hs\n", hitInfo.pGameObject->GetName());
            OutputDebugString(buffer);
        }
        else
        {

            targetSphere->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = brickTexture->GetTextureID();
            targetSphere2->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = brickTexture->GetTextureID();

            targetCube->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = metalTexture->GetTextureID();
            targetCube2->GetComponent<MeshRenderer>()->pMaterial->albedoTextureID = metalTexture->GetTextureID();
        }

        Vector3f32 middle = ray.origin + ray.direction * (distance * 0.5f);
        m_pOwner->transform.SetWorldPosition(middle);

        Vector3f32 rightHandConvert = Vector3f32(1, 1, -1);
        Quaternion lookRotation = Quaternion::RotationMatrix(Matrix::LookToLH(PlayerSphere->transform.GetWorldPosition(), ray.direction * rightHandConvert, PlayerSphere->transform.GetLocalUp()));

        Quaternion correction;
        correction.SetRotationEuler(-PI / 2.0f, 0, 0);

        m_pOwner->transform.SetLocalRotation(correction * lookRotation);

        m_pOwner->transform.SetLocalScale({ 1.f, distance * 0.5f, 1.f });

    }
}

END_SCRIPT


BEGIN_SUBTEST(Raycast, RaycastMouse)

    GameManager::Create();

    Scene& scene = Scene::Create();

    Geometry* const pSphere = GeometryFactory::CreateSphereGeo(0.5, 60, 60);
    Geometry* const pCube = GeometryFactory::CreateCubeGeo();
    Geometry* const pRay = GeometryFactory::CreateCylinderGeo(0.02f, 0.02f, 10.0f, 32, 1);

    D12PipelineObject pso = D12PipelineObject(
        SHADERS.VERTEX,
        SHADERS.PIXEL,
        SHADERS.HULL,
        SHADERS.DOMAIN_,
        SHADERS.ROOT_SIGNATURE);

    Texture metalTexture("res/Textures/metal_roughness.jpg");
    Texture brickTexture("res/Textures/bricks.jpg");
    Texture laserTexture("res/Textures/rouge-laser.jpg");

    GameObject& cameraObject = GameObject::Create(scene);
    cameraObject.transform.SetLocalPosition({ 0, 1,-5 });
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetType(PERSPECTIVE);
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
	camera.SetMainCamera();


    ////////////////////////////////////    Sphere 

    GameObject& targetSphere = GameObject::Create(scene);
    targetSphere.transform.SetLocalPosition({ -3.f, 0.f, 5.f });
    targetSphere.transform.SetLocalScale({ 1.f, 1.f, 1.f });


    MeshRenderer& meshRenderer = *targetSphere.AddComponent<MeshRenderer>();
    meshRenderer.pGeometry = SHAPES.SPHERE;
    meshRenderer.pPso = &pso;
    meshRenderer.pMaterial->albedoTextureID = brickTexture.GetTextureID();
    meshRenderer.pMaterial->useTextureAlbedo = 1;
    SphereCollider& col1 = *targetSphere.AddComponent<SphereCollider>();


    GameObject& targetSphere2 = GameObject::Create(scene);
    targetSphere2.transform.SetLocalPosition({ 3.f, 0.f, 5.f });
    targetSphere2.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    MeshRenderer& meshRenderer2 = *targetSphere2.AddComponent<MeshRenderer>();
    meshRenderer2.pGeometry = SHAPES.SPHERE;
    meshRenderer2.pPso = &pso;
    meshRenderer2.pMaterial->albedoTextureID = metalTexture.GetTextureID();
    meshRenderer2.pMaterial->useTextureAlbedo = 1;
    SphereCollider& col2 = *targetSphere2.AddComponent<SphereCollider>();

    targetSphere.m_name = "TargetSphere";
    targetSphere2.m_name = "TargetSphere2";
    ////////////////////////////////////    end Sphere 
    ////////////////////////////////////    Cube 

    GameObject& targetCube = GameObject::Create(scene);
    targetCube.transform.SetLocalPosition({ -5.f, 0.f, 5.f });
    targetCube.transform.SetLocalScale({ 1.f, 1.f, 1.f });


    MeshRenderer& meshRenderer3 = *targetCube.AddComponent<MeshRenderer>();
    meshRenderer3.pGeometry = SHAPES.CUBE;
    meshRenderer3.pPso = &pso;
    meshRenderer3.pMaterial->albedoTextureID = metalTexture.GetTextureID();
    meshRenderer3.pMaterial->useTextureAlbedo = 1;
    BoxCollider& col3 = *targetCube.AddComponent<BoxCollider>();


    GameObject& targetCube2 = GameObject::Create(scene);
    targetCube2.transform.SetLocalPosition({ 5.f, 0.f, 5.f });
    targetCube2.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    MeshRenderer& meshRenderer4 = *targetCube2.AddComponent<MeshRenderer>();
    meshRenderer4.pGeometry = SHAPES.CUBE;
    meshRenderer4.pPso = &pso;
    meshRenderer4.pMaterial->albedoTextureID = metalTexture.GetTextureID();
    meshRenderer4.pMaterial->useTextureAlbedo = 1;
    BoxCollider& col4 = *targetCube2.AddComponent<BoxCollider>();

    targetCube.m_name = "TargetCube";
    targetCube2.m_name = "TargetCube2";

    ////////////////////////////////////   end Cube 

    GameObject& PlayerSphere = GameObject::Create(scene);
    PlayerSphere.transform.SetLocalPosition({ 0.f, 0.f, 0.f });
    PlayerSphere.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    GameObject& Raycast = GameObject::Create(scene);
    RaycastScriptWindow* script = Raycast.AddScript<RaycastScriptWindow>();

    script->playerSphere = &PlayerSphere;
    script->cameraObject = &cameraObject;
    script->targetSphere = &targetSphere;
    script->targetSphere2 = &targetSphere2;
    script->targetCube = &targetCube;
    script->targetCube2 = &targetCube2;

    script->laserTexture = &laserTexture;
    script->brickTexture = &brickTexture;
    script->metalTexture = &metalTexture;


    WindowParam windowParam;
    GameManager::Run(windowParam);

    delete pSphere;
    delete pRay;
    delete pCube;
    delete &pso;

    GameManager::Destroy();
    return;

END_SUBTEST(RaycastMouse)

BEGIN_SUBTEST(Raycast, RaycastGO)
    GameManager::Create();

    Scene& scene = Scene::Create();

    Geometry* const pSphere = GeometryFactory::CreateSphereGeo(0.5, 60, 60);
    Geometry* const pCube = GeometryFactory::CreateCubeGeo();
    Geometry* const pRay = GeometryFactory::CreateCylinderGeo(0.02f, 0.02f, 1.5f, 32, 1);

    D12PipelineObject pso = D12PipelineObject(
        SHADERS.VERTEX,
        SHADERS.PIXEL,
        SHADERS.HULL,
        SHADERS.DOMAIN_,
        SHADERS.ROOT_SIGNATURE);

    Texture brickTexture("res/Textures/bricks.jpg");
    Texture metalTexture("res/Textures/metal.jpg");
    Texture stoneTexture("res/Textures/stone_color.jpg");
    Texture laserTexture("res/Textures/rouge-laser.jpg");

    GameObject& cameraObject = GameObject::Create(scene);
    cameraObject.transform.SetLocalPosition({ 0, 1,-5 });
    Camera& camera = *cameraObject.AddComponent<Camera>();
    camera.SetType(PERSPECTIVE);
    camera.perspective.fov = XM_PIDIV4;
    camera.perspective.nearPlane = 0.001f;
    camera.perspective.farPlane = 500.0f;
    camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    camera.SetMainCamera();

    ////////////////////////////////////    Sphere 

    GameObject& targetSphere = GameObject::Create(scene);
    targetSphere.transform.SetLocalPosition({ -3.f, 0.f, 5.f });
    targetSphere.transform.SetLocalScale({ 1.f, 1.f, 1.f });


    MeshRenderer& meshRenderer = *targetSphere.AddComponent<MeshRenderer>();
    meshRenderer.pGeometry = pSphere;
    meshRenderer.pPso = &pso;
    meshRenderer.pMaterial->albedoTextureID = brickTexture.GetTextureID();
    meshRenderer.pMaterial->useTextureAlbedo = 1;
    SphereCollider& col1 = *targetSphere.AddComponent<SphereCollider>();


    GameObject& targetSphere2 = GameObject::Create(scene);
    targetSphere2.transform.SetLocalPosition({ 3.f, 0.f, 5.f });
    targetSphere2.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    MeshRenderer& meshRenderer2 = *targetSphere2.AddComponent<MeshRenderer>();
    meshRenderer2.pGeometry = pSphere;
    meshRenderer2.pPso = &pso;
    meshRenderer2.pMaterial->albedoTextureID = brickTexture.GetTextureID();
    meshRenderer2.pMaterial->useTextureAlbedo = 1;
    SphereCollider& col2 = *targetSphere2.AddComponent<SphereCollider>();
    ////////////////////////////////////    end Sphere 
    ////////////////////////////////////    Cube 

    GameObject& targetCube = GameObject::Create(scene);
    targetCube.transform.SetLocalPosition({ -1.f, 0.f, 10.f });
    targetCube.transform.SetLocalScale({ 1.f, 1.f, 1.f });


    MeshRenderer& meshRenderer3 = *targetCube.AddComponent<MeshRenderer>();
    meshRenderer3.pGeometry = pCube;
    meshRenderer3.pPso = &pso;
    meshRenderer3.pMaterial->albedoTextureID = metalTexture.GetTextureID();
    meshRenderer3.pMaterial->useTextureAlbedo = 1;
    BoxCollider& col3 = *targetCube.AddComponent<BoxCollider>();


    GameObject& targetCube2 = GameObject::Create(scene);
    targetCube2.transform.SetLocalPosition({ 1.f, 0.f, 10.f });
    targetCube2.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    MeshRenderer& meshRenderer4 = *targetCube2.AddComponent<MeshRenderer>();
    meshRenderer4.pGeometry = pCube;
    meshRenderer4.pPso = &pso;
    meshRenderer4.pMaterial->albedoTextureID = metalTexture.GetTextureID();
    meshRenderer4.pMaterial->useTextureAlbedo = 1;
    BoxCollider& col4 = *targetCube2.AddComponent<BoxCollider>();

    ////////////////////////////////////   end Cube 

    GameObject& PlayerSphere = GameObject::Create(scene);
    PlayerSphere.AddScript<RotateScript>();
    PlayerSphere.transform.SetLocalPosition({ 0.f, 0.f, 0.f });
    PlayerSphere.transform.SetLocalScale({ 1.f, 1.f, 1.f });

    MeshRenderer& meshRenderer5 = *PlayerSphere.AddComponent<MeshRenderer>();
    meshRenderer5.pGeometry = pSphere;
    meshRenderer5.pPso = &pso;
    meshRenderer5.pMaterial->albedoTextureID = stoneTexture.GetTextureID();
    meshRenderer5.pMaterial->useTextureAlbedo = 1;

    GameObject& Raycast = GameObject::Create(scene);
    RaycastScript* script = Raycast.AddScript<RaycastScript>();

    script->PlayerSphere = &PlayerSphere;
    script->targetSphere = &targetSphere;
    script->targetSphere2 = &targetSphere2;
    script->targetCube = &targetCube;
    script->targetCube2 = &targetCube2;

    script->laserTexture = &laserTexture;
    script->brickTexture = &brickTexture;
    script->metalTexture = &metalTexture;

    MeshRenderer& meshRenderer6 = *Raycast.AddComponent<MeshRenderer>();
    meshRenderer6.pGeometry = pRay;
    meshRenderer6.pPso = &pso;
    meshRenderer6.pMaterial->albedoTextureID = laserTexture.GetTextureID();
    meshRenderer6.pMaterial->useTextureAlbedo = 1;

    WindowParam windowParam;
    GameManager::Run(windowParam);

    delete pSphere;
    delete pCube;
    delete pRay;
    delete &pso;

    GameManager::Destroy();
    return;

END_SUBTEST(RaycastGO)

#endif