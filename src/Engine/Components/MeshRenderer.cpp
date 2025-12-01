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
}


void MeshRenderer::UpdateWorld( uint8 const dirty )
{
    objData.world = m_pOwner->transform.GetWorldMatrix();
    objData.world.Transpose();
}


}
