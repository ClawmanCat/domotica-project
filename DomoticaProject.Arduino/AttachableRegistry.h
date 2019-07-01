#pragma once

#include "EthernetMessage.h"
#include "MagicEnum.h"
#include "Vector.h"
#include "Unique.h"
//#include "DataType.h"

#define AttRegistry AttachableRegistry::instance()

// An attachable represents an object that can be attached to an Arduino.
// i.e. a sensor or an actuator.
struct Attachable {
    typedef void (*OnAttachableInit)(Attachable*);
    typedef SafeCString (*OnAttachableCalled)(Attachable*, SafeCString&& msg);

    OnAttachableInit InitCallback;          // Called when the server initializes.
    OnAttachableCalled CallCallback;        // Called when the server receives a request for this attachable.
    //DataType ArgTypes[4];                 // Arguments the message should have. Enforced by the app, not the Arduino.
    //DataType RetType;                     // Same as above, but for the return type.
    SafeCString Name;                       // Used to identify this attachable in the app.

    // The attachables location in the network.
    #ifdef ServerOnly
    byte device, index;

    Attachable(OnAttachableInit init, OnAttachableCalled call, SafeCString&& name, byte device, byte index)
        : InitCallback(init), CallCallback(call), Name(move(name)), device(device), index(index) {}
    #elif defined(ClientOnly)
    byte index;

    Attachable(OnAttachableInit init, OnAttachableCalled call, SafeCString&& name, byte index)
        : InitCallback(init), CallCallback(call), Name(move(name)), index(index) {}
    #endif
};


class AttachableRegistry {
public:
    static AttachableRegistry& instance() {
        static AttachableRegistry i = AttachableRegistry();
        return i;
    }

    ~AttachableRegistry(void) {
        for (Attachable* att : attachables) delete att;
    }

    // Registry will take ownership of pointer after registration.
    void RegisterAttachable(Attachable* a) {
        attachables.push_back(a);
        a->InitCallback(a);
    }

    #ifdef ServerOnly
    SafeCString ListAttachables(void) {
        String result;

        for (Attachable* att : attachables) {
            result += att->device;
            result += ':';
            result += att->index;
            result += ':';
            result += att->Name.raw_ptr();
            result += '\n';
        }

        return SafeCString::FromCopy(result.c_str(), result.length() + 1);
    }

    Attachable* FindAttachable(byte device, byte index) {
        for (Attachable* att : attachables) {
            if (att->device == device && att->index == index) return att;
        }

        return nullptr;
    }
    #endif

    #ifdef ClientOnly
    SafeCString ListAttachables(void) {
        String result;

        for (Attachable* att : attachables) {
            result += CLIENT_DEVICE_ID;
            result += ':';
            result += att->index;
            result += ':';
            result += att->Name.raw_ptr();
            result += '\n';
        }

        return SafeCString::FromCopy(result.c_str(), result.length() + 1);
    }

    Attachable* FindAttachable(byte index) {
        for (Attachable* att : attachables) {
            if (att->index == index) return att;
        }

        return nullptr;
    }
    #endif

    byte size(void) const {
        return attachables.size();
    }
private:
    AttachableRegistry(void) = default;

    // Store pointers instead of objects, so the amount of wasted space is a multiple of sizeof(pointer) and not sizeof(Attachable).
    Vector<Attachable*> attachables;
};