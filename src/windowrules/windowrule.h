#pragma once

#include <QObject>
#include <QRegion>

#include "effect/effectwindow.h"
#include "windowruleproperty.h"

namespace KWin
{

class WindowRule : public QObject
{
public:
    bool enabled = true;
    QString name;

    bool matchAllWindows = false;

    bool matchWindowsByClass = false;
    QStringList windowClasses;
    enum WindowClassMatchingMode {
        Blacklist,
        Whitelist
    };
    bool windowClassWhitelist = true;

    bool matchWindowsByType = false;
    bool matchNormalWindows = false;
    bool matchDialogWindows = false;
    bool matchUtilityWindows = false;
    bool matchDockWindows = false;
    bool matchToolbarWindows = false;
    bool matchContextMenuWindows = false;


    WindowRuleProperty<bool> *drawImage = new WindowRuleProperty<bool>(false);
    WindowRuleProperty<QString> *imagePath = new WindowRuleProperty<QString>("");
    WindowRuleProperty<bool> *windowOpacityAffectsBlur = new WindowRuleProperty<bool>(true);

    WindowRuleProperty<bool> *blurDecorations = new WindowRuleProperty<bool>(false);
    WindowRuleProperty<bool> *forceBlur = new WindowRuleProperty<bool>(false);
    WindowRuleProperty<bool> *paintAsTranslucent = new WindowRuleProperty<bool>(false);

    WindowRuleProperty<int> *topCornerRadius = new WindowRuleProperty<int>(0);
    WindowRuleProperty<int> *bottomCornerRadius = new WindowRuleProperty<int>(0);
    WindowRuleProperty<bool> *roundCornersWhenMaximized = new WindowRuleProperty<bool>(false);

    bool matches(EffectWindow *w);

private:

};

}