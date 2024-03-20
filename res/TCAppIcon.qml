/****************************************************************************************
 *   FileName    : TCAppIcon.qml
 *   Description : QML script for app icon gui
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
import Launcher 1.0

Item {
	id: appIcon

    signal clicked

	property bool active: false
	property var imageNames: ["", "", ""]
	property int myID: -1

	onActiveChanged: {
		console.log("onActiveChanged");
		if (active)
		{
			iconImageNormal.visible = true;
			iconImageDisable.visible = false;
			mouseArea.enabled = true;
		}
		else
		{
			iconImageDisable.visible = true;
			iconImageNormal.visible = false;
			mouseArea.enabled = false;
		}
	}

	states: [
		State {
			name: "PRESSED"
			PropertyChanges {
				target: iconImagePressed
				visible: true
			}
			PropertyChanges {
				target: iconImageNormal
				visible: false
			}
			PropertyChanges {
				target: iconImageDisable
				visible: false
			}
		},
		State {
			name: "RELEASED"
			PropertyChanges {
				target: iconImagePressed
				visible: false
			}
			PropertyChanges {
				target: active ? iconImageNormal : iconImageDisable
				visible: true
			}
		}
	]

	Image {
		id: iconImageDisable

		source: "image://extern/" + imageNames[0]
	}
	Image {
		id: iconImageNormal

		visible: false
		source: "image://extern/" + imageNames[1]
	}
	Image {
		id: iconImagePressed

		visible: false
		source: "image://extern/" + imageNames[2]
	}

	MouseArea {
		id: mouseArea

		anchors.fill: iconImageDisable
		enabled: false

		onPressed: {
			appIcon.state = "PRESSED"
		}

		onReleased: {
			appIcon.state = "RELEASED"
		}

		onClicked: {
			appIcon.clicked()
		}
	}
}
