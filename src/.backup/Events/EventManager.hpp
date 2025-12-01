#ifndef ENGINE_EVENT_MANAGER_H_INCLUDED
#define ENGINE_EVENT_MANAGER_H_INCLUDED

#include <algorithm>
#include "Event.hpp"

namespace gce {
    /* @brief EventManager est un gestionnaire centralisé pour tous les événements du moteur.
       Il permet d'enregistrer, de désenregistrer, de diffuser (call) et de supprimer des événements génériques,
       avec support des arguments typés.  */
    class EventManager {
public:
    /* @brief Récupère une référence vers un événement spécifique basé sur son type.
        Si l'événement n'existe pas encore, il est créé dynamiquement.
        @tparam Args Types des arguments de l'événement.
        @return Référence vers l'instance d'Event correspondante. */
    template<typename... Args>
    Event<Args...>& GetEvent() {
        auto typeId = std::type_index(typeid(Event<Args...>));
        
        // Si aucun event de ce type n'existe encore, crée un nouveau.
        if (m_events.find(typeId) == m_events.end()) {
            m_events[typeId] = std::make_unique<Event<Args...>>();
        }
        return *static_cast<Event<Args...>*>(m_events[typeId].get());
    }

	// Supprime tous les événements enregistrés.
    void ClearAll() {
        m_events.clear();
    }

    /* @brief Enregistre un écouteur pour un type d'événement spécifique.
        @tparam T Type de la classe qui écoute l'événement.
        @tparam Args Types des arguments de l'événement.
        @param listener Pointeur vers l'objet qui écoutera l'événement.
        @param method Pointeur vers la méthode membre qui sera appelée.
        @param priority Priorité d'appel (plus haut = appelé avant).   */
    template<typename T, typename... Args>
    void Register(T* listener, void (T::* m_method)(Args...), int Priority = 0) {
        GetEvent<Args...>().Register(listener, m_method, Priority);
    }

    /* @brief Supprime un écouteur spécifique pour un événement.
        @tparam T Type de la classe qui écoutait l’événement.
        @tparam Args Types des arguments de l’événement.
        @param listener Objet concerné.
        @param method Méthode à désinscrire.   */
    template<typename T, typename... Args>
    void Unregister(T* listener, void (T::* m_method)(Args...)) {
        GetEvent<Args...>().Unregister(listener, m_method);
    }

    /* @brief Déclenche un événement (appel de tous les subscribers).
        @tparam Args Types des arguments de l’événement.
        @param args Arguments à transmettre à chaque listener.   */
    template<typename... Args>
    void CallEvent(Args... args) {
        GetEvent<Args...>().Call(std::forward<Args>(args)...);
    }

    /* @brief Supprime tous les handlers associés à un objet pour un type d’événement.
        @tparam T Type de l'objet.
        @tparam Args Types des arguments de l’événement.
        @param listener Objet à désinscrire.   */
    template<typename T, typename... Args>
    void RemoveAll(T* listener) {
        GetEvent<Args...>().RemoveAll(listener);
    }
    
    /* @brief Vérifie si un Event<T> existe déjà.
        @tparam Args Types des arguments de l’événement.
        @return true si l’Event existe dans la map, false sinon.   */
    template<typename... Args>
        bool HasEvent() const {
        return m_events.find(std::type_index(typeid(Event<Args...>))) != m_events.end();
    }

    /* @brief Récupère le nombre d'écouteurs (handlers) enregistrés pour un événement donné.
        @tparam Args Types des arguments que l’événement utilise (typiquement, un seul type comme `const MyEvent&`).
        @return int Nombre de handlers actuellement enregistrés pour cet événement. Retourne 0 si l’événement n’existe pas.   */
    template <typename... Args>
	int GetEventCount() const {
		auto it = m_events.find(std::type_index(typeid(Event<Args...>)));
		if (it != m_events.end()) {
			return static_cast<Event<Args...>*>(it->second.get())->GetCount();
		}
		return 0;
	}

    private:
        //stockage dynamique de tous les événements selon leur type.
        std::unordered_map<std::type_index, std::unique_ptr<BaseEvent>> m_events;
    };
} // namespace gce
#endif // ENGINE_EVENT_MANAGER_H_INCLUDED