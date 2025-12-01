#include "pch.h"
#include "Tests.h"

#include "RenderCamera.h"
#include "D12PipelineObject.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "RenderWindow.h"
#include "SplitScreenWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "CubeMap.h"
#include "RenderTransform.h"
#include "FrameResources.h"


namespace gce
{
	void TestSkyBox()
	{
		// Init Render
		RenderContext::Init();
		RenderContext::SetMSAA(MSAA_COUNT_X8);

		//Create Window 
		RenderWindow window;
		window.Create(L"Window 1", 900, 700);

		// Create pso for object in scene
		Shader vs("res/Render/Shaders/Lit.vs.cso");
		Shader ps("res/Render/Shaders/Lit.ps.cso");
		Shader rs("res/Render/Shaders/Lit.rs.cso");
		Shader ds("res/Render/Shaders/Lit.ds.cso");
		Shader hs("res/Render/Shaders/Lit.hs.cso");

		D12PipelineObject pso(vs, ps,hs,ds, rs);

		// Load Texture
		Texture texture("res/Textures/test1.png");

		// Create camera
		RenderTransform camTransform({0.0f,0.0f,-200.0f},{0.0f,0.0f,0.0f},{1.0f,1.0f,1.0f});

		RenderCamera cam(PERSPECTIVE);
		cam.perspective.fov = XM_PIDIV4;
		cam.perspective.aspectRatio = window.GetAspectRatio();
		cam.perspective.farPlane = 500.0f;
		cam.perspective.nearPlane = 0.001f;
		cam.perspective.up = { 0.0f, 1.0f, 0.0f };
		cam.SetMatrix(camTransform.m_matrix);
		cam.UpdateVPMatrix();

		CubeMap skyBox;
		// Choose a .dds file with 6 frame
		skyBox.Create("res/Textures/GrassCube.dds", {40.0f,40.0f,40.0f});

		// Create object in scene
		Geometry* pCylinderGeo = GeometryFactory::CreateCylinderGeo(0.5f, 0.5f, 1.0f, 40, 5);
		Geometry* pCubeGeo = GeometryFactory::CreateCubeGeo();

		RenderTransform cylinderTransform({ 0.0f, 0.0f, -10.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		cylinderTransform.UpdateRenderTransformMatrix();
		RenderTransform cubeTransform({ -5.0f, 0.0f, 0.0f }, { 0.0f, 45.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });

		window.OnWindowResizeCallBack([&](Window* w)->void
			{
				cam.perspective.aspectRatio = w->GetAspectRatio();
				cam.UpdateVPMatrix();
			});

		// create buffer for objects
		DynamicBuffer bufferCylinder{ sizeof(ObjectData) };

		ObjectData dataCylinder = {};
		dataCylinder.world = cylinderTransform.GetWorldMatrix();
		dataCylinder.material = {};

		bufferCylinder.CopyData(dataCylinder);

		DynamicBuffer bufferCube{ sizeof(ObjectData) };

		ObjectData dataCube = {};
		dataCube.world = cubeTransform.GetWorldMatrix();
		dataCube.material = {};

		bufferCube.CopyData(dataCube);

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

			// Draw Scene
			FrameResourcesManager::BeginFrame();

			window.Begin3D();
				window.SetCamera(cam);

				// Create skybox with its own pso and texture
				RT->DrawSkyBox(skyBox);

				//window.Draw(*pCylinderGeo, pso, bufferCylinder);
				//window.Draw(*pCubeGeo, pso, bufferCube, &texture);
			
			window.EndDraw();
			FrameResourcesManager::EndFrame();

			window.Display();

			//Scene Update
			camTransform.Rotate(0.00f, -0.01f, 0.00f);
			camTransform.UpdateRenderTransformMatrix();
			cam.SetMatrix(camTransform.m_matrix);

			window.Update();
		}

		pCylinderGeo->Release();
		pCubeGeo->Release();
	}
}
