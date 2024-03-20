/****************************************************************************************
 *   FileName    : NowPlayMusic.c
 *   Description : NowPlayMusic.c
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

#include <QImage>
#include "NowPlayMusic.h"
#include "MediaPlayerType.h"
#include "LauncherIFManager.h"
#include "ImageProvider.h"

extern int g_debug;
#define DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER(NOW PLAY MUSIC)] %s: " format "", __FUNCTION__, ##arg); \
	}

#define ANIMATION_TEXT_STOP_TIME	3

NowPlayMusic::NowPlayMusic(QQuickItem *parent) :
	QQuickItem(parent),
	_fileNumber(0),
	_status(PlayStatusNoStatement),
	_fileName(""),
	_totalTime(0),
	_playTime(0),
	_launcherViewOnNow(false),
	_currentDevice(TotalDeviceSources)

{
}

void NowPlayMusic::Initialize(void)
{
	QObject *temp;

	InitializeSignalSlots();

	for (int i = 0; i < TotalDeviceSources; i++)
	{
		_connectedDevices[i] = false;
	}

	temp = this->findChild<QObject*>("albumArt");
	_albumArtPainter = qobject_cast<AlbumArt *>(temp);

	_albumArt.load(":/images/nowplay/nowplay_albumart.png");
	_albumArtPainter->SetAlbumArtImage(_albumArt);
}

void NowPlayMusic::ResetInformation(void)
{
	_status = PlayStatusNoStatement;
	_fileName = "";
	_totalTime = 0;
	_playTime = 0;

	_albumArt.load(":/images/nowplay/nowplay_albumart.png");
	_albumArtPainter->SetAlbumArtImage(_albumArt);

	emit changedDevice();
	emit changedStatus();
	emit changedPlayTime();
	emit changedTotalTime();
	emit resetInformation();
}

bool NowPlayMusic::ChangeDevice(int device)
{
	bool changed = false;

	if (device >= DeviceSourceMyMusic && device < TotalDeviceSources)
	{
		if (_currentDevice != device)
		{
			_currentDevice = device;
			ResetInformation();
			changed = true;
			emit changedDevice();
		}
	}
	else
	{
		fprintf(stderr, "%s: invalid device(%d)\n", __PRETTY_FUNCTION__, device);
	}

	return changed;
}

void NowPlayMusic::PlayReStart(void)
{
	if (_status != PlayStatusNoStatement
				&& _launcherViewOnNow)
	{
		emit PlayStart(_currentDevice);
	}
}

void NowPlayMusic::AddDevice(int device)
{
	if (device >= DeviceSourceMyMusic && device < TotalDeviceSources)
	{
		_connectedDevices[device] = true;
	}
}

void NowPlayMusic::RemoveDevice(int device)
{
	if (device >= DeviceSourceMyMusic && device < TotalDeviceSources)
	{
		_connectedDevices[device] = false;
		if (_currentDevice == device)
		{
			_currentDevice = TotalDeviceSources;
		}
	}
}

void NowPlayMusic::DeviceReady(int device)
{
	if (_launcherViewOnNow)
	{
		emit ChangeMedia(device, true);
	}
}

bool NowPlayMusic::IsExistDevice(void)
{
	bool exist = false;

	for (int i = 0; (i < TotalDeviceSources) && !exist; i++)
	{
		if (_connectedDevices[i])
		{
			exist = true;
		}
	}
	return exist;
}

void NowPlayMusic::SetPlayStatus(AVPlayStatus status)
{
	if (status == PlayStatusPlaying)
	{
		_status = PlayStatusPlaying;
	}
	else if (status == PlayStatusPause)
	{
		_status = PlayStatusPause;
	}
	else if (status == PlayStatusStop)
	{
		_status = PlayStatusStop;
	}

	emit changedStatus();
}

void NowPlayMusic::SetFileNumber(int number)
{
	if (_fileNumber != number)
	{
		ResetInformation();
		_fileNumber = number;
	}
}

void NowPlayMusic::SetFileName(QString name)
{
	_fileName = name;

	emit changedFileName();
}

void NowPlayMusic::SetPlayTime(unsigned char hour, unsigned char min, unsigned char sec)
{
	_playTime = hour * 3600 + min * 60 + sec;
	emit changedPlayTime();
}

void NowPlayMusic::SetTotalTime(unsigned char hour, unsigned char min, unsigned char sec)
{
	_totalTime = hour * 3600 + min * 60 + sec;
	emit changedTotalTime();
}

void NowPlayMusic::SetAlbumArt(const unsigned char *buf, unsigned int length)
{
	bool decoded;
	decoded = _albumArt.loadFromData(buf, (int)length);

	if (decoded)
	{
		_albumArtPainter->SetAlbumArtImage(_albumArt);
	}
	else
	{
		fprintf(stderr, "%s: decode albumart failed\n", __PRETTY_FUNCTION__);
	}
}

void NowPlayMusic::SetNoAlbumArt(void)
{
	_albumArt.load(":/images/nowplay/nowplay_albumart.png");
	_albumArtPainter->SetAlbumArtImage(_albumArt);
}

int NowPlayMusic::status() const
{
	return (int)_status;
}

void NowPlayMusic::OnPlay(void)
{
	if ((_currentDevice != TotalDeviceSources)
			&& (_status == PlayStatusNoStatement)
			&& _launcherViewOnNow)
	{
		emit PlayStart(_currentDevice);
	}
}

void NowPlayMusic::OnClickedPlayResume(void)
{
	if (_currentDevice != TotalDeviceSources)
	{
		if (_status == PlayStatusStop || _status == PlayStatusNoStatement)
		{
			PlayReStart();
		}
		else
		{
			emit PlayResume(_currentDevice);
		}
	}
}

void NowPlayMusic::OnClickedPlayPause(void)
{
	if (_currentDevice != TotalDeviceSources)
	{
		emit PlayPause(_currentDevice);
	}
}

void NowPlayMusic::OnClickedTrackUp(void)
{
	if (_currentDevice != TotalDeviceSources)
	{
		emit TrackUp(_currentDevice);
	}
}

void NowPlayMusic::OnClickedTrackDown(void)
{
	if (_currentDevice != TotalDeviceSources)
	{
		emit TrackDown(_currentDevice);
	}
}

void NowPlayMusic::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(this, SIGNAL(ChangeMedia(unsigned char, bool)),
			LAUNCHER_IF_MANAGER, SLOT(OnChangeMedia(unsigned char, bool)), dbusConnectionType);
	connect(this, SIGNAL(PlayStart(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnPlayStart(int)), dbusConnectionType);
	connect(this, SIGNAL(PlayResume(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnPlayResume(int)), dbusConnectionType);
	connect(this, SIGNAL(PlayPause(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnPlayPause(int)), dbusConnectionType);
	connect(this, SIGNAL(TrackUp(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnTrackUp(int)), dbusConnectionType);
	connect(this, SIGNAL(TrackDown(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnTrackDown(int)), dbusConnectionType);
	connect(this, SIGNAL(TrackSeek(int, unsigned char, unsigned char, unsigned char)),
			LAUNCHER_IF_MANAGER, SLOT(OnTrackSeek(int, unsigned char, unsigned char, unsigned char)), dbusConnectionType);
}

void NowPlayMusic::OnSetLauncherVisible(bool visible)
{
	DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF("%d\n", visible);
	_launcherViewOnNow = visible;
}

QString NowPlayMusic::GetTimeString(unsigned int time)
{
	QString timeString;
	unsigned int sec = time % 60;
	unsigned int totalMin = time / 60;

	timeString = QString("%1:%2").arg(totalMin, 2, 10, QChar('0')).arg(sec, 2, 10, QChar('0'));

	return timeString;
}

AlbumArt::AlbumArt(QQuickPaintedItem *parent) :
	QQuickPaintedItem(parent)
{
}

AlbumArt::~AlbumArt()
{
}

void AlbumArt::paint(QPainter *painter)
{
	QImage scaled = _image.scaledToHeight(boundingRect().height());
	QPointF center = boundingRect().center() - scaled.rect().center();

	if(center.x() < 0)
	{
		center.setX(0);
	}
	if(center.y() < 0)
	{
		center.setY(0);
	}

	painter->drawImage(center, scaled);
	DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF("paint image\n");
}

void AlbumArt::SetAlbumArtImage(const QImage &albumArtImage)
{
	DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF("update image\n");
	_image = albumArtImage;
	update();
}

QImage AlbumArt::image() const
{
	DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF("return image\n");
	return this->_image;
}

void AlbumArt::setImage(const QImage &image)
{
	DEBUG_LAUNCHER_NOW_PLAY_MUSIC_PRINTF("set new image\n");
	this->_image = image;
	emit changedImage();
	update();
}
