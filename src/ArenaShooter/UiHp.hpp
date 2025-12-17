#ifndef UIHP_HPP_INCLUDED
#define UIHP_HPP_INCLUDED

#include "define.h"
#include "Script.h"
#include "GameObject.h"
#include "Health.h"

DECLARE_SCRIPT(UiHp, ScriptFlag::Update)

Health<int>* m_pPlayer = nullptr;
ImageUI* UiHpImage = nullptr;

void Update() override
{

}

void UiImageHp()
{
	delete UiHpImage->btmBrush;

    switch (m_pPlayer->GetHealth())
    {
        case 5:
        {
            UiHpImage->btmBrush = new BitMapBrush("res/ArenaShooter/stade1.png");
            break;
		}
        case 4:
        {
            UiHpImage->btmBrush = new BitMapBrush("res/ArenaShooter/stade2.png");
            break;
        }
        case 3:
        {
            UiHpImage->btmBrush = new BitMapBrush("res/ArenaShooter/stade3.png");
            break;
        }
        case 2:
        {
            UiHpImage->btmBrush = new BitMapBrush("res/ArenaShooter/stade4.png");
            break;
        }
        case 1:
        {
            UiHpImage->btmBrush = new BitMapBrush("res/ArenaShooter/stadefinale.png");
            break;
        }
     
    default:
        break;
    }
}


END_SCRIPT
#endif // !UIHP_HPP_INCLUDED
