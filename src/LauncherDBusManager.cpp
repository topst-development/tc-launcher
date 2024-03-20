/****************************************************************************************
 *   FileName    : LauncherDBusManager.cpp
 *   Description : LauncherDBusManager.cpp
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

#include <cstdio>
#include <cstdlib>
#include <dbus/dbus.h>
#include "DBusMsgDef.h"
#include "TCDBusRawAPI.h"
#include "LauncherIFManager.h"
#include "LauncherDBusManager.h"
#include "StatusWidget.h"

extern int g_launcherAppID;
static int g_musicAppID = 1;
static int g_carplayAppID = 3;
static int g_radioAppID = 11;
static int g_drmAppID = 12;
static int g_dabAppID = 13;
static int g_bluetoothAppID = 6;

extern int g_debug;
#define LAUNCHER_DBUS_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER DBUS MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

typedef void (*DBusMethodCallFunction)(DBusMessage *message);


static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface);
static DBusMsgErrorCode OnReceivedMethodCall(DBusMessage *message, const char *interface);

static void OnDBusInitialized(void);

static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message);
static void TMPlayerSignalDBusProcess(unsigned int id, DBusMessage *message);
static void RadioSignalDBusProcess(unsigned int id, DBusMessage *message);
static void DABSignalDBusProcess(unsigned int id, DBusMessage *message);
static void DRMSignalDBusProcess(unsigned int id, DBusMessage *message);
static void AppleDeviceManagerSignalDBusProcess(unsigned int id, DBusMessage *message);

static void LauncherDBusEmitSignal(unsigned int id, int value);
static void DBusMethodActiveApplication(DBusMessage *message);
static void DBusMethodStartApplications(DBusMessage *message);
static void DBusMethodPreemptKeyEvent(DBusMessage *message);
static void DBusMethodReleasePreemptKeyEvent(DBusMessage *message);

DBusMethodCallFunction _DBusMethodProcess[TotalMethodLauncherEvents] = {
	DBusMethodActiveApplication,
	DBusMethodStartApplications,
	DBusMethodPreemptKeyEvent,
	DBusMethodReleasePreemptKeyEvent
};

void InitializeLauncherDBusManager(void)
{
	SetDBusPrimaryOwner(LAUNCHER_PROCESS_DBUS_NAME);
	SetCallBackFunctions(OnReceivedDBusSignal, OnReceivedMethodCall);
	AddMethodInterface(LAUNCHER_EVENT_INTERFACE);
	AddSignalInterface(MODEMANAGER_EVENT_INTERFACE);
	AddSignalInterface(TMPLAYER_EVENT_INTERFACE);
	AddSignalInterface(RADIO_EVENT_INTERFACE);
	AddSignalInterface(DAB_EVENT_INTERFACE);
	AddSignalInterface(DRM_EVENT_INTERFACE);
	AddSignalInterface(ADM_MANAGER_INTERFACE);
	InitializeRawDBusConnection("LAUNCHER DBUS");
	SetDBusInitCallBackFunctions(OnDBusInitialized);
}

void ReleaseLauncherDBusManager(void)
{
	ReleaseRawDBusConnection();
}

void LauncherDBusEmitKeyPressed(int key)
{
	LauncherDBusEmitSignal(SignalLauncherKeyPressed, key);
}

void LauncherDBusEmitKeyLongPressed(int key)
{
	LauncherDBusEmitSignal(SignalLauncherKeyLongPressed, key);
}

void LauncherDBusEmitKeyLongLongPressed(int key)
{
	LauncherDBusEmitSignal(SignalLauncherKeyLongLongPressed, key);
}

void LauncherDBusEmitKeyReleased(int key)
{
	LauncherDBusEmitSignal(SignalLauncherKeyReleased, key);
}

void LauncherDBusEmitKeyClicked(int key)
{
	LauncherDBusEmitSignal(SignalLauncherKeyClicked, key);
}

void LauncherDBusEmitEnableKeyEvent(int app)
{
	LauncherDBusEmitSignal(SignalLauncherEnableKeyEvent, app);
}

void LauncherDBusEmitDisableKeyEvent(int app)
{
	LauncherDBusEmitSignal(SignalLauncherDisableKeyEvent, app);
}

void SendDBusChangeMode(const char* mode, int app)
{
	DBusMessage *message;
	int response;

	message = CreateDBusMsgMethodCall(MODEMANAGER_PROCESS_DBUS_NAME, MODEMANAGER_PROCESS_OBJECT_PATH,
			MODEMANAGER_EVENT_INTERFACE,
			CHANGE_MODE,
			DBUS_TYPE_STRING, &mode,
			DBUS_TYPE_INT32, &app,
			DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		DBusPendingCall* pending = NULL;
		if(SendDBusMessage(message, &pending))
		{
			if(pending != NULL )
			{
				if(GetArgumentFromDBusPendingCall(pending,
												  DBUS_TYPE_INT32, &response,
												  DBUS_TYPE_INVALID))
				{
					if(!response)
					{
						LAUNCHER_DBUS_MANAGER_PRINTF("%s: reply : %s\n", __FUNCTION__, response ? "ACK" : "NACK");
					}
				}
				else
				{
					LAUNCHER_DBUS_MANAGER_PRINTF("%s: GetArgmentFromDBusPendingCall return error\n", __FUNCTION__);
				}
				dbus_pending_call_unref(pending);
			}
		}
		else
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}
		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusEndMode(const char* mode, int app)
{
	DBusMessage *message;

	message = CreateDBusMsgMethodCall(MODEMANAGER_PROCESS_DBUS_NAME, MODEMANAGER_PROCESS_OBJECT_PATH,
			MODEMANAGER_EVENT_INTERFACE,
			END_MODE,
			DBUS_TYPE_STRING, &mode,
			DBUS_TYPE_INT32, &app,
			DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusReleaseResourceDone(int resources, int app)
{
	DBusMessage *message;

	message = CreateDBusMsgMethodCall(MODEMANAGER_PROCESS_DBUS_NAME, MODEMANAGER_PROCESS_OBJECT_PATH,
			MODEMANAGER_EVENT_INTERFACE,
			RELEASE_RESOURCE_DONE,
			DBUS_TYPE_INT32, &resources,
			DBUS_TYPE_INT32, &app,
			DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusChangeDevice(unsigned char device, bool play)
{
	DBusMessage *message;
	dbus_bool_t play_d;

	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	play_d = play ? TRUE : FALSE;
	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_CHANGE_DEVICE,
									  DBUS_TYPE_BYTE, &device,
									  DBUS_TYPE_BOOLEAN, &play_d,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusPlayStart(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_START,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusPlayResume(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_RESUME,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusPlayPause(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_PAUSE,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusTrackUp(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_UP,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusTrackDown(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_DOWN,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusTrackSeek(unsigned char hour, unsigned char min, unsigned char sec)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_SEEK,
									  DBUS_TYPE_BYTE, &hour,
									  DBUS_TYPE_BYTE, &min,
									  DBUS_TYPE_BYTE, &sec,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusTrackMove(int number)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(TMPLAYER_PROCESS_DBUS_NAME, TMPLAYER_PROCESS_OBJECT_PATH,
									  TMPLAYER_EVENT_INTERFACE,
									  METHOD_TMPLAYER_TRACK_MOVE,
									  DBUS_TYPE_INT32, &number,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusChangeRadioType(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(RADIO_PROCESS_DBUS_NAME, RADIO_PROCESS_OBJECT_PATH,
									  RADIO_EVENT_INTERFACE,
									  METHOD_RADIO_CHANGE_TYPE,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusSelectRadioPreset(int idx)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(RADIO_PROCESS_DBUS_NAME, RADIO_PROCESS_OBJECT_PATH,
									  RADIO_EVENT_INTERFACE,
									  METHOD_RADIO_SELECT_PRESET,
									  DBUS_TYPE_INT32, &idx,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusRadioTurnOn(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(RADIO_PROCESS_DBUS_NAME, RADIO_PROCESS_OBJECT_PATH,
									  RADIO_EVENT_INTERFACE,
									  METHOD_RADIO_TURN_ON,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusDABTurnOn(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(DAB_PROCESS_DBUS_NAME, DAB_PROCESS_OBJECT_PATH,
									  DAB_EVENT_INTERFACE,
									  METHOD_DAB_TURN_ON,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusDRMTurnOn(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(DRM_PROCESS_DBUS_NAME, DRM_PROCESS_OBJECT_PATH,
									  DRM_EVENT_INTERFACE,
									  METHOD_DRM_TURN_ON,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusSelectDRMPreset(int idx)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(DRM_PROCESS_DBUS_NAME, DRM_PROCESS_OBJECT_PATH,
									  DRM_EVENT_INTERFACE,
									  METHOD_DRM_TURN_ON,
									  DBUS_TYPE_INT32, &idx,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusBluetoothTurnOn(void)
{
	SendDBusChangeMode("audioplaybg", g_bluetoothAppID);
}

void SendDBusMicomManagerSVMOn(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MICOM_MANAGER_PROCESS_DBUS_NAME, MICOM_MANAGER_PROCESS_OBJECT_PATH,
									  MICOM_MANAGER_EVENT_INTERFACE,
									  METHOD_MICOM_MANAGER_SVM_ON,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusMicomManagerSVMOff(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MICOM_MANAGER_PROCESS_DBUS_NAME, MICOM_MANAGER_PROCESS_OBJECT_PATH,
									  MICOM_MANAGER_EVENT_INTERFACE,
									  METHOD_MICOM_MANAGER_SVM_OFF,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusMicomManagerFVCOn(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MICOM_MANAGER_PROCESS_DBUS_NAME, MICOM_MANAGER_PROCESS_OBJECT_PATH,
									  MICOM_MANAGER_EVENT_INTERFACE,
									  METHOD_MICOM_MANAGER_FVC_ON,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

void SendDBusMicomManagerFVCOff(void)
{
	DBusMessage *message;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	message = CreateDBusMsgMethodCall(MICOM_MANAGER_PROCESS_DBUS_NAME, MICOM_MANAGER_PROCESS_OBJECT_PATH,
									  MICOM_MANAGER_EVENT_INTERFACE,
									  METHOD_MICOM_MANAGER_FVC_OFF,
									  DBUS_TYPE_INVALID);
	if (message != NULL)
	{
		if (!SendDBusMessage(message, NULL))
		{
			fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
		}

		dbus_message_unref(message);
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}
}

static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface)
{
	DBusMsgErrorCode error = ErrorCodeNoError;

	if (message != NULL &&
		interface != NULL)
	{
		unsigned int idx;
		bool stop = false;

		if (strcmp(interface, MODEMANAGER_EVENT_INTERFACE) == 0)
		{
			for(idx = ChangedMode; idx < TotalSignalModeManagerEvent && !stop; idx++)
			{
				if(dbus_message_is_signal(message,
										  MODEMANAGER_EVENT_INTERFACE,
										  g_signalModeManagerEventNames[idx]))
				{
					ModeManagerSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}
		else if (strcmp(interface, TMPLAYER_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalTMPlayerPlayFailed; idx < TotalSignalTMPlayerEvent && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   TMPLAYER_EVENT_INTERFACE,
										   g_signalTMPlayerEventNames[idx]))
				{
					TMPlayerSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}
		else if (strcmp(interface, RADIO_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalRadioAMMode; idx < TotalSignalRadioEvent && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   RADIO_EVENT_INTERFACE,
										   g_signalRadioEventNames[idx]))
				{
					RadioSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}
		else if (strcmp(interface, DAB_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalDABMode; idx < TotalSignalDABEvent && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   DAB_EVENT_INTERFACE,
										   g_signalDABEventNames[idx]))
				{
					DABSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}
		else if (strcmp(interface, DRM_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalDRMMode; idx < TotalSignalDRMEvent && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   DRM_EVENT_INTERFACE,
										   g_signalDRMEventNames[idx]))
				{
					DRMSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}
		else if (strcmp(interface, ADM_MANAGER_INTERFACE) == 0)
		{
			for (idx = SignalAppleDeviceManageriAP1Connected; idx < TotalSignalAppleDeviceManagerEvents && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   ADM_MANAGER_INTERFACE,
										   g_signalADMEventNames[idx]))
				{
					AppleDeviceManagerSignalDBusProcess(idx, message);
					stop = true;
				}
			}
		}

		if (!stop)
		{
			error = ErrorCodeUnknown;
		}
	}
	return error;
}

static DBusMsgErrorCode OnReceivedMethodCall(DBusMessage *message, const char *interface)
{
	DBusMsgErrorCode error = ErrorCodeNoError;
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	if (message != NULL &&
		interface != NULL &&
		strcmp(interface, LAUNCHER_EVENT_INTERFACE) == 0)
	{
		unsigned int index;
		bool stop = false;

		for (index = MethodLauncherActiveApplication; index < TotalMethodLauncherEvents && !stop; index++)
		{
			if (dbus_message_is_method_call(message,
											LAUNCHER_EVENT_INTERFACE,
											g_methodLauncherEventNames[index]))
			{
				_DBusMethodProcess[index](message);
				stop = true;
			}
		}

		if (!stop)
		{
			error = ErrorCodeUnknown;
		}
	}

	return error;
}

static void OnDBusInitialized(void)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("D-Bus Initialize Done: %d\n", g_launcherAppID);
	SendDBusChangeMode("home", g_launcherAppID);
}

static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalModeManagerEvent)
	{
        LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalModeManagerEventNames[id], id);
		if (message != NULL)
		{
			if (id == ChangedMode)
			{
				const char* mode;
				int app;
				if (GetArgumentFromDBusMessage(message,
											   DBUS_TYPE_STRING, &mode,
											   DBUS_TYPE_INT32, &app,
											   DBUS_TYPE_INVALID))
				{
					if((strncmp(mode, "home", 4) == 0) && (app == g_launcherAppID))
					{
						LAUNCHER_IF_MANAGER->EmitSetGeometry(false);
						LAUNCHER_IF_MANAGER->EmitShowLauncher();
						//TODO
						//SendChangeMode Navi Applicaiton
					}
					else if((strncmp(mode, "view", 4) == 0) && (app == g_launcherAppID))
					{
						LAUNCHER_IF_MANAGER->EmitSetGeometry(true);
						LAUNCHER_IF_MANAGER->EmitShowLauncher();
					}
					else if (app == g_musicAppID &&
							 ((strncmp(mode, "audioplay", 9) == 0) || (strncmp(mode, "audioplaybg", 11) == 0) ||
							  (strncmp(mode, "videoplay", 9) == 0)))
					{
						LAUNCHER_IF_MANAGER->EmitChangeToMusicApp();
					}
					else if  ((app == g_radioAppID || app == g_drmAppID || app == g_dabAppID) &&
							  ((strncmp(mode, "audioplay", 9) == 0) || (strncmp(mode, "audioplaybg", 11) == 0)))
					{
						LAUNCHER_IF_MANAGER->EmitChangeToRadioApp();
					}
					else if (app == g_carplayAppID &&
							  ((strncmp(mode, "audioplay", 9) == 0) || (strncmp(mode, "audioplaybg", 11) == 0)))
					{
						LAUNCHER_IF_MANAGER->EmitChangeToCarplayApp();
					}
					else if (app == g_bluetoothAppID)
					{
						if ((strncmp(mode, "audioplay", 9) == 0) || (strncmp(mode, "audioplaybg", 11) == 0))
						{
							LAUNCHER_IF_MANAGER->EmitChangeToBluetoothApp();
						}
						else if (strncmp(mode, "idle", 4) == 0)
						{
							LAUNCHER_IF_MANAGER->EmitDisableBluetoothApp();
						}
					}
				}
			}
			else if(id == ReleaseResource)
			{
				int resources;
				int app;
				if (GetArgumentFromDBusMessage(message,
											   DBUS_TYPE_INT32, &resources,
											   DBUS_TYPE_INT32, &app,
											   DBUS_TYPE_INVALID))
				{
					if(app == g_launcherAppID)
					{
						LAUNCHER_IF_MANAGER->EmitHideLauncher(resources, app);
					}
				}
			}
			else if(id == SuspendMode)
			{
				LAUNCHER_IF_MANAGER->EmitChangeToSuspendMode();
			}
			else if(id == ResumeMode)
			{
				LAUNCHER_IF_MANAGER->EmitChangeToResumeMode();
			}
		}
	}
}

static void TMPlayerSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalTMPlayerEvent)
	{
		if (id != SignalTMPlayerPlayTimeChanged)
		{
			LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
										  g_signalTMPlayerEventNames[id], id);
		}

		if (message != NULL)
		{
			switch (id)
			{
				case SignalTMPlayerPlayStateChanged:
					LAUNCHER_IF_MANAGER->EmitPlayStateChanged(message);
					break;
				case SignalTMPlayerPlayTimeChanged:
					LAUNCHER_IF_MANAGER->EmitPlayTimeChanged(message);
					break;
				case SignalTMPlayerTotalTimeChanged:
					LAUNCHER_IF_MANAGER->EmitTotalTimeChange(message);
					break;
				case SignalTMPlayerAlbumArtKey:
					LAUNCHER_IF_MANAGER->EmitAlbumArtKey(message);
					break;
				case SignalTMPlayerAlbumArtCompleted:
					LAUNCHER_IF_MANAGER->EmitAlbumArtCompleted(message);
					break;
				case SignalTMPlayerNoDevice:
					LAUNCHER_IF_MANAGER->EmitNoDevice(message);
					break;
				case SignalTMPlayerContentChanged:
					LAUNCHER_IF_MANAGER->EmitContentChanged(message);
					break;
				case SignalTMPlayerDeviceChagned:
					LAUNCHER_IF_MANAGER->EmitDeviceChanged(message);
					break;
				case SignalTMPlayerDeviceConnected:
					LAUNCHER_IF_MANAGER->EmitDeviceConnected(message);
					break;
				case SignalTMPlayerDeviceDisConnected:
					LAUNCHER_IF_MANAGER->EmitDeviceDisconnected(message);
					break;
				case SignalTMPlayerResetInormation:
					LAUNCHER_IF_MANAGER->EmitResetInformation(message);
					break;
				case SignalTMPlayerFileNumberChanged:
					LAUNCHER_IF_MANAGER->EmitFileNumberChange(message);
					break;
				case SignalTMPlayerFileNumberUpdated:
					LAUNCHER_IF_MANAGER->EmitFileNumberUpdate(message);
					break;
				case SignalTMPlayerFileNameChanged:
					LAUNCHER_IF_MANAGER->EmitFileNameChange(message);
					break;
				case SignalTMPlayerDeviceReady:
					LAUNCHER_IF_MANAGER->EmitDeviceReady(message);
					break;
				case SignalTMPlayerPlayFailed:
				case SignalTMPlayerPlayModeChanged:
				case SignalTMPlayerSeekCompleted:
				case SignalTMPlayerID3Information:
				case SignalTMPlayerDeviceMountStop:
				case SignalTMPlayerNoSong:
				case SignalTMPlayerTotalNumberChanged:
				case SignalTMPlayerFileDBCompleted:
				case SignalTMPlayerFolderNameChanged:
				case SignalTMPlayerFileList:
				case SignalTMPlayerMetaCountChanged:
				case SignalTMPlayerMetaMenuChanged:
				case SignalTMPlayerMetaAddName:
				case SignalTMPlayerMetaAddNameComplate:
				case SignalTMPlayerMetaCreateTrackList:
				case SignalTMPlayerDisplayOn:
				case SignalTMPlayerDisplayOff:
					break;
				case SignalTMPlayerAlbumArtShmData:
					LAUNCHER_IF_MANAGER->EmitAlbumArtShmData(message);
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}
		}
	}
	else
	{
		LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%d]\n",id);
	}
}

static void RadioSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalRadioEvent)
	{
        LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalRadioEventNames[id], id);
		if (message != NULL)
		{
			switch (id)
			{
				case SignalRadioAMMode:
					LAUNCHER_IF_MANAGER->EmitRadioAMChanged(message);
					break;
				case SignalRadioFMMode:
					LAUNCHER_IF_MANAGER->EmitRadioFMChanged(message);
					break;
				case SignalRadioHDRMode:
					LAUNCHER_IF_MANAGER->EmitRadioHDRChanged(message);
					break;
				case SignalRadioAMPresetList:
					LAUNCHER_IF_MANAGER->EmitRadioAMPresetList(message);
					break;
				case SignalRadioFMPresetList:
					LAUNCHER_IF_MANAGER->EmitRadioFMPresetList(message);
					break;
				case SignalRadioPresetChanged:
					LAUNCHER_IF_MANAGER->EmitRadioPresetChanged(message);
					break;
				case SignalRadioFrequencyChanged:
					LAUNCHER_IF_MANAGER->EmitRadioFrequencyChanged(message);
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}
		}
	}
}

static void DABSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalDABEvent)
	{
        LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalDABEventNames[id], id);
		if (message != NULL)
		{
			switch (id)
			{
				case SignalDABMode:
					LAUNCHER_IF_MANAGER->EmitDABChanged(message);
					break;
				case SignalDABFrequencyChanged:
					LAUNCHER_IF_MANAGER->EmitRadioFrequencyChanged(message);
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}
		}
	}
}

static void DRMSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalDRMEvent)
	{
        LAUNCHER_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalDRMEventNames[id], id);
		if (message != NULL)
		{
			switch (id)
			{
				case SignalDRMMode:
					LAUNCHER_IF_MANAGER->EmitDRMChanged(message);
					break;
				case SignalDRMPresetChanged:
					LAUNCHER_IF_MANAGER->EmitDRMServicePresetChanged(message);
					break;
				case SignalDRMServiceChanged:
					LAUNCHER_IF_MANAGER->EmitDRMServiceChanged(message);
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}
		}
	}
}

static void AppleDeviceManagerSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalAppleDeviceManagerEvents)
	{
		if (message != NULL)
		{
			switch (id)
			{
				case SignalAppleDeviceManageriAP1Connected:
					break;
				case SignalAppleDeviceManageriAP1Disconnected:
					break;
				case SignalAppleDeviceManageriAP2Connected:
					LAUNCHER_IF_MANAGER->EmitConnectivitySourceConnected(StatusWidget::ConnectivitySourceiAP);
					break;
				case SignalAppleDeviceManageriAP2Disconnected:
					LAUNCHER_IF_MANAGER->EmitConnectivitySourceDisconnected(StatusWidget::ConnectivitySourceiAP);
					break;
				case SignalAppleDeviceManagerCarPlayConnected:
					LAUNCHER_IF_MANAGER->EmitConnectivitySourceConnected(StatusWidget::ConnectivitySourceCarPlay);
					break;
				case SignalAppleDeviceManagerCarPlayDisconnected:
					LAUNCHER_IF_MANAGER->EmitConnectivitySourceDisconnected(StatusWidget::ConnectivitySourceCarPlay);
					break;
				case SignalAppleDeviceManagerAppleDeviceConnected:
				case SignalAppleDeviceManagerAppleDeviceDisconnected:
				case SignalAppleDeviceManagerInitializeComplete:
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}

		}
	}
	else
	{
		fprintf(stderr, "RECEIVED SIGNAL[%d]\n", id);
	}
}

static void LauncherDBusEmitSignal(unsigned int id, int value)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	if (id < TotalSignalLauncherEvents)
	{
		DBusMessage *message;
		message = CreateDBusMsgSignal(LAUNCHER_PROCESS_OBJECT_PATH, LAUNCHER_EVENT_INTERFACE,
									  g_signalLauncherEventNames[id],
									  DBUS_TYPE_INT32, &value,
									  DBUS_TYPE_INVALID);
		if (message != NULL)
		{
			if (SendDBusMessage(message, NULL))
			{
				LAUNCHER_DBUS_MANAGER_PRINTF("EMIT SIGNAL([%d]%s)\n",
											 id, g_signalLauncherEventNames[id]);
			}
			else
			{
				fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
			}

			dbus_message_unref(message);
		}
		else
		{
			fprintf(stderr, "%s: CreateDBusMsgSignal failed\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: signal emit failed.(id[%u])\n", __FUNCTION__, id);

	}
}

static void DBusMethodActiveApplication(DBusMessage *message)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	if (message != NULL)
	{
		int app;
		dbus_bool_t active;
		if (GetArgumentFromDBusMessage(message,
									   DBUS_TYPE_INT32, &app,
									   DBUS_TYPE_BOOLEAN, &active,
									   DBUS_TYPE_INVALID))
		{
			LAUNCHER_DBUS_MANAGER_PRINTF("APPLICATION(%d), ACTIVE(%s)\n",
										 app, (active != 0) ? "TRUE" : "FALSE");
			LAUNCHER_IF_MANAGER->EmitActiveApplication(app, active);

			if ((app == g_carplayAppID) && !active)
			{
				LAUNCHER_IF_MANAGER->EmitNoCarplay();
			}
		}
		else
		{
			fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: mesage is NULL\n", __FUNCTION__);
	}
}

static void DBusMethodStartApplications(DBusMessage *message)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");
	LAUNCHER_IF_MANAGER->EmitStartApplications();
	(void)message;
}

static void DBusMethodPreemptKeyEvent(DBusMessage *message)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	if (message != NULL)
	{
		int app;
		if (GetArgumentFromDBusMessage(message,
									   DBUS_TYPE_INT32, &app,
									   DBUS_TYPE_INVALID))
		{
			LAUNCHER_DBUS_MANAGER_PRINTF("APPLICATION(%d)\n",
											  app);
			LAUNCHER_IF_MANAGER->EmitPreemptKeyEvent(app);
		}
		else
		{
			fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: mesage is NULL\n", __FUNCTION__);
	}
}

static void DBusMethodReleasePreemptKeyEvent(DBusMessage *message)
{
	LAUNCHER_DBUS_MANAGER_PRINTF("\n");

	if (message != NULL)
	{
		int app;
		if (GetArgumentFromDBusMessage(message,
									   DBUS_TYPE_INT32, &app,
									   DBUS_TYPE_INVALID))
		{
			LAUNCHER_DBUS_MANAGER_PRINTF("APPLICATION(%d)\n",
											  app);
			LAUNCHER_IF_MANAGER->EmitReleasePreemptKeyEvent(app);
		}
		else
		{
			fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
		}
	}
	else
	{
		fprintf(stderr, "%s: mesage is NULL\n", __FUNCTION__);
	}
}
