/****************************************************************************************
 *   FileName    : LauncherIFManager.h
 *   Description : LauncherIFManager.h
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

#ifndef LAUNCHER_IF_MANAGER_H
#define LAUNCHER_IF_MANAGER_H

#include <dbus/dbus.h>
#include <QThread>
#include "MediaPlayerType.h"

class LauncherIFManager : public QThread
{
	Q_OBJECT

	public:
		LauncherIFManager(QObject *parent = 0);
		void Initilaize(const char *device);
		void Release(void);
		void EmitSetGeometry(bool full);
		void EmitShowLauncher();
		void EmitHideLauncher(int resources, int app);
		void EmitChangeToMusicApp(void);
		void EmitChangeToRadioApp(void);
		void EmitChangeToCarplayApp(void);
		void EmitChangeToBluetoothApp(void);
		void EmitDisableBluetoothApp(void);
		void EmitChangeToSuspendMode(void);
		void EmitChangeToResumeMode(void);
		void EmitActiveApplication(int app, int active);
		void EmitStartApplications();
		void EmitPreemptKeyEvent(int app);
		void EmitReleasePreemptKeyEvent(int app);
		void EmitKeyboardPressedEvent(int key);
		void EmitKeyboardClickedEvent(int key);
		void EmitKeyboardLongPressedEvent(int key);
		void EmitKeyboardLongLongPressedEvent(int key);
		void EmitKeyboardReleasedEvent(int key);

		void EmitPlayStateChanged(DBusMessage *message);
		void EmitPlayTimeChanged(DBusMessage *message);
		void EmitTotalTimeChange(DBusMessage *message);
		void EmitAlbumArtKey(DBusMessage *message);
		void EmitAlbumArtCompleted(DBusMessage *message);
		void EmitAlbumArtShmData(DBusMessage *message);
		void EmitNoDevice(DBusMessage *message);
		void EmitContentChanged(DBusMessage *message);
		void EmitDeviceChanged(DBusMessage *message);
		void EmitDeviceConnected(DBusMessage *message);
		void EmitDeviceDisconnected(DBusMessage *message);
		void EmitDeviceReady(DBusMessage *message);
		void EmitResetInformation(DBusMessage *message);
		void EmitFileNumberChange(DBusMessage *message);
		void EmitFileNumberUpdate(DBusMessage *message);
		void EmitFileNameChange(DBusMessage *message);

		void EmitRadioAMChanged(DBusMessage *message);
		void EmitRadioFMChanged(DBusMessage *message);
		void EmitRadioHDRChanged(DBusMessage *message);
		void EmitDABChanged(DBusMessage *message);
		void EmitDRMChanged(DBusMessage *message);
		void EmitRadioPresetChanged(DBusMessage *message);
		void EmitRadioFrequencyChanged(DBusMessage *message);
		void EmitDRMServiceChanged(DBusMessage *message);
		void EmitDRMServicePresetChanged(DBusMessage *message);
		void EmitRadioAMPresetList(DBusMessage *message);
		void EmitRadioFMPresetList(DBusMessage *message);

		void EmitNoCarplay(void);

		void EmitConnectivitySourceConnected(int source);
		void EmitConnectivitySourceDisconnected(int source);

	private slots:
		void OnShowApplication(int app);
		void OnShowHome(int app);
		void OnHideCompleted(int resources, int app);
		void OnEnableKeyEvent(int app);
		void OnDisableKeyEvent(int app);

		void OnChangeMedia(unsigned char device, bool play);
		void OnPlayStart(int device);
		void OnPlayResume(int device);
		void OnPlayPause(int device);
		void OnTrackUp(int device);
		void OnTrackDown(int device);
		void OnTrackSeek(int device, unsigned char hour, unsigned char min, unsigned char sec);
		void OnTrackMove(int number);

		void OnChangeRadioType(void);
		void OnSelectRadioPreset(int idx);
		void OnRadioTurnOn(void);

		void OnBluetoothTurnOn(void);

		void OnSVMOn(void);
		void OnSVMOff(void);
		void OnFVCOn(void);
		void OnFVCOff(void);

	signals:
		void SetGeometry(bool full);
		void ShowLauncher();
		void HideLauncher(int resources, int app);
		void ChangeToMusicApp(void);
		void ChangeToRadioApp(void);
		void ChangeToCarplayApp(void);
		void ChangeToBluetoothApp(void);
		void DisableBluetoothApp(void);
		void ChangeToSuspendMode(void);
		void ChangeToResumeMode(void);
		void ActiveApplication(int app, int active);
		void StartApplications();
		void PreemptKeyEvent(int app);
		void ReleasePreemptKeyEvent(int app);
		void KeyboardPressed(int key);
		void KeyboardClicked(int key);
		void KeyboardLongPressed(int key);
		void KeyboardLongLongPressed(int key);
		void KeyboardReleased(int key);

		// For TMPlayer
		void PlayStateChanged(int status);
		void PlayTimeChanged(unsigned char hour, unsigned char min, unsigned char sec);
		void TotalTimeChanged(unsigned char hour, unsigned char min, unsigned char sec);
		void AlbumArtKey(int key, unsigned int);
		void AlbumArtCompleted(unsigned int length);
		void AlbumArtShmData(QString uri, unsigned int size);
		void NoDevice(void);
		void ContentChanged(int content);
		void DeviceChanged(int device, int content);
		void DeviceConnected(int device);
		void DeviceDisconnected(int device);
		void DeviceReady(int device);
		void ResetInformation();
		void FileNumberChanged(int number);
		void FileNumberUpdated(int device, int content, int number, int totalNumber);
		void FileNameChanged(QString name);

		// For Radio
		void RadioTypeChanged(int type);
		void RadioPresetChanged(int type, int freq, int idx);
		void RadioFrequencyChanged(int freq);
		void RadioServiceChanged(QString service);
		void RadioServicePresetChanged(int type, QString service, int idx);
		void HDRadioStatus(int status);

		//For CarPlay
		void NoCarplay(void);

		// For Connectivity
		void ConnectivitySourceConnected(int source);
		void ConnectivitySourceDisconnected(int source);
	private:
		int curr_radio;

};

LauncherIFManager *GetLauncherIFManager();
#define LAUNCHER_IF_MANAGER	(GetLauncherIFManager())

#endif // LAUNCHER_IF_MANAGER_H
