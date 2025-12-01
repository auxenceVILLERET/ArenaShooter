#include "pch.h"
#include "Tests.h"

#include "RenderCamera.h"
#include "D12PipelineObject.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "RenderWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTransform.h"



namespace gce
{
	void TestLight()
	{
		RenderContext::Init();
		RenderContext::SetMSAA(MSAA_COUNT_X4);

		RenderWindow window;
		window.Create(L"Window 1", 900, 700);

		// Light Shaders
		Shader domain("res/Render/Shaders/Lit.ds.cso");
		Shader hull("res/Render/Shaders/Lit.hs.cso");
		Shader psLight("res/Render/Shaders/Lit.ps.cso");
		Shader vsLight("res/Render/Shaders/Lit.vs.cso");
		Shader LitRS("res/Render/Shaders/Lit.rs.cso");

		// Textures Maps
		Texture textureMetalAlbedo("res/Textures/metal_color.jpg");
		Texture textureMetalNormal("res/Textures/metal_normal.jpg");
		Texture textureMetalAmbient("res/Textures/metal_ambient.jpg");
		Texture textureMetalRoughness("res/Textures/metal_roughness.jpg");
		Texture textureMetalMetalness("res/Textures/metal_metalness.jpg");
		Texture textureMetalDisplacement("res/Textures/metal_displacement.jpg");

		Texture textureStoneAlbedo("res/Textures/stone_color.jpg");
		Texture textureStoneNormal("res/Textures/stone_normal.jpg");
		Texture textureStoneAmbient("res/Textures/stone_ambient.jpg");
		Texture textureStoneRoughness("res/Textures/stone_roughness.jpg");
		Texture textureStoneDisplacement("res/Textures/stone_displacement.jpg");

		Texture textureMetalRAlbedo("res/Textures/metalR_albedo.jpg");
		Texture textureMetalRNormal("res/Textures/metalR_normal.jpg");
		Texture textureMetalRAmbient("res/Textures/metalR_ambient.jpg");
		Texture textureMetalRRoughness("res/Textures/metalR_roughness.jpg");
		Texture textureMetalRMetalness("res/Textures/metalR_metalness.jpg");
		Texture textureMetalRDisplacement("res/Textures/metalR_displacement.jpg");


		Texture textureStone2Albedo("res/Textures/stone2_albedo.jpg");
		Texture textureStone2Normal("res/Textures/stone2_normal.jpg");
		Texture textureStone2Ambient("res/Textures/stone2_ambient.jpg");
		Texture textureStone2Roughness("res/Textures/stone2_roughness.jpg");
		Texture textureStone2Displacement("res/Textures/stone2_displacement.jpg");
		Texture textureStone2Metalness("res/Textures/plastic_metallic.png");


		D12PipelineObject psoLight(vsLight, psLight,hull,domain, LitRS);


		RenderCamera cam(PERSPECTIVE);
		cam.perspective.fov = XM_PIDIV4;
		cam.perspective.aspectRatio = window.GetAspectRatio();
		cam.perspective.farPlane = 500.0f;
		cam.perspective.nearPlane = 0.001f;
		cam.perspective.up = { 0.0f, 1.0f, 0.0f };

		RenderTransform camTransform = RenderTransform({ 0.0f,0.0f,-10.0f }, { 0.0f,0.0f,0.0f }, { 1.0f,1.0f,1.0f });
		cam.SetMatrix(camTransform.m_matrix);
		cam.UpdateVPMatrix();


		Geometry* pCubeGeo = GeometryFactory::CreateCubeGeo();
		RenderTransform cubeTransform({ 0.0f, -1.5f, 0.0f }, { 90.0f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });
		cubeTransform.UpdateRenderTransformMatrix();

		Geometry* pCubeGeo2 = GeometryFactory::CreateCubeGeo();
		RenderTransform cubeTransform2({ 0.0f, 0.0f, 0.0f }, { 0.0f, 90.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });
		cubeTransform2.UpdateRenderTransformMatrix();


		auto test = sizeof(ObjectData);
		DynamicBuffer objectBuffer(sizeof(ObjectData));
		ObjectData metaldata = {};

		metaldata.material.fresnelR0 = Vector3f32(0.972f, 0.960f, 0.915f);

		metaldata.material.albedoTextureID = textureMetalRAlbedo.GetTextureID();

		metaldata.material.ambientTextureID = textureMetalRAmbient.GetTextureID();

		metaldata.material.roughnessTextureID = textureMetalRRoughness.GetTextureID();

		metaldata.material.normalTextureID = textureMetalRNormal.GetTextureID();

		metaldata.material.displacementTextureID = textureMetalRDisplacement.GetTextureID();

		metaldata.material.metalnessTextureID = textureMetalRMetalness.GetTextureID();


		metaldata.material.subsurface = 0.2f;
		metaldata.material.useTextureMetalness = 1;
		metaldata.material.useTextureAlbedo = 1;
		metaldata.material.useTextureAmbient = 1;
		metaldata.material.useTextureNormal = 1;
		metaldata.material.useTextureRoughness = 1;
		metaldata.material.useDisplacementTexture = 1;


		metaldata.world = cubeTransform2.GetWorldMatrix();
		objectBuffer.CopyData(metaldata);

		window.OnWindowResizeCallBack([&](Window* w)->void
			{
				cam.perspective.aspectRatio = w->GetAspectRatio();
			});

		RenderTarget* RT = window.GetRenderTarget();

		//RT->SetLight(
		//	Vector3f32(1.0f, 1.0f, 1.0f),
		//	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		//	Vector3f32(0.0f, 1.0f, 0.0f),
		//	Vector3f32(3.0f, 1.0f, 3.0f),
		//	1000.f,
		//	5.f,
		//	5.f,
		//	10.f,
		//	Vector3f32(0.3f, 0.4f, 0.5f),
		//	5.0f);

		while (window.IsOpen())
		{
			window.Update();

			window.Begin3D();

			window.SetCamera(cam);
			//window.Draw(*pCubeGeo, psoLight, objectBuffer);

			window.EndDraw();

			window.Display();

			//Scene Update
			cubeTransform2.Rotate(0.0008f, 0.0008f, 0.0f);
			cubeTransform2.UpdateRenderTransformMatrix();
			metaldata.world = cubeTransform2.GetWorldMatrix();
			objectBuffer.CopyData(metaldata);

			cubeTransform2.Rotate(0.00058f, 0.00f, 0.0f);
			cubeTransform2.UpdateRenderTransformMatrix();

		}

		pCubeGeo->Release();

	}
}