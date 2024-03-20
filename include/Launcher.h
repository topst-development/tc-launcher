/****************************************************************************************
 *   FileName    : Launcher.h
 *   Description : Launcher.h
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

#ifndef LAUNCHER_H
#define LAUNCHER_H

#include <QObject>
#include <QQuickView>
#include <QQuickWindow>
#include <QList>
#include <QRect>
#include <QAbstractListModel>
#include "HomeScreen.h"
#include "TCApplication.h"

class AppsInfo;

class Launcher : public QQuickView
{
    Q_OBJECT
	Q_PROPERTY(bool fullScreen MEMBER _fullScreen NOTIFY changedScreen)
	Q_PROPERTY(QAbstractListModel *appsInfo READ appsInfo NOTIFY changedApps)
	Q_PROPERTY(NOTIFY changedAppActive)

	public:
		Launcher(QQuickView *parent = 0);
		~Launcher();
		bool Initialize(const char *config = NULL);
		void ExitApplications(void);
		AppsInfo *GetAppsInfo(void);

		QAbstractListModel *appsInfo() const;

		Q_INVOKABLE void RequestAppShow(int myID);

	protected:
		void showEvent(QShowEvent *event);
		void hideEvent(QHideEvent * event);

	private:
		void InitializeSignalSlots(void);

	private slots:
		void OnShowAllApplications(void);
		void OnSetGeometry(bool full);
		void OnShowLauncher();
		void OnHideLauncher(int resources, int app);
		void OnKeyboardClicked(int key);
		void OnChangeToSuspendMode(void);
		void OnChangeAppActive(int app, bool active);

	signals:
		void ShowApplication(int app);
		void ShowHome(int app);
		void HideCompleted(int resources, int app);

		void changedScreen(void);
		void changedApps(void);
		void changedAppActive(int app, bool active);

	private:
		HomeScreen				*_homeScreenWidget;
		AppsInfo				*_appsInfo;
		QRect					_screenRect;
		QRect					_secondScreenRect;
		bool					_toExit;
		bool					_enableKey;
		bool					_fullScreen;
};

class AppsInfo : public QAbstractListModel
{
    Q_OBJECT

	enum AppIconRoles {
		AppActiveRole = Qt::UserRole + 1,
		ImageNamesRole,
		AppIDRole
	};

	public:
		explicit AppsInfo(QAbstractListModel *parent = 0);
		~AppsInfo();
		void ExitApplications(void);
		bool LoadConfig(const char *path);
		void StartApplicastions(bool launch);
		void ReleaseApplications(void);
		void AddApplication(TCApplication *app);

		int rowCount(const QModelIndex &parent = QModelIndex()) const;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

	protected:
		QHash<int, QByteArray> roleNames() const;

	private slots:
		void OnAppLaunched();
		void OnAppStarted();
		void OnAppFinished();
		void OnAppErrorOccurred(QProcess::ProcessError err);
		void OnActiveApplication(int app, int active);
		void OnStartApplications();
		void OnPreemptKeyEvent(int app);
		void OnReleasePreemptKeyEvent(int app);

	signals:
		void ShowApplication(int app);
		void ShowHome(int app);
		void EnableKeyEvent(int app);
		void DisableKeyEvent(int app);
		void ChangeAppActive(int app, bool active);

	private:
		QList<TCApplication *>	_applications;
		bool					_enableKey;
};
#endif // LAUNCHER_H
