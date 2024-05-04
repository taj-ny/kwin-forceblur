/*
    SPDX-FileCopyrightText: 2010 Fredrik Höglund <fredrik@kde.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/
#include "blur_config.h"
#include "config/config.h"

//#include <config-kwin.h>

// KConfigSkeleton

#include <KPluginFactory>
#include <QAbstractItemView>
#include <QCheckBox>
#include <QLabel>
#include "kwineffects_interface.h"

namespace KWin
{

K_PLUGIN_CLASS(BlurEffectConfig)

BlurEffectConfig::BlurEffectConfig(QObject *parent, const KPluginMetaData &data)
    : KCModule(parent, data)
{
    ui.setupUi(widget());
    BlurConfig::instance()->read();

    ui.rulesTable->horizontalHeader()->setStretchLastSection(true);
    ui.rulesTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.rulesTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.rulesTable->setRowCount(BlurConfig::instance()->rules()->rules.size());

    int row = 0;
    for (const auto &rule : BlurConfig::instance()->rules()->rules) {
        auto checkBox = new QCheckBox();
        checkBox->setChecked(rule->enabled);
        connect(checkBox, &QCheckBox::checkStateChanged, [=](const Qt::CheckState &checkState) {
            rule->enabled = checkState == Qt::CheckState::Checked;
            markAsChanged();
        });

        ui.rulesTable->setCellWidget(row, 0, checkBox);
        ui.rulesTable->setCellWidget(row, 1, new QLabel(rule->name));
        row++;
    }
}

BlurEffectConfig::~BlurEffectConfig()
{
}

void BlurEffectConfig::save()
{
    KCModule::save();
    BlurConfig::instance()->write();

    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"),
                                         QStringLiteral("/Effects"),
                                         QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("forceblur"));
}

} // namespace KWin

#include "blur_config.moc"

#include "moc_blur_config.cpp"
