/****************************************************************************************
 *   FileName    : LauncherDBusManager.h
 *   Description : LauncherDBusManager.h
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

#ifndef LAUNCHER_DBUS_MANAGER_H
#define LAUNCHER_DBUS_MANAGER_H

void InitializeLauncherDBusManager(void);
void ReleaseLauncherDBusManager(void);
void LauncherDBusEmitKeyPressed(int key);
void LauncherDBusEmitKeyLongPressed(int key);
void LauncherDBusEmitKeyLongLongPressed(int key);
void LauncherDBusEmitKeyReleased(int key);
void LauncherDBusEmitKeyClicked(int key);
void LauncherDBusEmitEnableKeyEvent(int app);
void LauncherDBusEmitDisableKeyEvent(int app);

void SendDBusChangeMode(const char* mode, int app);
void SendDBusEndMode(const char* mode, int app);
void SendDBusReleaseResourceDone(int resources, int app);

// For TMPlayer
void SendDBusChangeDevice(unsigned char device, bool play);
void SendDBusPlayStart(void);
void SendDBusPlayResume(void);
void SendDBusPlayPause(void);
void SendDBusTrackUp(void);
void SendDBusTrackDown(void);
void SendDBusTrackSeek(unsigned char hour, unsigned char min, unsigned char sec);
void SendDBusTrackMove(int number);

// For Radio Application
void SendDBusChangeRadioType(void);
void SendDBusSelectRadioPreset(int idx);
void SendDBusRadioTurnOn(void);

void SendDBusDABTurnOn(void);
void SendDBusDRMTurnOn(void);
void SendDBusSelectDRMPreset(int idx);

void SendDBusBluetoothTurnOn(void);

// For MicomManager
void SendDBusMicomManagerSVMOn(void);
void SendDBusMicomManagerSVMOff(void);
void SendDBusMicomManagerFVCOn(void);
void SendDBusMicomManagerFVCOff(void);


#endif // LAUNCHER_DBUS_MANAGER_H
