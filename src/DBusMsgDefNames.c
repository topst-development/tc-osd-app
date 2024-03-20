/****************************************************************************************
 *   FileName    : DBusMsgDefNames.c
 *   Description : DBusMsgDefNames.c
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided “AS IS” and nothing contained in this source code
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

#include "DBusMsgDef.h"
#include "MediaPlayerType.h"

const char *g_methodModeManagerEventNames[TotalMethodModeManagerEvent] = {
	CHANGE_MODE,
	RELEASE_RESOURCE_DONE,
	END_MODE,
	MODE_ERROR_OCCURED
};

const char *g_signalModeManagerEventNames[TotalSignalModeManagerEvent] = {
	CHANGED_MODE,
	RELEASE_RESOURCE,
};

const char *g_methodAudioManagerEventNames[TotalMethodAudioManagerEvents] = {
	METHOD_AUDIOMANAGER_GET_MAINSINKS,
	METHOD_AUDIOMANAGER_GET_MAINSOURCES,
	METHOD_AUDIOMANAGER_GET_VOLUME,
	METHOD_AUDIOMANAGER_SET_VOLUME
};

const char *g_signalAudioManagerEventNames[TotalSignalAudioManagerEvents] = {
	SIGNAL_AUDIOMANAGER_VOLUME_CHANGED,
	SIGNAL_AUDIOMANAGER_SOURCE_CHANGED
};

const char *g_methodTMPlayerEventNames[TotalMethodTMPlayerEvent] = {
	METHOD_TMPLAYER_SET_APP_ID,
	METHOD_TMPLAYER_CONNECT_DEBUG,
	METHOD_TMPLAYER_DB_DEBUG,
	METHOD_TMPLAYER_CHANGE_CONTENT_TYPE,
	METHOD_TMPLAYER_CHANGE_NEXT_DEVICE,
	METHOD_TMPLAYER_CHANGE_DEVICE,
	METHOD_TMPLAYER_SET_BROWSING_MODE,
	METHOD_TMPLAYER_CHANGE_PLAYMODE,
	METHOD_TMPLAYER_TRACK_START,
	METHOD_TMPLAYER_TRACK_STOP,
	METHOD_TMPLAYER_TRACK_RESUME,
	METHOD_TMPLAYER_TRACK_PAUSE,
	METHOD_TMPLAYER_TRACK_UP,
	METHOD_TMPLAYER_TRACK_DOWN,
	METHOD_TMPLAYER_TRACK_MOVE,
	METHOD_TMPLAYER_TRACK_SEEK,
	METHOD_TMPLAYER_UPDATE_DISPLAY_INFO,
	METHOD_TMPLAYER_REQUEST_ALBUMART_KEY,
	METHOD_TMPLAYER_REQUEST_FILELIST,
	METHOD_TMPLAYER_META_BROWSING_START,
	METHOD_TMPLAYER_META_BROWSING_REQUEST,
	METHOD_TMPLAYER_META_BROWSING_SELECT,
	METHOD_TMPLAYER_META_BROWSING_HOME,
	METHOD_TMPLAYER_META_BROWSING_UNDO,
	METHOD_TMPLAYER_META_BROWSING_END,
	METHOD_TMPLAYER_GET_VOLUME_STEP,
	METHOD_TMPLAYER_VOLUME_UP,
	METHOD_TMPLAYER_VOLUME_DOWN,
	METHOD_TMPLAYER_REQUEST_DISPLAY,
	METHOD_TMPLAYER_RELEASE_DISPLAY,
	METHOD_TMPLAYER_NOTIFY_DISPLAY_DONE
};

const char *g_signalTMPlayerEventNames[TotalSignalTMPlayerEvent] = {
	SIGNAL_TMPLAYER_PLAY_FAILED,
	SIGNAL_TMPLAYER_PLAYSTATE_CHANGED,
	SIGNAL_TMPLAYER_PLAYTIME_CHANGED,
	SIGNAL_TMPLAYER_PLAYMODE_CHANGED,
	SIGNAL_TMPLAYER_TOTALTIME_CHANGED,
	SIGNAL_TMPLAYER_SEEK_COMPLETED,
	SIGNAL_TMPLAYER_ID3_INFOMATION,
	SIGNAL_TMPLAYER_ALBUMART_KEY,
	SIGNAL_TMPLAYER_ALBUMART_COMPLETED,
	SIGNAL_TMPLAYER_NO_DEVICE,
	SIGNAL_TMPLAYER_CONTENT_CHANGED,
	SIGNAL_TMPLAYER_DEVICE_CHANGED,
	SIGNAL_TMPLAYER_DEVICE_CONNECTED,
	SIGNAL_TMPLAYER_DEVICE_DISCONNECTED,
	SIGNAL_TMPLAYER_DEVICE_READY,
	SIGNAL_TMPLAYER_DEVICE_MOUNT_STOP,
	SIGNAL_TMPLAYER_NO_SONG,
	SIGNAL_TMPLAYER_RESET_INFORMATION,
	SIGNAL_TMPLAYER_FILENUMBER_CHANGED,
	SIGNAL_TMPLAYER_FILENUMBER_UPDATED,
	SIGNAL_TMPLAYER_TOTALNUMBER_CHAGNED,
	SIGNAL_TMPLAYER_FILE_DB_COMPLETED,
	SIGNAL_TMPLAYER_FILENAME_CHANGED,
	SIGNAL_TMPLAYER_FOLDERNAME_CHANGED,
	SIGNAL_TMPLAYER_FILELIST,
	SIGNAL_TMPLAYER_META_COUNT_CHANGED,
	SIGNAL_TMPLAYER_META_MENU_CHANGED,
	SIGNAL_TMPLAYER_META_ADD_NAME,
	SIGNAL_TMPLAYER_META_ADD_NAME_COMPLETED,
	SIGNAL_TMPLAYER_META_CREATE_TRACKLIST,
	SIGNAL_TMPLAYER_VOLUME_CHANGED,
	SIGNAL_TMPLAYER_DISPLAY_ON,
	SIGNAL_TMPLAYER_DISPLAY_OFF
};

const char *g_deviceSourceNames[TotalDeviceSources] = {
	"MyMusic",
	"USB1",
	"USB2",
	"USB3",
	"SDMMC",
	"Aux",
	"iAP2",
	"Bluetooth"
};

const char *g_AVPlayStatusNames[TotalPlayStatus] = {
	"Play Status(Stop)",
	"Play Status(Playing)",
	"Play Status(Pause)",
	"Play Status(Fast Forward)",
	"Play Status(Fast Rewind)",
	"Play Status(Forward/Rewind End)"
	"Play Status(Turbo Fast Forward)",
	"Play Status(Turbo Fast Rewind)",
	"Play Status(NoStatement)",
};

const char *g_methodADMEventNames[TotalMethodAppleDeviceManagerEvents] = {
	METHOD_ADM_IAP1_INITIALIZE,
	METHOD_ADM_IAP2_INITIALIZE,
	METHOD_ADM_CARPLAY_INITIALIZE,
	METHOD_ADM_IAP1_RELEASE,
	METHOD_ADM_IAP2_RELEASE,
	METHOD_ADM_CARPLAY_RELEASE,
	METHOD_ADM_SUPPORT,
	METHOD_ADM_SUPPORT_EA_NATIVE,
	METHOD_ADM_SET_EA_PROTOCOL_NAME,
	METHOD_ADM_SUPPORT_BY_USER_INPUT,
};

const char *g_signalADMEventNames[TotalSignalAppleDeviceManagerEvents] = {
	SIGNAL_ADM_IAP1_CONNECTED,
	SIGNAL_ADM_IAP1_DISCONNECTED,
	SIGNAL_ADM_IAP2_CONNECTED,
	SIGNAL_ADM_IAP2_DISCONNECTED,
	SIGNAL_ADM_CARPLAY_CONNECTED,
	SIGNAL_ADM_CARPLAY_DISCONNECTED,
	SIGNAL_ADM_APPLE_DEVICE_CONNECTED,
	SIGNAL_ADM_APPLE_DEVICE_DISCONNECTED,
	SIGNAL_ADM_INITIALIZE_COMPLETE
};

const char *g_signalLauncherEventNames[TotalSignalLauncherEvents] = {
	SIGNAL_LAUNCHER_KEY_PRESSED,
	SIGNAL_LAUNCHER_KEY_LONG_PRESSED,
	SIGNAL_LAUNCHER_KEY_LONG_LONG_PRESSED,
	SIGNAL_LAUNCHER_KEY_RELEASED,
	SIGNAL_LAUNCHER_KEY_CLICKED
};

const char *g_methodLauncherEventNames[TotalMethodLauncherEvents] = {
	METHOD_LAUNCHER_ACTIVE_APPLICATION,
	METHOD_LAUNCHER_START_APPLICATIONS
};

/* End of file */
