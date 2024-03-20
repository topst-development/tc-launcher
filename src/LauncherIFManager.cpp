/****************************************************************************************
 *   FileName    : LauncherIFManager.cpp
 *   Description : LauncherIFManager.cpp
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

#include <cstdlib>
#include <cstdio>
#include <unistd.h>
#include "TCInput.h"
#include "TCDBusRawAPI.h"
#include "LauncherDBusManager.h"
#include "LauncherIFManager.h"

extern int g_debug;
#define LAUNCHER_IF_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER IF MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

static void InitializeInputInterface(const char *device);
static void ReleaseInputInterface(void);
static void OnKeyboardPressed(int key);
static void OnKeyboardLongPressed(int key);
static void OnKeyboardLongLongPressed(int key);
static void OnKeyboardReleased(int key);
static void OnKeyboardClicked(int key);
static void OnRotaryEvent(int key);

LauncherIFManager::LauncherIFManager(QObject *parent) :
		QThread(parent)
{
}

void LauncherIFManager::Initilaize(const char *device)
{
	this->start();
	InitializeInputInterface(device);
	InitializeLauncherDBusManager();
}

void LauncherIFManager::Release(void)
{
	ReleaseInputInterface();
	ReleaseLauncherDBusManager();
	this->exit();
	this->wait();
}

void LauncherIFManager::EmitSetGeometry(bool full)
{
	LAUNCHER_IF_MANAGER_PRINTF("\n");
	emit SetGeometry(full);
}

void LauncherIFManager::EmitShowLauncher()
{
	LAUNCHER_IF_MANAGER_PRINTF("\n");
	emit ShowLauncher();
}

void LauncherIFManager::EmitHideLauncher(int resources, int app)
{
	LAUNCHER_IF_MANAGER_PRINTF("\n");
	emit HideLauncher(resources, app);
}

void LauncherIFManager::EmitChangeToMusicApp(void)
{
	emit ChangeToMusicApp();
}

void LauncherIFManager::EmitChangeToRadioApp(void)
{
	emit ChangeToRadioApp();
}

void LauncherIFManager::EmitChangeToCarplayApp(void)
{
	emit ChangeToCarplayApp();
}

void LauncherIFManager::EmitChangeToBluetoothApp(void)
{
	emit ChangeToBluetoothApp();
}

void LauncherIFManager::EmitDisableBluetoothApp(void)
{
	emit DisableBluetoothApp();
}

void LauncherIFManager::EmitChangeToSuspendMode(void)
{
	emit ChangeToSuspendMode();
}

void LauncherIFManager::EmitChangeToResumeMode(void)
{
	emit ChangeToResumeMode();
}

void LauncherIFManager::EmitActiveApplication(int app, int active)
{
	LAUNCHER_IF_MANAGER_PRINTF("APPLICATION(%d), ACTIVE(%s)\n", 
							   app, (active != 0) ? "TRUE" : "FALSE");
	emit ActiveApplication(app, active);
}

void LauncherIFManager::EmitStartApplications()
{
	LAUNCHER_IF_MANAGER_PRINTF("\n");
	emit StartApplications();
}

void LauncherIFManager::EmitPreemptKeyEvent(int app)
{
	LAUNCHER_IF_MANAGER_PRINTF("APPLICATION(%d)\n",
							   app);
	emit PreemptKeyEvent(app);
}

void LauncherIFManager::EmitReleasePreemptKeyEvent(int app)
{
	LAUNCHER_IF_MANAGER_PRINTF("APPLICATION(%d)\n",
							   app);
	emit ReleasePreemptKeyEvent(app);
}

void LauncherIFManager::EmitKeyboardPressedEvent(int key)
{
	LAUNCHER_IF_MANAGER_PRINTF("KEY(%d)\n", 
									  key);
	emit KeyboardPressed(key);
}

void LauncherIFManager::EmitKeyboardClickedEvent(int key)
{
	LAUNCHER_IF_MANAGER_PRINTF("KEY(%d)\n", 
									  key);
	emit KeyboardClicked(key);
}

void LauncherIFManager::EmitKeyboardLongPressedEvent(int key)
{
	LAUNCHER_IF_MANAGER_PRINTF("KEY(%d)\n", 
									  key);
	emit KeyboardLongPressed(key);
}

void LauncherIFManager::EmitKeyboardLongLongPressedEvent(int key)
{
	LAUNCHER_IF_MANAGER_PRINTF("KEY(%d)\n", 
									  key);
	emit KeyboardLongLongPressed(key);
}

void LauncherIFManager::EmitKeyboardReleasedEvent(int key)
{
	LAUNCHER_IF_MANAGER_PRINTF("KEY(%d)\n", 
									  key);
	emit KeyboardReleased(key);
}

void LauncherIFManager::EmitPlayStateChanged(DBusMessage *message)
{
	unsigned char status;
	LAUNCHER_IF_MANAGER_PRINTF("\n");

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &status,
								   DBUS_TYPE_INVALID))
	{
		emit PlayStateChanged((int)status);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitPlayTimeChanged(DBusMessage *message)
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &hour,
								   DBUS_TYPE_BYTE, &min,
								   DBUS_TYPE_BYTE, &sec,
								   DBUS_TYPE_INVALID))
	{
		emit PlayTimeChanged(hour, min, sec);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitTotalTimeChange(DBusMessage *message)
{
	unsigned char hour;
	unsigned char min;
	unsigned char sec;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &hour,
								   DBUS_TYPE_BYTE, &min,
								   DBUS_TYPE_BYTE, &sec,
								   DBUS_TYPE_INVALID))
	{
		emit TotalTimeChanged(hour, min, sec);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitAlbumArtKey(DBusMessage *message)
{
	int key;
	unsigned int size;
	LAUNCHER_IF_MANAGER_PRINTF("\n");

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &key,
								   DBUS_TYPE_UINT32, &size,
								   DBUS_TYPE_INVALID))
	{
		emit AlbumArtKey(key, size);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitAlbumArtCompleted(DBusMessage *message)
{
	int number;
	unsigned int length;
	LAUNCHER_IF_MANAGER_PRINTF("\n");

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &number,
								   DBUS_TYPE_UINT32, &length,
								   DBUS_TYPE_INVALID))
	{
		(void)number;
		emit AlbumArtCompleted(length);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitAlbumArtShmData(DBusMessage *message)
{
	const char *uri;
	unsigned int size;
	LAUNCHER_IF_MANAGER_PRINTF("\n");

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_STRING, &uri,
								   DBUS_TYPE_UINT32, &size,
								   DBUS_TYPE_INVALID))
	{
		emit AlbumArtShmData(QString(uri), size);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitNoDevice(DBusMessage *message)
{
	emit NoDevice();
	(void)message;
}

void LauncherIFManager::EmitContentChanged(DBusMessage *message)
{
	unsigned char contentType;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &contentType,
								   DBUS_TYPE_INVALID))
	{
		emit ContentChanged((int)contentType);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitDeviceChanged(DBusMessage *message)
{
	unsigned char device;
	unsigned char contentType;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_BYTE, &contentType,
								   DBUS_TYPE_INVALID))
	{
		emit DeviceChanged((int)device, (int)contentType);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitDeviceConnected(DBusMessage *message)
{
	unsigned char device;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_INVALID))
	{
		emit DeviceConnected((int)device);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitDeviceDisconnected(DBusMessage *message)
{
	unsigned char device;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_INVALID))
	{
		emit DeviceDisconnected(device);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitDeviceReady(DBusMessage *message)
{
	unsigned char device;

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_INVALID))
	{
		if (device < TotalDeviceSources)
		{
			emit DeviceReady(device);
		}
		else
		{
			fprintf(stderr, "%s: invalid device(%d)\n", __FUNCTION__, device);
		}
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitResetInformation(DBusMessage *message)
{
	LAUNCHER_IF_MANAGER_PRINTF("\n");
	emit ResetInformation();
	(void)message;
}

void LauncherIFManager::EmitFileNumberChange(DBusMessage *message)
{
	int number;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &number,
								   DBUS_TYPE_INVALID))
	{
		emit FileNumberChanged(number);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitFileNumberUpdate(DBusMessage *message)
{
	unsigned char device;
	unsigned char content;
	int number;
	int totalNumber;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_BYTE, &content,
								   DBUS_TYPE_INT32, &number,
								   DBUS_TYPE_INT32, &totalNumber,
								   DBUS_TYPE_INVALID))
	{
		emit FileNumberUpdated(device,content,number, totalNumber);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitFileNameChange(DBusMessage *message)
{
	const char *name;
	unsigned int length;

	LAUNCHER_IF_MANAGER_PRINTF("\n");
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_STRING, &name,
								   DBUS_TYPE_UINT32, &length,
								   DBUS_TYPE_INVALID))
	{
		//QString fileName = GetNameString(name, length);
		QString fileName = name;
		emit FileNameChanged(fileName);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitRadioAMChanged(DBusMessage *message)
{
	curr_radio = RadioTypeAM;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INVALID))
	{
		emit RadioTypeChanged(curr_radio);
	}
}

void LauncherIFManager::EmitRadioFMChanged(DBusMessage *message)
{
	curr_radio = RadioTypeFM;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INVALID))
	{
		emit RadioTypeChanged(curr_radio);
	}
}

void LauncherIFManager::EmitRadioHDRChanged(DBusMessage *message)
{
	int hdr;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &hdr,
								   DBUS_TYPE_INVALID))
	{
		emit HDRadioStatus(hdr);
	}
}

void LauncherIFManager::EmitDABChanged(DBusMessage *message)
{
	curr_radio = RadioTypeDAB;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INVALID))
	{
		emit RadioTypeChanged(curr_radio);
	}
}

void LauncherIFManager::EmitDRMChanged(DBusMessage *message)
{
	curr_radio = RadioTypeDRM;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INVALID))
	{
		emit RadioTypeChanged(curr_radio);
	}
}
void LauncherIFManager::EmitRadioPresetChanged(DBusMessage *message)
{
	int freq;
	int idx;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &freq,
								   DBUS_TYPE_INT32, &idx,
								   DBUS_TYPE_INVALID))
	{
		emit RadioPresetChanged(curr_radio, freq, idx);
	}
}

void LauncherIFManager::EmitRadioFrequencyChanged(DBusMessage *message)
{
	int freq;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_INT32, &freq,
								   DBUS_TYPE_INVALID))
	{
		emit RadioFrequencyChanged(freq);
	}
}

void LauncherIFManager::EmitDRMServiceChanged(DBusMessage *message)
{
	const char* service;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_STRING, &service,
								   DBUS_TYPE_INVALID))
	{
		emit RadioServiceChanged(QString(service));
	}
}

void LauncherIFManager::EmitDRMServicePresetChanged(DBusMessage *message)
{
	const char* service;
	int idx;
	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_STRING, &service,
								   DBUS_TYPE_INT32, &idx,
								   DBUS_TYPE_INVALID))
	{
		emit RadioServicePresetChanged(RadioTypeDRM, QString(service), idx);
	}
}

void LauncherIFManager::EmitRadioAMPresetList(DBusMessage *message)
{
	int *presets;
	int count;

	if(GetArgumentFromDBusMessage(message,
								  DBUS_TYPE_ARRAY, DBUS_TYPE_INT32, &presets, &count,
								  DBUS_TYPE_INVALID))
	{
		for (int i = 0; i < count; i++)
		{
			emit RadioPresetChanged(RadioTypeAM, presets[i], i);
		}
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitRadioFMPresetList(DBusMessage *message)
{
	int *presets;
	int count;

	if(GetArgumentFromDBusMessage(message,
								  DBUS_TYPE_ARRAY, DBUS_TYPE_INT32, &presets, &count,
								  DBUS_TYPE_INVALID))
	{
		for (int i = 0; i < count; i++)
		{
			emit RadioPresetChanged(RadioTypeFM, presets[i], i);
		}
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void LauncherIFManager::EmitNoCarplay(void)
{
	emit NoCarplay();
}

void LauncherIFManager::EmitConnectivitySourceConnected(int source)
{
	emit ConnectivitySourceConnected(source);
}

void LauncherIFManager::EmitConnectivitySourceDisconnected(int source)
{
	emit ConnectivitySourceDisconnected(source);
}

void LauncherIFManager::OnShowApplication(int app)
{
	SendDBusChangeMode("view", app);
}

void LauncherIFManager::OnShowHome(int app)
{
	SendDBusChangeMode("home", app);
}

void LauncherIFManager::OnHideCompleted(int resources, int app)
{
	SendDBusReleaseResourceDone(resources, app);
}

void LauncherIFManager::OnEnableKeyEvent(int app)
{
	LauncherDBusEmitEnableKeyEvent(app);
}

void LauncherIFManager::OnDisableKeyEvent(int app)
{
	LauncherDBusEmitDisableKeyEvent(app);
}

void LauncherIFManager::OnChangeMedia(unsigned char device, bool play)
{
	SendDBusChangeDevice(device, play);
}

void LauncherIFManager::OnPlayStart(int device)
{
	SendDBusPlayStart();
	(void)device;
}

void LauncherIFManager::OnPlayResume(int device)
{
	SendDBusPlayResume();
	(void)device;
}

void LauncherIFManager::OnPlayPause(int device)
{
	SendDBusPlayPause();
	(void)device;
}

void LauncherIFManager::OnTrackUp(int device)
{
	SendDBusTrackUp();
	(void)device;
}

void LauncherIFManager::OnTrackDown(int device)
{
	SendDBusTrackDown();
	(void)device;
}

void LauncherIFManager::OnTrackSeek(int device, unsigned char hour, unsigned char min, unsigned char sec)
{
	SendDBusTrackSeek(hour, min, sec);
	(void)device;
}

void LauncherIFManager::OnTrackMove(int number)
{
	SendDBusTrackMove(number);
}

void LauncherIFManager::OnChangeRadioType(void)
{
	SendDBusChangeRadioType();
}

void LauncherIFManager::OnSelectRadioPreset(int idx)
{
	if(curr_radio == RadioTypeDAB)
	{
		//TODO
		;
	}
	else if(curr_radio == RadioTypeDRM)
	{
		SendDBusSelectDRMPreset(idx);
	}
	else
	{
		SendDBusSelectRadioPreset(idx);
	}
}

void LauncherIFManager::OnRadioTurnOn(void)
{
	if(curr_radio == RadioTypeDAB)
	{
		SendDBusDABTurnOn();
	}
	else if(curr_radio == RadioTypeDRM)
	{
		SendDBusDRMTurnOn();
	}
	else
	{
		SendDBusRadioTurnOn();
	}

}

void LauncherIFManager::OnBluetoothTurnOn(void)
{
	SendDBusBluetoothTurnOn();
}

void LauncherIFManager::OnSVMOn(void)
{
	SendDBusMicomManagerSVMOn();
}

void LauncherIFManager::OnSVMOff(void)
{
	SendDBusMicomManagerSVMOff();
}

void LauncherIFManager::OnFVCOn(void)
{
	SendDBusMicomManagerFVCOn();
}

void LauncherIFManager::OnFVCOff(void)
{
	SendDBusMicomManagerFVCOff();
}

static LauncherIFManager _launcherIFManager;
LauncherIFManager *GetLauncherIFManager()
{
	return &_launcherIFManager;
}

static void InitializeInputInterface(const char *device)
{
	int start;
	InitialzieInputProcess(device);
	
	SetPressedEvent(OnKeyboardPressed);
	SetLongPressedEvent(OnKeyboardLongPressed);
	SetLongLongPressedEvent(OnKeyboardLongLongPressed);
	SetReleasedEvent(OnKeyboardReleased);
	SetClickedEvent(OnKeyboardClicked);
	SetRotaryEvent(OnRotaryEvent);
	
	start = StartInputProcess();
	if (!start)
	{
		fprintf(stderr, "input process start failed.\n");
	}
}

static void ReleaseInputInterface(void)
{
	ExitInputProcess();
}

static void OnKeyboardPressed(int key)
{
	LauncherDBusEmitKeyPressed(key);
	LAUNCHER_IF_MANAGER->EmitKeyboardPressedEvent(key);
}

static void OnKeyboardLongPressed(int key)
{
	LauncherDBusEmitKeyLongPressed(key);
	LAUNCHER_IF_MANAGER->EmitKeyboardLongPressedEvent(key);
}

static void OnKeyboardLongLongPressed(int key)
{
	LauncherDBusEmitKeyLongLongPressed(key);
	LAUNCHER_IF_MANAGER->EmitKeyboardLongLongPressedEvent(key);
}

static void OnKeyboardReleased(int key)
{
	LauncherDBusEmitKeyReleased(key);
	LAUNCHER_IF_MANAGER->EmitKeyboardReleasedEvent(key);
}

static void OnKeyboardClicked(int key)
{
	LauncherDBusEmitKeyClicked(key);
	LAUNCHER_IF_MANAGER->EmitKeyboardClickedEvent(key);
}

static void OnRotaryEvent(int key)
{
	if (key == 1)
	{
		key = g_knobKeys[TCKeyVolumeUp];
	}
	else if (key == -1)
	{
		key = g_knobKeys[TCKeyVolumeDown];
	}

	LAUNCHER_IF_MANAGER->EmitKeyboardClickedEvent(key);
	LauncherDBusEmitKeyClicked(key);
}
