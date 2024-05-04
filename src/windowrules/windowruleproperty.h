#pragma once

namespace KWin
{

template <typename T> class WindowRuleProperty
{
public:
    explicit WindowRuleProperty(T defaultValue)
        : value(defaultValue), defaultValue(defaultValue)
    {
    }

    bool enabled = false;
    T value;
    T defaultValue;
};

}