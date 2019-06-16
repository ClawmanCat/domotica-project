#pragma once

#include "Message.h"
#include "../Shared/MagicEnum.h"
#include "../Shared/AttachableRegistry.h"

#define CommandHandler ServerCommandHandler::instance()

class ServerCommandHandler {
public:
    // LIST_COMMANDS should always be zero, so the app can easily request the other commands.
    MAGIC_ENUM(Command, byte, LIST_COMMANDS, PING, LIST_DEVICES, LIST_ATTACHABLES)

    static ServerCommandHandler& instance(void) {
        static ServerCommandHandler i = ServerCommandHandler();
        return i;
    }

    String HandleCommand(const Message& msg) {
        if (msg.device != 0 || msg.port >= NumCommands) return;
        switch ((Command) msg.port) {
            case LIST_COMMANDS:
                return HandleListCommands(msg);
            case PING:
                return HandlePingCommand(msg);
            case LIST_DEVICES:
                return HandleListDevicesCommand(msg);
            case LIST_ATTACHABLES:
                return HandleListAttachablesCommand(msg);
        }
    }
private:
    ServerCommandHandler(void) = default;

    // TODO: Refactor server commands to seperate classes.
    String HandleListCommands(const Message& msg) {
        String result;
        for (byte i = 0; i < NumCommands; ++i) {
            char intstr[5];
            sprintf(intstr, "%03.3i ", (int) i);

            result += intstr;
            result += CommandToString((Command) i);
            if (i != (NumCommands - 1)) result += '\n';
        }
        
        return result;
    }

    String HandlePingCommand(const Message& msg) {
        return "Pong!";
    }

    String HandleListDevicesCommand(const Message& msg) {
        return "This command is currently not implemented.";
    }

    String HandleListAttachablesCommand(const Message& msg) {
        //return AttRegistry.GetAttachableDisplayData();
        return "";
    }
};