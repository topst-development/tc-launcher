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

#include <unistd.h>
#include <sys/signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <cstdio>
#include <QTextCodec>
#include <QQmlEngine>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

#include "LauncherIFManager.h"
#include "LauncherDBusManager.h"
#include "Launcher.h"
#include "TCApplication.h"
#include "ImageProvider.h"

static void NewExceptionHandler();
static void SignalHandler(int sig);
static void PrintQtMessage(QtMsgType type, const QMessageLogContext &context, const QString &msg);
static void CreatePIDFile();

int g_launcherAppID = 0;
int g_debug = 0;
bool g_launch = true;
bool _force_exit = false;
Launcher *_launcher = NULL;
const char *_pid_file = "/var/run/tc-launcher.pid";
QTextCodec *g_codecUtf16 = NULL;
QTextCodec *g_codecUtf8 = NULL;

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);

	int ret = -1;
	QString argument;
	QString config;
	QString inputKeyboard = NULL;
	QQmlApplicationEngine engine;
	QObject *window;

	for (int i = 1; i < argc; i++)
	{
		argument = argv[i];
		if (argument == "--debug")
		{
			g_debug = 1;
		}
		else if (argument == "--only-launcher")
		{
			g_launch = false;
		}
		else if (argument == "--force-exit")
		{
			_force_exit = true;
		}
		else if (argument.left(9) == "--config=")
		{
			config = argument.mid(9);
		}
		else if (argument.left(11) == "--keyboard=")
		{
			inputKeyboard = argument.mid(11);
		}
	}

	CreatePIDFile();

	qInstallMessageHandler(PrintQtMessage);

	signal(SIGINT, SignalHandler);
	signal(SIGTERM, SignalHandler);
	signal(SIGABRT, SignalHandler);

	std::set_new_handler(NewExceptionHandler);
	g_codecUtf8 = QTextCodec::codecForName("UTF-8");
	g_codecUtf16 = QTextCodec::codecForName("UTF-16");

	qmlRegisterType<Launcher>("Launcher", 1, 0, "TCLauncherWin");
	qmlRegisterType<HomeScreen>("Launcher", 1, 0, "TCHome");
	qmlRegisterType<StatusWidget>("Launcher", 1, 0, "TCStatus");
	qmlRegisterType<NowPlayMusic>("Launcher", 1, 0, "TCNowPlayMusic");
	qmlRegisterType<AlbumArt>("Launcher", 1, 0, "TCAlbumArt");
	qmlRegisterType<NowPlayRadio>("Launcher", 1, 0, "TCNowPlayRadio");
	qmlRegisterType<NowPlayCarplay>("Launcher", 1, 0, "TCNowPlayCarplay");

	engine.addImageProvider(QString("extern"), ImageProvider::instance());
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	window = engine.rootObjects().value(0);

	_launcher = qobject_cast<Launcher*>(window);

	if (_launcher != NULL)
	{
		if (_launcher->Initialize(config.isEmpty() ? NULL : config.toLatin1().data()))
		{
			LAUNCHER_IF_MANAGER->Initilaize(inputKeyboard.isEmpty() ? NULL : inputKeyboard.toLatin1().data());

			fprintf(stderr, "LAUNCHER PROCESS STARTED.\n");

			if (_force_exit)
			{
				ret = 0;
			}
			else
			{
				ret = app.exec();
			}

			LAUNCHER_IF_MANAGER->Release();
		}
		else
		{
			fprintf(stderr, "%s: initialize launcher failed\n", __func__);
		}

		if (_launcher != NULL)
		{
			delete _launcher;
			_launcher = NULL;
		}
	}
	else
	{
		fprintf(stderr, "%s: create lancher process failed\n", __func__);
	}

	if (access(_pid_file, F_OK) == 0)
	{
		if (unlink(_pid_file) != 0)
		{
			perror("delete pid file failed");
		}
	}

	fprintf(stderr, "LAUNCHER PROCESS RELEASED.\n");
	return ret;
}

static void NewExceptionHandler()
{
	fprintf(stderr, "[LAUNCHER PROCESS] FATAL ERROR : MEMORY ALLOCATION FAILED\n");
	throw std::bad_alloc();
}

static void SignalHandler(int sig)
{
	fprintf(stderr, "[LAUNCHER PROCESS] %s: received signal(%d)\n", __func__, sig);
	if (_launcher != NULL)
	{
		_launcher->ExitApplications();
	}
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

static void CreatePIDFile()
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

