#pragma once

#include "MagicEnum.h"
#include "ProgmemUtils.h"
#include "Unique.h"
#include "EthernetMessage.h"
#include "AttachableRegistry.h"


// To register a new command, simply add its name to the sequence below, and create a method to handle the command.
// The method should have the signature SafeCString HC_<CommandName>(EthernetMessage& msg)
// Command 0, 1 and 2 should be LIST_COMMANDS, GET_FANCY_NAME and GET_COMMAND_ARGS respectively.
// Other than that, the order may be changed at will.
#define COMMAND_HANDLER_CMD_SEQ (LIST_COMMANDS)(GET_FANCY_NAME)(PING_SERVER)(LIST_DEVICES)(LIST_ATTACHABLES)

#define CmdHandler CommandHandler::instance()
#define _CMD_HANDLER_IMPL_MACRO(Index, Data, Elem) case Command::Elem: return BOOST_PP_CAT(HC_, Elem) (msg);


// User-friendly command names.
MAKE_PROGMEM_STRING_ARRAY(
    FancyCommandNames,
    "List all commands",
    "Get user-friendly command name",
    "Ping the server",
    "List all connected devices",
    "List all connected attachables"
);

// Command responses
MAKE_PROGMEM_STRING_ARRAY(
    CommandResponses,
    "Pong!",
    "Incorrect Argument(s)",
    "This command is currently not implemented."
);

// Singleton-class for handling commands to be executed locally on the server.
class CommandHandler {
public:
    MAGIC_ENUM_CLASS(Command, byte, BOOST_PP_SEQ_ENUM(COMMAND_HANDLER_CMD_SEQ));

    static CommandHandler& instance(void) {
        static CommandHandler i = CommandHandler();
        return i;
    }

    // Run the command and get a response.
    SafeCString HandleCommandMessage(EthernetMessage& msg) {
        switch ((Command) msg.index) {
            BOOST_PP_SEQ_FOR_EACH(
                _CMD_HANDLER_IMPL_MACRO,
                _,
                COMMAND_HANDLER_CMD_SEQ
            )
        }
    }
private:
    CommandHandler(void) = default;


    SafeCString HC_LIST_COMMANDS(EthernetMessage& msg) {
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


    SafeCString HC_GET_FANCY_NAME(EthernetMessage& msg) {
        unsigned long i = atoi(msg.data.raw_ptr());
        if (i > NumFancyCommandNames) return ProgmemUtils::ReadProgmemString(CommandResponses, 1);

        return ProgmemUtils::ReadProgmemString(FancyCommandNames, i);
    }


    SafeCString HC_PING_SERVER(EthernetMessage& msg) {
        return ProgmemUtils::ReadProgmemString(CommandResponses, 0);
    }


    SafeCString HC_LIST_DEVICES(EthernetMessage& msg) {
        return ProgmemUtils::ReadProgmemString(CommandResponses, 2);
    }


    SafeCString HC_LIST_ATTACHABLES(EthernetMessage& msg) {
        return AttRegistry.ListAttachables();
    }
};