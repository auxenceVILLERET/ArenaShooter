// $(SolutionDir)..\src;$(SolutionDir)..\src\Tool;$(SolutionDir)..\src\Core;% (AdditionalIncludeDirectories);$(SolutionDir)..\src\Tool\Lib
#ifndef TEST_SPRITE_SHEET_HPP_INCLUDED
#define TEST_SPRITE_SHEET_HPP_INCLUDED

#include <Render.h>
#include <Tool/Image.h>
#include <Tool/SpriteSheet.h>
#include <stdio.h>
#include "define.h"
using namespace gce;


BEGIN_TEST( SpriteSheet )

    //Besoin de récupérer les images 
    const char* pFiles[] = { RES_PATH"res/Tool/pink.jpg", RES_PATH"res/Tool/purple.jpg" };
    const int fileCount = sizeof(pFiles) / sizeof(pFiles[0]);

    SpriteSheet spriteSheet;

    for (int i = 0; i < fileCount; ++i)
    {
        const char* pFilename = pFiles[i];
        gce::Image* pImage = nullptr;

        if (Image::HasExtension(pFilename, ".bmp"))
            pImage = new ImageBMP();
        else if (Image::HasExtension(pFilename, ".jpg") || Image::HasExtension(pFilename, ".png"))
            pImage = new ImageSTB();
        else
        {
            MessageBoxA(nullptr, "Unsupported file format", "Error", MB_OK);
            continue;
        }

        if (pImage->Load(pFilename))
        {
            spriteSheet.AddImage(pImage, pFilename);
        }
        else
        {
            char msg[256];
            sprintf_s(msg, "Failed to load image: %s", pFilename);
            MessageBoxA(nullptr, msg, "Error", MB_OK);
            delete pImage;
        }
    }

    // Génération et sauvegarde
    if (spriteSheet.GenerateSpriteSheet(SpriteSheet::Orientation::Horizontal, RES_PATH"res/Tool/GeneratedAtlas"))
        MessageBoxA(nullptr, "Sprite sheet generated and saved successfully!", "Success", MB_OK);
    else
        MessageBoxA(nullptr, "Failed to generate sprite sheet", "Error", MB_OK);

END_TEST_MAIN_FUNCTION( SpriteSheet )
END_TEST( SpriteSheet )


#endif