/****************************************************************************************
 *   FileName    : ClockWidget.qml
 *   Description : QML script for clock gui
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
import "types"

Item {
	id: clockWidget

	property var size: "large"

	function timeChanged() {
		var date = new Date
		var index
		var hours = date.getHours()
		var minutes = date.getMinutes()

		if (hours >= 12) {
			timeSmallMeridian.source = "images/home_screen/home_time_s_pm.png"
			timeLargeMeridian.source = "images/home_screen/home_time_l_pm.png"
			hours -= 12
		} else {
			timeSmallMeridian.source = "images/home_screen/home_time_s_am.png"
			timeLargeMeridian.source = "images/home_screen/home_time_l_am.png"
		}

		index = parseInt(hours / 10)
		timeSmallHour1.source = "images/home_screen/home_time_s_num_" + index + ".png"
		timeLargeHour1.source = "images/home_screen/home_time_l_num_" + index + ".png"

		index = hours % 10
		timeSmallHour2.source = "images/home_screen/home_time_s_num_" + index + ".png"
		timeLargeHour2.source = "images/home_screen/home_time_l_num_" + index + ".png"

		index = parseInt(minutes / 10)
		timeSmallMin1.source = "images/home_screen/home_time_s_num_" + index + ".png"
		timeLargeMin1.source = "images/home_screen/home_time_l_num_" + index + ".png"

		index = minutes % 10
		timeSmallMin2.source = "images/home_screen/home_time_s_num_" + index + ".png"
		timeLargeMin2.source = "images/home_screen/home_time_l_num_" + index + ".png"
	}

	Item {
		id: smallClock

		visible: clockWidget.size == "small" ? true : false

		Image {
			id: timeSmallHour1

			x: 286
			y: 101
			width: 25
			height: 36

			source: ""
		}

		Image {
			id: timeSmallHour2

			x: 311
			y: 101
			width: 25
			height: 36

			source: ""
		}

		Image {
			id: timeSmallColon

			x: 336
			y: 106
			width: 10
			height: 27

			source: "images/home_screen/home_time_s_dot.png"
		}

		Image {
			id: timeSmallMin1

			x: 344
			y: 101
			width: 25
			height: 36

			source: ""
		}

		Image {
			id: timeSmallMin2

			x: 369
			y: 101
			width: 25
			height: 36

			source: ""
		}

		Image {
			id: timeSmallMeridian

			x: 237
			y: 112
			width: 45
			height: 28

			source: ""
		}
	}

	Item {
		id: largeClock

		visible: clockWidget.size == "large" ? true : false


		Image {
			id: timeLargeHour1

			x: 176
			y: 247
			width: 70
			height: 105

			source: ""
		}

		Image {
			id: timeLargeHour2

			x: 246
			y: 247
			width: 70
			height: 105

			source: ""
		}

		Image {
			id: timeLargeColon

			x: 316
			y: 273
			width: 20
			height: 54

			source: "images/home_screen/home_time_l_dot.png"
		}

		Image {
			id: timeLargeMin1

			x: 339
			y: 247
			width: 70
			height: 105

			source: ""
		}

		Image {
			id: timeLargeMin2

			x: 409
			y: 247
			width: 70
			height: 105

			source: ""
		}

		Image {
			id: timeLargeMeridian

			x: 306
			y: 189
			width: 55
			height: 35

			source: ""
		}
	}

	Timer {
		id: updateTimer

		interval: 1000
		repeat: true
		running: true
		triggeredOnStart: true

		onTriggered: {
			clockWidget.timeChanged()
		}
	}
}
