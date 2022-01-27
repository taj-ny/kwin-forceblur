/*
    SPDX-FileCopyrightText: 2020 Aleix Pol Gonzalez <aleixpol@kde.org>

    SPDX-License-Identifier: GPL-2.0-only OR GPL-3.0-only OR LicenseRef-KDE-Accepted-GPL
*/


import QtQuick 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 2.3 as QQC2
import org.kde.kirigami 2.6 as Kirigami
import org.kde.kcm 1.3 as KCM
import org.kde.kwin.kwinxwaylandsettings 1.0
import org.kde.kquickcontrols 2.0

KCM.SimpleKCM {
    id: root
    KCM.ConfigModule.buttons: KCM.ConfigModule.Default | KCM.ConfigModule.Apply
    KCM.ConfigModule.quickHelp: i18n("This module lets configure which keyboard events are forwarded to X11 apps regardless of their focus.")
    KCM.SettingStateBinding {
        configObject: kcm.settings
        settingName: "Xwayland"
    }
    implicitWidth: Kirigami.Units.gridUnit * 48
    implicitHeight: Kirigami.Units.gridUnit * 33

    QQC2.ButtonGroup {
        buttons: column.children
        exclusive: true
        checkedButton: buttons[kcm.settings.xwaylandEavesdrops]
        onCheckedButtonChanged: {
            let idx = -1
            for (const x in buttons) {
                if (buttons[x] === checkedButton) {
                    idx = x
                }
            }
            kcm.settings.xwaylandEavesdrops = idx
        }
    }

    ColumnLayout {
        id: column
        QQC2.Label {
            Layout.fillWidth: true
            text: i18n("Allow legacy X11 apps to read keystrokes typed in all apps:")
        }

        QQC2.RadioButton {
            text: i18n("Never")
        }
        QQC2.RadioButton {
            text: i18n("Only Meta, Control, Alt, and Shift keys")
        }
        QQC2.RadioButton {
            text: i18n("All keys, but only while Meta, Ctrl, Alt, or Shift keys are pressed")
        }
        QQC2.RadioButton {
            id: always
            text: i18n("Always")
        }

        Kirigami.InlineMessage {
            Layout.fillWidth: true
            type: Kirigami.MessageType.Warning
            text: i18n("Note that using this setting will reduce system security to that of the X11 session by permitting malicious software to steal passwords and spy on the text that you type. Make sure you understand and accept this risk.")
            visible: always.checked
        }
    }
}
