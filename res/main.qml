/****************************************************************************************
 *   FileName    : main.qml
 *   Description : QML script for main window
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided "AS IS" and nothing contained in this source code
shall constitute any express or implied warranty of any kind, including without limitation,
any warranty of merchantability, fitness for a particular purpose or non-infringement of any patent,
copyright or other third party intellectual property right.
No warranty is made, express or implied, regarding the information’s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/

import QtQuick 2.6
import QtQuick.Window 2.2
import Launcher 1.0

TCLauncherWin {
	id: launcherMain

	x: Screen.width - 674
	y: 0
	width: 674
	height: Screen.height

	flags: Qt.FramelessWindowHint

	title: qsTr("Telechips Launcher")
	visible: false
	color: "transparent"

	onChangedAppActive: {
		for (var i = 0; i < appRepeater.count; i++) {
			if (appRepeater.itemAt(i).myID == app) {
				console.log(appRepeater.itemAt(i).myID + " app is " + active ? "active" : "inactive")
				appRepeater.itemAt(i).active = active
				break
			}
		}
	}

	HomeLauncher {
		id: launcherHome

		objectName: "launcherHome"

		anchors.fill: parent
		visible: !launcherMain.fullScreen
	}

	Image {
		id: launcherApps

		objectName: "launcherApps"

		anchors.fill: parent
		visible: launcherMain.fullScreen
		source: "/images/linux_avn_background.png"

		Flickable {
			anchors.fill: parent

			Grid {
				anchors.centerIn: parent

				columns: 5
				columnSpacing: Window.width / 8
				rowSpacing: Window.height / 20

				Repeater {
					id: appRepeater

					anchors.fill: parent
					model: launcherMain.appsInfo

					delegate: TCAppIcon {
						width: 132
						height: 167

						active: appActive
						imageNames: iconImageNames
						myID: appID

						onClicked: {
							launcherMain.RequestAppShow(myID)
						}
					}
				}
			}
		}
	}
}
