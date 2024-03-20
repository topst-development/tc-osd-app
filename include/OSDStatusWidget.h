/****************************************************************************************
 *   FileName    : OSDStatusWidget.h
 *   Description : OSDStatusWidget.h
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

#ifndef MEDIA_STATUS_WIDGET_H
#define MEDIA_STATUS_WIDGET_H

#include <QObject>
#include <QQuickItem>
#include "MediaPlayerType.h"

#define MAX_VOLUME_COUNT	11

class OSDStatusWidget : public QQuickItem
{
	Q_OBJECT
	Q_ENUMS(ConnectivitySource)
	Q_ENUMS(OSDType)
	Q_PROPERTY(int type READ type NOTIFY typeChanged)
	Q_PROPERTY(int volume READ volume NOTIFY volumeChanged)
	Q_PROPERTY(bool usb1 READ usb1 NOTIFY deviceChanged)
	Q_PROPERTY(bool usb2 READ usb2 NOTIFY deviceChanged)
	Q_PROPERTY(bool sdmmc READ sdmmc NOTIFY deviceChanged)
	Q_PROPERTY(bool aux READ aux NOTIFY deviceChanged)
	Q_PROPERTY(bool iAP2 READ iAP2 NOTIFY deviceChanged)
	Q_PROPERTY(bool carPlay READ carPlay NOTIFY deviceChanged)
	Q_PROPERTY(bool bluetooth READ bluetooth NOTIFY deviceChanged)

	public:
		typedef enum {
			ConnectivitySourceUSB1,
			ConnectivitySourceUSB2,
			ConnectivitySourceSDMMC,
			ConnectivitySourceAux,
			ConnectivitySourceiAP2,
			ConnectivitySourceCarPlay,
			ConnectivitySourceBluetooth,
			TotalConnectivitySources
		}ConnectivitySource;
		typedef enum {
			OSDTypeDefault,
			OSDTypePhone,
			OSDTypeSetting,
			TotalOSDTypes
		}OSDType;
		OSDStatusWidget(QQuickItem *parent = 0);
		~OSDStatusWidget();
		void Initialize(void);
		void SetApplication(int app);
		void SetEnableDevice(OSDStatusWidget::ConnectivitySource device);
		void SetDisableDevice(OSDStatusWidget::ConnectivitySource device);
		void SetVolume(int volume);

		int type() const;
		int volume() const;
		bool usb1() const;
		bool usb2() const;
		bool sdmmc() const;
		bool aux() const;
		bool iAP2() const;
		bool carPlay() const;
		bool bluetooth() const;

		Q_INVOKABLE void goToHome(void);
		Q_INVOKABLE void increaseVolume(void);
		Q_INVOKABLE void decreaseVolume(void);

	signals:
		void GoToHome();
		void VolumeUp();
		void VolumeDown();

		void setVisible();
		void setInvisible();
		void typeChanged();
		void volumeChanged();
		void deviceChanged();

	private:
		int _volume;
		bool _devices[TotalConnectivitySources];
		OSDType _type;
};

#endif // MEDIA_STATUS_WIDGET_H
