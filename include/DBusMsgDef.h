/****************************************************************************************
 *   FileName    : DBusMsgDef.h
 *   Description : DBusMsgDef.h
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

#ifndef DBUS_MSG_DEF_H
#define DBUS_MSG_DEF_H

/*================================================ LAUNCHER PROCESS DEFINE START ================================================*/

#define LAUNCHER_PROCESS_DBUS_NAME					"telechips.launcher.process"
#define LAUNCHER_PROCESS_OBJECT_PATH				"/telechips/launcher/process"

#define LAUNCHER_EVENT_INTERFACE					"launcher.event"
// LAUNCHER SIGNAL EVENT DEFINES
#define SIGNAL_LAUNCHER_KEY_PRESSED					"signal_launcher_key_pressed"
#define SIGNAL_LAUNCHER_KEY_LONG_PRESSED			"signal_launcher_key_long_pressed"
#define SIGNAL_LAUNCHER_KEY_LONG_LONG_PRESSED		"signal_launcher_key_long_long_pressed"
#define SIGNAL_LAUNCHER_KEY_RELEASED				"signal_launcher_key_released"
#define SIGNAL_LAUNCHER_KEY_CLICKED					"signal_launcher_key_clicked"
#define SIGNAL_LAUNCHER_ENABLE_KEY_EVENT			"signal_launcher_enable_key_event"
#define SIGNAL_LAUNCHER_DISABLE_KEY_EVENT			"signal_launcher_disable_key_event"

typedef enum {
	SignalLauncherKeyPressed,
	SignalLauncherKeyLongPressed,
	SignalLauncherKeyLongLongPressed,
	SignalLauncherKeyReleased,
	SignalLauncherKeyClicked,
	SignalLauncherEnableKeyEvent,
	SignalLauncherDisableKeyEvent,
	TotalSignalLauncherEvents
} SignalLauncherEvent;
extern const char *g_signalLauncherEventNames[TotalSignalLauncherEvents];

// LAUNCHER METHOD EVENT DEFINES
#define METHOD_LAUNCHER_ACTIVE_APPLICATION			"method_launcher_active_application"
#define METHOD_LAUNCHER_START_APPLICATIONS			"method_launcher_start_applications"
#define METHOD_LAUNCHER_PREEMPT_KEY_EVENT			"method_launcher_preempt_key_event"
#define METHOD_LAUNCHER_RELEASE_PREEMPT_KEY_EVENT	"method_launcher_release_preempt_key_event"

typedef enum {
	MethodLauncherActiveApplication,
	MethodLauncherStartApplications,
	MethodLauncherPreemptKeyEvent,
	MethodLauncherReleasePreemptKeyEvent,
	TotalMethodLauncherEvents
} MethodLauncherEvent;
extern const char *g_methodLauncherEventNames[TotalMethodLauncherEvents];

/*================================================ LAUNCHER PROCESS DEFINE END ================================================*/

/*================================================ MODEMANAGER PROCESS DEFINE START ================================================*/
#define MODEMANAGER_PROCESS_DBUS_NAME					"telechips.mode.manager"
#define MODEMANAGER_PROCESS_OBJECT_PATH					"/telechips/mode/manager"
#define MODEMANAGER_EVENT_INTERFACE						"mode.manager"

#define CHANGE_MODE										"change_mode"
#define RELEASE_RESOURCE_DONE							"release_resource_done"
#define END_MODE										"end_mode"
#define MODE_ERROR_OCCURED								"mode_error_occured"

typedef enum{
	ChangeMode,
	ReleaseResource_Done,
	EndMode,
	ModeErrorOccured,
	TotalMethodModeManagerEvent
}MethodModeManagerEvent;
extern const char* g_methodModeManagerEventNames[TotalMethodModeManagerEvent];

#define CHANGED_MODE									"changed_mode"
#define RELEASE_RESOURCE								"release_resource"
#define ENDED_MODE										"ended_mode"
#define SUSPEND_MODE									"suspend_mode"
#define RESUME_MODE										"resume_mode"

typedef enum{
	ChangedMode,
	ReleaseResource,
	EndedMode,
	SuspendMode,
	ResumeMode,
	TotalSignalModeManagerEvent
}SignalModeManagerEvent;
extern const char* g_signalModeManagerEventNames[TotalSignalModeManagerEvent];
/*================================================ MODEMANAGER PROCESS DEFINE END ================================================*/

