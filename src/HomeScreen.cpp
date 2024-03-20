/****************************************************************************************
 *   FileName    : HomeScreen.cpp
 *   Description : HomeScreen.cpp
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

#include <unistd.h>
#include <sys/shm.h>
#include <QDate>
#include <QFile>
#include <QXmlStreamReader>
#include <QProcess>
#include <QTextCodec>
#include "TCInput.h"
#include "LauncherIFManager.h"
#include "HomeScreen.h"

extern int g_debug;
extern QTextCodec *g_codecUtf16;
extern QTextCodec *g_codecUtf8;
extern QString GetNameString(const char *buffer, unsigned int length, bool utf16 = false);
extern StatusWidget::ConnectivitySource GetSourceFromMediaDevice(int device);

#define DEBUG_LAUNCHER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER HOME SCREEN] %s: " format "", __FUNCTION__, ##arg); \
	}

HomeScreen::HomeScreen(QQuickItem *parent) :
	QQuickItem(parent),
	_mediaType(NoMediaType),
	_nowplaying(false),
	_launcherViewOnHome(false),
	_albumArtKey(-1),
	_albumArtSize(0),
	_albumArtID(-1)
{
}

HomeScreen::~HomeScreen()
{
}

bool HomeScreen::Initialize(void)
{
	bool initialized = false;
	QObject *temp;


	temp = this->findChild<QObject *>("nowPlayMusic");
	_nowPlayMusic = qobject_cast<NowPlayMusic *>(temp);

	temp = this->findChild<QObject *>("nowPlayRadio");
	_nowPlayRadio = qobject_cast<NowPlayRadio *>(temp);

	temp = this->findChild<QObject *>("nowPlayCarplay");
	_nowPlayCarplay = qobject_cast<NowPlayCarplay *>(temp);

	temp = this->findChild<QObject *>("statusWidget");
	_status = qobject_cast<StatusWidget *>(temp);

	_nowPlayMusic->Initialize();
	_nowPlayRadio->Initialize();
	_nowPlayCarplay->Initialize();
	_status->Initialize();

	for (int idx = 0; idx < TotalMediaTypes; idx++)
	{
		_availableMedia[idx] = false;
	}

	InitializeSignalSlots();

	initialized = true;

	return initialized;
}

void HomeScreen::ToggleNowPlaying(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	_nowplaying = !_nowplaying;

	if (_nowplaying && _launcherViewOnHome)
	{
		if (_mediaType == MediaTypeMusic)
		{
			DEBUG_LAUNCHER_PROCESS_PRINTF("Start Music\n");
			_nowPlayMusic->PlayReStart();
		}
		else if (_mediaType == MediaTypeRadio)
		{
			DEBUG_LAUNCHER_PROCESS_PRINTF("Turn on Radio\n");
			_nowPlayRadio->TurnOnRadio();
		}
		else if (_mediaType == MediaTypeCarplay)
		{
			DEBUG_LAUNCHER_PROCESS_PRINTF("Start Carplay\n");
		}
		else
		{
			_nowplaying = false;
			DEBUG_LAUNCHER_PROCESS_PRINTF("Set other Devices\n");
		}
	}

	emit changedMediaType();
	emit changedNowPlay();
}

int HomeScreen::mediaType() const
{
	return (int)_mediaType;
}

void HomeScreen::ShowApplications(void)
{
	emit ShowAllApplications();
}

void HomeScreen::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	connect(this, SIGNAL(changedLauncherVisible(bool)),
			_nowPlayMusic, SLOT(OnSetLauncherVisible(bool)));

	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToMusicApp()),
			this, SLOT(OnChangeToMusicApp()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToRadioApp()),
			this, SLOT(OnChangeToRadioApp()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToCarplayApp()),
			this, SLOT(OnChangeToCarplayApp()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToBluetoothApp()),
			this, SLOT(OnChangeToBluetoothApp()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(DisableBluetoothApp()),
			this, SLOT(OnDisableBluetoothApp()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToSuspendMode()),
			this, SLOT(OnChangeToSuspendMode()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToResumeMode()),
			this, SLOT(OnChangeToResumeMode()), dbusConnectionType);

	// For TMPlayer
	connect(LAUNCHER_IF_MANAGER, SIGNAL(PlayStateChanged(int)),
			this, SLOT(OnPlayStateChanged(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(PlayTimeChanged(unsigned char, unsigned char, unsigned char)),
			this, SLOT(OnPlayTimeChanged(unsigned char, unsigned char, unsigned char)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(TotalTimeChanged(unsigned char, unsigned char, unsigned char)),
			this, SLOT(OnTotalTimeChanged(unsigned char, unsigned char, unsigned char)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(AlbumArtKey(int, unsigned int)),
			this, SLOT(OnAlbumArtKey(int, unsigned int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(AlbumArtCompleted(unsigned int)),
			this, SLOT(OnAlbumArtCompleted(unsigned int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(AlbumArtShmData(QString, unsigned int)),
			this, SLOT(OnAlbumArtShmData(QString, unsigned int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(NoDevice(void)),
			this, SLOT(OnNoDevice(void)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ContentChanged(int)),
			this, SLOT(OnContentChanged(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(DeviceChanged(int, int)),
			this, SLOT(OnDeviceChanged(int, int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(DeviceConnected(int)),
			this, SLOT(OnDeviceConnected(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(DeviceDisconnected(int)),
			this, SLOT(OnDeviceDisconnected(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(DeviceReady(int)),
			this, SLOT(OnDeviceReady(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ResetInformation(void)),
			this, SLOT(OnResetInformation(void)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(FileNumberChanged(int)),
			this, SLOT(OnFileNumberChanged(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(FileNumberUpdated(int, int,int, int)),
			this, SLOT(OnFileNumberUpdated(int, int,int, int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(FileNameChanged(QString)),
			this, SLOT(OnFileNameChanged(QString)), dbusConnectionType);

	connect(LAUNCHER_IF_MANAGER, SIGNAL(RadioTypeChanged(int)),
			this, SLOT(OnRadioTypeChanged(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(HDRadioStatus(int)),
			this, SLOT(OnHDRadioStatus(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(RadioPresetChanged(int, int, int)),
			this, SLOT(OnRadioPresetChanged(int, int, int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(RadioFrequencyChanged(int)),
			this, SLOT(OnRadioFrequencyChanged(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(RadioServiceChanged(QString)),
			this, SLOT(OnRadioServiceChanged(QString)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(RadioServicePresetChanged(int, QString, int)),
			this, SLOT(OnRadioServicePresetChanged(int, QString, int)), dbusConnectionType);

	connect(LAUNCHER_IF_MANAGER, SIGNAL(NoCarplay(void)),
			this, SLOT(OnNoCarplay(void)), dbusConnectionType);

	connect(LAUNCHER_IF_MANAGER, SIGNAL(ConnectivitySourceConnected(int)),
			this, SLOT(OnConnectivitySourceConnected(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ConnectivitySourceDisconnected(int)),
			this, SLOT(OnConnectivitySourceDisconnected(int)), dbusConnectionType);
}

void HomeScreen::ChangeMediaType(MediaType type)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("CURRENT TYPE(%d), INCOMING TYPE(%d)\n", _mediaType, type);

	if (_mediaType != type)
	{
		_mediaType = type;
		if (_mediaType == MediaTypeMusic)
		{
			_nowPlayMusic->PlayReStart();
		}
		else if (_mediaType == MediaTypeRadio)
		{
			_nowPlayRadio->TurnOnRadio();
		}

		emit changedMediaType();
	}
}

void HomeScreen::ChangeNextMediaType(void)
{
	int idx, foundIdx = -1;

	for (idx = MediaTypeMusic; idx < TotalMediaTypes && foundIdx == -1; idx++)
	{
		if (_availableMedia[idx])
		{
			foundIdx = idx;
		}
	}

	DEBUG_LAUNCHER_PROCESS_PRINTF("FOUND AVALABLE TYPE(%d)\n", foundIdx);

	if (foundIdx != -1)
	{
		ChangeMediaType((MediaType)foundIdx);
	}
	else
	{
		_mediaType = TotalMediaTypes;
		_nowplaying = false;

		emit changedMediaType();
		emit changedNowPlay();
	}
}

void HomeScreen::OnChangedLauncherVisible(bool visible)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("%d\n", visible);
	_launcherViewOnHome = visible;
	emit changedLauncherVisible(visible);
}

void HomeScreen::OnChangeToMusicApp(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	if (_availableMedia[MediaTypeMusic])
	{
		ChangeMediaType(MediaTypeMusic);
	}
}

void HomeScreen::OnChangeToRadioApp(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	ChangeMediaType(MediaTypeRadio);
}

void HomeScreen::OnChangeToCarplayApp(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	ChangeMediaType(MediaTypeCarplay);
}

void HomeScreen::OnChangeToBluetoothApp(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	ChangeMediaType(MediaTypeBluetooth);
	_availableMedia[MediaTypeBluetooth] = true;
}

void HomeScreen::OnDisableBluetoothApp(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	_availableMedia[MediaTypeBluetooth] = false;
	ChangeNextMediaType();
}

void HomeScreen::OnChangeToSuspendMode(void)
{
	if (_nowplaying)
	{
		ToggleNowPlaying();
	}
}

void HomeScreen::OnChangeToResumeMode(void)
{
}

void HomeScreen::OnPlayStateChanged(int status)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("STATUS(%d)\n", status);

	_nowPlayMusic->SetPlayStatus((NowPlayMusic::AVPlayStatus)status);

	if (!_nowplaying && status == NowPlayMusic::PlayStatusPlaying)
	{
		ToggleNowPlaying();
	}
}

void HomeScreen::OnPlayTimeChanged(unsigned char hour, unsigned char min, unsigned char sec)
{
	_nowPlayMusic->SetPlayTime(hour, min, sec);
}

void HomeScreen::OnTotalTimeChanged(unsigned char hour, unsigned char min, unsigned char sec)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("HOUR(%u), MIN(%u), SEC(%u)\n", hour, min, sec);

	_nowPlayMusic->SetTotalTime(hour, min, sec);
}

void HomeScreen::OnAlbumArtKey(int key, unsigned int size)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	_albumArtKey = key;
	_albumArtSize = size;
	_albumArtID = shmget(_albumArtKey, _albumArtSize, 0);
}

void HomeScreen::OnAlbumArtCompleted(unsigned int length)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	if (_albumArtKey != -1 && _albumArtSize > 0)
	{
		if (_albumArtID != -1)
		{
			struct shmid_ds shmStat;

			if (shmctl(_albumArtID, IPC_STAT, &shmStat) != -1)
			{
				if (shmStat.shm_segsz >= length)
				{
					void *buf;

					buf = shmat(_albumArtID, (void *)0, SHM_RDONLY);
					if (buf != (void *)-1)
					{
						_nowPlayMusic->SetAlbumArt((const unsigned char *)buf, length);

						if (shmdt(buf) == -1)
						{
							fprintf(stderr, "%s:", __PRETTY_FUNCTION__);
							perror("shmdt failed");
						}
					}
					else
					{
						fprintf(stderr, "%s:", __PRETTY_FUNCTION__);
						perror("shmat failed: ");
					}
				}
				else
				{
					fprintf(stderr, "%s: not enough memory. album art length(%u), shm segment size(%lu)\n", __PRETTY_FUNCTION__,
							length, shmStat.shm_segsz);
				}
			}
			else
			{
				fprintf(stderr, "%s:", __PRETTY_FUNCTION__);
				perror("shmctl failed");
				exit(1);
			}
		}
		else
		{
			fprintf(stderr, "%s:", __PRETTY_FUNCTION__);
			perror("shmget failed: ");
		}
	}
	else
	{
		fprintf(stderr, "%s: not initialize shared memory information. key(%d), size(%u)\n", __PRETTY_FUNCTION__,
				_albumArtKey, _albumArtSize);
	}
}

void HomeScreen::OnAlbumArtShmData(QString uri, unsigned int length)
{
    DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	if (length > 0)
	{
		FILE *fp = fopen(uri.toLatin1().data(), "r");

		if (fp != NULL)
		{
			unsigned char *buffer;
			buffer = (unsigned char *)malloc(length);

			if (buffer != NULL)
			{
				unsigned char *bufPtr= buffer;
				unsigned int bytesToRead = length;
				unsigned int bufSize = 0;
				bool failed = false;

				while (!failed && bytesToRead)
				{
					bufSize = fread(bufPtr, 1, bytesToRead, fp);
					if (bufSize > 0)
					{
						bufPtr += bufSize;
						bytesToRead -= bufSize;
					}
					else
					{
						failed = true;
					}
				}

				bufSize = length - bytesToRead;
				_nowPlayMusic->SetAlbumArt((const unsigned char *)buffer, bufSize);
				free(buffer);

				if (bufSize != length)
				{
					fprintf(stderr, "%s: file read failed. length(%d), bytes to read(%d)\n", __PRETTY_FUNCTION__, length, bufSize);
				}

				if (fclose(fp) < 0)
				{
					fprintf(stderr, "%s: fclose fail\n", __PRETTY_FUNCTION__);
				}
			}
			else
			{
				fprintf(stderr, "%s: Error malloc AlbumArt\n", __PRETTY_FUNCTION__);
			}
		}
		else
		{
			fprintf(stderr, "%s: Error fopen AlbumArt file (%s)\n", __PRETTY_FUNCTION__, uri.toLatin1().data());
		}
	}
	else
	{
		_nowPlayMusic->SetNoAlbumArt();
		fprintf(stderr, "%s: No Album Art\n", __PRETTY_FUNCTION__);
	}
}

void HomeScreen::OnNoDevice(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	_nowPlayMusic->ResetInformation();
	_availableMedia[MediaTypeMusic] = false;
	ChangeNextMediaType();
}

void HomeScreen::OnContentChanged(int content)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("CONTENT(%d)\n", content);
}

void HomeScreen::OnDeviceChanged(int device, int content)
{
	if (_nowPlayMusic->ChangeDevice(device))
	{
		ChangeMediaType(MediaTypeMusic);
		if (!_nowplaying)
		{
			ToggleNowPlaying();
		}
	}
	(void)content;
}

void HomeScreen::OnDeviceConnected(int device)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("DEVICE(%d)\n", device);

	_status->SetEnableDevice(GetSourceFromMediaDevice(device));
	_nowPlayMusic->AddDevice(device);
}

void HomeScreen::OnDeviceDisconnected(int device)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("DEVICE(%d)\n", device);

	_status->SetDisableDevice(GetSourceFromMediaDevice(device));
	_nowPlayMusic->RemoveDevice(device);
}

void HomeScreen::OnDeviceReady(int device)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("DEVICE(%d)\n", device);

	_nowPlayMusic->DeviceReady(device);
	_availableMedia[MediaTypeMusic] = true;
}

void HomeScreen::OnResetInformation(void)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	_nowPlayMusic->ResetInformation();
}

void HomeScreen::OnFileNumberChanged(int number)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("NUMBER(%u)\n", number);

	_nowPlayMusic->SetFileNumber(number);
}

void HomeScreen::OnFileNumberUpdated(int device, int content,int number, int totalNumber)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("DEVICE([%d] %s), CONTENT(%d), NUMBER(%u), TOTAL(%u)\n",
							  device,
							  (device != TotalDeviceSources) ? g_deviceSourceNames[device] : "UNKNOWN",
							  content, number,totalNumber);
#if 0
	if (device == _device)
	{
		/* Set filenumber */
		if ((_contentType == MultiMediaContentTypeAudio)&&(_contentType == content))
		{
			_nowPlayMusic->SetFileNumberWithoutClear(number);
		}
	}
