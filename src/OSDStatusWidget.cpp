/****************************************************************************************
 *   FileName    : OSDStatusWidget.cpp
 *   Description : OSDStatusWidget.cpp
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

#include "OSDStatusWidget.h"
#include "DBusMsgDef.h"
#include "OSDIFManager.h"

extern int g_debug;


#define OSD_APP_STATUS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[OSD APP STATUS] %s: " format "", __FUNCTION__, ##arg); \
	}

OSDStatusWidget::OSDStatusWidget(QQuickItem *parent) :
	QQuickItem(parent),
	_volume(0),
	_type(OSDTypeDefault)
{
	int i;

	for (i = 0; i < TotalConnectivitySources; i++)
	{
		_devices[i] = false;
	}
}

OSDStatusWidget::~OSDStatusWidget()
{
}

void OSDStatusWidget::Initialize(void)
{
}

void OSDStatusWidget::SetApplication(int app)
{
	if (app == APP_ID_PHONE)
	{
		_type = OSDTypePhone;
	}
	else if (app == APP_ID_SETTING)
	{
		_type = OSDTypeSetting;
	}
	else
	{
		_type = OSDTypeDefault;
	}
}

void OSDStatusWidget::SetEnableDevice(OSDStatusWidget::ConnectivitySource device)
{
	int index = (int)device;

	OSD_APP_STATUS_PRINTF("%d device is connected\n", index);

	if (index >= ConnectivitySourceUSB1 && index < TotalConnectivitySources)
	{
		_devices[index] = true;
		emit deviceChanged();
	}
}

void OSDStatusWidget::SetDisableDevice(OSDStatusWidget::ConnectivitySource device)
{
	int index = (int)device;

	OSD_APP_STATUS_PRINTF("%d device is disconnected\n", index);

	if (index >= ConnectivitySourceUSB1 && index < TotalConnectivitySources)
	{
		_devices[index] = false;
		emit deviceChanged();
	}
}

void OSDStatusWidget::SetVolume(int volume)
{
	_volume = volume;
	if ((_volume < MAX_VOLUME_COUNT) && (_volume >= 0))
	{
		emit volumeChanged();
	}
	else
	{
		fprintf(stderr, "%s: invalid volume(%u)\n", __PRETTY_FUNCTION__, volume);
	}
}

int OSDStatusWidget::type() const
{
	return (int)_type;
}

int OSDStatusWidget::volume() const
{
	return (int)_volume;
}

bool OSDStatusWidget::usb1() const
{
  return _devices[ConnectivitySourceUSB1];
}

bool OSDStatusWidget::usb2() const
{
  return _devices[ConnectivitySourceUSB2];
}

bool OSDStatusWidget::sdmmc() const
{
  return _devices[ConnectivitySourceSDMMC];
}

bool OSDStatusWidget::aux() const
{
  return _devices[ConnectivitySourceAux];
}

bool OSDStatusWidget::iAP2() const
{
  return _devices[ConnectivitySourceiAP2];
}

bool OSDStatusWidget::carPlay() const
{
  return _devices[ConnectivitySourceCarPlay];
}

bool OSDStatusWidget::bluetooth() const
{
  return _devices[ConnectivitySourceBluetooth];
}

void OSDStatusWidget::goToHome(void)
{
	emit GoToHome();
}

void OSDStatusWidget::increaseVolume(void)
{
	emit VolumeUp();
}

void OSDStatusWidget::decreaseVolume(void)
{
	emit VolumeDown();
}
