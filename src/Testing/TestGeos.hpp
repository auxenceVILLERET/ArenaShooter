#ifndef TEST_GEOS_HPP_INCLUDED
#define TEST_GEOS_HPP_INCLUDED

#include <Render.h>
#include <Engine.h>
#include "define.h"
using namespace gce;

BEGIN_TEST( Geos )
{
    WindowParam param;
    param.screenDisposition = HORIZONTAL_2_PLAYERS;
    param.isSplitScreen = false;

    GameManager::Create();
    Scene& scene = Scene::Create();

    D12PipelineObject texturePso(
        SHADERS.VERTEX,
        SHADERS.PIXEL,
        SHADERS.HULL,
        SHADERS.DOMAIN_,
        SHADERS.ROOT_SIGNATURE
    );

    Texture colorText("res/Textures/Rifle_Color.png");
    Texture metallnessText("res/Textures/Rifle_Mettalic.png");
    Texture roughnessText("res/Textures/Rifle_Roughness.png");
    
    // Camera 1
    {
        GameObject& object = GameObject::Create( scene );
        object.transform.LocalTranslate( { 0, 0, -10 } );

        MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
        meshRenderer.pGeometry = SHAPES.CUBE;
        meshRenderer.pPso = &texturePso;

        Camera& camera = *object.AddComponent<Camera>();
        camera.SetMainCamera();
        camera.SetType( PERSPECTIVE );
        camera.screenId = 1;
        camera.perspective.fov = PI/4.0f;
        camera.perspective.nearPlane = 0.001f;
        camera.perspective.farPlane = 500.0f;
        camera.perspective.up = { 0.0f, 1.0f, 0.0f };
    }

    GameObject& lightObject = GameObject::Create(scene);
    Light* light = lightObject.AddComponent<Light>();
    light->DefaultDirectionLight();
    light->intensity = 2.0f; 
    light->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    
    GameObject& object = GameObject::Create( scene );
    object.transform.SetLocalPosition( { 0.f, 0.f, 0.f } );
    object.AddScript<MovementScript>();
    MeshRenderer& meshRenderer = *object.AddComponent<MeshRenderer>();
    meshRenderer.pGeometry = GeometryFactory::LoadGeometry(RES_PATH"res/Test/Obj/Rifle.obj");
    meshRenderer.pPso = &texturePso;
    meshRenderer.pMaterial->useTextureAlbedo = 1;
    meshRenderer.pMaterial->useTextureMetalness = 1;
    meshRenderer.pMaterial->useTextureRoughness = 1;
    meshRenderer.pMaterial->subsurface = 0.2f;
    meshRenderer.pMaterial->albedoTextureID = colorText.GetTextureID();
    meshRenderer.pMaterial->roughnessTextureID = roughnessText.GetTextureID();
    meshRenderer.pMaterial->metalnessTextureID = metallnessText.GetTextureID();
    
    GameManager::Run( param );

    GameManager::Destroy();
}
END_TEST_MAIN_FUNCTION( Geos )

