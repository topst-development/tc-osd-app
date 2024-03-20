/****************************************************************************************
 *   FileName    : OSDStatusWidget.qml
 *   Description : QML script for osd status widget gui
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided “AS IS” and nothing contained in this source code
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
import OSDApp 1.0
import "types"

TCOSD {
	id: osdStatusWidget

	signal showWindow
	signal hideWindow

	Image {
		id: osdBG

		anchors.fill: parent

		source: {
			if (osdStatusWidget.type == osdStatusWidget.OSDTypePhone) {
				return "images/osd_bg_top_phone.png"
			} else if (osdStatusWidget.type == osdStatusWidget.OSDTypePhone) {
				return "images/osd_bg_top_setting.png"
			} else {
				return "images/osd_bg_top_media.png"
			}
		}

		ImageButton {
			id: homeButton

			x: 43
			y: 5
			width: 60
			height: 55

			source: {
				pressed ? "images/osd_home_btn_p.png" : "images/osd_home_btn_n.png"
			}

			onClicked: {
				console.log("Go to Home")
				osdStatusWidget.goToHome()
			}
		}

		Row {
			x: 168

			spacing: 0

			ImageButton {
				id: volumeMin

				width: 59
				height: 55

				source: {
					if (osdStatusWidget.volume == 0) {
						return pressed ? "images/osd_vol_mute_btn_p.png" : "images/osd_vol_mute_btn_n.png"
					} else {
						return pressed ? "images/osd_vol_down_btn_p.png" : "images/osd_vol_down_btn_n.png"
					}
				}

				onClicked: {
					console.log("Volume down")
					osdStatusWidget.decreaseVolume()
				}
			}

			Image {
				id: volume

				y: 23
				width: 168
				height: 13

				source: {
					if (osdStatusWidget.volume == 10) {
						return "images/osd_vol_gage_10.png"
					} else {
						return "images/osd_vol_gage_0" + osdStatusWidget.volume + ".png"
					}
				}
			}

			ImageButton {
				id: volumePlus

				width: 59
				height: 55

				source: pressed ? "images/osd_vol_up_btn_p.png" : "images/osd_vol_up_btn_n.png"

				onClicked: {
					console.log("Volume up")
					osdStatusWidget.increaseVolume()
				}
			}
		}

		Row {
			id: icons

			x: 516
			y: 12
			width: 358
			height: 40

			spacing: 13

			Item {
				width: 40
				height: 40

				Image {
					id: usb1

					width: 40
					height: 40

					visible: osdStatusWidget.usb1

					source: "images/osd_status_usb.png"
				}

				Image {
					id: usb2

					width: 40
					height: 40

					visible: osdStatusWidget.usb2

					source: "images/osd_status_usb.png"
				}
			}

			Image {
				id: sdmmc

				width: 40
				height: 40

				visible: osdStatusWidget.sdmmc

				source: "images/osd_status_sdmmc.png"
			}

			Image {
				id: aux

				width: 40
				height: 40

				visible: osdStatusWidget.aux

				source: "images/osd_status_aux.png"
			}

			Image {
				id: iap

				width: 40
				height: 40

				visible: osdStatusWidget.iAP2

				source: "images/osd_status_iap.png"
			}

			Image {
				id: carplay

				width: 40
				height: 40

				visible: osdStatusWidget.carPlay

				source: "images/osd_status_carplay.png"
			}

			Image {
				id: bluetooth

				width: 40
				height: 40

				visible: osdStatusWidget.bluetooth

				source: "images/osd_status_bluetooth.png"
			}
		}

		Item {
			id: clock

			x: 884
			y: 22

			property int hours: 0
			property int minutes: 0
			property int seconds: 0

			function timeChanged() {
				var date = new Date;
				var index;

				hours = date.getHours();
				minutes = date.getMinutes();

				if (hours >= 12) {
					meridian.source = "images/osd_time_am.png"
					hours -= 12;
				} else {
					meridian.source = "images/osd_time_pm.png"
				}

				index = parseInt(hours / 10);
				hour1.source = "images/osd_time_num_" + index + ".png"

				index = hours % 10;
				hour2.source = "images/osd_time_num_" + index + ".png"

				index = parseInt(minutes / 10);
				min1.source = "images/osd_time_num_" + index + ".png"

				index = minutes % 10;
				min2.source = "images/osd_time_num_" + index + ".png"
			}

			Image {
				id: meridian

				x: 0
				y: 6
				width: 28
				height: 17

				source: ""
			}

			Image {
				id: hour1

				x: 30
				y: 0
				width: 14
				height: 21

				source: ""
			}

			Image {
				id: hour2

				x: 44
				y: 0
				width: 14
				height: 21

				source: ""
			}

			Image {
				id: colon

				x: 58
				y: 0
				width: 5
				height: 21

				source: "images/osd_time_dot.png"
			}

			Image {
				id: min1

				x: 63
				y: 0
				width: 14
				height: 21

				source: ""
			}

			Image {
				id: min2

				x: 77
				y: 0
				width: 14
				height: 21

				source: ""
			}
		}
	}

	Timer {
		id: updateTimer
		interval: 1000
		repeat: true
		running: true
		triggeredOnStart: true

		onTriggered: clock.timeChanged()
	}
}
