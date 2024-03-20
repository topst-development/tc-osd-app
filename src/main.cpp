/****************************************************************************************
 *   FileName    : main.cpp
 *   Description : main.cpp
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

#include <cstdio>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/stat.h>
#include <QQuickItem>
#include <QQuickWindow>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "OSDDBusManager.h"
#include "OSDIFManager.h"
#include "OSDApp.h"

static void NewExceptionHandler();
static void SignalHandler(int sig);
static void PrintQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
static void CreatePIDFile(void);

#define APP_WIDTH  1024
#define APP_HEIGHT 65

int g_debug = 0;

const char *_pid_file = "/var/run/tc-osd-app.pid";

OSDApp *_osdApp = NULL;

int main(int argc, char *argv[])
{
	int ret = -1;

	qInstallMessageHandler(PrintQtMessage);
	QGuiApplication app(argc, argv);

	QQmlApplicationEngine engine;
	QObject* window;

	signal(SIGTERM, SignalHandler);
	signal(SIGCHLD, SIG_IGN);

	QString argument;

	for (int i = 1; i < argc; i++)
	{
		argument = argv[i];
		argument = argument.toUpper();
		if (argument == "--DEBUG")
		{
			g_debug = 1;
		}
	}

	CreatePIDFile();

	std::set_new_handler(NewExceptionHandler);

	qmlRegisterType<OSDApp>("OSDApp", 1, 0, "TCOSDWin");
	qmlRegisterType<OSDStatusWidget>("OSDApp", 1, 0, "TCOSD");
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	window = engine.rootObjects().value(0);
	_osdApp = qobject_cast<OSDApp*>(window);
	if (_osdApp != NULL)
	{
		_osdApp->Initialize();
		OSD_IF_MANAGER->Initilaize();
		fprintf(stderr, "OSD APPLICATION PROCESS STARTED.\n");

		ret = app.exec();

		if (_osdApp != NULL)
		{
			delete _osdApp;
			_osdApp = NULL;
		}
		OSD_IF_MANAGER->Release();
	}

	return ret;
}

static void NewExceptionHandler()
{
	fprintf(stderr, "[OSD] FATAL ERROR : MEMORY ALLOCATION FAILED\n");
	throw std::bad_alloc();
}

static void SignalHandler(int sig)
{
	fprintf(stderr, "[OSD] %s: received signal(%d)\n", __FUNCTION__, sig);
	QGuiApplication::exit(sig);
}

static void PrintQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
	QByteArray localMsg = msg.toLocal8Bit();
	switch ((int)type)
	{
		case QtDebugMsg:
			fprintf(stderr, "QtDebug: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtWarningMsg:
			fprintf(stderr, "QtWarning: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtCriticalMsg:
			fprintf(stderr, "QtCritical: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			break;
		case QtFatalMsg:
			fprintf(stderr, "QtFatal: %s (%s:%u, %s)\n", localMsg.constData(), context.file, context.line, context.function);
			abort();
	}
}

static void CreatePIDFile(void)
{
	FILE *pid_fp;

	// umask the file mode
	umask(0);

	// create pid file
	pid_fp = fopen(_pid_file, "w+");
	if (pid_fp != NULL)
	{
		fprintf(pid_fp, "%d\n", getpid());
		fclose(pid_fp);
		pid_fp = NULL;
	}
	else
	{
		perror("pid file open failed");
	}
}
