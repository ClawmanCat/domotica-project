#pragma once

#include "EthernetServer.h"
#include "Message.h"
#include "../Shared/MagicEnum.h"

#define CommandHandler ServerCommandHandler::instance()

class ServerCommandHandler {
public:
    // LIST_COMMANDS should always be zero, so the app can easily request the other commands.
    MAGIC_ENUM(Command, byte, LIST_COMMANDS, PING, LIST_DEVICES, LIST_ATTACHABLES)

    static ServerCommandHandler& instance(void) {
        static ServerCommandHandler i = ServerCommandHandler();
        return i;
    }

    void HandleCommand(EthernetClient& src, const Message& msg) {
        if (msg.device != 0 || msg.port >= NumCommands) return;
        switch ((Command) msg.port) {
            case LIST_COMMANDS:
                HandleListCommands(src, msg);
                break;
            case PING:
                HandlePingCommand(src, msg);
                break;
            case LIST_DEVICES:
                HandleListDevicesCommand(src, msg);
                break;
            case LIST_ATTACHABLES:
                HandleListAttachablesCommand(src, msg);
                break;
        }
    }
private:
    ServerCommandHandler(void) = default;

    // TODO: Refactor server commands to seperate classes.
    void HandleListCommands(EthernetClient& src, const Message& msg) {
        String result;
        for (byte i = 0; i < NumCommands; ++i) {
            result += CommandToString((Command) i);
            if (i != (NumCommands - 1)) result += '\n';
        }
        
        src.write((byte*) msg.MessageID, sizeof(long long));
        src.write(result.c_str());
    }

    void HandlePingCommand(EthernetClient& src, const Message& msg) {
        src.write((byte*) msg.MessageID, sizeof(long long));
        src.write("Pong!");
    }

    void HandleListDevicesCommand(EthernetClient& src, const Message& msg) {
        src.write((byte*) msg.MessageID, sizeof(long long));
        src.write("This command is currently not implemented.");
    }

    void HandleListAttachablesCommand(EthernetClient& src, const Message& msg) {
        src.write((byte*) msg.MessageID, sizeof(long long));
        src.write("This command is currently not implemented.");
    }
};