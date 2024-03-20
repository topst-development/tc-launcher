/****************************************************************************************
 *   FileName    : StatusWidget.h
 *   Description : StatusWidget.h
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

#ifndef STATUSWIDGET_H
#define STATUSWIDGET_H

#include <QObject>
#include <QQuickItem>
#include "MediaPlayerType.h"

class StatusWidget : public QQuickItem
{
	Q_OBJECT
	Q_ENUMS(ConnectivitySource)
	Q_PROPERTY(bool usb1 READ usb1 NOTIFY deviceChanged)
	Q_PROPERTY(bool usb2 READ usb2 NOTIFY deviceChanged)
	Q_PROPERTY(bool usb3 READ usb3 NOTIFY deviceChanged)
	Q_PROPERTY(bool sdmmc READ sdmmc NOTIFY deviceChanged)
	Q_PROPERTY(bool aux READ aux NOTIFY deviceChanged)
	Q_PROPERTY(bool iAP2 READ iAP2 NOTIFY deviceChanged)
	Q_PROPERTY(bool carPlay READ carPlay NOTIFY deviceChanged)
	Q_PROPERTY(bool bluetooth READ bluetooth NOTIFY deviceChanged)

	public:
		typedef enum {
			ConnectivitySourceUSB1,
			ConnectivitySourceUSB2,
			ConnectivitySourceUSB3,
			ConnectivitySourceSDMMC,
			ConnectivitySourceAux,
			ConnectivitySourceiAP,
			ConnectivitySourceCarPlay,
			ConnectivitySourceBluetooth,
			TotalConnectivitySources
		}ConnectivitySource;
		StatusWidget(QQuickItem *parent = 0);
		void Initialize(void);
		void SetEnableDevice(StatusWidget::ConnectivitySource device);
		void SetDisableDevice(StatusWidget::ConnectivitySource device);

		bool usb1() const;
		bool usb2() const;
		bool usb3() const;
		bool sdmmc() const;
		bool aux() const;
		bool iAP2() const;
		bool carPlay() const;
		bool bluetooth() const;

	signals:
		void deviceChanged();

	private:
		bool	_icoDevice[TotalConnectivitySources];
};

#endif // STATUSWIDGET_H
