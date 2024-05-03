#pragma once

#include <QRegion>
#include <QString>

namespace KWin
{

struct MergedWindowRuleProperties
{
    bool drawImage = false;
    QString imagePath;
    bool windowOpacityAffectsBlur = false;
    bool blurDecorations = false;
    bool forceBlur = false;
    bool paintAsTranslucent = false;
    bool topCornerRadius = false;
    bool bottomCornerRadius = false;
    bool roundCornersWhenMaximized = false;

    QRegion topLeftCorner;
    QRegion topRightCorner;
    QRegion bottomLeftCorner;
    QRegion bottomRightCorner;
};

}