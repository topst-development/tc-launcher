/****************************************************************************************
 *   FileName    : NowPlayMusic.h
 *   Description : NowPlayMusic.h
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

#ifndef NOWPLAY_MUSIC_H
#define NOWPLAY_MUSIC_H

#include <QObject>
#include <QQuickItem>
#include <QQuickPaintedItem>
#include <QPainter>
#include <QImage>
#include "MediaPlayerType.h"

class AlbumArt;

class NowPlayMusic : public QQuickItem
{
	Q_OBJECT
	Q_ENUMS(AVPlayStatus)
	Q_PROPERTY(int status READ status NOTIFY changedStatus)
	Q_PROPERTY(QString fileName MEMBER _fileName NOTIFY changedFileName)
	Q_PROPERTY(unsigned int playTime MEMBER _playTime NOTIFY changedPlayTime)
	Q_PROPERTY(unsigned int totalTime MEMBER _totalTime NOTIFY changedTotalTime)
	Q_PROPERTY(int currentDevice MEMBER _currentDevice NOTIFY changedDevice)

	public:
		typedef enum {
			PlayStatusStop,
			PlayStatusPlaying,
			PlayStatusPause,
			PlayStatusFF,
			PlayStatusRew,
			PlayStatusEndFFRew,
			PlayStatusTurboFF,
			PlayStatusTurboRew,
			PlayStatusNoStatement,
			TotalPlayStatus
		}AVPlayStatus;
		NowPlayMusic(QQuickItem *parent = 0);
		void Initialize(void);
		void ResetInformation(void);
		bool ChangeDevice(int device);
		void PlayReStart(void);
		void AddDevice(int device);
		void RemoveDevice(int device);
		void DeviceReady(int device);
		bool IsExistDevice(void);
		void SetPlayStatus(AVPlayStatus status);
		void SetFileNumber(int number);
		void SetFileNumberWithoutClear(int number);
		void SetFileName(QString name);
		void SetPlayTime(unsigned char hour, unsigned char min, unsigned char sec);
		void SetTotalTime(unsigned char hour, unsigned char min, unsigned char sec);
		void SetAlbumArt(const unsigned char *buf, unsigned int length);
		void SetNoAlbumArt(void);

		int status() const;

		Q_INVOKABLE void OnPlay(void);
		Q_INVOKABLE void OnClickedPlayResume(void);
		Q_INVOKABLE void OnClickedPlayPause(void);
		Q_INVOKABLE void OnClickedTrackUp(void);
		Q_INVOKABLE void OnClickedTrackDown(void);
		Q_INVOKABLE QString GetTimeString(unsigned int time);

	private:
		void InitializeSignalSlots(void);

	private slots:
		void OnSetLauncherVisible(bool visible);

	signals:
		void ChangeMedia(unsigned char device, bool play);
		void PlayStart(int device);
		void PlayResume(int device);
		void PlayPause(int device);
		void TrackUp(int device);
		void TrackDown(int device);
		void TrackSeek(int device, unsigned char hour, unsigned char min, unsigned char sec);

		void changedStatus();
		void resetInformation();
		void changedFileName();
		void changedPlayTime();
		void changedTotalTime();
		void changedDevice();

	private:
		int					_fileNumber;
		AVPlayStatus		_status;
		QString				_fileName;
		unsigned int		_totalTime;
		unsigned int		_playTime;
		bool				_connectedDevices[TotalDeviceSources];
		bool				_launcherViewOnNow;
		int					_currentDevice;
		QImage				_albumArt;
		AlbumArt			*_albumArtPainter;
};

class AlbumArt : public QQuickPaintedItem
{
	Q_OBJECT
	Q_PROPERTY(QImage image READ image WRITE setImage NOTIFY changedImage)

	public:
		AlbumArt(QQuickPaintedItem *parent = 0);
		~AlbumArt();
		void paint(QPainter *painter);
		void SetAlbumArtImage(const QImage &albumArtImage);

		QImage image() const;

		Q_INVOKABLE void setImage(const QImage &image);

	signals:
		void changedImage();

	private:
		QImage _image;
};
#endif // NOWPLAY_MUSIC_H
