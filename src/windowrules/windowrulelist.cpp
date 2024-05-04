#include "windowrulelist.h"

#include <ranges>
#include "window.h"

namespace KWin
{

void WindowRuleList::clear()
{
    for (const auto &rule : rules) {
        delete rule;
    }
    rules.clear();
}

const MergedWindowRuleProperties *WindowRuleList::getProperties(EffectWindow *window) const
{
    auto properties = new MergedWindowRuleProperties();
    for (auto rule : std::ranges::reverse_view(rules)) {
        if (!rule->matches(window))
            continue;

        if (rule->imagePath->enabled) {
            properties->imagePath = rule->imagePath->value;
        }
        if (rule->windowOpacityAffectsBlur->enabled) {
            properties->windowOpacityAffectsBlur = rule->windowOpacityAffectsBlur->value;
        }
        if (rule->blurDecorations->enabled) {
            properties->blurDecorations = rule->blurDecorations->value;
        }
        if (rule->forceBlur->enabled) {
            properties->forceBlur = rule->forceBlur->value;
        }
        if (rule->paintAsTranslucent->enabled) {
            properties->paintAsTranslucent = rule->paintAsTranslucent->value;
        }
        if (rule->topCornerRadius->enabled) {
            properties->topCornerRadius = rule->topCornerRadius->value;
        }
        if (rule->bottomCornerRadius->enabled) {
            properties->bottomCornerRadius = rule->bottomCornerRadius->value;
        }
        if (rule->roundCornersWhenMaximized->enabled) {
            properties->roundCornersWhenMaximized = rule->roundCornersWhenMaximized->value;
        }
    }

    if (properties->topCornerRadius || properties->bottomCornerRadius) {
        QRegion square = QRegion(0, 0, properties->topCornerRadius, properties->topCornerRadius);
        QRegion circle = QRegion(0, 0, 2 * properties->topCornerRadius, 2 * properties->topCornerRadius, QRegion::RegionType::Ellipse);
        properties->topLeftCorner = QRegion(0, 0, properties->topCornerRadius, properties->topCornerRadius);
        properties->topRightCorner = QRegion(0, 0, properties->topCornerRadius, properties->topCornerRadius);

        properties->topLeftCorner &= circle;
        properties->topLeftCorner ^= square;
        circle.translate(-properties->topCornerRadius, 0);
        properties->topRightCorner &= circle;
        properties->topRightCorner ^= square;

        square = QRegion(0, 0, properties->bottomCornerRadius, properties->bottomCornerRadius);
        circle = QRegion(0, 0, 2 * properties->bottomCornerRadius, 2 * properties->bottomCornerRadius, QRegion::RegionType::Ellipse);

        properties->bottomLeftCorner = QRegion(0, 0, properties->bottomCornerRadius, properties->bottomCornerRadius);
        properties->bottomRightCorner = QRegion(0, 0, properties->bottomCornerRadius, properties->bottomCornerRadius);
        circle.translate(0, -properties->bottomCornerRadius);
        properties->bottomLeftCorner &= circle;
        properties->bottomLeftCorner ^= square;

        circle.translate(0, properties->bottomCornerRadius);
        circle.translate(-properties->bottomCornerRadius, 0);
        circle.translate(0, -properties->bottomCornerRadius);

        properties->bottomRightCorner &= circle;
        properties->bottomRightCorner ^= square;
    }

    return properties;
}

}