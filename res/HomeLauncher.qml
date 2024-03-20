/****************************************************************************************
 *   FileName    : HomeLauncher.qml
 *   Description : QML script for launcher home gui
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
import "types"

TCHome {
	id: launcherHome

	Image {
		id: homeBackground
		objectName: "homeBackground"

		source: "images/home_screen/home_background.png"

		anchors.fill: parent

		StatusWidget {
			id: statusWidget
			objectName: "statusWidget"

			x: 0
			y: 0
			width: launcherHome.width
			height: 65
		}

		Item {
			anchors.fill: parent

			ClockWidget {
				id: clockWidget

				x: 0
				y: 0
				width: 674
				height: 511

				size: {
					if (launcherHome.nowplaying == true) {
						return "small"
					} else {
						return "large"
					}
				}
			}

			Item { // Consider to alter Loader to NowPlay Class
				id: nowPlay

				x: (parent.width - 382) / 2
				y: 148
				width: 382
				height: 382

				visible: launcherHome.nowplaying

				NowPlayMusic {
					id: nowPlayMusic
					objectName: "nowPlayMusic"

					visible: launcherHome.mediaType == 1

					anchors.fill: parent
				}

				NowPlayRadio {
					id: nowPlayRadio
					objectName: "nowPlayRadio"

					visible: launcherHome.mediaType == 3

					anchors.fill: parent
				}

				NowPlayCarplay {
					id: nowPlayCaplay
					objectName: "nowPlayCarplay"

					visible: launcherHome.mediaType == 4

					anchors.fill: parent
				}
			}
		}

		ImageButton {
			id: appsButton

			x: launcherHome.width - 110
			y: launcherHome.height - 120
			width: 100
			height: 127

			source: pressed ? "/images/home_screen/home_apps_p.png" : "/images/home_screen/home_apps_n.png"

			onClicked: {
				launcherHome.ShowApplications()
			}
		}
	}
}
