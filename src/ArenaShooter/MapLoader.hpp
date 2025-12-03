#ifndef MAP_LOADER_HPP_INCLUDED
#define MAP_LOADER_HPP_INCLUDED

#include "Engine.h"
#include <fstream>
#include "Core/nlohmann.hpp"
#include "Core/Maths/Quaternion.h"

using json = nlohmann::json;
using namespace gce;

struct MapLoader
{
    static void LoadMap(String const& path, Scene* pScene, D12PipelineObject* pso)
    {
        if (pScene == nullptr) return;

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
            GameObject& gameObject = GameObject::Create(*pScene);
            MeshRenderer& mesh = *gameObject.AddComponent<MeshRenderer>();
            mesh.pGeometry = GeometryFactory::LoadJsonGeometry(currObject);
            mesh.pPso = pso;
            gameObject.AddComponent<BoxCollider>();
            gameObject.SetName(currObject["name"].get<String>().c_str());
            
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
                float32 rotY = currObject["rotation"][2].get<float>();
                float32 rotZ = currObject["rotation"][1].get<float>();
                float32 rotW = currObject["rotation"][3].get<float>();
                Quaternion rotation(rotX, rotY, rotZ, rotW);
                Quaternion convert = Quaternion::RotationEuler(Vector3f32(90.0f, 0.0f, -90.0f));
                gameObject.transform.SetWorldRotation(rotation * convert);
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
    }
};

#endif