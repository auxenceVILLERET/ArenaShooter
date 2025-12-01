#include "TestEvent.h"

using namespace gce;
void RunEventTest()
{
    //@brief
    // Fonction de test pour vérifier le bon fonctionnement du système d'événements.
    // Elle crée deux GameObjects, enregistre un écouteur sur un événement,
    // envoie cet événement, puis utilise toutes les fonctions du EventManager
    // pour illustrer leur usage.

    // Initialisation du moteur
    GameManager::Create();
    Scene& scene = Scene::Create();
    EventManager& evtMgr = GameManager::s_pInstance->m_eventManager;

    // Ressources graphiques
    Geometry* const pCube = GeometryFactory::CreateCubeGeo();
    D12PipelineObject* const pDefaultPSO = new D12PipelineObject(
        GameManager::s_pInstance->m_vertexShader,
        GameManager::s_pInstance->m_pixelShader,
        GameManager::s_pInstance->m_hullShader,
        GameManager::s_pInstance->m_domainShader,
        GameManager::s_pInstance->m_rootSignature
    );

    // Création d'un écouteur (listener) pour l'événement de rotation
    RotationListener* listener = new RotationListener();

    // Enregistrement du listener pour recevoir les événements OnRotateEvent
    evtMgr.Register<RotationListener, const OnRotateEvent&>(listener, &RotationListener::OnRotate, /*priority*/ 10);

    // Vérification du nombre d'écouteurs enregistrés (test de GetEventCount)
    int handlerCount = evtMgr.GetEventCount<const OnRotateEvent&>();
	OutputDebugStringA(("Handler count for OnRotateEvent: " + std::to_string(handlerCount) + "\n").c_str());
    
    GameObject& third = GameObject::Create(scene, [](GameObject* pGameObject) {
        });
    MeshRenderer& meshRenderer3 = third.AddComponent<MeshRenderer>();
    meshRenderer3.pGeometry = pCube;
    meshRenderer3.pPSO = pDefaultPSO;

    GameObject& second = GameObject::Create(scene, [](GameObject* pGameObject) {
        pGameObject->transform.GlobalRotate(0.0f, 0.8f * GameManager::DeltaTime(), 0.0f);

        // Création et émission de l'événement
        OnRotateEvent evt{ pGameObject, GameManager::DeltaTime() };
        GameManager::s_pInstance->m_eventManager.CallEvent<const OnRotateEvent&>(evt);
        });
    second.transform.SetGlobalPosition({ 2, 0, 0 });

    MeshRenderer& meshRenderer2 = second.AddComponent<MeshRenderer>();
    meshRenderer2.pGeometry = pCube;
    meshRenderer2.pPSO = pDefaultPSO;

    // Pour tester Unregister (suppression d'un seul handler)
    // evtMgr.Unregister<RotationListener, const OnRotateEvent&>(listener, &RotationListener::OnRotate);

    // Pour tester RemoveAll (supprime tous les handlers pour ce listener spécifique)
    // evtMgr.RemoveAll<RotationListener, const OnRotateEvent&>(listener);
    WindowParam windowParam;
    GameManager::Run(windowParam);

    evtMgr.RemoveAll<RotationListener, const OnRotateEvent&>(listener); // Optionnel ici si déjà fait
    evtMgr.ClearAll(); // Supprime tous les événements (vide la map)

    delete listener;
    delete pCube;
    delete pDefaultPSO;

    GameManager::Destroy();
}
