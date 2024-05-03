#pragma once

#include <QRegion>
#include <QString>

namespace KWin
{

struct MergedWindowRuleProperties
{
    QString imagePath;
    bool windowOpacityAffectsBlur = false;
    bool blurDecorations = false;
    bool forceBlur = false;
    bool paintAsTranslucent = false;
    int topCornerRadius = false;
    int bottomCornerRadius = false;
    bool roundCornersWhenMaximized = false;

    QRegion topLeftCorner;
    QRegion topRightCorner;
    QRegion bottomLeftCorner;
    QRegion bottomRightCorner;
};

}