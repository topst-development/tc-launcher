/****************************************************************************************
 *   FileName    : NowPlayCarplay.qml
 *   Description : QML script for now play caplry gui
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

TCNowPlayCarplay {
	id: nowPlayCarplay

	Image {
		id: background

		x: 11
		y: 11
		width: nowPlayCarplay.width - background.x * 2
		height: nowPlayCarplay.height - background.y * 2

		source: "images/nowplay/nowplay_albumart.png"

		Text {
			x: 10
			y: 10
			width: parent.width
			height: parent.height

			color: "black"
			text: "Carplay"
			font.pixelSize: 40
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}
	}
}
