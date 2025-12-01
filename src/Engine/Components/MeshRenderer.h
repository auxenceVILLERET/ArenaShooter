#ifndef ENGINE_MESH_RENDERER_H_INCLUDED
#define ENGINE_MESH_RENDERER_H_INCLUDED

#include "define.h"
#include <Structs.h>
#include "Base.h"

namespace gce {

class Geometry;
class D12PipelineObject;
class Texture;

DECLARE_COMPONENT( MeshRenderer, ComponentBase ), protected RenderItem
{
    using RenderItem::pGeometry;
    using RenderItem::pPso;
    Material* pMaterial = &objData.material;

protected:
    void Init() override;

    void UpdateWorld( uint8 dirty );

    friend class RenderSystem;
};


}

#endif