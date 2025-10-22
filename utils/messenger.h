#pragma once

#include "logger.h"
#include "../ui/gui/igui.h"
#include "../common/constants.h"

using namespace std;

class Messenger {
public:
    Messenger() = default;
    Messenger(const Messenger&) = delete;
    Messenger& operator=(const Messenger&) = delete;

    ~Messenger() {
        stopLogger();
    }

    static Messenger& getInstance() {
        static Messenger instance; // Guaranteed to be created once
        return instance;
    }

    void send(const int type, const string& source, const string& message) {
        switch (type) {
            case MSG_ALL: sendToLogger(source, message); sendToGui(message); break;
            case MSG_LOG: sendToLogger(source, message); break;
            case MSG_GUI: sendToGui(message); break;
            default:
                throw new runtime_error("unexpected message type");
        }
    }

    void registerGui(IGui& gui) {
        this->gui = &gui;
    }

    void startLogger() {
        logger.on();
    }

    void stopLogger() {
        logger.off();
    }

private:
    Logger& logger = Logger::getInstance();
    IGui* gui = new IGui();
    bool console = false;

    void sendToLogger(const string& source, const string& message) {
        logger.log(format("{}: {}", source, message));
    }

    void sendToGui(const string& message) {
        gui->showMessage(message);
    }

};