/*================================================ TMPLAYER PROCESS DEFINE START ================================================*/
#define TMPLAYER_PROCESS_DBUS_NAME							"telechips.tmplayer.process"
#define TMPLAYER_PROCESS_OBJECT_PATH						"/telechips/tmplayer/process"
#define TMPLAYER_EVENT_INTERFACE							"tmplayer.event"


// TMPLAYER SIGNAL EVENT DEFINES
#define SIGNAL_TMPLAYER_PLAY_FAILED							"signal_tmplayer_play_failed"
#define SIGNAL_TMPLAYER_PLAYSTATE_CHANGED					"signal_tmplayer_playstate_changed"
#define SIGNAL_TMPLAYER_PLAYTIME_CHANGED					"signal_tmplayer_playtime_changed"
#define SIGNAL_TMPLAYER_PLAYMODE_CHANGED					"signal_tmplayer_playmode_changed"
#define SIGNAL_TMPLAYER_TOTALTIME_CHANGED					"signal_tmplayer_totaltime_chagned"
#define SIGNAL_TMPLAYER_SEEK_COMPLETED						"signal_tmplayer_seek_completed"
#define SIGNAL_TMPLAYER_ID3_INFOMATION						"signal_tmplayer_id3_infomation"
#define SIGNAL_TMPLAYER_ALBUMART_KEY						"signal_tmplayer_albumart_key"
#define SIGNAL_TMPLAYER_ALBUMART_COMPLETED					"signal_tmplayer_albumart_completed"
#define SIGNAL_TMPLAYER_NO_DEVICE							"signal_tmplayer_no_device"
#define SIGNAL_TMPLAYER_CONTENT_CHANGED						"signal_tmplayer_content_changed"
#define SIGNAL_TMPLAYER_DEVICE_CHANGED						"signal_tmplayer_device_changed"
#define SIGNAL_TMPLAYER_DEVICE_CONNECTED					"signal_tmplayer_device_connected"
#define SIGNAL_TMPLAYER_DEVICE_DISCONNECTED					"signal_tmplayer_device_disconnect"
#define SIGNAL_TMPLAYER_DEVICE_READY						"signal_tmplayer_device_ready"
#define SIGNAL_TMPLAYER_DEVICE_MOUNT_STOP					"signal_tmplayer_device_mount_stop"
#define SIGNAL_TMPLAYER_NO_SONG								"signal_tmplayer_no_song"
#define SIGNAL_TMPLAYER_RESET_INFORMATION					"signal_tmplayer_reset_information"
#define SIGNAL_TMPLAYER_FILENUMBER_CHANGED					"signal_tmplayer_filenumber_changed"
#define SIGNAL_TMPLAYER_FILENUMBER_UPDATED					"signal_tmplayer_filenumber_updated"
#define SIGNAL_TMPLAYER_TOTALNUMBER_CHAGNED					"signal_tmplayer_totalnumber_changed"
#define SIGNAL_TMPLAYER_FILE_DB_COMPLETED					"signal_tmplayer_file_db_completed"
#define SIGNAL_TMPLAYER_FILENAME_CHANGED					"signal_tmplayer_filename_changed"
#define SIGNAL_TMPLAYER_FOLDERNAME_CHANGED					"signal_tmplayer_foldername_changed"
#define SIGNAL_TMPLAYER_FILELIST							"signal_tmplayer_filelist"
#define SIGNAL_TMPLAYER_META_COUNT_CHANGED					"signal_tmplayer_meta_count_changed"
#define SIGNAL_TMPLAYER_META_MENU_CHANGED					"signal_tmplayer_meta_menu_changed"
#define SIGNAL_TMPLAYER_META_ADD_NAME						"signal_tmplayer_meta_add_name"
#define SIGNAL_TMPLAYER_META_ADD_NAME_COMPLETED				"signal_tmplayer_meta_add_name_completed"
#define SIGNAL_TMPLAYER_META_CREATE_TRACKLIST				"signal_tmplayer_meta_create_tracklist"
#define SIGNAL_TMPLAYER_DISPLAY_ON							"signal_tmplayer_display_on"
#define SIGNAL_TMPLAYER_DISPLAY_OFF							"signal_tmplayer_display_off"
#define SIGNAL_TMPLAYER_ALBUMART_SHMDATA					"signal_tmplayer_albumart_shmdata"

