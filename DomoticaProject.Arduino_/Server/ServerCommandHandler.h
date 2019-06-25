#pragma once

#include "Message.h"
#include "../Shared/MagicEnum.h"
#include "../Shared/AttachableRegistry.h"
#include "../Shared/ProgmemUtils.h"

#define CommandHandler ServerCommandHandler::instance()

MAKE_PROGMEM_STRING_ARRAY(
    CommandStrings,
    "Pong!",
    "This command is currently not implemented."
);

class ServerCommandHandler {
public:
    // LIST_COMMANDS should always be zero, so the app can easily request the other commands.
    MAGIC_ENUM(Command, byte, LIST_COMMANDS, PING_SERVER, LIST_DEVICES, LIST_ATTACHABLES)

    static ServerCommandHandler& instance(void) {
        static ServerCommandHandler i = ServerCommandHandler();
        return i;
    }

    SafeCString HandleCommand(const Message& msg) {
        if (msg.device != 0 || msg.port >= NumCommands) return;
        switch ((Command) msg.port) {
            case LIST_COMMANDS:
                return HandleListCommands(msg);
            case PING_SERVER:
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
    SafeCString HandleListCommands(const Message& msg) {
        String result;
        for (byte i = 0; i < NumCommands; ++i) {
            char intstr[5];
            sprintf(intstr, "%03.3i ", (int) i);

            result += intstr;
            result += CommandToString((Command) i).raw_ptr();
            if (i != (NumCommands - 1)) result += '\n';
        }
        
        SafeCString cs = new char[result.length() + 1];
        memcpy(cs.raw_ptr(), result.c_str(), result.length() + 1);
        return cs;
    }

    SafeCString HandlePingCommand(const Message& msg) {
        return ProgmemUtils::ReadProgmemString(CommandStrings, 0);
    }

    SafeCString HandleListDevicesCommand(const Message& msg) {
        return ProgmemUtils::ReadProgmemString(CommandStrings, 1);
    }

    SafeCString HandleListAttachablesCommand(const Message& msg) {
        return ProgmemUtils::ReadProgmemString(CommandStrings, 1);
    }
};