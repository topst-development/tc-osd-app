/****************************************************************************************
 *   FileName    : OSDApp.cpp
 *   Description : OSDApp.cpp
 ****************************************************************************************
 *
 *   TCC Version 1.0
 *   Copyright (c) Telechips Inc.
 *   All rights reserved

This source code contains confidential information of Telechips.
Any unauthorized use without a written permission of Telechips including not limited
to re-distribution in source or binary form is strictly prohibited.
This source code is provided ¡°AS IS¡± and nothing contained in this source code
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

#include <cstdio>
#include <QQuickItem>
#include <QGuiApplication>
#include "TCInput.h"
#include "OSDIFManager.h"
#include "OSDDBusManager.h"
#include "OSDApp.h"

extern int g_debug;
extern OSDStatusWidget::ConnectivitySource GetSourceFromMediaDevice(int device);

#define OSD_APP_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[OSD APP] %s: " format "", __FUNCTION__, ##arg); \
	}

#define TOTAL_AUDIO_TYPE_COUNT			5
#define OSD_GUI_WIDTH					1024
#define OSD_GUI_HEIGHT					600
#define OSD_HEIGHT						65

OSDApp::OSDApp(QQuickWindow *parent) :
	QQuickWindow(parent),
	_currentAppID(-1),
	_statusWidget(NULL),
	_currSource(100)
{
}

OSDApp::~OSDApp()
{
	ReleaseOSDDBusManager();
	OSD_IF_MANAGER->Release();
}

void OSDApp::Initialize(void)
{
	QObject *temp;

	temp = this->findChild<QObject*>("statusWidget");
	_statusWidget = qobject_cast<OSDStatusWidget*>(temp);
	_statusWidget->Initialize();

	InitializeSignalSlots();

	OnApplicationChanged(5);
}

void OSDApp::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	// TMPlayer SIGNAL/SLOT
	connect(OSD_IF_MANAGER, SIGNAL(DeviceConnected(int)),
			this, SLOT(OnDeviceConnected(int)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(DeviceDisconnected(int)),
			this, SLOT(OnDeviceDisconnected(int)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(ConnectivitySourceConnected(int)),
			this, SLOT(OnConnectivitySourceConnected(int)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(ConnectivitySourceDisconnected(int)),
			this, SLOT(OnConnectivitySourceDisconnected(int)), dbusConnectionType);

	// Mode Manager SIGNAL/SLOT
	connect(OSD_IF_MANAGER, SIGNAL(AudioInitialize(void)),
			this, SLOT(OnAudioInitialize(void)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(ApplicationChanged(int)),
			this, SLOT(OnApplicationChanged(int)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(ShowDisplay(bool)),
			this, SLOT(OnShowDisplay(bool)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(ReleaseAudioResource(unsigned char, int)),
			this, SLOT(OnReleaseAudioResource(unsigned char, int)), dbusConnectionType);

	// Audio Manager SIGNAL/SLOT
	connect(OSD_IF_MANAGER, SIGNAL(VolumeChanged(unsigned short, short)),
			this, SLOT(OnVolumeChanged(unsigned short, short)), dbusConnectionType);
	connect(OSD_IF_MANAGER, SIGNAL(AudioTypeChanged(unsigned short)),
			this, SLOT(OnAudioTypeChanged(unsigned short)), dbusConnectionType);

	// Launcher Key SIGNAL/SLOT
	connect(OSD_IF_MANAGER, SIGNAL(KeyboardClicked(int)),
			this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(_statusWidget, SIGNAL(GoToHome()),
			this, SLOT(OnGoToHome()), dbusConnectionType);
	connect(_statusWidget, SIGNAL(VolumeUp()),
			this, SLOT(OnVolumeUp()), dbusConnectionType);
	connect(_statusWidget, SIGNAL(VolumeDown()),
			this, SLOT(OnVolumeDown()), dbusConnectionType);
}

void OSDApp::OnAudioInitialize(void)
{
	FILE *fp;
	short volumeDB[5];

	if((fp = fopen("/home/root/.telechips/AudioManager.txt", "r")) == NULL)
	{
		for(unsigned short i = 100; i < 105; i++)
		{
			SendDBusGetVolumeStep(i);
		}
	}
	else
	{
		int count = 0;
		count = fread(volumeDB, sizeof(short), 5, fp);
		for(int i = 0; i < 5; i++)
		{
			short setvolume = 0;
			if(volumeDB[i] > -1 && volumeDB[i] < 11 && count == 5)
			{
				_sourceIDtoVolume[i + 100] = volumeDB[i];
				setvolume = _sourceIDtoVolume[i + 100] * 10;
				SendDBusSetVolume(i + 100, setvolume);
			}
			else
			{
				SendDBusGetVolumeStep(i + 100);
			}
		}
		fclose(fp);
	}
}

void OSDApp::OnDeviceConnected(int device)
{
	_statusWidget->SetEnableDevice(GetSourceFromMediaDevice(device));
}

void OSDApp::OnDeviceDisconnected(int device)
{
	_statusWidget->SetDisableDevice(GetSourceFromMediaDevice(device));
}

void OSDApp::OnConnectivitySourceConnected(int source)
{
	_statusWidget->SetEnableDevice((OSDStatusWidget::ConnectivitySource)source);
}

void OSDApp::OnConnectivitySourceDisconnected(int source)
{
	_statusWidget->SetDisableDevice((OSDStatusWidget::ConnectivitySource)source);
}

void OSDApp::OnApplicationChanged(int app)
{
	_currentAppID = app;

	OSD_APP_PROCESS_PRINTF("APP(%d)\n", app);
	_statusWidget->SetApplication(app);
}

void OSDApp::OnShowDisplay(bool show)
{
	fprintf(stderr, "%s:show(%s)\n", __PRETTY_FUNCTION__, show ? "true" : "false");
	if (show)
	{
		this->show();
	}
	else
	{
		this->hide();
	}
}

void OSDApp::OnReleaseAudioResource(unsigned char audioType, int app)
{
	_currSource = 100;
	// remove warning message
	(void)audioType;
	(void)app;
}

void OSDApp::OnAudioTypeChanged(unsigned short audioType)
{
	OSD_APP_PROCESS_PRINTF("AUDIO Type Change :  %u\n",audioType);
	unsigned short newSource;
	newSource = audioType;
	if(_currSource != newSource )
	{
		_currSource = newSource;
		_statusWidget->SetVolume(_sourceIDtoVolume[_currSource]);
	}
}

void OSDApp::OnGoToHome(void)
{
	SendDBusChangeMode("home", APP_ID_LAUNCHER);
}

void OSDApp::OnVolumeUp(void)
{
	if(_sourceIDtoVolume[_currSource] < MAX_VOLUME_COUNT - 1)
	{
		OSD_APP_PROCESS_PRINTF("VOLUME LEVEL(%hd)\n", _sourceIDtoVolume[_currSource]);
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[_currSource];
		setvolume = (setvolume + 1) * 10;
		SendDBusSetVolume(_currSource,setvolume);
	}
}

void OSDApp::OnVolumeDown(void)
{
	OSD_APP_PROCESS_PRINTF("VOLUME LEVEL(%hu)\n", _sourceIDtoVolume[_currSource]);
	if(_sourceIDtoVolume[_currSource] > 0)
	{
		short setvolume = 0;
		setvolume = _sourceIDtoVolume[_currSource];
		setvolume = (setvolume - 1) * 10;
		SendDBusSetVolume(_currSource, setvolume);
	}
}

void OSDApp::OnVolumeChanged(unsigned short sourceID, short volume)
{
	OSD_APP_PROCESS_PRINTF("VOLUME LEVEL(%hd)\n", volume);

	_sourceIDtoVolume[sourceID] = volume;

	if(sourceID == _currSource)
	{
		_statusWidget->SetVolume(volume);
	}
}

void OSDApp::OnKeyboardClicked(int key)
{
	OSD_APP_PROCESS_PRINTF("Clicked Key: %d\n", key);

	if (this->isVisible())
	{
		if (key == g_knobKeys[TCKeyVolumeUp])
		{
			OnVolumeUp();
		}
		else if (key == g_knobKeys[TCKeyVolumeDown])
		{
			OnVolumeDown();
		}
	}
}

OSDStatusWidget::ConnectivitySource GetSourceFromMediaDevice(int device)
{
	OSDStatusWidget::ConnectivitySource deviceSource = OSDStatusWidget::TotalConnectivitySources;

	switch(device)
	{
		case DeviceSourceUSB1:
			deviceSource = OSDStatusWidget::ConnectivitySourceUSB1;
			break;
		case DeviceSourceUSB2:
			deviceSource = OSDStatusWidget::ConnectivitySourceUSB2;
			break;
		case DeviceSourceSDMMC:
			deviceSource = OSDStatusWidget::ConnectivitySourceSDMMC;
			break;
		case DeviceSourceAUX:
			deviceSource = OSDStatusWidget::ConnectivitySourceAux;
			break;
		case DeviceSourceiAP2:
			deviceSource = OSDStatusWidget::ConnectivitySourceiAP2;
			break;
		case DeviceSourceBluetooth:
			deviceSource = OSDStatusWidget::ConnectivitySourceBluetooth;
			break;
	}

	return deviceSource;
}
