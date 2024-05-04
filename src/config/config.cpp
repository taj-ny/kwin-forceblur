#include "config.h"
#include <KConfig>
#include <KConfigGroup>
#include <QImage>
#include <QLoggingCategory>

const char *configName = "kwin-effects-forceblurrc";
const int configVersion = 2;

namespace KWin
{

BlurConfig::BlurConfig()
{
    m_rules = new WindowRuleList();
}

BlurConfig *BlurConfig::instance()
{
    if (s_instance == nullptr)
        s_instance = new BlurConfig();

    return s_instance;
}

WindowRuleList *BlurConfig::rules() const
{
    return m_rules;
}

void BlurConfig::migrateFrom(int version)
{
    if (version != 1)
        return;

    KConfig kwinrc("kwinrc", KConfig::SimpleConfig);
    if (!kwinrc.hasGroup("Effect-blurplus"))
        return;

    auto forceBlurGroup = kwinrc.group("Effect-blurplus");
    auto v1Rule = new WindowRule();

    blurStrength = forceBlurGroup.readEntry("BlurStrength", 15);
    noiseStrength = forceBlurGroup.readEntry("NoiseStrength", 5);

    v1Rule->name = "v1 configuration";
    v1Rule->matchWindowsByClass = true;
    v1Rule->windowClasses = forceBlurGroup.readEntry("WindowClasses", QString("class1\nclass2\nclass3").split("\n"));
    v1Rule->windowClassWhitelist = forceBlurGroup.readEntry("BlurMatching", true);

    v1Rule->matchWindowsByType = true;
    v1Rule->matchNormalWindows = true;
    v1Rule->matchDialogWindows = true;
    v1Rule->matchUtilityWindows = true;
    v1Rule->matchDockWindows = forceBlurGroup.readEntry("BlurDocks", false);
    v1Rule->matchToolbarWindows = true;
    v1Rule->matchContextMenuWindows = forceBlurGroup.readEntry("BlurDocks", false);

    v1Rule->imagePath->value = forceBlurGroup.readEntry("FakeBlurImage", "");
    v1Rule->imagePath->enabled = forceBlurGroup.readEntry("FakeBlur", false);
    v1Rule->windowOpacityAffectsBlur->enabled = v1Rule->windowOpacityAffectsBlur->value = forceBlurGroup.readEntry("TransparentBlur", true);
    v1Rule->blurDecorations->enabled = v1Rule->blurDecorations->value = forceBlurGroup.readEntry("BlurDecorations", false);
    v1Rule->forceBlur->enabled = v1Rule->forceBlur->value = true;
    v1Rule->paintAsTranslucent->enabled = v1Rule->paintAsTranslucent->value = forceBlurGroup.readEntry("PaintAsTranslucent", false);
    v1Rule->topCornerRadius->enabled = v1Rule->topCornerRadius->value = forceBlurGroup.readEntry("TopCornerRadius", 0);
    v1Rule->bottomCornerRadius->enabled = v1Rule->bottomCornerRadius->value = forceBlurGroup.readEntry("BottomCornerRadius", 0);
    v1Rule->roundCornersWhenMaximized->enabled = v1Rule->roundCornersWhenMaximized->value = forceBlurGroup.readEntry("RoundCornersOfMaximizedWindows", false);

    m_rules->rules.insert(m_rules->rules.begin(), v1Rule);
}

template <typename T>
void BlurConfig::readProperty(const KConfigGroup &propertiesGroup, const QString &name, WindowRuleProperty<T> *property)
{
    auto propertyGroup = propertiesGroup.group(name);
    property->enabled = propertyGroup.readEntry("Enabled", property->enabled);
    property->value = propertyGroup.readEntry("Value", property->defaultValue);
}

void BlurConfig::read()
{
    m_rules->clear();

    KConfig config(configName, KConfig::SimpleConfig);
    auto root = config.group("");
    m_version = root.readEntry("Version", configVersion);
    m_migratedFromV1 = root.readEntry("MigratedFromV1", m_migratedFromV1);

    if (!m_migratedFromV1) {
        migrateFrom(1);
        m_migratedFromV1 = true;
        write();
        read();
        return;
    }

    auto blurGroup = config.group("Blur");
    blurStrength = blurGroup.readEntry("BlurStrength", blurStrength);
    noiseStrength = blurGroup.readEntry("NoiseStrength", noiseStrength);

    auto rulesGroup = config.group("Rules");
    int i = 0;
    for (const auto &ruleGroupName : rulesGroup.groupList()) {
        auto *rule = new WindowRule();
        m_rules->rules.push_back(rule);

        auto ruleGroup = rulesGroup.group(ruleGroupName);
        rule->enabled = ruleGroup.readEntry("Enabled", rule->enabled);
        rule->name = ruleGroup.readEntry("Name", "Window rule " + QString::number(i++));

        auto matchingGroup = ruleGroup.group("Matching");
        rule->matchAllWindows = matchingGroup.readEntry("MatchAllWindows", rule->matchAllWindows);

        auto matchingClassGroup = matchingGroup.group("Class");
        rule->matchWindowsByClass = matchingClassGroup.readEntry("Enable", rule->matchWindowsByClass);
        rule->windowClasses = matchingClassGroup.readEntry("Classes", rule->windowClasses);
        rule->windowClassWhitelist = matchingClassGroup.readEntry("Mode", rule->windowClassWhitelist);

        auto matchingTypeGroup = matchingGroup.group("Type");
        rule->matchWindowsByType = matchingTypeGroup.readEntry("Enable", rule->matchWindowsByType);
        rule->matchNormalWindows = matchingTypeGroup.readEntry("Normal", rule->matchNormalWindows);
        rule->matchDialogWindows = matchingTypeGroup.readEntry("Dialog", rule->matchDialogWindows);
        rule->matchUtilityWindows = matchingTypeGroup.readEntry("Utility", rule->matchUtilityWindows);
        rule->matchDockWindows = matchingTypeGroup.readEntry("Dock", rule->matchDockWindows);
        rule->matchToolbarWindows = matchingTypeGroup.readEntry("Toolbar", rule->matchToolbarWindows);
        rule->matchContextMenuWindows = matchingTypeGroup.readEntry("ContextMenu", rule->matchContextMenuWindows);

        auto propertiesGroup = ruleGroup.group("Properties");
        readProperty(propertiesGroup, "ImagePath", rule->imagePath);
        readProperty(propertiesGroup, "WindowOpacityAffectsBlur", rule->windowOpacityAffectsBlur);
        readProperty(propertiesGroup, "BlurDecorations", rule->blurDecorations);
        readProperty(propertiesGroup, "ForceBlur", rule->forceBlur);
        readProperty(propertiesGroup, "PaintAsTranslucent", rule->paintAsTranslucent);
        readProperty(propertiesGroup, "TopCornerRadius", rule->topCornerRadius);
        readProperty(propertiesGroup, "BottomCornerRadius", rule->bottomCornerRadius);
        readProperty(propertiesGroup, "RoundCornersWhenMaximized", rule->roundCornersWhenMaximized);
    }
}

template <typename T>
void BlurConfig::writeProperty(KConfigGroup &propertiesGroup, const QString &name, WindowRuleProperty<T> *property) const
{
    auto propertyGroup = propertiesGroup.group(name);
    propertyGroup.writeEntry("Enabled", property->enabled);
    propertyGroup.writeEntry("Value", property->value);
    propertyGroup.sync();
}

void BlurConfig::write() const
{
    KConfig config(configName, KConfig::SimpleConfig);
    for (const auto &group : config.groupList()) {
        config.deleteGroup(group);
    }

    auto root = config.group("");
    root.writeEntry("Version", m_version);
    root.writeEntry("MigratedFromV1", m_migratedFromV1);
    root.sync();

    auto blurGroup = config.group("Blur");
    blurGroup.writeEntry("BlurStrength", blurStrength);
    blurGroup.writeEntry("NoiseStrength", noiseStrength);
    blurGroup.sync();

    auto rulesGroup = config.group("Rules");
    int i = 0;
    for (const auto &rule : m_rules->rules) {
        auto ruleGroup = rulesGroup.group(QString::number(i++));
        ruleGroup.writeEntry("Enabled", rule->enabled);
        ruleGroup.writeEntry("Name", rule->name);
        ruleGroup.sync();

        auto matchingGroup = ruleGroup.group("Matching");
        matchingGroup.writeEntry("MatchAllWindows", rule->matchAllWindows);
        matchingGroup.sync();

        auto matchingClassGroup = matchingGroup.group("Class");
        matchingClassGroup.writeEntry("Enable", rule->matchWindowsByClass);
        matchingClassGroup.writeEntry("Classes", rule->windowClasses);
        matchingClassGroup.writeEntry("Mode", rule->windowClassWhitelist);
        matchingClassGroup.sync();

        auto matchingTypeGroup = matchingGroup.group("Type");
        matchingTypeGroup.writeEntry("Enable", rule->matchWindowsByType);
        matchingTypeGroup.writeEntry("Normal", rule->matchNormalWindows);
        matchingTypeGroup.writeEntry("Dialog", rule->matchDialogWindows);
        matchingTypeGroup.writeEntry("Utility", rule->matchUtilityWindows);
        matchingTypeGroup.writeEntry("Dock", rule->matchDockWindows);
        matchingTypeGroup.writeEntry("Toolbar", rule->matchToolbarWindows);
        matchingTypeGroup.writeEntry("ContextMenu", rule->matchContextMenuWindows);
        matchingTypeGroup.sync();

        auto propertiesGroup = ruleGroup.group("Properties");
        writeProperty(propertiesGroup, "ImagePath", rule->imagePath);
        writeProperty(propertiesGroup, "WindowOpacityAffectsBlur", rule->windowOpacityAffectsBlur);
        writeProperty(propertiesGroup, "BlurDecorations", rule->blurDecorations);
        writeProperty(propertiesGroup, "ForceBlur", rule->forceBlur);
        writeProperty(propertiesGroup, "PaintAsTranslucent", rule->paintAsTranslucent);
        writeProperty(propertiesGroup, "TopCornerRadius", rule->topCornerRadius);
        writeProperty(propertiesGroup, "BottomCornerRadius", rule->bottomCornerRadius);
        writeProperty(propertiesGroup, "RoundCornersWhenMaximized", rule->roundCornersWhenMaximized);
        propertiesGroup.sync();
    }

    rulesGroup.sync();
    config.sync();
}

}