#endif
}

void HomeScreen::OnFileNameChanged(QString name)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("NAME(%s)\n", name.toLatin1().data());

	_nowPlayMusic->SetFileName(name);
}

void HomeScreen::OnRadioTypeChanged(int type)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("TYPE(%d)\n", type);

	_nowPlayRadio->SetRadioType(type);
}

void HomeScreen::OnHDRadioStatus(int status)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("Status(%d)\n", status);

	_nowPlayRadio->SetHDRadio(status);
}

void HomeScreen::OnRadioPresetChanged(int type, int freq, int idx)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("TYPE(%d), FREQUENCY(%d), INDEX(%d)\n", type, freq, idx);

	_nowPlayRadio->SetRadioPreset(type, freq, idx);
}

void HomeScreen::OnRadioFrequencyChanged(int freq)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("FREQUENCY(%d)\n", freq);

	_nowPlayRadio->SetRadioFrequency(freq);
	_availableMedia[MediaTypeRadio] = true;
	ChangeMediaType(MediaTypeRadio);
	if (!_nowplaying)
	{
		ToggleNowPlaying();
	}
}

void HomeScreen::OnRadioServiceChanged(QString service)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("Service(%s)\n",
								  service.toLatin1().data());
	_nowPlayRadio->SetRadioService(service);
	if (!_nowplaying)
	{
		ToggleNowPlaying();
	}
}

