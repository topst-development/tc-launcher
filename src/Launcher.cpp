/****************************************************************************************
 *   FileName    : Launcher.cpp
 *   Description : Launcher.cpp
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

#include <stdlib.h>
#include <unistd.h>
#ifdef USE_SYSTEMD
#include <systemd/sd-daemon.h>
#endif
#include <QDate>
#include <QFile>
#include <QScreen>
#include <QXmlStreamReader>
#include <QProcess>
#include <QGuiApplication>
#include "TCInput.h"
#include "LauncherIFManager.h"
#include "LauncherDBusManager.h"
#include "Launcher.h"

extern int g_debug;
extern bool g_launch;
extern int g_launcherAppID;

#define DEBUG_LAUNCHER_PROCESS_PRINTF(format, arg...) \
	if (g_debug) \
	{ \
		fprintf(stderr, "[LAUNCHER PROCESS] %s: " format "", __FUNCTION__, ##arg); \
	}

#define DEFAULT_CONFIG_FILE_PATH		"/usr/share/tcc_launcher/config.xml"
#define RELEASEDISPLAY					1
#define HOME_SCREEN_WIDTH				674

Launcher::Launcher(QQuickView *parent) :
	QQuickView(parent),
	_homeScreenWidget(NULL),
	_toExit(false),
	_enableKey(false),
	_fullScreen(false)
{
}

Launcher::~Launcher()
{
	_toExit = true;
	_appsInfo->ReleaseApplications();
}

bool Launcher::Initialize(const char *config)
{
	bool initialized = false;
	bool loaded = false;
	char *env;
	int displayNumber = 0;
	QObject *temp;
	QList<QScreen *> screenList = QGuiApplication::screens();

	_appsInfo = new AppsInfo;

	if (config != NULL)
	{
		loaded = _appsInfo->LoadConfig(config);
	}
	else
	{
		loaded = _appsInfo->LoadConfig(DEFAULT_CONFIG_FILE_PATH);
	}

	if (loaded)
	{
#if 0
		if (desktop->screenCount() > 1)
		{
			env = getenv("PASSENGER_DISPLAY_NUMBER");
			if (env != NULL)
			{
				displayNumber = atoi(env);
			}

			_secondScreenRect = desktop->screenGeometry(displayNumber);
			x = _secondScreenRect.x();
			y = _secondScreenRect.y();
			width = _secondScreenRect.width();
			height = _secondScreenRect.height();
			fprintf(stderr, "%s:display number(%d), x(%d), y(%d), width(%d), height(%d)\n", __PRETTY_FUNCTION__,
					displayNumber, x, y, width, height);

			_passengerScreenWidget = new PassengerScreen;
			_passengerScreenWidget->Initialize(x, y, width, height);
			_passengerScreenWidget->show();
		}
#endif

		env = getenv("IVI_DISPLAY_NUMBER");
		if (env != NULL)
		{
			displayNumber = atoi(env);
		}

		if (screenList.count() > displayNumber)
		{
			_screenRect = screenList[displayNumber]->geometry();
		}

		temp = this->findChild<QObject*>("launcherHome");
		_homeScreenWidget = qobject_cast<HomeScreen *>(temp);
		_homeScreenWidget->Initialize();
		_homeScreenWidget->ToggleNowPlaying();
		InitializeSignalSlots();

		initialized = true;
		_appsInfo->StartApplicastions(g_launch);
	}
	else
	{
		fprintf(stderr, "%s: load %s file failed\n", __PRETTY_FUNCTION__,
				(config != NULL) ? config : DEFAULT_CONFIG_FILE_PATH);
	}

	this->setResizeMode(QQuickView::SizeRootObjectToView);

	emit changedScreen();
	emit changedApps();

	return initialized;
}

void Launcher::ExitApplications(void)
{
	_toExit = true;
	_appsInfo->ExitApplications();
}

AppsInfo * Launcher::GetAppsInfo(void)
{
	return _appsInfo;
}

QAbstractListModel *Launcher::appsInfo() const
{
	return (QAbstractListModel *)_appsInfo;
}

void Launcher::RequestAppShow(int myID)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICATION(%d) REQUEST SHOW\n", myID);

	if(myID == g_launcherAppID)
	{
		emit ShowHome(g_launcherAppID);
	}
	else
	{
		emit ShowApplication(myID);
	}
}

void Launcher::showEvent(QShowEvent *event)
{
	QQuickView::showEvent(event);

	_enableKey = true;
#ifdef USE_SYSTEMD
	sd_notify(0, "READY=1");
#endif
}

void Launcher::hideEvent(QHideEvent * event)
{
	QQuickView::hideEvent(event);
	_enableKey = false;
}

void Launcher::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	connect(LAUNCHER_IF_MANAGER, SIGNAL(KeyboardClicked(int)),
			this, SLOT(OnKeyboardClicked(int)), dbusConnectionType);

	connect(LAUNCHER_IF_MANAGER, SIGNAL(SetGeometry(bool)),
			this, SLOT(OnSetGeometry(bool)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ShowLauncher()),
			this, SLOT(OnShowLauncher()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(HideLauncher(int, int)),
			this, SLOT(OnHideLauncher(int, int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ChangeToSuspendMode(void)),
			this, SLOT(OnChangeToSuspendMode(void)), dbusConnectionType);

	connect(_homeScreenWidget, SIGNAL(ShowAllApplications(void)),
			this, SLOT(OnShowAllApplications(void)), dbusConnectionType);

	connect(this, SIGNAL(visibleChanged(bool)),
			_homeScreenWidget, SLOT(OnChangedLauncherVisible(bool)));
	connect(this, SIGNAL(ShowApplication(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnShowApplication(int)), dbusConnectionType);
	connect(this, SIGNAL(ShowHome(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnShowHome(int)), dbusConnectionType);
	connect(this, SIGNAL(HideCompleted(int, int)),
			LAUNCHER_IF_MANAGER, SLOT(OnHideCompleted(int, int)), dbusConnectionType);

	connect(LAUNCHER_IF_MANAGER, SIGNAL(ActiveApplication(int,int)),
			_appsInfo, SLOT(OnActiveApplication(int,int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(StartApplications()),
			_appsInfo, SLOT(OnStartApplications()), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(PreemptKeyEvent(int)),
			_appsInfo, SLOT(OnPreemptKeyEvent(int)), dbusConnectionType);
	connect(LAUNCHER_IF_MANAGER, SIGNAL(ReleasePreemptKeyEvent(int)),
			_appsInfo, SLOT(OnReleasePreemptKeyEvent(int)), dbusConnectionType);

	connect(_appsInfo, SIGNAL(ChangeAppActive(int, bool)),
			this, SLOT(OnChangeAppActive(int, bool)), dbusConnectionType);
}

void Launcher::OnShowAllApplications(void)
{
	emit ShowApplication(g_launcherAppID);
}

void Launcher::OnSetGeometry(bool full)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	if(_fullScreen != full)
	{
		this->show();
		_fullScreen = full;
		if (_fullScreen)
		{
			DEBUG_LAUNCHER_PROCESS_PRINTF("%d %d %d %d\n", _screenRect.x(), _screenRect.y(), _screenRect.width(), _screenRect.height());
			this->setGeometry(_screenRect.x(), _screenRect.y(), _screenRect.width(), _screenRect.height());
		}
		else
		{
			DEBUG_LAUNCHER_PROCESS_PRINTF("%d %d %d %d\n", _screenRect.width() - HOME_SCREEN_WIDTH, _screenRect.y(), HOME_SCREEN_WIDTH, _screenRect.height());
			this->setGeometry(_screenRect.width() - HOME_SCREEN_WIDTH, _screenRect.y(), HOME_SCREEN_WIDTH, _screenRect.height());
		}
		this->hide();

		emit changedScreen();
	}

}

void Launcher::OnShowLauncher()
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	this->show();
}

void Launcher::OnHideLauncher(int resources, int app)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	if(resources == RELEASEDISPLAY)
	{
		emit HideCompleted(resources, app);
		this->hide();
	}
}

void Launcher::OnChangeAppActive(int app, bool active)
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	emit changedAppActive(app, active);
}

void Launcher::OnKeyboardClicked(int key)
{
	if (_enableKey)
	{
		if (key == g_knobKeys[TCKeyHome] && _fullScreen)
		{
			emit ShowHome(g_launcherAppID);
		}
	}

	if (key == g_knobKeys[TCKeyPower])
	{
		_homeScreenWidget->ToggleNowPlaying();
	}
}

void Launcher::OnChangeToSuspendMode(void)
{
	SendDBusChangeMode("home", g_launcherAppID);
}

AppsInfo::AppsInfo(QAbstractListModel *parent) :
	QAbstractListModel(parent),
	_enableKey(false)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;

	connect(this, SIGNAL(ShowApplication(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnShowApplication(int)), dbusConnectionType);
	connect(this, SIGNAL(ShowHome(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnShowHome(int)), dbusConnectionType);
	connect(this, SIGNAL(EnableKeyEvent(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnEnableKeyEvent(int)), dbusConnectionType);
	connect(this, SIGNAL(DisableKeyEvent(int)),
			LAUNCHER_IF_MANAGER, SLOT(OnDisableKeyEvent(int)), dbusConnectionType);
}

AppsInfo::~AppsInfo()
{
}

void AppsInfo::ExitApplications(void)
{
	int count = _applications.count();
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	for (int i = 0; i < count; i++)
	{
		_applications[i]->Exit();
	}
}

bool AppsInfo::LoadConfig(const char *path)
{
	bool loaded = false;
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	if (path != NULL)
	{
		QFile xmlFile(path);

		if (!xmlFile.exists() || (xmlFile.error() != QFile::NoError))
		{
			fprintf(stderr, "%s: Unable to open config file(%s)\n", __PRETTY_FUNCTION__, path);
		}
		else
		{
			loaded = true;
		}

		xmlFile.open(QIODevice::ReadOnly);
		QXmlStreamReader reader(&xmlFile);
		QXmlStreamAttributes attributes;

		bool stop = false;
		while (!reader.atEnd())
		{
			reader.readNext();

			if (reader.isStartElement() && reader.name() == "applications")
			{
				QStringRef appID;
				QStringRef appPath;
				QStringRef appName;
				QStringRef appImagePath;
				QStringRef appImageName;
				QStringRef appDisplay;
				QStringRef appArgument;
				QStringRef appStatus;
				QStringRef appLanguage;
				TCApplication *app;
				bool isRear;

				stop = false;
				while (!reader.atEnd() && !stop)
				{
					reader.readNext();
					if (reader.isStartElement() && reader.name() == "app")
					{
						attributes = reader.attributes();
						appPath = attributes.value("path");
						if (!appPath.isEmpty())
						{
							appID = attributes.value("id");
							appName = attributes.value("name");
							appImagePath = attributes.value("image_path");
							appImageName = attributes.value("image_name");
							appArgument = attributes.value("args");
							appDisplay = attributes.value("display");
							isRear = false;
							appStatus = attributes.value("status");
							appLanguage = attributes.value("language");

							app = new TCApplication;
							app->Initialize(appID.toString().toInt(),
													appPath.toString(),
													appImagePath.toString(),
													appImageName.toString(),
													isRear,
													appName.toString(),
													appArgument.toString().split(" "));
							AddApplication(app);
							app->SetIndex(_applications.count());

							DEBUG_LAUNCHER_PROCESS_PRINTF("ADD APPLICATION. PATH(%s), NAME(%s), IMAGE_PATH(%s), IMAGE_NAME(%s), DISPLAY(%s), ARGS(%s)\n",
														  appPath.toString().toLatin1().data(),
														  appName.toString().toLatin1().data(),
														  appImagePath.toString().toLatin1().data(),
														  appImageName.toString().toLatin1().data(),
														  isRear ? "REAR" : "FRONT",
														  appArgument.toString().toLatin1().data());

							connect(app, SIGNAL(AppStarting()), this, SLOT(OnAppLaunched()));
							connect(app, SIGNAL(AppStarted()), this, SLOT(OnAppStarted()));
							connect(app, SIGNAL(AppFinished()), this, SLOT(OnAppFinished()));
							connect(app, SIGNAL(AppErrorOccurred(QProcess::ProcessError)), this, SLOT(OnAppErrorOccurred(QProcess::ProcessError)));

							if (!appLanguage.isEmpty())
							{
								app->SetLanguage(appLanguage.toString().toLatin1().data());
							}

							QString status = appStatus.toString();

							if (status == "active")
							{
								app->SetActive(true);
								emit ChangeAppActive(appID.toString().toInt(), true);
							}
						}
					}
					else if(reader.isEndElement() && reader.name() == "applications")
					{
						stop = true;
					}
				}
			}
		}

		if (reader.hasError())
		{
			fprintf(stderr, "%s: Error parsing %s on line %d column %d: \n\t%s\n", __PRETTY_FUNCTION__,
					path,
					(int)reader.lineNumber(),
					(int)reader.columnNumber(),
					reader.errorString().toLatin1().data());
		}
	}

	return loaded;
}

void AppsInfo::StartApplicastions(bool launch)
{
	if (launch)
	{
		int count = _applications.count();

		for (int i = 0; i < count; i++)
		{
			if(_applications[i]->GetID() != 0)
			{
				_applications[i]->Launch();
			}
		}
	}
}

void AppsInfo::ReleaseApplications(void)
{
	int count = _applications.count();

	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");
	for (int i = 0; i < count; i++)
	{
		_applications[i]->Exit();
		delete _applications[i];
	}
	_applications.clear();
}

void AppsInfo::OnAppLaunched()
{
	TCApplication *app = qobject_cast<TCApplication *>(sender());

	if (app != NULL)
	{
		DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICATION(%s) LAUNCHED\n", app->GetCaption().toLatin1().data());
	}
	else
	{
		fprintf(stderr, "%s: unknown sender\n", __PRETTY_FUNCTION__);
	}
}

void AppsInfo::OnAppStarted()
{
	TCApplication *app = qobject_cast<TCApplication *>(sender());

	if (app != NULL)
	{
		DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICATION(%s) STARTED\n",
									  app->GetCaption().toLatin1().data());
	}
	else
	{
		fprintf(stderr, "%s: unknown sender\n", __PRETTY_FUNCTION__);
	}
}

void AppsInfo::OnAppFinished()
{
	TCApplication *app = qobject_cast<TCApplication *>(sender());

	if (app != NULL)
	{
		DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICATION(%s) FINISHED\n", app->GetCaption().toLatin1().data());

		app->SetActive(false);
		emit ChangeAppActive(app->GetID(), false);
	}
	else
	{
		fprintf(stderr, "%s: unknown sender\n", __PRETTY_FUNCTION__);
	}
}

void AppsInfo::OnAppErrorOccurred(QProcess::ProcessError err)
{
	TCApplication *app = qobject_cast<TCApplication *>(sender());

	if (app != NULL)
	{
		fprintf(stderr, "%s: application(%s) error(%d) occurred\n", __PRETTY_FUNCTION__,
				app->GetCaption().toLatin1().data(), err);

		app->SetActive(false);
		emit ChangeAppActive(app->GetID(), false);
	}
	else
	{
		fprintf(stderr, "%s: unknown sender. error(%d)\n", __PRETTY_FUNCTION__, err);
	}
}

void AppsInfo::AddApplication(TCApplication *app)
{
	beginInsertRows(QModelIndex(), rowCount(), rowCount());
	_applications << app;
	endInsertRows();
}

void AppsInfo::OnActiveApplication(int app, int active)
{
	int count = _applications.count();
	bool stop = false;

	DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICAIONT(%d), ACTIVE(%d)\n", app, active);

	for (int i = 0; i < count && !stop; i++)
	{
		if (_applications[i]->GetID() == app)
		{
			_applications[i]->SetActive(active);
			emit ChangeAppActive(app, active);
			stop = true;
		}
	}
}

void AppsInfo::OnStartApplications()
{
	DEBUG_LAUNCHER_PROCESS_PRINTF("\n");

	_enableKey = true;
#ifdef USE_SYSTEMD
	sd_notify(0, "READY=1");
#endif

	StartApplicastions(true);
}

void AppsInfo::OnPreemptKeyEvent(int app)
{
	int count = _applications.count();
	bool stop = false;

	DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICAIONT(%d)\n", app);

	for (int i = 0; i < count && !stop; i++)
	{
		if (_applications[i]->GetID() == app)
		{
			_applications[i]->SetPreemptKeyEvent(true);
			emit DisableKeyEvent(app);
			stop = true;
		}
	}

	if (!stop)
	{
		fprintf(stderr, "%s: not found application(%d)\n", __PRETTY_FUNCTION__, app);
	}
}

void AppsInfo::OnReleasePreemptKeyEvent(int app)
{
	int count = _applications.count();
	bool stop = false;

	DEBUG_LAUNCHER_PROCESS_PRINTF("APPLICAIONT(%d)\n", app);

	for (int i = 0; i < count && !stop; i++)
	{
		if (_applications[i]->GetID() == app)
		{
			if (_applications[i]->IsPreemptKeyEvent())
			{
				_applications[i]->SetPreemptKeyEvent(false);
				emit EnableKeyEvent(app);
				stop = true;
			}
			else
			{
				fprintf(stderr, "%s: not matched application id(%d)\n", __PRETTY_FUNCTION__, app);
			}
		}
	}

	if (!stop)
	{
		fprintf(stderr, "%s: not found application(%d)\n", __PRETTY_FUNCTION__, app);
	}
}

int AppsInfo::rowCount(const QModelIndex & parent) const
{
	Q_UNUSED(parent)
	return _applications.count();
}

QVariant AppsInfo::data(const QModelIndex & index, int role) const
{
	QVariant var;

	if (index.row() > 0 || index.row() < _applications.count())
	{
		TCApplication *app = _applications[index.row()];
		app->SetIndex(index.row());

		if (role == AppActiveRole)
		{
			var = app->getActive();
		}
		else if (role == ImageNamesRole)
		{
			var = app->GetIconNames();
		}
		else if (role == AppIDRole)
		{
			var = app->GetID();
		}
	}

	return var;
}

QHash<int, QByteArray> AppsInfo::roleNames() const
{
	QHash<int, QByteArray> roles;

	roles[AppActiveRole] = "appActive";
	roles[ImageNamesRole] = "iconImageNames";
	roles[AppIDRole] = "appID";

	return roles;
}
