#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include "Engine.h"
#include <fstream>
#include <wincodec.h>

#include "Core/nlohmann.hpp"
#include "Core/Maths/Quaternion.h"

#include "SceneManager.h"
#include "CustomScene.h"

using json = nlohmann::json;
using namespace gce;

struct Spawn
{
    Vector3f32 startPos;
    Vector3f32 endPos;
};

struct MapProperties
{
    Vector3f32 pos;
    Vector3f32 size;
    Vector<Spawn> vSpawns;
};

struct MapLoader
{
    static MapProperties LoadMap(String const& path, CustomScene* menu, D12PipelineObject* pso)
    {
        if (menu == nullptr) return {};

        MapProperties mapProperties;
        
        Map<GameObject*, String> objects;
        
        std::ifstream file(path);
        if (!file.is_open()) {
            std::cerr << "Impossible d'ouvrir le fichier JSON\n";
        }
        json j;
        try {
            file >> j;
        }
        catch (json::parse_error& e) {
            std::cerr << "Erreur de parsing: " << e.what() << "\n";
        }

        json jObjects = j["objects"];
        for (int i = 0; i < jObjects.size(); ++i)
        {
            json currObject = jObjects[i];
            std::string name = currObject["name"].get<std::string>();
            std::string type = currObject["type"].get<std::string>();

            if (name == "Container")
            {
                Vector3f32 position;
                position.x = currObject["position"][0].get<float>();
                position.y = currObject["position"][2].get<float>();
                position.z = currObject["position"][1].get<float>();
                
                Vector3f32 scale;
                scale.x = currObject["scale"][0].get<float>();
                scale.y = currObject["scale"][2].get<float>();
                scale.z = currObject["scale"][1].get<float>();

                mapProperties.pos = position;
                mapProperties.size = scale * 2.0f;
                continue;
            }

            if ( type == "ARROW" )
            {
                Vector3f32 start;
                start.x = currObject["start"][0].get<float>();
                start.y = currObject["start"][2].get<float>();
                start.z = currObject["start"][1].get<float>();

                Vector3f32 end;
                end.x = currObject["end"][0].get<float>();
                end.y = currObject["end"][2].get<float>();
                end.z = currObject["end"][1].get<float>();

                Spawn spawn;
                spawn.startPos = start;
                spawn.endPos = end;

                mapProperties.vSpawns.PushBack(spawn);
                continue;
            }
            
            GameObject& gameObject = menu->AddObject();
            MeshRenderer& mesh = *gameObject.AddComponent<MeshRenderer>();
            mesh.pGeometry = GeometryFactory::LoadJsonGeometry(currObject);
            mesh.pPso = pso;
            gameObject.SetName(name);
            
            if (currObject["has_collider"].get<bool>() == true)
            {
                // gameObject.AddComponent<PhysicComponent>()->SetGravityScale(0.0f);
                gameObject.AddComponent<BoxCollider>();
                // if (currObject["destructible"].get<bool>() == true)
                // {
                //     Health* h = gameObject.AddScript<Health>();
                //     h->maxHealth = 1.0f;
                // }
            }
            
            if (currObject.contains("texture") && currObject["texture"].is_string())
            {
                std::string texturePath = RES_PATH;
                texturePath.append("res/Textures/");
                texturePath.append(currObject["texture"].get<std::string>());
                Texture* texture = new Texture(texturePath);
                mesh.pMaterial->albedoTextureID = texture->GetTextureID();
                mesh.pMaterial->useTextureAlbedo = 1;
                mesh.pMaterial->subsurface = 0.2f;
            }
            
            // Pos / Scale / Rot
            {
                Vector3f32 position;
                position.x = currObject["position"][0].get<float>();
                position.y = currObject["position"][2].get<float>();
                position.z = currObject["position"][1].get<float>();
                gameObject.transform.SetLocalPosition(position);

                Vector3f32 scale;
                scale.x = currObject["scale"][0].get<float>();
                scale.y = currObject["scale"][2].get<float>();
                scale.z = currObject["scale"][1].get<float>();
                gameObject.transform.SetLocalScale(scale);
            
                float32 rotX = currObject["rotation"][0].get<float>();
                float32 rotY = currObject["rotation"][1].get<float>();
                float32 rotZ = currObject["rotation"][2].get<float>();
                float32 rotW = currObject["rotation"][3].get<float>();
                Quaternion rotation(rotX, rotY, rotZ, rotW);

                Matrix m_engineRot = Matrix::Rotation(rotation);
                Matrix B;
                B.SetValue(0, 0, 1.0f);
                B.SetValue(1, 2, 1.0f);
                B.SetValue(2, 1, 1.0f);
                B.SetValue(1, 1, 0.0f);
                B.SetValue(2, 2, 0.0f);

                Matrix m_blenderRot = B * m_engineRot * B;
                Quaternion final = Quaternion::RotationMatrix(m_blenderRot);
                
                gameObject.transform.SetWorldRotation(final);
            }

            if (currObject.contains("parent") && currObject["parent"].is_string())
            {
                std::pair<GameObject*, String> el;
                el.first = &gameObject;
                el.second = currObject["parent"].get<std::string>();
                objects.insert(el);
            }
        }

        for (std::pair<GameObject*, String> el : objects)
        {
            for (std::pair<GameObject*, String> el2 : objects)
            {
                if (el.first == el2.first) continue;
                if (el.second == el2.first->GetName())
                    el.first->SetParent(*el2.first);
            }
        }

        return mapProperties;
    }
};

#endif