#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <iostream>
#include "BlockingEvents/EventChannel.h"
#include "Logger.h"

namespace AnalyticalApproach::EventSystem
{
    class EventManager
    {
    public:
        static EventManager& GetInstance();

        template <typename T>
        T* GetEventChannel();

        template <typename T>
        void RegisterEventChannel(T* eventChannel);

    private:
        EventManager() = default;
        ~EventManager();

        EventManager(const EventManager&) = delete;
        EventManager& operator=(const EventManager&) = delete;

        std::unordered_map<std::type_index, EventChannel*> m_eventChannels;
    };

    
    template <typename T>
    T* EventManager::GetEventChannel()
    {
        std::type_index type = typeid(T);

        auto it = m_eventChannels.find(type);
        if (it != m_eventChannels.end())
        {
            return dynamic_cast<T*>(it->second);
        }
        else
        {
            // Not found: create automatically!
            T* newChannel = new T();
            m_eventChannels[type] = newChannel;
            std::string logMsg = std::string("Auto-created EventChannel of type: ") + type.name();
            LOG_DEBUG(logMsg); 
            return newChannel;
        }
    }

    template <typename T>
    void EventManager::RegisterEventChannel(T* eventChannel)
    {
        std::type_index type = typeid(T);

        if (m_eventChannels.find(type) != m_eventChannels.end())
        {
            LOG_DEBUG("EventChannel of this type already registered!");
            return;
        }

        m_eventChannels[type] = eventChannel;
    }
}
