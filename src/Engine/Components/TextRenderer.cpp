#include "pch.h"
#include "TextRenderer.h"
#include <Font.h>
#include <Structs.h>
#include <FrameResources.h>

namespace gce {
	void TextRenderer::Init()
	{
		pFont = new Font(L"Arial");
		pBrush = new ColorBrush(Color::White);
		text = L"Hello, World!";
		rectPosF = new RectanglePosF(0.0f, 0.0f, 200.0f, 50.0f);
	}

} // namespace gce