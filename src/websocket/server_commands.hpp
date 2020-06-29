#ifndef SERVERCOMMANDS_H
#define SERVERCOMMANDS_H

namespace Murli
{
    namespace Server
    {
        enum CommandType
        {        
            MESH_CONNECTION,
            MESH_COUNTED,
            MESH_UPDATED,
            MOD_DISTRIBUTED
        };

        struct CountedCommand
        {
            uint32_t meshLedCount;
        };

        struct Command
        {
            uint64_t id;
            CommandType commandType;
            union
            {
                CountedCommand countedCommand;
            };
        };
    }
}

#endif // SERVERCOMMANDS_H