#include "pch.h"

#include "Tests.h"

#include "Maths.h"

using namespace gce;
using namespace DirectX;

struct ObjectData
{
	gce::Matrix world;
};

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	Console::Init();
	//TestFrameResources();
	TestSkyBox();
	//TestSplitScreen();
	//Test2D();
	return 0;
}