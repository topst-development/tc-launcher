/****************************************************************************************
 *   FileName    : MediaPlayerType.h
 *   Description : MediaPlayerType.h
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

#ifndef MEDIA_PLAYER_TYPE_H
#define MEDIA_PLAYER_TYPE_H

typedef enum {
	DeviceSourceMyMusic,
	DeviceSourceUSB1,
	DeviceSourceUSB2,
	DeviceSourceUSB3,
	DeviceSourceSDMMC,
	DeviceSourceAUX,
	DeviceSourceiAP2,
	DeviceSourceBluetooth,
	TotalDeviceSources
} DeviceSource;

typedef enum {
	MultiMediaContentTypeAudio,
	MultiMediaContentTypeVideo,
	TotalMultiMediaContentTypes
} MultiMediaContentType;

typedef enum {
	Audio_Entertainment,		// Entertainment volume type
	Audio_Navigation,			// Navigation volume type
	Audio_Alert_Text,			// Alert (Text) volume type
	Audio_Alert_Call,			// Alert (Call) volume type
	Audio_Alert_Emer,			// Alert (Emergency) volume type
	Audio_Type_Max
} TcAudioStreamType;

extern const char *g_deviceSourceNames[TotalDeviceSources];

#define MAX_NAME_LENGTH		512

typedef enum {
	RadioTypeFM,
	RadioTypeAM,
	RadioTypeDAB,
	RadioTypeDRM,
	TotalRadioTypes
} RadioTypes;

extern const char *g_radioTypeNames[TotalRadioTypes];

#endif /* MEDIA_PLAYER_TYPE_H */
