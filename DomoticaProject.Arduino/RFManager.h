#pragma once

#include <RH_ASK.h>
#include <RHMesh.h>

#include "RFMessage.h"
#include "Unique.h"
#include "AttachableRegistry.h"

#define RF RFManager::instance()

class RFManager {
public:
    #ifdef ServerOnly
    const static byte MESH_ADDRESS = 1;
    #else
    const static byte MESH_ADDRESS = CLIENT_DEVICE_ID;
    #endif

    const static byte SEND_PIN = 2;
    const static byte RECV_PIN = 3;

    static RFManager& instance(void) {
        static RFManager i = RFManager();
        return i;
    }

    void SendMessage(RFMessage& msg, byte dest) {
        UniqueArray<byte> msgbuf = msg.Serialize();
        byte msglen = strlen(msg.data.raw_ptr()) + 2;

        network.sendtoWait(msgbuf.raw_ptr(), msglen, dest);
    }

    Unique<RFMessage> SendAndAwaitResponse(RFMessage& msg, byte dest, short timeout) {
        UniqueArray<byte> msgbuf = msg.Serialize();
        byte msglen = strlen(msg.data.raw_ptr()) + 2;

        network.sendtoWait(msgbuf.raw_ptr(), msglen, dest);

        msgbuf = new byte[255];
        msglen = 255;

        if (network.recvfromAckTimeout(msgbuf.raw_ptr(), &msglen, timeout, &dest)) {
            return Unique<RFMessage>::FromCopy(&RFMessage::Deserialize(msgbuf));
        } else {
            return Unique<RFMessage>(nullptr);
        }
    }

    RFMessage ListenResponse(byte src) {
        UniqueArray<byte> msgbuf = new byte[255];
        byte msglen = 255;

        network.recvfromAck(msgbuf.raw_ptr(), &msglen, &src);
        return RFMessage::Deserialize(msgbuf);
    }


    RH_ASK& GetDriver(void) {
        return driver;
    }


    #ifdef ServerOnly
    void FetchAttachables(void) {
        bool done = false;
        byte next = 2;

        while (!done) {
            RFMessage msg = RFMessage(0, SafeCString::FromCopy("REQ_ATH", 8));

            Unique<RFMessage> response = SendAndAwaitResponse(
                msg,
                next,
                1000
            );

            if (response.raw_ptr() == nullptr) {
                done = true;
                continue;
            }

            AttRegistry.RegisterAttachable(new Attachable(
                [](Attachable* thisptr) {},
                [](Attachable* thisptr, SafeCString&& msg) {
                    return RF.RequestAttachableAction(thisptr->device, thisptr->index, move(msg));
                },
                SafeCString::FromCopy((char*) response->data.raw_ptr() + 2, strlen((char*) response->data.raw_ptr() + 2)),
                next,
                (byte) *(response->data.raw_ptr() + 1)
            ));

            ++next;
        }
    }

    SafeCString RequestAttachableAction(byte dest, byte index, SafeCString&& data) {
        RFMessage msg = RFMessage(index, move(data));
        Unique<RFMessage> response = SendAndAwaitResponse(msg, dest, 1000);

        if (response.raw_ptr() == nullptr) {
            return SafeCString(nullptr);
        } else {
            return move(response->data);
        }
    }
    #endif

    #ifdef ClientOnly
    void HandleClientRequest(void) {
        RFMessage request = ListenResponse(1);

        if (strcmp((char*) request.data.raw_ptr(), "REQ_ATH") == 0) {
            SendMessage(
                RFMessage(0, AttRegistry.ListAttachables()),
                1
            );
        } else {
            Attachable* a = AttRegistry.FindAttachable(request.index);
            a->CallCallback(a, move(request.data));
        }
    }
    #endif
private:
    RFManager(void) : driver(2000, RECV_PIN, SEND_PIN), network(driver, MESH_ADDRESS) {}

    RH_ASK driver;
    RHMesh network;
};