static void HandleCameraMovements( GameObject* const pCamera )
{
    if ( pCamera == nullptr ) return;

    float32 const deltaTime = GameManager::DeltaTime();

    // Position
    Vector3f32 translation {};
    translation += pCamera->transform.GetLocalRight() * static_cast<float32>( GetKey( Keyboard::D ) - GetKey( Keyboard::Q ) );
    translation += pCamera->transform.GetLocalUp() * static_cast<float32>( GetKey( Keyboard::SPACE ) - GetKey( Keyboard::LCONTROL ) );
    translation += pCamera->transform.GetLocalForward() * static_cast<float32>( GetKey( Keyboard::Z ) - GetKey( Keyboard::S ) );
    translation *= deltaTime;

    // Scale
    float32 const allScale = static_cast<float32>( GetKey( Keyboard::NUMPAD_ADD ) - GetKey( Keyboard::NUMPAD_SUBTRACT ) );

    Vector3f32 scale;
    scale.x = static_cast<float32>( GetKey( Keyboard::INSERT ) - GetKey( Keyboard::DELETE_ ) ) + allScale;
    scale.y = static_cast<float32>( GetKey( Keyboard::HOME ) - GetKey( Keyboard::END ) ) + allScale;
    scale.z = static_cast<float32>( GetKey( Keyboard::PAGE_UP ) - GetKey( Keyboard::PAGE_DOWN ) ) + allScale;
    scale *= deltaTime;

    Vector3f32 one;
    one.SetOne();
    scale += one;

    Vector2i32 const middlePosition( GameManager::GetWindow()->GetWidth() / 2, GameManager::GetWindow()->GetHeight() / 2 );

    Vector2i32 mouseDirection = GetMousePosition();
    mouseDirection.x -= middlePosition.x;
    mouseDirection.y -= middlePosition.y;

    SetMousePosition( middlePosition );

    // Rotation
    Vector3f32 localRotation {
        static_cast<float32>( mouseDirection.y ),
        0.0f,
        static_cast<float32>( GetKey( Keyboard::NUMPAD7 ) - GetKey( Keyboard::NUMPAD9 ) )
    };
    localRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

    Vector3f32 worldRotation {
        0.0f,
        static_cast<float32>( mouseDirection.x ),
        0.0f
    };
    worldRotation *= 3.14f / 180.0f * deltaTime * 5.0f;

    pCamera->transform.LocalTranslate( translation );
    pCamera->transform.LocalScale( scale );
    pCamera->transform.LocalRotate( localRotation );
    pCamera->transform.WorldRotate( worldRotation );
}

DECLARE_SCRIPT(CameraScript, ScriptFlag::Update)

void Update() override
{
    if (GetKeyDown(Keyboard::L))
    {
        if (IsMouseCursorLocked()) UnlockMouseCursor();
        else LockMouseCursor();
    }
    if (GetKeyDown(Keyboard::H))
    {
        if (IsMouseCursorVisible()) HideMouseCursor();
        else ShowMouseCursor();
    }
    HandleCameraMovements(m_pOwner);
}

END_SCRIPT

DECLARE_SCRIPT(MovementScript, ScriptFlag::Update)

void Update() override
{
    float32 const deltaTime = GameManager::DeltaTime();

    // Position
    Vector3f32 translation;
    translation.x = static_cast<float32>( GetKey( Keyboard::D ) - GetKey( Keyboard::Q ) );
    translation.y = static_cast<float32>( GetKey( Keyboard::SPACE ) - GetKey( Keyboard::LCONTROL ) );
    translation.z = static_cast<float32>( GetKey( Keyboard::Z ) - GetKey( Keyboard::S ) );
    translation *= deltaTime;

    // Scale
    float32 const allScale = static_cast<float32>( GetKey( Keyboard::NUMPAD_ADD ) - GetKey( Keyboard::NUMPAD_SUBTRACT ) );

    Vector3f32 scale;
    scale.x = static_cast<float32>( GetKey( Keyboard::INSERT ) - GetKey( Keyboard::DELETE_ ) ) + allScale;
    scale.y = static_cast<float32>( GetKey( Keyboard::HOME ) - GetKey( Keyboard::END ) ) + allScale;
    scale.z = static_cast<float32>( GetKey( Keyboard::PAGE_UP ) - GetKey( Keyboard::PAGE_DOWN ) ) + allScale;
    scale *= deltaTime;

    Vector3f32 one;
    one.SetOne();
    scale += one;

    // Rotation
    Vector3f32 rotation;
    rotation.x = static_cast<float32>( GetKey( Keyboard::NUMPAD8 ) - GetKey( Keyboard::NUMPAD5 ) );
    rotation.y = static_cast<float32>( GetKey( Keyboard::NUMPAD4 ) - GetKey( Keyboard::NUMPAD6 ) );
    rotation.z = static_cast<float32>( GetKey( Keyboard::NUMPAD7 ) - GetKey( Keyboard::NUMPAD9 ) );
    rotation *= 3.14f / 180.0f * deltaTime * 50.0f;

    m_pOwner->transform.LocalTranslate( translation );
    m_pOwner->transform.LocalScale( scale );
    m_pOwner->transform.LocalRotate( { rotation.x, rotation.y, rotation.z } );
}

END_SCRIPT

END_TEST( Geos )

#endif