typedef enum{
	SignalTMPlayerPlayFailed,
	SignalTMPlayerPlayStateChanged,
	SignalTMPlayerPlayTimeChanged,
	SignalTMPlayerPlayModeChanged,
	SignalTMPlayerTotalTimeChanged,
	SignalTMPlayerSeekCompleted,
	SignalTMPlayerID3Information,
	SignalTMPlayerAlbumArtKey,
	SignalTMPlayerAlbumArtCompleted,
	SignalTMPlayerNoDevice,
	SignalTMPlayerContentChanged,
	SignalTMPlayerDeviceChagned,
	SignalTMPlayerDeviceConnected,
	SignalTMPlayerDeviceDisConnected,
	SignalTMPlayerDeviceReady,
	SignalTMPlayerDeviceMountStop,
	SignalTMPlayerNoSong,
	SignalTMPlayerResetInormation,
	SignalTMPlayerFileNumberChanged,
	SignalTMPlayerFileNumberUpdated,
	SignalTMPlayerTotalNumberChanged,
	SignalTMPlayerFileDBCompleted,
	SignalTMPlayerFileNameChanged,
	SignalTMPlayerFolderNameChanged,
	SignalTMPlayerFileList,
	SignalTMPlayerMetaCountChanged,
	SignalTMPlayerMetaMenuChanged,
	SignalTMPlayerMetaAddName,
	SignalTMPlayerMetaAddNameComplate,
	SignalTMPlayerMetaCreateTrackList,
	SignalTMPlayerDisplayOn,
	SignalTMPlayerDisplayOff,
	SignalTMPlayerAlbumArtShmData,
	TotalSignalTMPlayerEvent
}SignalTMPlayerEvent;
extern const char* g_signalTMPlayerEventNames[TotalSignalTMPlayerEvent];

//  TMPLAYER METHOD EVENT DEFINES
#define METHOD_TMPLAYER_SET_APP_ID							"method_tmplayer_set_app_id"
#define METHOD_TMPLAYER_CONNECT_DEBUG						"method_tmplayer_connect_debug"
#define METHOD_TMPLAYER_DB_DEBUG							"method_tmplayer_db_debug"
#define METHOD_TMPLAYER_CHANGE_CONTENT_TYPE					"method_tmplayer_change_content_type"
#define METHOD_TMPLAYER_CHANGE_NEXT_DEVICE					"method_tmplayer_change_next_device"
#define METHOD_TMPLAYER_CHANGE_DEVICE						"method_tmplayer_change_device"
#define METHOD_TMPLAYER_SET_BROWSING_MODE					"method_tmplayer_set_browsing_mode"
#define METHOD_TMPLAYER_CHANGE_REPEATMODE					"method_tmplayer_change_repeatmode"
#define METHOD_TMPLAYER_CHANGE_SHUFFLEMODE					"method_tmplayer_change_shufflemode"
#define METHOD_TMPLAYER_TRACK_START							"method_tmplayer_track_start"
#define METHOD_TMPLAYER_TRACK_STOP							"method_tmplayer_track_stop"
#define METHOD_TMPLAYER_TRACK_RESUME						"method_tmplayer_track_resume"
#define METHOD_TMPLAYER_TRACK_PAUSE							"method_tmplayer_track_pause"
#define METHOD_TMPLAYER_TRACK_UP							"method_tmplayer_track_up"
#define METHOD_TMPLAYER_TRACK_DOWN							"method_tmplayer_track_down"
#define METHOD_TMPLAYER_TRACK_MOVE							"method_tmplayer_track_move"
#define METHOD_TMPLAYER_TRACK_SEEK							"method_tmplayer_track_seek"
#define METHOD_TMPLAYER_UPDATE_DISPLAY_INFO					"method_tmplayer_update_display_info"
#define METHOD_TMPLAYER_REQUEST_ALBUMART_KEY				"method_tmplayer_request_albumart_key"
#define METHOD_TMPLAYER_REQUEST_FILELIST					"method_tmplayer_request_filelist"
#define METHOD_TMPLAYER_META_BROWSING_START					"method_tmplayer_meta_browsing_start"
#define METHOD_TMPLAYER_META_BROWSING_REQUEST				"method_tmplayer_meta_browsing_request"
#define METHOD_TMPLAYER_META_BROWSING_SELECT				"method_tmplayer_meta_browsing_select"
#define METHOD_TMPLAYER_META_BROWSING_HOME					"method_tmplayer_meta_browsing_home"
#define METHOD_TMPLAYER_META_BROWSING_UNDO					"method_tmplayer_meta_browsing_undo"
#define METHOD_TMPLAYER_META_BROWSING_END					"method_tmplayer_meta_browsing_end"
#define METHOD_TMPLAYER_REQUEST_DISPLAY						"method_tmplayer_request_display"
#define METHOD_TMPLAYER_GO_LAUNCHER_HOME					"method_tmplayer_go_launcher_home"
#define METHOD_TMPLAYER_GO_LAUNCHER_BACK					"method_tmplayer_go_launcher_back"
#define METHOD_TMPLAYER_GO_AV_ON_OFF						"method_tmplayer_go_av_on_off"
#define METHOD_TMPLAYER_NOTIFY_DISPLAY_DONE					"method_tmplayer_notify_display_done"

