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
        uint16_t meshLEDCount;
        uint16_t previousLEDCount;
    };

    struct MurliCountData
    {
        MurliCommand countCommand;        
        MurliCommand updateCommand;
        uint32_t answers;
        bool active;
    };
}

#endif // MURLICOMMAND_H