/****************************************************************************************
 *   FileName    : NowPlayMusic.qml
 *   Description : QML script for now play music gui
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

//TODO: use enum from NowPlayMusic class

TCNowPlayMusic {
	id: nowPlayMusic

	property real playGage: 0

	onChangedPlayTime: {
		playGage = nowPlayMusic.playTime / nowPlayMusic.totalTime
		sliderPlayFg.width = sliderPlayBg.width * playGage
	}

	onVisibleChanged: {
		if (this.visible) {
			nowPlayMusic.OnPlay()
		}
	}

	onChangedStatus: {
		if (nowPlayMusic.status != 1) {
			fileName.pause()
		} else {
			fileName.restart()
		}
	}

	TCAlbumArt {
		id: albumArt
		objectName: "albumArt"

		x: 11
		y: 11
		width: nowPlayMusic.width - albumArt.x * 2
		height: nowPlayMusic.height - albumArt.y * 2

		Image {
			id: backgroundControl

			x: 0
			y: 201
			width: parent.width
			height: albumArt.height - this.y

			visible: nowPlayMusic.currentDevice != 5

			source: "images/nowplay/nowplay_control_bg.png"

			AnimationText {
				id: fileName

				x: 46
				y: 123
				width: 267
				height: 25

				color: "white"
				text: nowPlayMusic.fileName
				size: 18
			}

			Text {
				id: playTime

				x: 46
				y: 62
				width: 45
				height: 14

				color: "white"
				text: {
					return nowPlayMusic.GetTimeString(nowPlayMusic.playTime)
				}

				font.pixelSize: 15
				renderType: Text.NativeRendering
			}

			Text {
				id: totalTime

				x: parent.width - 91
				y: 62
				width: 45
				height: 14

				color: "white"
				text: {
					return nowPlayMusic.GetTimeString(nowPlayMusic.totalTime)
				}

				font.pixelSize: 15
				renderType: Text.NativeRendering
			}

			Image {
				id: device

				x: 46
				y: 93
				width: 85
				height: 20

				source: {
					if (nowPlayMusic.currentDevice == 1) {
						return "images/nowplay/nowplay_device_usb.png"
					} else if (nowPlayMusic.currentDevice == 2) {
						return "images/nowplay/nowplay_device_usb.png"
					} else if (nowPlayMusic.currentDevice == 3) {
						return "images/nowplay/nowplay_device_usb.png"
					} else if (nowPlayMusic.currentDevice == 4) {
						return "images/nowplay/nowplay_device_sdmmc.png"
					} else if (nowPlayMusic.currentDevice == 6) {
						return "images/nowplay/nowplay_device_iap.png"
					} else if (nowPlayMusic.currentDevice == 7) {
						return "images/nowplay/nowplay_device_bluetooth.png"
					} else {
						return ""
					}
				}
			}

			Item {
				x: 46
				y: 81
				width: parent.width - 92
				height: 2

				Image {
					id: sliderPlayBg

					anchors.fill: parent

					source: "images/nowplay/nowplay_progress_bg.png"
				}

				Image {
					id: sliderPlayFg

					width: 0
					height: 2

					source: "images/nowplay/nowplay_progress_fg.png"
				}
			}

			ImageButton {
				id: playButton

				x: (parent.width - 64) / 2
				y: 4
				width: 66
				height: 69

				visible: nowPlayMusic.status != 1

				source: pressed ? "images/nowplay/nowplay_play_btn_p.png" : "images/nowplay/nowplay_play_btn_n.png"

				onClicked: {
					nowPlayMusic.OnClickedPlayResume();
				}
			}

			ImageButton {
				id: pauseButton

				x: (parent.width - 64) / 2
				y: 4
				width: 66
				height: 69

				visible: nowPlayMusic.status == 1

				source: pressed ? "images/nowplay/nowplay_pause_btn_p.png" : "images/nowplay/nowplay_pause_btn_n.png"

				onClicked: {
					nowPlayMusic.OnClickedPlayPause();
				}
			}

			ImageButton {
				id: rewindButton

				x: 44
				y: 4
				width: 64
				height: 51

				source: pressed ? "images/nowplay/nowplay_prev_btn_p.png" : "images/nowplay/nowplay_prev_btn_n.png"

				onClicked: {
					nowPlayMusic.OnClickedTrackDown();
				}
			}

			ImageButton {
				id: forwardButton

				x: parent.width - 108
				y: 4
				width: 64
				height: 51

				source: pressed ? "images/nowplay/nowplay_next_btn_p.png" : "images/nowplay/nowplay_next_btn_n.png"

				onClicked: {
					nowPlayMusic.OnClickedTrackUp();
				}
			}
			//SequentialAnimation on x {
				//loops: Animation.Infinite
				//running: true
				//PropertyAnimation {
					//target: fileName
					//from: 46
					//to: 313
					//duration: 10000
				//}
			//}
		}

		Text {
			id: aux

			x: 0
			y: backgroundControl.y
			width: backgroundControl.width
			height: backgroundControl.height

			visible: nowPlayMusic.currentDevice == 5

			color: "black"
			text: "AUX"

			font.pixelSize: 35
			horizontalAlignment: Text.AlignHCenter
			verticalAlignment: Text.AlignVCenter
			renderType: Text.NativeRendering
		}
	}
}
