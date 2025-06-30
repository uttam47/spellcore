// Platform/IPlatform.h

namespace AnalyticalApproach::WindowSystem
{
    enum class PlatformType
    {
        Windows,
        Linux,
        MacOS,
        Android,
        iOS,
        Unknown
    };

    class IPlatform
    {
    public:
        virtual ~IPlatform() = default;
        virtual PlatformType GetPlatformType() const = 0;
        virtual const char *GetPlatformName() const = 0;
        virtual bool ExecuteSystemCommand(const char *cmd) = 0;
        // More low-level stuff
    };

}