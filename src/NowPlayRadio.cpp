/****************************************************************************************
 *   FileName    : NowPlayRadio.c
 *   Description : NowPlayRadio.c
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

#include "NowPlayRadio.h"
#include "MediaPlayerType.h"
#include "LauncherIFManager.h"

extern int g_debug;
#define DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER(NOW PLAY RADIO)] %s: " format "", __FUNCTION__, ##arg); \
	}

NowPlayRadio::NowPlayRadio(QQuickItem *parent) :
	QQuickItem(parent),
	_channel(0),
	_type(RadioTypeFM)
{
}

void NowPlayRadio::Initialize(void)
{
	_frequency = "";
	_hdRadio = false;

	for (int i = 0; i < TotalRadioTypes; i++)
	{
		for (int idx = 0; idx < PRESET_COUNT; idx++)
		{
			_presets[i].append("------");
		}
	}

	InitializeSignalSlots();

	emit changedFrequency();
	emit changedHDRadio();
	emit changedPresets();
}

void NowPlayRadio::ResetInformation(void)
{
}

void NowPlayRadio::TurnOnRadio(void)
{
	emit RadioTurnOn();
}

bool NowPlayRadio::SetRadioType(int type)
{
	bool changed = false;

	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("TYPE(%d)\n", type);

	if (type >= RadioTypeFM && type < TotalRadioTypes)
	{
		_type = (RadioTypes)type;

		emit changedType();
		emit changedPresets();

		changed = true;
	}

	return changed;
}

void NowPlayRadio::SetRadioFrequency(int freq)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("FREQUENCY(%d)\n", freq);

	if (_type == RadioTypeFM)
	{
		_frequency = QString("%1.%2 MHz").arg(freq / 1000).arg((freq % 1000) / 100);
	}
	else if (_type == RadioTypeAM)
	{
		_frequency = QString("%1 KHz").arg(freq);
	}

	emit changedFrequency();
}

void NowPlayRadio::SetRadioService(QString service)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("Service(%s)\n", service.toLatin1().data());

	if(service.count() < 12)
	{
		_frequency = service;
	}
	else
	{
		_frequency = service.left(12);
	}

	emit changedFrequency();
}

void NowPlayRadio::SetRadioPreset(int type, int freq, int idx)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("TYPE(%d), FREQUENCY(%d), INDEX(%d)\n", type, freq, idx);

	if (type >= RadioTypeFM && type < TotalRadioTypes && idx >= 0 && idx < PRESET_COUNT)
	{
		if (type == RadioTypeFM)
		{
			_presets[type].replace(idx, QString("%1.%2 MHz").arg(freq / 1000).arg((freq % 1000) / 100));
		}
		else if (type == RadioTypeAM)
		{
			_presets[type].replace(idx, QString("%1 KHz").arg(freq));
		}

		if (type == _type)
		{
			emit changedPresets();
		}
	}
}

void NowPlayRadio::SetRadioServicePreset(int type, QString service, int idx)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("TYPE(%d), SERVICE(%s), INDEX(%d)\n", type, service.toLatin1().data(), idx);

	QString serviceStr;
	if (service.count() < 8)
	{
		_presets[type].replace(idx, service);
	}
	else
	{
		_presets[type].replace(idx, service.left(8));
	}

	if (type == _type)
	{
		emit changedPresets();
	}
}

void NowPlayRadio::SetHDRadio(int status)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("HDR(%d))\n", status);
	int hdr;
	hdr = status;

	if (hdr)
	{
		_hdRadio = true;
	}
	else
	{
		_hdRadio = false;
	}

	emit changedHDRadio();
}

int NowPlayRadio::type() const
{
	return (int)_type;
}

QStringList NowPlayRadio::presets() const
{
	return _presets[_type];
}

void NowPlayRadio::OnClickedRadioType(void)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("\n");
	emit ChangeRadioType();
}

void NowPlayRadio::OnClickedPreset(int idx)
{
	DEBUG_LAUNCHER_NOW_PLAY_RADIO_PRINTF("EMIT SELECT PRESET(%d)\n", idx);
	emit SelectRadioPreset(idx);
}

void NowPlayRadio::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(this, SIGNAL(ChangeRadioType(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnChangeRadioType(void)), dbusConnectionType);
	connect(this, SIGNAL(SelectRadioPreset(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnSelectRadioPreset(int)), dbusConnectionType);
	connect(this, SIGNAL(RadioTurnOn(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnRadioTurnOn(void)), dbusConnectionType);
}