void HomeScreen::OnRadioServicePresetChanged(int type, QString service, int idx)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("TYPE(%d), SERIVCE(%s), INDEX(%d)\n",
								  type, service.toLatin1().data(), idx);
	_nowPlayRadio->SetRadioServicePreset(type, service, idx);
}

void HomeScreen::OnNoCarplay(void)
{
	if (_mediaType == MediaTypeCarplay)
	{
		_availableMedia[MediaTypeCarplay] = false;
		ChangeNextMediaType();
	}
}

void HomeScreen::OnConnectivitySourceConnected(int source)
{
	_status->SetEnableDevice((StatusWidget::ConnectivitySource)source);
}

void HomeScreen::OnConnectivitySourceDisconnected(int source)
{
	_status->SetDisableDevice((StatusWidget::ConnectivitySource)source);
}

QString GetNameString(const char *buffer, unsigned int length, bool utf16)
{
	QString text;
	if (utf16)
	{
		if (g_codecUtf16 != NULL)
		{
			text = g_codecUtf16->toUnicode(buffer, length);
		}
	}
	else
	{
		if (g_codecUtf8 != NULL)
		{
			text = g_codecUtf8->toUnicode(buffer, length);
		}
	}
	return text;
}

StatusWidget::ConnectivitySource GetSourceFromMediaDevice(int device)
{
	StatusWidget::ConnectivitySource deviceSource = StatusWidget::TotalConnectivitySources;

	switch(device)
	{
		case DeviceSourceUSB1:
			deviceSource = StatusWidget::ConnectivitySourceUSB1;
			break;
		case DeviceSourceUSB2:
			deviceSource = StatusWidget::ConnectivitySourceUSB2;
			break;
		case DeviceSourceUSB3:
			deviceSource = StatusWidget::ConnectivitySourceUSB3;
			break;
		case DeviceSourceSDMMC:
			deviceSource = StatusWidget::ConnectivitySourceSDMMC;
			break;
		case DeviceSourceAUX:
			deviceSource = StatusWidget::ConnectivitySourceAux;
			break;
		case DeviceSourceiAP2:
			deviceSource = StatusWidget::ConnectivitySourceiAP;
			break;
		case DeviceSourceBluetooth:
			deviceSource = StatusWidget::ConnectivitySourceBluetooth;
			break;
	}

	return deviceSource;
}
