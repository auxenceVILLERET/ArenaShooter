#include "pch.h"

#include "RenderCamera.h"
#include "D12ComputePipelineObject.h"
#include "D12PipelineObject.h"
#include "Geometry.h"
#include "RenderTarget.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "RenderWindow.h"
#include "SplitScreenWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTransform.h"
#include "FrameResources.h"
#include "Maths.h"
#include "LightManager.h"

#include "Tests.h"

namespace gce
{
	void TestFrameResources()
	{
		RenderContext::Init();
		RenderContext::SetMSAA(MSAA_COUNT_X4);

		RenderWindow window;
		window.Create(L"Window 1", 1920, 1080);

		auto& frameMng = FrameResourcesManager::Instance();

		Shader rootSig("res/Render/Shaders/Default.rs.cso");
		Shader vs("res/Render/Shaders/Default.vs.cso");
		Shader ps("res/Render/Shaders/Default.ps.cso");

		Shader psTexture("res/Render/Shaders/TextureUnlit.ps.cso");
		Shader rootSigTexture("res/Render/Shaders/TextureUnlit.rs.cso");

		Shader domain("res/Render/Shaders/Lit.ds.cso");
		Shader hull("res/Render/Shaders/Lit.hs.cso");
		Shader psLight("res/Render/Shaders/Lit.ps.cso");
		Shader vsLight("res/Render/Shaders/Lit.vs.cso");
		Shader vsRootSig("res/Render/Shaders/Lit.rs.cso");

		Texture textureMetalAlbedo("res/Textures/metalR_albedo.jpg");
		Texture textureMetalNormal("res/Textures/metalR_normal.jpg");
		Texture textureMetalAmbient("res/Textures/metalR_ambient.jpg");
		Texture textureMetalRoughness("res/Textures/metalR_roughness.jpg");
		Texture textureMetalMetalness("res/Textures/metalR_metalness.jpg");
		Texture textureMetalDisplacement("res/Textures/metalR_displacement.jpg");

		Texture textureStoneAlbedo("res/Textures/stone2_albedo.jpg");
		Texture textureStoneNormal("res/Textures/stone2_normal.jpg");
		Texture textureStoneAmbient("res/Textures/stone2_ambient.jpg");
		Texture textureStoneRoughness("res/Textures/stone2_roughness.jpg");
		Texture textureStoneDisplacement("res/Textures/stone2_displacement.jpg");

		D12PipelineObject pso(vs, ps, rootSig);
		D12PipelineObject psoLit(vsLight, psLight, hull, domain, vsRootSig);

		RenderTransform camTransform({ 0.0f,0.0f,-10.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });

		RenderCamera cam(PERSPECTIVE);
		cam.perspective.fov = XM_PIDIV4;
		cam.perspective.aspectRatio = window.GetAspectRatio();
		cam.perspective.farPlane = 500.0f;
		cam.perspective.nearPlane = 0.001f;
		cam.SetMatrix(camTransform.m_matrix);
 		cam.UpdateVPMatrix();

		Vector<LightData> lights;

		LightManager::SetLightsProperties(8.0f, 100.0f, 2.0f, 32.0f,0.5f);

		// Lumi re directionnelle principale 
	

		LightData directionalLight = LightManager::CreateDirectionalLight(Vector3f32(0.0f, 0.0f, 1.0f), Vector4(1.0f, 1.0f, 1.0f, 1.0f), 1.0f, 1.0f);
	    LightManager::AddLight(directionalLight);

		LightData redPointLight = LightManager::CreatePointLight(
			Vector3f32(0.0f, 1.0f, 0.0f),  // Position
			Vector4(1.0f, 0.0f, 0.0f, 1.0f),  // Rouge
			8.0f,
			0.5f,
			0.8f
		);
		// LightManager::AddLight(redPointLight);

		Geometry* pCapsuleGeo = GeometryFactory::CreateCylinderGeo(0.5f, 0.5f, 1.0f, 40, 5);
		Geometry* pDonutGeo = GeometryFactory::CreateDonutGeo(1.0f, 0.5f, 40, 20);

		// Projecteur (spot light)
		LightData spotLight = LightManager::CreateSpotLight(
			Vector3f32(0.0f, 0.0f, -1.0f),
			Vector3f32(0.0f, 0.0f, 1.0f),
			Vector4(0.0f, 1.0f, 0.0f, 1.0f),
			30.0f,
			45.0f,
			2.0f,
			0.1f,
			0.5f);

		// LightManager::AddLight(spotLight);

		// Projecteur (spot light)
		LightData spotLight2 = LightManager::CreateSpotLight(
			Vector3f32(0.0f, 0.0f, -1.0f),
			Vector3f32(0.0f, 0.0f, 1.0f),
			Vector4(0.0f, 0.0f, 1.0f, 1.0f),
			30.0f,
			45.0f,
			2.0f,
			0.1f,
			0.5f);
		// LightManager::AddLight(spotLight2);

		RenderTransform capsuleTransform({ 3.0f, 0.0f, 1.0f }, { 35.0f, 0.0f, 0.0f }, { 1.5f, 1.5f,  1.5f });
		capsuleTransform.UpdateRenderTransformMatrix();

		RenderTransform donutTransform({ -2.0f, 0.0f, 1.0f }, { 25.0f, 45.0f, 45.0f }, { 1.5f, 1.5f,  1.5f });
		donutTransform.UpdateRenderTransformMatrix();

		RenderItem donutItem;
		donutItem.pGeometry = pDonutGeo;
		donutItem.pPso = &psoLit;
		donutItem.objData.world = donutTransform.GetWorldMatrix();

		donutItem.objData.indexCount = pDonutGeo->GetIndicesCount();

		donutItem.objData.material.albedoTextureID = textureMetalAlbedo.GetTextureID();
		donutItem.objData.material.ambientTextureID = textureMetalAmbient.GetTextureID();
		donutItem.objData.material.roughnessTextureID = textureMetalRoughness.GetTextureID();
		donutItem.objData.material.normalTextureID = textureMetalNormal.GetTextureID();
		donutItem.objData.material.displacementTextureID = textureMetalDisplacement.GetTextureID();
		donutItem.objData.material.fresnelR0 = Vector3f32(0.972f, 0.960f, 0.915f);
		donutItem.objData.material.metalnessTextureID = textureMetalMetalness.GetTextureID();

		donutItem.objData.material.subsurface = 0.2f;
		donutItem.objData.material.useTextureMetalness = 1;
		donutItem.objData.material.useTextureAlbedo = 1;
		donutItem.objData.material.useTextureAmbient = 1;
		donutItem.objData.material.useTextureNormal = 1;
		donutItem.objData.material.useTextureRoughness = 1;
		donutItem.objData.material.useDisplacementTexture = 1;

		RenderItem capsuleItem;
		capsuleItem.pGeometry = pCapsuleGeo;
		capsuleItem.pPso = &psoLit;
		capsuleItem.objData.world = capsuleTransform.GetWorldMatrix();
		capsuleItem.objData.indexCount = pCapsuleGeo->GetIndicesCount();

		capsuleItem.objData.material.albedoTextureID = textureMetalAlbedo.GetTextureID();

		capsuleItem.objData.material.ambientTextureID = textureMetalAmbient.GetTextureID();
		capsuleItem.objData.material.roughnessTextureID = textureMetalRoughness.GetTextureID();
		capsuleItem.objData.material.normalTextureID = textureMetalNormal.GetTextureID();
		capsuleItem.objData.material.displacementTextureID = textureMetalDisplacement.GetTextureID();
		capsuleItem.objData.material.metalnessTextureID = textureMetalMetalness.GetTextureID();

		capsuleItem.objData.material.subsurface = 0.2f;
		capsuleItem.objData.material.useTextureAlbedo = 1;
		capsuleItem.objData.material.useTextureMetalness = 1;
		capsuleItem.objData.material.useTextureAmbient = 1;
		capsuleItem.objData.material.useTextureNormal = 1;
		capsuleItem.objData.material.useTextureRoughness = 1;
		capsuleItem.objData.material.useDisplacementTexture = 1;

		frameMng.AddRenderItem(donutItem);
		frameMng.AddRenderItem(capsuleItem);

		window.OnWindowResizeCallBack([&](Window* w)->void
			{
				cam.perspective.aspectRatio = w->GetAspectRatio();
				cam.UpdateVPMatrix();
			});

		while (window.IsOpen())
		{

			frameMng.BeginFrame();

			//Update Buffers
			frameMng.UpdateObjCBuffer();

			LightManager::UpdateLightCB();

			window.Begin3D();
				window.SetCamera(cam);
				window.Draw(donutItem);
				window.Draw(capsuleItem);
			window.EndDraw();

			frameMng.EndFrame();

			window.Display();

			capsuleTransform.Rotate(0.008f, 0.0f, 0.0f);
			capsuleTransform.UpdateRenderTransformMatrix();
			capsuleItem.objData.world = capsuleTransform.GetWorldMatrix();

			donutTransform.Rotate(0.0f, 0.008f, 0.008f);
			donutTransform.UpdateRenderTransformMatrix();
			donutItem.objData.world = donutTransform.GetWorldMatrix();

			//capsuleItem.m_numFramesDirty = frameMng.GetNumFrameResources();
			//donutItem.m_numFramesDirty = frameMng.GetNumFrameResources();
			
			window.Update();
			
			if (GetAsyncKeyState('B'))
				window.SetFullScreen(FullScreenMode::BORDERLESS);
			if (GetAsyncKeyState('F'))
				window.SetFullScreen(FullScreenMode::EXCLUSIVE_FS);
			if (GetAsyncKeyState('W'))
				window.SetFullScreen(FullScreenMode::WINDOWED);
			if (GetAsyncKeyState('M'))
				window.SetFullScreen(FullScreenMode::WINDOWED_MAXIMISE);
		}

 		pCapsuleGeo->Release();
		pDonutGeo->Release();
	}
}