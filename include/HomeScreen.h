/****************************************************************************************
 *   FileName    : HomeScreen.h
 *   Description : HomeScreen.h
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

#ifndef HOME_SCREEN_H
#define HOME_SCREEN_H

#include <sys/types.h>
#include <QObject>
#include <QQuickItem>
#include <QList>
#include <QRect>
#include "TCApplication.h"
#include "NowPlayMusic.h"
#include "NowPlayRadio.h"
#include "NowPlayCarplay.h"
#include "StatusWidget.h"
#include "MediaPlayerType.h"

class HomeScreen : public QQuickItem
{
    Q_OBJECT
	Q_ENUMS(MediaType)
	Q_PROPERTY(bool nowplaying MEMBER _nowplaying NOTIFY changedNowPlay)
	Q_PROPERTY(int mediaType READ mediaType NOTIFY changedMediaType)

	public:
		typedef enum {
			NoMediaType,
			MediaTypeMusic,
			MediaTypeBluetooth,
			MediaTypeRadio,
			MediaTypeCarplay,
			TotalMediaTypes
		} MediaType;

		HomeScreen(QQuickItem *parent = 0);
		~HomeScreen();
		bool Initialize(void);
		void ToggleNowPlaying(void);

		int mediaType() const;

		Q_INVOKABLE void ShowApplications(void);

	private:
		void InitializeSignalSlots(void);
		void ChangeMediaType(MediaType type);
		void ChangeNextMediaType(void);

	private slots:
		void OnChangedLauncherVisible(bool visible);
		void OnChangeToMusicApp(void);
		void OnChangeToRadioApp(void);
		void OnChangeToCarplayApp(void);
		void OnChangeToBluetoothApp(void);
		void OnDisableBluetoothApp(void);
		void OnChangeToSuspendMode(void);
		void OnChangeToResumeMode(void);

		// For TMPlayer
		void OnPlayStateChanged(int status);
		void OnPlayTimeChanged(unsigned char hour, unsigned char min, unsigned char sec);
		void OnTotalTimeChanged(unsigned char hour, unsigned char min, unsigned char sec);
		void OnAlbumArtKey(int key, unsigned int size);
		void OnAlbumArtCompleted(unsigned int length);
		void OnAlbumArtShmData(QString uri, unsigned int length);
		void OnNoDevice(void);
		void OnContentChanged(int content);
		void OnDeviceChanged(int device, int content);
		void OnDeviceConnected(int device);
		void OnDeviceDisconnected(int device);
		void OnDeviceReady(int device);
		void OnResetInformation(void);
		void OnFileNumberChanged(int number);
		void OnFileNumberUpdated(int device, int content,int number, int totalNumber);
		void OnFileNameChanged(QString name);

		// For Radio
		void OnRadioTypeChanged(int type);
		void OnRadioPresetChanged(int type, int freq, int idx);
		void OnRadioFrequencyChanged(int freq);
		void OnRadioServiceChanged(QString service);
		void OnRadioServicePresetChanged(int type, QString service, int idex);
		void OnHDRadioStatus(int status);

		// For CarPlay
		void OnNoCarplay(void);

		// For Connectivity
		void OnConnectivitySourceConnected(int source);
		void OnConnectivitySourceDisconnected(int source);

	signals:
		void ShowAllApplications();

		void changedLauncherVisible(bool visible);
		void changedNowPlay();
		void changedMediaType();

	private:
		NowPlayMusic			*_nowPlayMusic;
		NowPlayRadio			*_nowPlayRadio;
		NowPlayCarplay			*_nowPlayCarplay;
		StatusWidget			*_status;
		MediaType				_mediaType;
		bool					_nowplaying;
		bool					_launcherViewOnHome;
		key_t					_albumArtKey;
		unsigned int			_albumArtSize;
		int						_albumArtID;
		bool					_availableMedia[TotalMediaTypes];
};

#endif // HOME_SCREEN_H
