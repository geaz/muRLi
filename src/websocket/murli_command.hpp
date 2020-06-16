#ifndef MURLICOMMAND_H
#define MURLICOMMAND_H

namespace Murli
{
    enum Command
    {
        MESH_CONNECTION,
        MESH_COUNT,
        MESH_UPDATE,
        ANALYZER_UPDATE
    };

    struct MurliCommand
    {
        Command command;
        float decibel;
        uint16_t frequency;
        uint16_t meshLEDCount;
    };
}

#endif // MURLICOMMAND_H