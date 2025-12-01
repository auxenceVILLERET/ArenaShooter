#include "pch.h"
#include "Tests.h"

#include "RenderCamera.h"
#include "D12PipelineObject.h"
#include "Font.h"
#include "Geometry.h"
#include "GeometryFactory.h"
#include "RenderContext.h"
#include "RenderWindow.h"
#include "Shader.h"
#include "Texture.h"
#include "RenderTransform.h"

namespace gce
{
	void Test2D()
	{
		//Init the render context
		RenderContext::Init();
		//Init the UI module (Direct2D)
		RenderContext::InitUI();
		//Enable MSAA antialiasing x4
		RenderContext::SetMSAA(MSAA_COUNT_X4);

		RenderWindow window;
		window.Create(L"Window 1", 900, 700);

		//Load vs, ps and rs
		Shader vs("res/Render/Shaders/Lit.vs.cso");
		Shader psTexture("res/Render/Shaders/TextureUnlit.ps.cso");
		Shader rsTexture("res/Render/Shaders/TextureUnlit.rs.cso");

		Shader ds("res/Render/Shaders/Lit.ds.cso");
		Shader hs("res/Render/Shaders/Lit.hs.cso");

		D12PipelineObject pso(vs, psTexture,hs,ds, rsTexture);

		//Load Textures
		Texture texture1("res/Textures/bricks.jpg");
		Texture texture2("res/Textures/test-pixel-art.gif");
		Texture texture3("res/Textures/metal.jpg");

		Font font(L"Colibri");

		//Create an orthographic camera to display the 2D scene
		RenderCamera cam2d(ORTHOGRAPHIC);
		cam2d.orthographic.viewWidth = static_cast<float32>(window.GetWidth());
		cam2d.orthographic.viewHeight = static_cast<float32>(window.GetHeight());
		cam2d.orthographic.farPlane = 500.0f;
		cam2d.orthographic.nearPlane = 0.01f;
		cam2d.SetMatrix(RenderTransform({ 0.0f, 0.0f, -10.0 }, { 0.0f, 0.0f, 0.0f }, { 1.f, 1.0f, 1.0 }).m_matrix);

		//Create Geometry & transform
		Geometry* spriteGeo = GeometryFactory::Create2DPlane();
		RenderTransform transform({ 100.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f }, { 200.0f, 200.0f, 0.0f });

		//======
		ObjectData objectData = {};
		objectData.world = transform.GetWorldMatrix();

		//Create a dynamic buffer to store sprite data
		DynamicBuffer objectBuffer(sizeof(ObjectData));
		objectBuffer.CopyData(objectData);
		//======

		//Resize camera when window is resized
		window.OnWindowResizeCallBack([&](Window* w)->void
			{
				cam2d.orthographic.viewWidth = static_cast<float32>(w->GetWidth());
				cam2d.orthographic.viewHeight = static_cast<float32>(w->GetHeight());
				cam2d.UpdateVPMatrix();

			});

		RenderTarget* RT = window.GetRenderTarget();
		//LightData light;

		//RT->AddLight(light);
		//RT->SetLight(
		//Vector3f32(1.0f, 1.0f, 1.0f),
		//DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
		//Vector3f32(0.0f, 1.0f, 0.0f),
		//Vector3f32(3.0f, 1.0f, 3.0f),
		//1000.f,
		//5.f,
		//5.f,
		//10.f,
		//Vector3f32(0.3f,0.4f,0.5f),
		//5.0f);

		Color colors[2];
		colors[0] = Color::White;
		colors[1] = Color::Blue;

		Color colorsLi[4];
		colorsLi[0] = Color::Magenta;
		colorsLi[1] = Color::Green;
		colorsLi[2] = Color::Cyan;
		colorsLi[3] = Color::Blue;

		ColorBrush color(Color::Yellow,01.0f);

		LinearGradientBrush linearBrush(RectanglePosF(450.0f, 0.0f, 800.0f, 50.0f),2,colorsLi,0.5f);

		RadialGradientBrush RadialBrush({ 500.0f,500.0f },{-20.0f,20.0f},100.0f,50.0f,2,colors,1.0f);

		RadialGradientBrush RadialBrush2({ 100.0f,200.0f }, { 0.0f,-100.0f },50.0f,100.0f,2,colors,1.0f);

		// BitMapBrush bitMapBrush("res/Textures/test1.png",1.0f);

		Ellipse ellipse({ 250.0f,250.0f }, 100.0f,50.0f);
		RoundRectanglePosF rect2(450.0f, 0.0f, 800.0f, 50.0f,20.0f,50.0f);
		Line line({ 10.0f,10.0f },{100.0f,50.0f},20.0f);

		RectanglePosF rectBitMap(0.0f, 0.0f, 256.0f, 256.0f);

		while (window.IsOpen())
		{


			window.Begin3D();
				window.SetCamera(cam2d);
				//window.Draw(*spriteGeo, pso, objectBuffer, &texture1);
			window.EndDraw();

			window.BeginUI();

				window.SetActiveBrush(RadialBrush);
				window.DrawShapeUI(ellipse,true);

				window.SetActiveBrush(RadialBrush2);
				window.DrawTextUI(L"Brr Brr Patapim", font, RectanglePosF(5.0f, 110.0f, 200.0f, 300.0f));

				window.SetActiveBrush(linearBrush);
				window.DrawShapeUI(rect2,true);

				window.SetActiveBrush(color);
				window.DrawShapeUI(line,true);
				
				// window.SetActiveBrush(bitMapBrush);
				window.DrawShapeUI(rectBitMap, true);

			window.EndUI();

			window.Display();
			window.Update();
		}

		spriteGeo->Release();
	}
}