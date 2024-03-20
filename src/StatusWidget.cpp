/****************************************************************************************
 *   FileName    : StatusWidget.c
 *   Description : StatusWidget.c
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

#include "LauncherIFManager.h"
#include "StatusWidget.h"

extern int g_debug;
#define DEBUG_LAUNCHER_STATUS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER STATUS] %s: " format "", __FUNCTION__, ##arg); \
	}

StatusWidget::StatusWidget(QQuickItem *parent) :
	QQuickItem(parent)
{
}

void StatusWidget::Initialize(void)
{
	int index;

	for (index = 0; index < TotalConnectivitySources; index++)
	{
		_icoDevice[index] = false;
	}

	emit deviceChanged();
}

void StatusWidget::SetEnableDevice(StatusWidget::ConnectivitySource device)
{
	int index = (int)device;

	if (index >= ConnectivitySourceUSB1 && index < TotalConnectivitySources)
	{
		_icoDevice[index] = true;
	}

	emit deviceChanged();
}

void StatusWidget::SetDisableDevice(StatusWidget::ConnectivitySource device)
{
	int index = (int)device;

	if (index >= ConnectivitySourceUSB1 && index < TotalConnectivitySources)
	{
		_icoDevice[index] = false;
	}

	emit deviceChanged();
}

bool StatusWidget::usb1() const
{
	return _icoDevice[ConnectivitySourceUSB1];
}

bool StatusWidget::usb2() const
{
	return _icoDevice[ConnectivitySourceUSB2];
}

bool StatusWidget::usb3() const
{
	return _icoDevice[ConnectivitySourceUSB3];
}

bool StatusWidget::sdmmc() const
{
	return _icoDevice[ConnectivitySourceSDMMC];
}

bool StatusWidget::aux() const
{
	return _icoDevice[ConnectivitySourceAux];
}

bool StatusWidget::iAP2() const
{
	return _icoDevice[ConnectivitySourceiAP];
}

bool StatusWidget::carPlay() const
{
	return _icoDevice[ConnectivitySourceCarPlay];
}

bool StatusWidget::bluetooth() const
{
	return _icoDevice[ConnectivitySourceBluetooth];
}
