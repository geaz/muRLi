#include <toml11/toml.hpp>
#include <iostream>
#include <client_commands.hpp>
#include <server_commands.hpp>

#include "murli_desktop.hpp"

int main(int argc, char* argv[])
{
    const auto tomlConfig  = toml::parse("config.toml");
    const auto& murliConfig = toml::find(tomlConfig, "murli");
    const auto url  = toml::find<std::string>(murliConfig, "url");

    Murli::Desktop::MurliDesktop desktopApp(url);
    desktopApp.run();
}