#ifndef ENGINE_GAME_MANAGER_H_INCLUDED
#define ENGINE_GAME_MANAGER_H_INCLUDED

#include <SplitScreenWindow.h>
#include <Shader.h>
#include "define.h"

#include "Chrono.h"
#include "Systems.h"
#include "Components/Camera.h"

namespace gce {

struct WindowParam
{
    WStringView title = L"Game Window"; // Do not work because of GameManager::HandleFPS
	uint16 width = 1280;
	uint16 height = 720;
	bool isFullScreen = false;
    bool isSplitScreen = false;
	SplitScreenDisposition screenDisposition = SQUARE_4_PLAYERS;
    
    WindowParam() = default;
};

class Scene;
class Window;

class GameManager final
{
public:
    static void Create();
    static void Run(WindowParam& param);
    static void Destroy();

    [[nodiscard]] static float32 DeltaTime();
    [[nodiscard]] static float32 FixedDeltaTime();
    [[nodiscard]] static float32 FPS();
	[[nodiscard]] static Window* GetWindow();
	[[nodiscard]] static WindowParam& GetWindowParam();
    [[nodiscard]] static RenderSystem& GetRenderSystem();
    [[nodiscard]] static LifespanSystem& GetLifespanSystem();
    [[nodiscard]] static ScriptSystem& GetScriptSystem();
	[[nodiscard]] static PhysicSystem& GetPhysicSystem();
	[[nodiscard]] static UiSystem& GetUiSystem();
    [[nodiscard]] static Vector<Camera*>& GetMainCameras();

    GameManager();
    ~GameManager();

    void GameLoop(WindowParam& param);

public: // Temporary (should be named and used as private)
    inline static GameManager* s_pInstance = nullptr;

    void HandleFPS();

    Window* m_pWindow;
    WindowParam m_windowParam;

    float m_timeElapsed;
    uint16 m_frameCount;

    Chrono m_chrono;
    float32 m_deltaTime;
    float32 m_fixedDeltaTime = 1.0f / 120.0f;
    float32 m_timeSinceFixedUpdate;
    float32 m_fps;

    RenderSystem m_renderSystem;
    LifespanSystem m_lifespanSystem;
    PhysicSystem m_physicSystem;
    ScriptSystem m_scriptSystem;
    UiSystem m_uiSystem;
    
    Vector<Scene*> m_scenes;
    Vector<Camera*> m_mainCameras;
    
    // Shader vertexShader { "res/Render/Shaders/Lit.vs.cso" };
    // Shader pixelShader { "res/Render/Shaders/Lit.ps.cso" };
    // Shader rootSignature { "res/Render/Shaders/Lit.rs.cso" };
    // Shader hullShader { "res/Render/Shaders/Lit.hs.cso" };
    // Shader domainShader { "res/Render/Shaders/Lit.ds.cso" };
    //
    // Shader pixelShaderTexture{ "res/Render/Shaders/TextureUnlit.ps.cso" };
    // Shader rootSignatureTexture{ "res/Render/Shaders/TextureUnlit.rs.cso" };

    friend class Scene;
};


}

#include "GameManager.inl"

#endif