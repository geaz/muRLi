#include <ESP8266WebServer.h>

namespace Murli
{
    class MurliWeb
    {
        public:
            MurliWeb();

            void loop();
            void connectNetwork();

        private:
            void saveSSID();

            ESP8266WebServer _server;
    };
}