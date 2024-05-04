#pragma once

#include <map>
#include <vector>
#include "effect/effectwindow.h"
#include "windowrules/windowrulelist.h"
#include "windowrules/windowruleproperty.h"

#include <KConfig>

namespace KWin
{

class BlurConfig
{

public:
    static BlurConfig *instance();

    int blurStrength = 15;
    int noiseStrength = 5;

    WindowRuleList *rules() const;

    void migrateFrom(int version);
   
    void read();

    template <typename T>
    void readProperty(const KConfigGroup &propertiesGroup, const QString &name, WindowRuleProperty<T> *property);

    void write() const;

    template <typename T>
    void writeProperty(KConfigGroup &propertiesGroup, const QString &name, WindowRuleProperty<T> *property) const;

private:
    BlurConfig();

    inline static BlurConfig *s_instance;

    int m_version = 0;
    bool m_migratedFromV1 = false;

    WindowRuleList *m_rules;
};

}