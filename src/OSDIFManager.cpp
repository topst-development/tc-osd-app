/****************************************************************************************
 *   FileName    : OSDIFManager.cpp
 *   Description : OSDIFManager.cpp
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

#include <cstdlib>
#include <cstdio>
#include "TCDBusRawAPI.h"
#include "OSDDBusManager.h"
#include "OSDIFManager.h"

extern int g_debug;

#define OSD_IF_MANAGER_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[OSD IF MANAGER] %s: " format "", __FUNCTION__, ##arg); \
	}

OSDIFManager::OSDIFManager(QObject *parent) :
		QObject(parent)
{
}

void OSDIFManager::Initilaize()
{
	OSD_IF_MANAGER_PRINTF("\n");
	InitilaizeOSDDBusManager();
	
}

void OSDIFManager::Release(void)
{
	OSD_IF_MANAGER_PRINTF("\n");
	ReleaseOSDDBusManager();
}

void OSDIFManager::EmitDBusInitialized(void)
{
	OSD_IF_MANAGER_PRINTF("\n");
	emit AudioInitialize();
}

void OSDIFManager::EmitApplicationChanged(int app)
{
	OSD_IF_MANAGER_PRINTF("APP(%d)\n", app);
	emit ApplicationChanged(app);
}

void OSDIFManager::EmitShowDisplay(bool show)
{
	OSD_IF_MANAGER_PRINTF("SHOW(%s)\n", show ? "TRUE" : "FALSE");
	emit ShowDisplay(show);
}

void OSDIFManager::EmitReleaseAudioResource(unsigned char audioType, int app)
{
	OSD_IF_MANAGER_PRINTF("\n");
	emit ReleaseAudioResource(audioType, app);
}

void OSDIFManager::EmitAudioTypeChanged(unsigned short audioType)
{
	OSD_IF_MANAGER_PRINTF("AUDIO TYPE(%hu)\n",
						  audioType);
	emit AudioTypeChanged(audioType);
}

void OSDIFManager::EmitDeviceConnected(DBusMessage *message)
{
	unsigned char device;

	if (GetArgumentFromDBusMessage(message,
								   DBUS_TYPE_BYTE, &device,
								   DBUS_TYPE_INVALID))
	{
		emit DeviceConnected(device);
	}
	else
	{
		fprintf(stderr, "%s: GetArgumentFromDBusMessage failed\n", __PRETTY_FUNCTION__);
	}
}

void OSDIFManager::EmitDeviceDisconnected(DBusMessage *message)
{
	unsigned char device;

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

void OSDIFManager::EmitConnectivitySourceConnected(int source)
{
	emit ConnectivitySourceConnected(source);
}

void OSDIFManager::EmitConnectivitySourceDisconnected(int source)
{
	emit ConnectivitySourceDisconnected(source);
}

void OSDIFManager::EmitAudioVolumeChanged(unsigned short sourceID,  short volume)
{
	OSD_IF_MANAGER_PRINTF("SOURCE ID: %hu volume %hd\n", 
						  sourceID, volume);
	emit VolumeChanged(sourceID, volume/10);
}

void OSDIFManager::EmitKeyboardPressedEvent(int key)
{
	emit KeyboardPressed(key);
}

void OSDIFManager::EmitKeyboardLongPressedEvent(int key)
{
	emit KeyboardLongPressed(key);
}

void OSDIFManager::EmitKeyboardLongLongPressedEvent(int key)
{
	emit KeyboardLongLongPressed(key);
}

void OSDIFManager::EmitKeyboardReleasedEvent(int key)
{
	emit KeyboardReleased(key);
}

void OSDIFManager::EmitKeyboardClickedEvent(int key)
{
	emit KeyboardClicked(key);
}

static OSDIFManager _osdIFManager;
OSDIFManager *GetOSDIFManager()
{
	return &_osdIFManager;
}
