#pragma once

#include "../Server/Message.h"
#include "../Shared/MagicEnum.h"
#include "../Shared/Vector.h"
#include "../Shared/Unique.h"

#define AttRegistry AttachableRegistry::instance()

// An attachable represents an object that can be attached to an Arduino.
// i.e. a sensor or an actuator.
struct Attachable {
    typedef void(*OnAttachableInit)(void);
    typedef void(*OnAttachableCalled)(const Message& msg);

    MAGIC_ENUM_CLASS(DataType, byte, NONE, STRING, INT, FLOAT, BOOL);


    OnAttachableInit InitCallback;          // Called when the server initializes.
    OnAttachableCalled CallCallback;        // Called when the server receives a request for this attachable.
    DataType ArgTypes[4];                   // Arguments the message should have. Enforced by the app, not the Arduino.
    DataType RetType;                       // Same as above, but for the return type.
    SafeCString Name;                       // Used to identify this attachable in the app.
};


class AttachableRegistry {
public:
    static AttachableRegistry& instance() {
        static AttachableRegistry i = AttachableRegistry();
        return i;
    }

    ~AttachableRegistry(void) {
        for (Attachable* a : attachables) delete a;
    }

    // Registry will take ownership of pointer after registration.
    void RegisterAttachable(Attachable* a) {
        attachables.push_back(a);
        a->InitCallback();
    }

    // Address of attachable will be in the vector, so just deduce its index 
    // rather than doing a slower name-based search.
    void RemoveAttachable(Attachable* a) {
        if (a < *attachables.begin() || a >= *attachables.end()) {
            // Not in array.
            return;
        }

        byte addr = a - *attachables.begin();
        attachables.erase(addr);
    }

    // Lifetime objects, so returning a pointer is safe.
    Attachable* FindAttachable(SafeCString& name) {
        for (Attachable* a : attachables) {
            if (strcmp(a->Name.raw_ptr(), name.raw_ptr()) == 0) return a;
        }

        return nullptr;
    }

    Attachable* GetAttachable(byte index) {
        return attachables[index];
    }

    byte size(void) const {
        return attachables.size();
    }
private:
    AttachableRegistry(void) = default;

    // Store pointers instead of objects, so the amount of wasted space is a multiple of sizeof(pointer) and not sizeof(Attachable).
    Vector<Attachable*> attachables;
};