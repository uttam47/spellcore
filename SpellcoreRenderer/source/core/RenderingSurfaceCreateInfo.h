#pragma once

namespace AnalyticalApproach::Spellcore
{
   struct RenderingSurfaceCreateInfo
   {
      void* nativeWindowHandle = nullptr; 
      int width = 0; 
      int height = 0; 
      bool vsynchEnabled = 0;  
   }; 
}