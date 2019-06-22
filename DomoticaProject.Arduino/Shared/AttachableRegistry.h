#pragma once

#pragma once

#include <Arduino.h>

#include "../Shared/Vector.h"
#include "../Shared/MagicEnum.h"
#include "../Server/Message.h"

/*
#define AttRegistry AttachableRegistry::instance()


typedef void (*OnAttachableInit)(void);
typedef String (*OnAttachableCall)(const Message& msg);

struct Attachable {
    MAGIC_ENUM_CLASS(DataType, byte, NONE, TEXT, INT, FLOAT);

    OnAttachableInit OnInit;
    OnAttachableCall OnCall;

    char* DisplayName, *Description;
    DataType RequestType, ReturnType;

    Attachable(OnAttachableInit init, OnAttachableCall call, const char* display, const char* desc, DataType request, DataType ret)
        : OnInit(init), OnCall(call), RequestType(request), ReturnType(ret) {

        DisplayName = new char[strlen(display) + 1];
        strcpy(DisplayName, display);

        Description = new char[strlen(desc) + 1];
        strcpy(Description, desc);
    }

    Attachable(const Attachable& other)
        : OnInit(other.OnInit), OnCall(other.OnCall), RequestType(other.RequestType), ReturnType(other.ReturnType) {

        DisplayName = new char[strlen(other.DisplayName) + 1];
        strcpy(DisplayName, other.DisplayName);

        Description = new char[strlen(other.Description) + 1];
        strcpy(Description, other.Description);
    }

    ~Attachable(void) {
        delete[] DisplayName;
        delete[] Description;
    }
};


class AttachableRegistry {
public:
    static AttachableRegistry& instance(void) {
        static AttachableRegistry i = AttachableRegistry();
        return i;
    }

    void init(void) {
        for (Attachable& a : attachables) a.OnInit();
    }

    void RegisterLocal(Attachable& a) {
        attachables.push_back(a);
    }

    Attachable& GetAttachable(byte index) {
        return attachables[index];
    }

    String GetAttachableDisplayData(void) {
        String result;

        byte i = 0;
        for (Attachable& a : attachables) {
            char index[4];
            sprintf(index, "%03.3i", i++);

            result += index;
            result += ": ";
            result += a.DisplayName;
            result += " (";
            result += a.Description;
            result += ")\n";
        }

        return result;
    }

    byte size(void) const {
        return attachables.size();
    }
private:
    Vector<Attachable> attachables;
};*/