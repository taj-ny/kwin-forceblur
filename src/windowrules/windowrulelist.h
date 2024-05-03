#pragma once

#include <vector>
#include "effect/effectwindow.h"
#include "mergedwindowruleproperties.h"
#include "windowrule.h"
#include "windowruleproperty.h"

#include <KConfig>

namespace KWin
{

class WindowRuleList
{

public:
    std::vector<WindowRule*> rules;

    void clear();
    const MergedWindowRuleProperties *getProperties(EffectWindow *window) const;
};

}