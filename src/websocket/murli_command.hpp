#ifndef MURLICOMMAND_H
#define MURLICOMMAND_H

namespace Murli
{
    enum Command
    {
        MESH_CONNECTION,
        MESH_COUNT_REQUEST,
        MESH_COUNTED,
        MESH_UPDATE,
        MESH_UPDATED,
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
}

#endif // MURLICOMMAND_H