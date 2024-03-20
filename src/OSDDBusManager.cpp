/****************************************************************************************
 *   FileName    : OSDDBusManager.cpp
 *   Description : OSDDBusManager.cpp
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

#include <stdlib.h>
#include <stdio.h>
#include <dbus/dbus.h>
#include "DBusMsgDef.h"
#include "TCDBusRawAPI.h"
#include "OSDIFManager.h"
#include "OSDStatusWidget.h"
#include "OSDDBusManager.h"

extern int g_debug;

#define OSD_DBUS_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[OSD DBUS MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface);
static void OnDBusInitialized(void);
static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message);
static void GeniviAudioManagerSignalDBusProcess(unsigned int id, DBusMessage * message);
static void TMPlayerSignalDBusProcess(unsigned int id, DBusMessage * message);
static void AppleDeviceManagerSignalDBusProcess(unsigned int id, DBusMessage *message);
static void LauncherSignalDBusProcess(unsigned int id, DBusMessage *message);

void InitilaizeOSDDBusManager(void)
{
	OSD_DBUS_MANAGER_PRINTF("\n");
	SetCallBackFunctions(OnReceivedDBusSignal, NULL);
	AddSignalInterface(MODEMANAGER_EVENT_INTERFACE);
	AddSignalInterface(AUDIOMANAGER_EVENT_INTERFACE);
	AddSignalInterface(TMPLAYER_EVENT_INTERFACE);
	AddSignalInterface(ADM_MANAGER_INTERFACE);
	AddSignalInterface(LAUNCHER_EVENT_INTERFACE);
	InitializeRawDBusConnection("OSD DBUS");
	SetDBusInitCallBackFunctions(OnDBusInitialized);
}

void ReleaseOSDDBusManager(void)
{
	OSD_DBUS_MANAGER_PRINTF("\n");
	ReleaseRawDBusConnection();
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
						OSD_DBUS_MANAGER_PRINTF("%s: reply : %s\n", __FUNCTION__, response ? "ACK" : "NACK");
					}
				}
				else
				{
					OSD_DBUS_MANAGER_PRINTF("%s: GetArgmentFromDBusPendingCall return error\n", __FUNCTION__);
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

void SendDBusSetVolume(unsigned short sourceID, short volume)
{
	OSD_DBUS_MANAGER_PRINTF("sourceID : %hu setVolume : %hd \n", 
							sourceID, volume);
	DBusMessage* message;
	short error;
	message = CreateDBusMsgMethodCall(AUDIOMANAGER_PROCESS_DBUS_NAME,AUDIOMANAGER_PROCESS_OBJECT_PATH,
									AUDIOMANAGER_EVENT_INTERFACE,
									METHOD_AUDIOMANAGER_SET_VOLUME,
									DBUS_TYPE_UINT16, &sourceID,
									DBUS_TYPE_INT16, &volume,
									DBUS_TYPE_INVALID);
	if(message != NULL)
	{
		DBusPendingCall* pending = NULL;
		if(SendDBusMessage(message, &pending))
		{
			if(pending != NULL)
			{
				if(GetArgumentFromDBusPendingCall(pending,
													DBUS_TYPE_INT16, &error,
													DBUS_TYPE_INVALID))
				{
					if(!error)
					{
						OSD_DBUS_MANAGER_PRINTF("GetArgmentFromDBusPendingCall, volume set Complete \n");
					}
				}
				else
				{
					fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
				}
				dbus_pending_call_unref(pending);
			}
		}
	}
	else
	{
		fprintf(stderr, "%s: CreateDBusMsgMethodCall failed\n", __FUNCTION__);
	}

}

void SendDBusGetVolumeStep(unsigned short sourceID)
{
	OSD_DBUS_MANAGER_PRINTF("\n");
	DBusMessage* message;
	short volume;
	short error;
	
	message = CreateDBusMsgMethodCall(AUDIOMANAGER_PROCESS_DBUS_NAME,AUDIOMANAGER_PROCESS_OBJECT_PATH,
									AUDIOMANAGER_EVENT_INTERFACE,
									METHOD_AUDIOMANAGER_GET_VOLUME,
									DBUS_TYPE_UINT16, &sourceID,
									DBUS_TYPE_INVALID);

	if(message != NULL)
	{
		DBusPendingCall* pending = NULL;
		if(SendDBusMessage(message, &pending))
		{
			if(pending != NULL)
			{
				if(GetArgumentFromDBusPendingCall(pending,
													DBUS_TYPE_INT16, &error,
													DBUS_TYPE_INT16, &volume,
													DBUS_TYPE_INVALID))
				{
					if(!error)
					{
						OSD_DBUS_MANAGER_PRINTF("GetArgmentFromDBusPendingCall, volume : %hd \n", volume);
						OSD_IF_MANAGER->EmitAudioVolumeChanged(sourceID, volume);
					}
				}
				else
				{
					fprintf(stderr, "%s: SendDBusMessage failed\n", __FUNCTION__);
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

static DBusMsgErrorCode OnReceivedDBusSignal(DBusMessage *message, const char *interface)
{
	DBusMsgErrorCode error = ErrorCodeNoError;

	if (message != NULL &&
		interface != NULL)
	{
		unsigned int idx;
		unsigned int stop = 0;

		if (strcmp(interface, MODEMANAGER_EVENT_INTERFACE) == 0)
		{
			for(idx = ChangedMode; idx < TotalSignalModeManagerEvent && !stop; idx++)
			{
				if(dbus_message_is_signal(message,
										  MODEMANAGER_EVENT_INTERFACE,
										  g_signalModeManagerEventNames[idx]))
				{
					ModeManagerSignalDBusProcess(idx, message);
					stop = 1;
				}
			}
		}
		else if (strcmp(interface, AUDIOMANAGER_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalAudioManagerVolumeChanged; idx < TotalSignalAudioManagerEvents && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   AUDIOMANAGER_EVENT_INTERFACE,
										   g_signalAudioManagerEventNames[idx]))
				{

					GeniviAudioManagerSignalDBusProcess(idx, message);
					stop = 1;
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
					stop = 1;
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
					stop = 1;
				}
			}
		}
		else if (strcmp(interface, LAUNCHER_EVENT_INTERFACE) == 0)
		{
			for (idx = SignalLauncherKeyPressed; idx < TotalSignalLauncherEvents && !stop; idx++)
			{
				if (dbus_message_is_signal(message,
										   LAUNCHER_EVENT_INTERFACE,
										   g_signalLauncherEventNames[idx]))
				{
					LauncherSignalDBusProcess(idx, message);
					stop = 1;
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

static void OnDBusInitialized(void)
{
	OSD_DBUS_MANAGER_PRINTF("D-Bus Initialize Done: %d\n", APP_ID_OSD);
	OSD_IF_MANAGER->EmitDBusInitialized();
}

static void ModeManagerSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalModeManagerEvent)
	{
        OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
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
					if(strncmp(mode, "view", 4) == 0)
					{
						if (app == APP_ID_OSD)
						{
							OSD_IF_MANAGER->EmitShowDisplay(true);
						}
						else
						{
							OSD_IF_MANAGER->EmitApplicationChanged(app);
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
					if((app == APP_ID_OSD) &&
						(resources & 0x01))
					{
						OSD_IF_MANAGER->EmitShowDisplay(false);
					}
				}
			}
		}
	}
}

static void GeniviAudioManagerSignalDBusProcess(unsigned int id, DBusMessage * message)
{
	if(id < TotalSignalAudioManagerEvents)
	{
		OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalAudioManagerEventNames[id], id);
		if (message != NULL)
		{
			unsigned short sourceID = 0;
			short volume = -1;
			switch(id)
			{
				case SignalAudioManagerVolumeChanged:
					if (GetArgumentFromDBusMessage(message,
													DBUS_TYPE_UINT16, &sourceID,
													DBUS_TYPE_INT16, &volume,
													DBUS_TYPE_INVALID))
					OSD_IF_MANAGER->EmitAudioVolumeChanged(sourceID, volume);
					break;
				case SignalAudioManagerSourceChanged:
					if (GetArgumentFromDBusMessage(message,
													DBUS_TYPE_UINT16, &sourceID,
													DBUS_TYPE_INVALID))
					OSD_IF_MANAGER->EmitAudioTypeChanged(sourceID);
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
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
			OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
										  g_signalTMPlayerEventNames[id], id);
		}

		if (message != NULL)
		{
			switch (id)
			{
				case SignalTMPlayerPlayStateChanged:
				case SignalTMPlayerPlayTimeChanged:
				case SignalTMPlayerTotalTimeChanged:
				case SignalTMPlayerAlbumArtKey:
				case SignalTMPlayerAlbumArtCompleted:
				case SignalTMPlayerNoDevice:
				case SignalTMPlayerContentChanged:
				case SignalTMPlayerDeviceChagned:
					break;
				case SignalTMPlayerDeviceConnected:
					OSD_IF_MANAGER->EmitDeviceConnected(message);
					break;
				case SignalTMPlayerDeviceDisConnected:
					OSD_IF_MANAGER->EmitDeviceDisconnected(message);
					break;
				case SignalTMPlayerResetInormation:
				case SignalTMPlayerFileNumberChanged:
				case SignalTMPlayerFileNumberUpdated:
				case SignalTMPlayerFileNameChanged:
				case SignalTMPlayerDeviceReady:
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
				case SignalTMPlayerVolumeChanged:
				case SignalTMPlayerDisplayOn:
				case SignalTMPlayerDisplayOff:
					break;
				default:
					fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
					break;
			}
		}
	}
	else
	{
		OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%d]\n",id);
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
					OSD_IF_MANAGER->EmitConnectivitySourceConnected(OSDStatusWidget::ConnectivitySourceiAP2);
					break;
				case SignalAppleDeviceManageriAP2Disconnected:
					OSD_IF_MANAGER->EmitConnectivitySourceDisconnected(OSDStatusWidget::ConnectivitySourceiAP2);
					break;
				case SignalAppleDeviceManagerCarPlayConnected:
					OSD_IF_MANAGER->EmitConnectivitySourceConnected(OSDStatusWidget::ConnectivitySourceCarPlay);
					break;
				case SignalAppleDeviceManagerCarPlayDisconnected:
					OSD_IF_MANAGER->EmitConnectivitySourceDisconnected(OSDStatusWidget::ConnectivitySourceCarPlay);
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

static void LauncherSignalDBusProcess(unsigned int id, DBusMessage *message)
{
	if (id < TotalSignalLauncherEvents)
	{
		OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%s(%d)]\n",
									  g_signalLauncherEventNames[id], id);

		if (message != NULL)
		{
			int value = -1;
			if (GetArgumentFromDBusMessage(message,
										   DBUS_TYPE_INT32, &value,
										   DBUS_TYPE_INVALID))
			{
				switch (id)
				{
					case SignalLauncherKeyPressed:
						OSD_IF_MANAGER->EmitKeyboardPressedEvent(value);
						break;
					case SignalLauncherKeyLongPressed:
						OSD_IF_MANAGER->EmitKeyboardLongPressedEvent(value);
						break;
					case SignalLauncherKeyLongLongPressed:
						OSD_IF_MANAGER->EmitKeyboardLongLongPressedEvent(value);
						break;
					case SignalLauncherKeyReleased:
						OSD_IF_MANAGER->EmitKeyboardReleasedEvent(value);
						break;
					case SignalLauncherKeyClicked:
						OSD_IF_MANAGER->EmitKeyboardClickedEvent(value);
						break;
					default:
						fprintf(stderr, "%s: unknown signal id(%d)\n", __FUNCTION__, id);
						break;
				}
			}
		}
		else
		{
			fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __FUNCTION__);
		}
	}
	else
	{
		OSD_DBUS_MANAGER_PRINTF("RECEIVED SIGNAL[%d]\n",
									  id);
	}
}
