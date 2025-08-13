#include "SandboxApp.h"
#include "Logger.h"
#include <filesystem> // for std::filesystem::path

int main(int argc, char** argv)
{
    AnalyticalApproach::SandboxApp app;
    return app.Run();
}
