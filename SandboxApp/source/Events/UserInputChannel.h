#pragma once
#include "BlockingEvents\EventChannel.h"
#include "glm\vec2.hpp"

using namespace AnalyticalApproach::EventSystem;

namespace AnalyticalApproach::Spellcast
{
    class UserInputChannel : public EventChannel
    {

        public:
        Action<int> on_key_pressed;
        Action<glm::vec2> on_mouse_moved;
        Action<glm::vec2> on_mouse_delta;
        Action<glm::vec2, int> on_mouse_button_clicked;

        void RaiseOnKeyPressed(int keyCode)
        {
            on_key_pressed(keyCode); 
        }

        void RaiseMouseButtonPressed(glm::vec2 mPos, int buttonCode)
        {
            on_mouse_button_clicked(mPos, buttonCode); 
        }

    
        void RaiseMouseMoved(glm::vec2 mPos)
        {
            on_mouse_moved(mPos); 
        }


        void RaiseMouseDelta(glm::vec2 mDelta)
        {
            on_mouse_delta(mDelta); 
        }

        ~UserInputChannel()
        {
        }
    };
}