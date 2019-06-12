#pragma once

#include "EthernetServer.h"
#include "../Shared/MagicEnum.h"

#define CommandHandler ServerCommandHandler::instance()

class ServerCommandHandler {
public:
    MAGIC_ENUM(Command, byte, LIST_COMMANDS, PING, LIST_DEVICES, LIST_ATTACHABLES)

    static ServerCommandHandler& instance(void) {
        static ServerCommandHandler i = ServerCommandHandler();
        return i;
    }

    void HandleCommand(EthernetClient& src, byte command, const byte* data) {
        if (command >= NumCommands) return;
        switch ((Command) command) {
            case LIST_COMMANDS:
                HandleListCommands(src, data);
                break;
            case PING:
                HandlePingCommand(src, data);
                break;
            case LIST_DEVICES:
                HandleListDevicesCommand(src, data);
                break;
            case LIST_ATTACHABLES:
                HandleListAttachablesCommand(src, data);
                break;
        }
    }
private:
    ServerCommandHandler(void) = default;

    // TODO: Refactor server commands to seperate classes.
    void HandleListCommands(EthernetClient& src, const byte* data) {
        String result;
        for (byte i = 0; i < NumCommands; ++i) {
            result += CommandToString((Command) i);
            if (i != (NumCommands - 1)) result += '\n';
        }
        
        src.write(result.c_str());
    }

    void HandlePingCommand(EthernetClient& src, const byte* data) {
        src.write("Pong!");
    }

    void HandleListDevicesCommand(EthernetClient& src, const byte* data) {
        src.write("This command is currently not implemented.");
    }

    void HandleListAttachablesCommand(EthernetClient& src, const byte* data) {
        src.write("This command is currently not implemented.");
    }
};