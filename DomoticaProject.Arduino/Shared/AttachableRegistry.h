#pragma once

#include <Arduino.h>
#include <Vector.h>

struct Attachable;

typedef const char* (*OnAttachableRequest)(const char* data, const Attachable* owner);
typedef void (*AttachableInit)(void);

struct Attachable {
    // Function called when the server is initializing.
    AttachableInit InitCallback;
    // Function called when the server receives a request for this attachable.
    OnAttachableRequest RequestCallback;
    // Does calling the callback function return data? (if no, the callback should return nullptr.)
    bool HasReturn;
    // The name for this attachable, to be displayed in the app.
    const char* DisplayName;
    // Identifiers for locating the attachable.
    byte device, port;

    Attachable(AttachableInit CBInit, OnAttachableRequest CBRequest, bool returns, const char* name, byte device, byte port)
        : InitCallback(CBInit), RequestCallback(CBRequest), HasReturn(returns), DisplayName(name), device(device), port(port) {}

    ~Attachable(void) {
        delete[] DisplayName;
    }
};


class AttachableRegistry {
public:
    // Singleton object.
    static AttachableRegistry& instance(void) {
        static AttachableRegistry i = AttachableRegistry();
        return i;
    }

    // Register a new attachable.
    void RegisterAttachable(Attachable& attachable) {
        attachables.push_back(attachable);
    }

    // Find the attachable at the given location. Returns null if the attachable doesn't exist.
    const Attachable* find(byte device, byte port) {
        for (byte i = 0; i < attachables.size(); ++i) {
            Attachable& a = attachables[i];
            if (a.device == device && a.port == port) return &a;
        }

        return nullptr;
    }
private:
    AttachableRegistry(void) = default;

    Vector<Attachable> attachables;
};