typedef enum{
	MethodTMPlayerSetAppID,
	MethodTMPlayerConnectDebug,
	MethodTMPlayerDBDebug,
	MethodTMPlayerChangeContentType,
	MethodTMPlayerChangeNextDevice,
	MethodTMPlayerChangeDevice,
	MethodTMPlayerSetBrowsingMode,
	MethodTMPlayerChangeRepeatMode,
	MethodTMPlayerChangeShuffleMode,
	MethodTMPlayerTrackStart,
	MethodTMPlayerTrackStop,
	MethodTMPlayerTrackResume,
	MethodTMPlayerTrackPause,
	MethodTMPlayerTrackUp,
	MethodTMPlayerTrackDown,
	MethodTMPlayerTrackMove,
	MethodTMPlayerTrackSeek,
	MethodTMPlayerUpdateOverlayInfo,
	MethodTMPlayerRequestAlbumArtKey,
	MethodTMPlayerRequestList,
	MethodTMPlayerMetaBrowsingStart,
	MethodTMPlayerMetaBrowsingRequest,
	MethodTMPlayerMetaBrowsingSelect,
	MethodTMPlayerMetaBrowsingHome,
	MethodTMPlayerMetaBrowsingUndo,
	MethodTMPlayerMetaBrowsingEnd,
	MethodTMPlayerRequestDisplay,
	MethodTMPlayerGoLauncherHome,
	MethodTMPlayerGoLuancherBack,
	MethodTMPlayerGoAVOnOff,
	MethodTMPlayerNotifyDisplayDone,
	TotalMethodTMPlayerEvent
}MethodTMPlayerEvent;
extern const char* g_methodTMPlayerEventNames[TotalMethodTMPlayerEvent];

/*================================================ TMPLAYER PROCESS DEFINE END ================================================*/


/*================================================ RADIO PROCESS DEFINE START ================================================*/
#define RADIO_PROCESS_DBUS_NAME						"telechips.radio.process"
#define RADIO_PROCESS_OBJECT_PATH					"/telechips/radio/process"
#define RADIO_EVENT_INTERFACE						"radio.event"

#define METHOD_RADIO_CHANGE_TYPE					"method_radio_change_type"
#define METHOD_RADIO_SELECT_PRESET					"method_radio_select_preset"
#define METHOD_RADIO_TURN_ON						"method_radio_turn_on"

typedef enum{
	MethodRadioChangeType,
	MethodRadioSelectPreset,
	MethodRadioTurnOn,
	TotalMethodRadioEvent
}MethodRadioEvent;
extern const char* g_methodRadioEventNames[TotalMethodRadioEvent];

#define SIGNAL_RADIO_AM_MODE						"signal_radio_am_mode"
#define SIGNAL_RADIO_FM_MODE						"signal_radio_fm_mode"
#define SIGNAL_RADIO_HDR_MODE						"signal_radio_hdr_mode"
#define SIGNAL_RADIO_AM_PRESET_LIST					"signal_radio_am_preset_list"
#define SIGNAL_RADIO_FM_PRESET_LIST					"signal_radio_fm_preset_list"
#define SIGNAL_RADIO_PRESET_CHANGED					"signal_radio_preset_changed"
#define SIGNAL_RADIO_FREQUENCY_CHANGED				"signal_radio_frequency_changed"

