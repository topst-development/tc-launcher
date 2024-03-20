/****************************************************************************************
 *   FileName    : StatusWidget.qml
 *   Description : QML script for status bar gui
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

TCStatus {
	id: statusWidget

	Image {
		anchors.fill: parent

		source: "images/osd/osd_background.png"

		Row {
			x: (statusWidget.width - 358) / 2
			y: 12
			width: 358
			height: 40

			spacing: 13

			Image {
				id: usb1Icon

				width: 40
				height: 40

				visible: statusWidget.usb1

				source: "images/osd/osd_usb.png"
			}

			Image {
				id: usb2Icon

				width: 40
				height: 40

				visible: statusWidget.usb2

				source: "images/osd/osd_usb.png"
			}

			Image {
				id: usb3Icon

				width: 40
				height: 40

				visible: statusWidget.usb3

				source: "images/osd/osd_usb.png"
			}

			Image {
				id: sdmmcIcon

				width: 40
				height: 40

				visible: statusWidget.sdmmc

				source: "images/osd/osd_sdmmc.png"
			}

			Image {
				id: auxIcon

				width: 40
				height: 40

				visible: statusWidget.aux

				source: "images/osd/osd_aux.png"
			}

			Image {
				id: iapIcon

				width: 40
				height: 40

				visible: statusWidget.iAP2

				source: "images/osd/osd_iap.png"
			}

			Image {
				id: carplayIcon

				width: 40
				height: 40

				visible: statusWidget.carPlay

				source: "images/osd/osd_carplay.png"
			}

			Image {
				id: bluetoothIcon

				width: 40
				height: 40

				visible: statusWidget.bluetooth

				source: "images/osd/osd_bluetooth.png"
			}
		}
	}
}

