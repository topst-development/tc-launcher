/****************************************************************************************
 *   FileName    : NowPlayRadio.qml
 *   Description : QML script for now play radio gui
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

TCNowPlayRadio {
	id: nowPlayRadio

	Image {
		id: background

		x: 11
		y: 11
		width: nowPlayRadio.width - background.x * 2
		height: nowPlayRadio.height - background.y * 2

		source: "images/nowplay/nowplay_albumart.png"

		MouseArea {
			x: 10
			y: 10
			width: 70
			height: 50

			onClicked: {
				nowPlayRadio.OnClickedRadioType()
			}

			Text {
				id: radioType

				anchors.fill: parent

				color: "yellow"
				text: {
					if (nowPlayRadio.type == 0) {
						return "FM"
					} else if (nowPlayRadio.type == 1) {
						return "AM"
					} else if (nowPlayRadio.type == 2) {
						return "DAB"
					} else if (nowPlayRadio.type == 3) {
						return "DRM"
					} else {
						return "---"
					}
				}

				font.pixelSize: 18
			}
		}

		Text {
			id: frequency

			x: 0
			y: 0
			width: parent.width
			height: 212

			color: "black"
			text: nowPlayRadio.frequency == "" ? "------" : nowPlayRadio.frequency

			font.pixelSize: 33
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}

		Text {
			id: hdRadio

			x: 10
			y: 10
			width: parent.width
			height: 50

			visible: nowPlayRadio.hdRadio
			color: "black"
			text: "HD Radio"

			font.pixelSize: 23
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
		}

		Image {
			id: backgroundControl

			x: 0
			y: 190
			width: 360
			height: 170

			source: "images/nowplay/nowplay_control_bg.png"

			Grid {
				anchors.centerIn: parent

				columns: 3
				spacing: 10

				Repeater {
					model: nowPlayRadio.presets

					MouseArea {
						width: 110
						height: 80

						onClicked: {
							nowPlayRadio.OnClickedPreset(index)
						}

						Text {
							anchors.fill: parent

							color: "yellow"
							text: modelData

							font.pixelSize: 18
							horizontalAlignment: Text.AlignHCenter
							verticalAlignment: Text.AlignVCenter
						}
					}
				}
			}
		}
	}
}