typedef enum{
	SignalRadioAMMode,
	SignalRadioFMMode,
	SignalRadioHDRMode,
	SignalRadioAMPresetList,
	SignalRadioFMPresetList,
	SignalRadioPresetChanged,
	SignalRadioFrequencyChanged,
	TotalSignalRadioEvent
}SignalRadioEvent;
extern const char* g_signalRadioEventNames[TotalSignalRadioEvent];

/*================================================ RADIO PROCESS DEFINE END ================================================*/

/*================================================ DAB PROCESS DEFINE START ================================================*/
#define DAB_PROCESS_DBUS_NAME                     "telechips.dab.process"
#define DAB_PROCESS_OBJECT_PATH                   "/telechips/dab/process"
#define DAB_EVENT_INTERFACE                       "dab.event"

#define METHOD_DAB_TURN_ON                        "method_dab_turn_on"
#define METHOD_DAB_SELECT_PRESET				  "method_radio_select_preset"

typedef enum{
	MethodDABTurnOn,
	MethodDABSelectPreset,
	TotalMethodDABEvent
}MethodDABEvent;
extern const char* g_methodDABEventNames[TotalMethodDABEvent];

#define SIGNAL_DAB_MODE                         "signal_dab_mode"
#define SIGNAL_DAB_FREQUENCY_CHANGED            "signal_dab_frequency_changed"

typedef enum{
	SignalDABMode,
	SignalDABFrequencyChanged,
	TotalSignalDABEvent
}SignalDABEvent;
extern const char* g_signalDABEventNames[TotalSignalDABEvent];

/*================================================= DAB PROCESS DEFINE END =================================================*/

/*================================================ DRM PROCESS DEFINE START ================================================*/

#define DRM_PROCESS_DBUS_NAME                     "telechips.drm.process"
#define DRM_PROCESS_OBJECT_PATH                   "/telechips/drm/process"
#define DRM_EVENT_INTERFACE                       "drm.event"

#define METHOD_DRM_TURN_ON                        "method_drm_turn_on"
#define METHOD_DRM_SELECT_PRESET				  "method_radio_select_preset"

typedef enum{
	MethodDRMTurnOn,
	MethodDRMSelectPreset,
	TotalMethodDRMEvent
}MethodDRMEvent;
extern const char* g_methodDRMEventNames[TotalMethodDRMEvent];

#define SIGNAL_DRM_MODE                         "signal_drm_mode"
#define SIGNAL_DRM_PRESET_CHANGED	            "signal_drm_preset_changed"
#define SIGNAL_DRM_SERVICE_CHANGED	            "signal_drm_service_changed"

typedef enum{
	SignalDRMMode,
	SignalDRMPresetChanged,
	SignalDRMServiceChanged,
	TotalSignalDRMEvent
}SignalDRMEvent;
extern const char* g_signalDRMEventNames[TotalSignalDRMEvent];
/*================================================= DRM PROCESS DEFINE END =================================================*/

/*========================= APPLE DEVICE MANAGER DEFINE START ===========================*/
#define ADM_DBUS_NAME								"telechips.appledevice.manager"
#define ADM_OBJECT_PATH 							"/telechips/appledevice/manager"
#define ADM_MANAGER_INTERFACE						"appledevice.manager"

// Apple Device Manager Daemon Method defines
#define METHOD_ADM_IAP1_INITIALIZE					"method_adm_iap1_initialize"
#define METHOD_ADM_IAP2_INITIALIZE					"method_adm_iap2_initialize"
#define METHOD_ADM_CARPLAY_INITIALIZE				"method_adm_carplay_initialize"
#define METHOD_ADM_IAP1_RELEASE 					"method_adm_iap1_release"
#define METHOD_ADM_IAP2_RELEASE 					"method_adm_iap2_release"
#define METHOD_ADM_CARPLAY_RELEASE					"method_adm_carplay_release"
#define METHOD_ADM_SUPPORT							"method_adm_support"
#define METHOD_ADM_SUPPORT_EA_NATIVE				"method_adm_support_ea_native"
#define METHOD_ADM_SET_EA_PROTOCOL_NAME				"method_adm_set_ea_protocol_name"
#define METHOD_ADM_SUPPORT_BY_USER_INPUT			"method_adm_support_by_user_input"

