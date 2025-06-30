#pragma once
#include "BlockingEvents\EventChannel.h"
#include "glm\vec2.hpp"

using namespace AnalyticalApproach::EventSystem;

namespace AnalyticalApproach::WindowSystem
{
    class WindowEventChannel : public EventChannel
    {
        public:
        Action<glm::vec2> on_window_resized;
        Action<glm::vec2> on_window_moved;
        Action<> on_window_closed;

        void RaiseOnWindowResized(glm::vec2 windowSize)
        {
            on_window_resized(windowSize); 
        }

        void RaiseWindowMoved(glm::vec2 windowPos)
        {
            on_window_moved(windowPos); 
        }

        void RaiseWindowClosed()
        {
            on_window_closed(); 
        }

        ~WindowEventChannel()
        {
        }
    };
}