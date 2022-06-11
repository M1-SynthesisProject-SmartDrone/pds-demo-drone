#include "ConfigParser.h"

#include <libconfig.h++>

using namespace std;
namespace fs = std::filesystem;

string getConfigPath(char* argv[])
{
    std::filesystem::path exePath(argv[0]);
    std::filesystem::path exeDirectoryPath = exePath.parent_path();
    return exeDirectoryPath.string() + "/config.cfg";
}

ConfigParser::ConfigParser(int argc, char* argv[])
{
    // The file must be aside the executable normally (done at cmake step)
    // If there is an issue, we must stop the app anyways, so let the exception throw
    string filename = getConfigPath(argv);
    m_confFilePath = fs::path(filename);
    if (!fs::exists(m_confFilePath))
    {
        throw runtime_error("Config file not found: " + filename);
    }
}

ConfigParser::~ConfigParser() {}

ConfigParams ConfigParser::parse()
{
    libconfig::Config config;

    config.readFile(m_confFilePath.c_str());

    const auto& root = config.getRoot();

    const auto& windowSettings = root["window"];
    auto windowConfig = ConfigWindow{
        windowSettings["name"],
        windowSettings["width"],
        windowSettings["height"],
        windowSettings["enable_vsync"],
        windowSettings["framerate"]
    };

    const auto& imageSettings = root["image"];
    auto imageConfig = ConfigImage{
        imageSettings["folder"],
        imageSettings["enable_smooth"],
    };

    const auto& mapSettings = root["map"];
    auto mapConfig = ConfigMap{
        mapSettings["width"],
        mapSettings["height"],
    };

    return ConfigParams(
        windowConfig,
        imageConfig,
        mapConfig
    );
}

