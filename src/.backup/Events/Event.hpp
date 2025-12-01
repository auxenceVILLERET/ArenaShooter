#ifndef ENGINE_EVENT_HPP_INCLUDED
#define ENGINE_EVENT_HPP_INCLUDED

#include "GameObject.h"
#include <typeindex>
#include <memory>

namespace gce {

    class BaseEvent {
    public:
        virtual ~BaseEvent() = default;
    };

    /* @brief Classe template Event, permettant de définir des événements typés.
        Elle permet l'enregistrement, la suppression et l'appel de callbacks (méthodes membres).
        @tparam Args Les types d'arguments passés à l'événement lors de son appel.  */
    template <typename... Args>
    class Event : public BaseEvent {
    public:
        // Interface de base pour tous les handlers (abstraction des types)
        class HandlerBase {
        public:
            virtual ~HandlerBase() = default;
            virtual void Invoke(Args... args) = 0;
            virtual void* GetInstance() const = 0;
        };
         
        //@brief Handler spécifique pour une instance donnée et une méthode membre donnée.
        template <typename T>
        class Handler : public HandlerBase {
        public:
            using Method = void (T::*)(Args...);

            Handler(T* instance, Method method)
                : m_instance(instance), m_method(method) {
            }

            void Invoke(Args... args) override {
                (m_instance->*m_method)(std::forward<Args>(args)...);
            }

            void* GetInstance() const override {
                return static_cast<void*>(m_instance);
            }

        private:
            T* m_instance;
            Method m_method;
            
            friend class Event;
        };

        //@brief Représente une souscription à un événement avec une priorité.
        struct Subscription {
            int priority;
            HandlerBase* handler;
        };

        Event() = default;
        ~Event() {
            Clear();
        }

        Event(const Event&) = delete;
        Event& operator=(const Event&) = delete;

        void Clear() {
            for (Subscription& sub : m_subscribers) {
                delete sub.handler;
            }
            m_subscribers.clear();
        }

        int GetCount() const {
            return static_cast<int>(m_subscribers.size());
        }

        void Call(Args... args) {
            for (Subscription& sub : m_subscribers) {
                sub.handler->Invoke(std::forward<Args>(args)...);
            }
        }

        template <typename T>
        void Register(T* receiver, typename Handler<T>::Method method, int priority = 0) {
            Subscription sub;
            sub.priority = priority;
            sub.handler = new Handler<T>(receiver, method);

            m_subscribers.push_back(sub);

            std::sort(m_subscribers.begin(), m_subscribers.end(),
                [](const Subscription& a, const Subscription& b) {
                    return a.priority > b.priority;
                });
        }

        template <typename T>
        void Unregister(T* receiver, typename Handler<T>::Method method) {
            auto it = std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                [&](const Subscription& sub) {
                    auto* handler = dynamic_cast<Handler<T>*>(sub.handler);
                    bool match = (handler && handler->GetInstance() == receiver && handler->m_method == method);
                    if (match) delete sub.handler;
                    return match;
                });
            m_subscribers.erase(it, m_subscribers.end());
        }

        template <typename T>
        void RemoveAll(T* receiver) {
            auto it = std::remove_if(m_subscribers.begin(), m_subscribers.end(),
                [&](const Subscription& sub) {
                    if (sub.handler->GetInstance() == receiver) {
                        delete sub.handler;
                        return true;
                    }
                    return false;
                });
            m_subscribers.erase(it, m_subscribers.end());
        }

    private:
        std::vector<Subscription> m_subscribers; ///< Liste des souscriptions
    };
}

#endif // ENGINE_EVENT_HPP_INCLUDED
