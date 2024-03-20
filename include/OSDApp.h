/****************************************************************************************
 *   FileName    : OSDApp.h
 *   Description : OSDApp.h
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
No warranty is made, express or implied, regarding the information¡¯s accuracy,
completeness, or performance.
In no event shall Telechips be liable for any claim, damages or other liability arising from,
out of or in connection with this source code or the use in the source code.
This source code is provided subject to the terms of a Mutual Non-Disclosure Agreement
between Telechips and Company.
*
****************************************************************************************/

#ifndef OSD_APP_PROCESS_H
#define OSD_APP_PROCESS_H

#include <QObject>
#include <QQuickWindow>
#include "OSDStatusWidget.h"

class OSDApp : public QQuickWindow
{
	Q_OBJECT

	public:
		OSDApp(QQuickWindow *parent = 0);
		~OSDApp();
		void Initialize(void);

	private:
		void InitializeSignalSlots(void);

	private slots:
		void OnAudioInitialize(void);
		void OnDeviceConnected(int device);
		void OnDeviceDisconnected(int device);
		void OnConnectivitySourceConnected(int source);
		void OnConnectivitySourceDisconnected(int source);
		void OnApplicationChanged(int app);
		void OnShowDisplay(bool show);
		void OnReleaseAudioResource(unsigned char audioType, int app);
		void OnAudioTypeChanged(unsigned short audioType);
		void OnGoToHome(void);
		void OnVolumeUp(void);
		void OnVolumeDown(void);
		void OnVolumeChanged(unsigned short sourceID, short volume);
		void OnKeyboardClicked(int key);

	signals:
		void GetVolume();

	private:
		int					_currentAppID;
		OSDStatusWidget		*_statusWidget;
		unsigned short		_currSource;
		std::map<unsigned short, short> _sourceIDtoVolume;
};

#endif // OSD_APP_PROCESS_H
