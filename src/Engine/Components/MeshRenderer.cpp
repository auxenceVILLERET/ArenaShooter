#include "pch.h"
#include "MeshRenderer.h"

#include <Geometry.h>
#include <FrameResources.h>

#include "GameObject.h"

namespace gce {


void MeshRenderer::Init()
{
    m_pOwner->transform.dirtyEvent.AddListener( &MeshRenderer::UpdateWorld, this );
    FrameResourcesManager::AddRenderItem( *this );
    pPso = GameManager::GetDefaultPso();
    pMaterial->subsurface = 0;
    pMaterial->useDisplacementTexture = 0;
    pMaterial->useTextureAlbedo = 0;
    pMaterial->useTextureAmbient = 0;
    pMaterial->useTextureMetalness = 0;
    pMaterial->useTextureNormal = 0;
    pMaterial->useTextureRoughness = 0;
}


void MeshRenderer::UpdateWorld( uint8 const dirty )
{
    objData.world = m_pOwner->transform.GetWorldMatrix();
    objData.world.Transpose();
}


}
