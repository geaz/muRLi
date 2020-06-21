#ifndef MURLICOMMAND_H
#define MURLICOMMAND_H

namespace Murli
{
    enum Command
    {
        MESH_CONNECTION,
        MESH_COUNT,
        MESH_UPDATE,
        MOD_DISTRIBUTED,
        ANALYZER_UPDATE
    };

    struct MurliCommand
    {
        uint64_t id;
        Command command;
        uint8_t volume;
        uint16_t frequency;
        uint32_t meshLedCount;
        uint32_t previousLedCount;
        uint32_t previousNodeCount;
    };

    struct MurliCountData
    {
        MurliCommand countCommand;        
        MurliCommand updateCommand;
        uint16_t answers;
        bool active;
    };
}

#endif // MURLICOMMAND_H