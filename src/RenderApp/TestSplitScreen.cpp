#include "pch.h"

#include "RenderCamera.h"
#include "D12PipelineObject.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "RenderWindow.h"
#include "SplitScreenWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTransform.h"

namespace gce
{
	void TestSplitScreen()
	{
		// Init Render
		RenderContext::Init();
		RenderContext::SetMSAA(MSAA_COUNT_X8);

		//Create Split Screen Window and choose preset disposition
		SplitScreenWindow window;
		window.Create(L"Window 1", 900, 700,SQUARE_4_PLAYERS);

		// Create pso for object in scene
		Shader vs("res/Render/Shaders/Lit.vs.cso");
		Shader ps("res/Render/Shaders/Lit.ps.cso");
		Shader rs("res/Render/Shaders/Lit.rs.cso");
		Shader ds("res/Render/Shaders/Lit.ds.cso");
		Shader hs("res/Render/Shaders/Lit.hs.cso");

		D12PipelineObject pso(vs, ps,hs,ds, rs);

		// Load Texture
		Texture texture("res/Textures/test1.png");
		Texture texture2("res/Textures/tuile_albedo.jpg");

		// Create cameras
		RenderCamera cam(PERSPECTIVE);
		cam.perspective.fov = XM_PIDIV4;
		cam.perspective.aspectRatio = window.GetAspectRatio();
		cam.perspective.farPlane = 500.0f;
		cam.perspective.nearPlane = 0.001f;
		cam.SetMatrix(RenderTransform({ 0.0f, 0.0f, -10.0 }, { 0.0f, 0.0f, 0.0f }, { 1.f, 1.0f, 1.0 }).m_matrix);
		cam.UpdateVPMatrix();

		RenderCamera cam1(PERSPECTIVE);
		cam1.perspective.fov = XM_PIDIV4;
		cam1.perspective.aspectRatio = window.GetAspectRatio();
		cam1.perspective.farPlane = 500.0f;
		cam1.perspective.nearPlane = 0.001f;
		cam1.SetMatrix(RenderTransform({ 0.0f, 0.0f, -10.0 }, { 0.0f, 0.0f, 0.0f }, { 1.f, 1.0f, 1.0 }).m_matrix);
		cam1.UpdateVPMatrix();

		RenderCamera cam2(PERSPECTIVE);
		cam2.perspective.fov = XM_PIDIV4;
		cam2.perspective.aspectRatio = window.GetAspectRatio();
		cam2.perspective.aspectRatio = window.GetAspectRatio();
		cam2.perspective.farPlane = 500.0f;
		cam2.perspective.nearPlane = 0.001f;
		cam2.SetMatrix(RenderTransform({ 0.0f, 0.0f, -10.0 }, { 0.0f, 0.0f, 0.0f }, { 1.f, 1.0f, 1.0 }).m_matrix);
		cam2.UpdateVPMatrix();

		RenderCamera cam3(PERSPECTIVE);
		cam3.perspective.fov = XM_PIDIV4;
		cam3.perspective.aspectRatio = window.GetAspectRatio();
		cam3.perspective.farPlane = 500.0f;
		cam3.perspective.nearPlane = 0.001f;
		cam3.SetMatrix(RenderTransform({ 0.0f, 0.0f, -10.0 }, { 0.0f, 0.0f, 0.0f }, { 1.f, 1.0f, 1.0 }).m_matrix);
		cam3.UpdateVPMatrix();

		// Create object in scene
		Geometry* pCylinderGeo = GeometryFactory::CreateCylinderGeo(0.5f, 0.5f, 1.0f, 40, 5);
		Geometry* pCubeGeo = GeometryFactory::CreateCubeGeo();
		Geometry* pDonutGeo = GeometryFactory::CreateDonutGeo(1.0f,0.5f,40,20);
		Geometry* pSphereGeo = GeometryFactory::CreateSphereGeo(0.5f,20,20);

		RenderTransform cylinderTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f });
		cylinderTransform.UpdateRenderTransformMatrix();

		RenderTransform cubeTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 45.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });
		cubeTransform.UpdateRenderTransformMatrix();

		RenderTransform donutTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 20.0f, 20.0f }, { 1.0f, 1.0f, 1.0f });
		donutTransform.UpdateRenderTransformMatrix();

		RenderTransform sphereTransform({ 0.0f, 0.0f, 0.0f }, { 0.0f, 45.0f, 0.0f }, { 2.0f, 2.0f, 2.0f });
		sphereTransform.UpdateRenderTransformMatrix();

		window.OnWindowResizeCallBack([&](Window* w)->void
			{
				cam.perspective.aspectRatio = w->GetAspectRatio();
			});

		cam.UpdateVPMatrix();

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

		DynamicBuffer bufferDonut{ sizeof(ObjectData) };

		ObjectData dataDonut = {};
		dataDonut.world = donutTransform.GetWorldMatrix();
		dataDonut.material = {};

		bufferDonut.CopyData(dataDonut);

		DynamicBuffer bufferSphere{ sizeof(ObjectData) };

		ObjectData dataSphere = {};
		dataSphere.world = sphereTransform.GetWorldMatrix();
		dataSphere.material = {};

		bufferSphere.CopyData(dataSphere);

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
			//Scene Update

			// Draw Scene
			window.Begin3D();

			//Choose Camera to draw with
			window.SetCamera(cam);

			// Choose Screen to draw on
			window.SetScreenToDraw(0);
			//window.Draw(rI1);

			window.SetScreenToDraw(1);
			//window.Draw(rI2);

			window.SetScreenToDraw(2);
			//window.Draw(rI3);

			window.SetScreenToDraw(3);
			//window.Draw(rI4);

			window.EndDraw();

			window.Display();

			window.Update();
		}

		pCylinderGeo->Release();
		pCubeGeo->Release();
		pDonutGeo->Release();
		pSphereGeo->Release();
	}
}
