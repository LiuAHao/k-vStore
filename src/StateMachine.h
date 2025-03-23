#pragma once
#include "LogEntry.h"

class StateMachine {
public:
    virtual ~StateMachine() {}
    virtual void apply(const LogEntry& entry) = 0;
};