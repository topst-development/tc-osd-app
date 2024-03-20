/****************************************************************************************
 *   FileName    : OSDIFManager.h
 *   Description : OSDIFManager.h
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

#ifndef OSD_IF_MANAGER_H
#define OSD_IF_MANAGER_H

#include <dbus/dbus.h>
#include <QObject>

class OSDIFManager : public QObject
{
	Q_OBJECT

	public:
		OSDIFManager(QObject *parent = 0);
        void Initilaize();
		void Release(void);
		int GetCurrentApplication(void);

		void EmitDBusInitialized(void);

		void EmitApplicationChanged(int app);
		void EmitShowDisplay(bool show);
		void EmitReleaseAudioResource(unsigned char audioType, int app);

		void EmitDeviceConnected(DBusMessage *message);
		void EmitDeviceDisconnected(DBusMessage *message);
		void EmitConnectivitySourceConnected(int source);
		void EmitConnectivitySourceDisconnected(int source);

		void EmitAudioVolumeChanged(unsigned short sourceID, short volume);
		void EmitAudioTypeChanged(unsigned short audioType);

		void EmitKeyboardPressedEvent(int key);
		void EmitKeyboardLongPressedEvent(int key);
		void EmitKeyboardLongLongPressedEvent(int key);
		void EmitKeyboardReleasedEvent(int key);
		void EmitKeyboardClickedEvent(int key);

	signals:
		void AudioInitialize(void);
		void ApplicationChanged(int app);
		void ShowDisplay(bool show);
		void ReleaseAudioResource(unsigned char audioType, int app);

		void DeviceConnected(int device);
		void DeviceDisconnected(int device);
		void ConnectivitySourceConnected(int source);
		void ConnectivitySourceDisconnected(int source);

		void VolumeChanged(unsigned short sourceID, short volume);
		void AudioTypeChanged(unsigned short type);

		void KeyboardPressed(int key);
		void KeyboardLongPressed(int key);
		void KeyboardLongLongPressed(int key);
		void KeyboardReleased(int key);
		void KeyboardClicked(int key);
};

OSDIFManager *GetOSDIFManager();
#define OSD_IF_MANAGER	(GetOSDIFManager())

#endif // OSD_IF_MANAGER_H