typedef enum {
	MethodAppleDeviceManageriAP1Initialize,
	MethodAppleDeviceManageriAP2Initialize,
	MethodAppleDeviceManagerCarPlayInitialize,
	MethodAppleDeviceManageriAP1Release,
	MethodAppleDeviceManageriAP2Release,
	MethodAppleDeviceManagerCarPlayRelease,
	MethodAppleDeviceManagerSupport,
	MethodAppleDeviceManagerSupportEANative,
	MethodAppleDeviceManagerSetEAProtocolName,
	MethodAppleDeviceManagerSupportByUserInput,
	TotalMethodAppleDeviceManagerEvents
} MethodAppleDeviceManagerEvent;
extern const char *g_methodADMEventNames[TotalMethodAppleDeviceManagerEvents];

// Apple Device Manager Daemon Signal defines
#define SIGNAL_ADM_IAP1_CONNECTED					"signal_adm_iap1_connected"
#define SIGNAL_ADM_IAP1_DISCONNECTED				"signal_adm_iap1_disconnected"
#define SIGNAL_ADM_IAP2_CONNECTED					"signal_adm_iap2_connected"
#define SIGNAL_ADM_IAP2_DISCONNECTED				"signal_adm_iap2_disconnected"
#define SIGNAL_ADM_CARPLAY_CONNECTED				"signal_adm_carplay_connected"
#define SIGNAL_ADM_CARPLAY_DISCONNECTED 			"signal_adm_carplay_disconnected"
#define SIGNAL_ADM_APPLE_DEVICE_CONNECTED			"signal_adm_apple_device_connected"
#define SIGNAL_ADM_APPLE_DEVICE_DISCONNECTED		"signal_adm_apple_device_disconnected"
#define SIGNAL_ADM_INITIALIZE_COMPLETE				"signal_adm_initialize_complete"

typedef enum {
	SignalAppleDeviceManageriAP1Connected,
	SignalAppleDeviceManageriAP1Disconnected,
	SignalAppleDeviceManageriAP2Connected,
	SignalAppleDeviceManageriAP2Disconnected,
	SignalAppleDeviceManagerCarPlayConnected,
	SignalAppleDeviceManagerCarPlayDisconnected,
	SignalAppleDeviceManagerAppleDeviceConnected,
	SignalAppleDeviceManagerAppleDeviceDisconnected,
	SignalAppleDeviceManagerInitializeComplete,
	TotalSignalAppleDeviceManagerEvents
} SignalAppleDeviceManagerEvent;
extern const char *g_signalADMEventNames[TotalSignalAppleDeviceManagerEvents];
/*========================== APPLE DEVICE MANAGER DEFINE END ============================*/

/*================================================ Micom Manager PROCESS DEFINE START ================================================*/

#define MICOM_MANAGER_PROCESS_DBUS_NAME                     "telechips.micom_manager.process"
#define MICOM_MANAGER_PROCESS_OBJECT_PATH                   "/telechips/micom_manager/process"
#define MICOM_MANAGER_EVENT_INTERFACE                       "micom_manager.event"

#define METHOD_MICOM_MANAGER_SVM_ON			                "method_micom_manager_svm_on"
#define METHOD_MICOM_MANAGER_SVM_OFF						"method_micom_manager_svm_off"
#define METHOD_MICOM_MANAGER_FVC_ON			                "method_micom_manager_fvc_on"
#define METHOD_MICOM_MANAGER_FVC_OFF						"method_micom_manager_fvc_off"

typedef enum{
	MethodMicomManagerSVMOn,
	MethodMicomManagerSVMOff,
	MethodMicomManagerFVCOn,
	MethodMicomManagerFVCOff,
	TotalMethodMicomManagerEvent
} MethodMicomManagerEvent;
extern const char* g_methodMicomManagerEventNames[TotalMethodMicomManagerEvent];

/*================================================= Micom Manager PROCESS DEFINE END =================================================*/

#endif // DBUS_MSG_DEF_H

