#include "EventManager.h"

namespace AnalyticalApproach::EventSystem
{
    EventManager& EventManager::GetInstance()
    {
        static EventManager instance;
        return instance;
    }

    EventManager::~EventManager()
    {
        // Clean up dynamically created event channels
        for (auto& pair : m_eventChannels)
        {
            delete pair.second;
        }
        m_eventChannels.clear();
    }
}
