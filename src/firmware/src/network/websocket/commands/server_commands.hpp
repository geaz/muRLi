#ifndef SERVERCOMMANDS_H
#define SERVERCOMMANDS_H

#include <array>

namespace Murli
{
    namespace Server
    {
        enum CommandType
        {        
            MESH_CONNECTION,
            MESH_COUNTED,
            MESH_UPDATED,
            MOD_DISTRIBUTED,
            EXTERNAL_ANALYZER
        };

        struct CountedCommand
        {
            uint32_t meshLedCount;
        };

        struct ExternalAnalyzerCommand
        {
            float decibel;
            uint8_t volume;
            uint16_t frequency;
            std::array<uint8_t, 17> buckets;
        };

        struct Command
        {
            uint64_t id;
            CommandType commandType;
            union
            {
                CountedCommand countedCommand;
                ExternalAnalyzerCommand externalAnalyzerCommand;
            };
        };
    }
}

#endif // SERVERCOMMANDS_H