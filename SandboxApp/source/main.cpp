#include "SandboxApp.h"
#include "Logger.h"

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv, argv + argc);
    AnalyticalApproach::SandboxApp app(args);
    return app.Run();
}