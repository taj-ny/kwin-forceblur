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

        if (rule->drawImage->enabled) {
            properties->drawImage = rule->drawImage->value;
        }
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

        if (rule->topCornerRadius->enabled || rule->bottomCornerRadius->enabled) {
            QRegion square = QRegion(0, 0, rule->topCornerRadius->value, rule->topCornerRadius->value);
            QRegion circle = QRegion(0, 0, 2 * rule->topCornerRadius->value, 2 * rule->topCornerRadius->value, QRegion::RegionType::Ellipse);
            properties->topLeftCorner = QRegion(0, 0, rule->topCornerRadius->value, rule->topCornerRadius->value);
            properties->topRightCorner = QRegion(0, 0, rule->topCornerRadius->value, rule->topCornerRadius->value);

            properties->topLeftCorner &= circle;
            properties->topLeftCorner ^= square;
            circle.translate(-rule->topCornerRadius->value, 0);
            properties->topRightCorner &= circle;
            properties->topRightCorner ^= square;

            square = QRegion(0, 0, rule->bottomCornerRadius->value, rule->bottomCornerRadius->value);
            circle = QRegion(0, 0, 2 * rule->bottomCornerRadius->value, 2 * rule->bottomCornerRadius->value, QRegion::RegionType::Ellipse);

            properties->bottomLeftCorner = QRegion(0, 0, rule->bottomCornerRadius->value, rule->bottomCornerRadius->value);
            properties->bottomRightCorner = QRegion(0, 0, rule->bottomCornerRadius->value, rule->bottomCornerRadius->value);
            circle.translate(0, -rule->bottomCornerRadius->value);
            properties->bottomLeftCorner &= circle;
            properties->bottomLeftCorner ^= square;

            circle.translate(0, rule->bottomCornerRadius->value);
            circle.translate(-rule->bottomCornerRadius->value, 0);
            circle.translate(0, -rule->bottomCornerRadius->value);

            properties->bottomRightCorner &= circle;
            properties->bottomRightCorner ^= square;
        }
    }

    return properties;
}

}