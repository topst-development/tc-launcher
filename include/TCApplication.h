/****************************************************************************************
 *   FileName    : TCApplication.h
 *   Description : TCApplication.h
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

#ifndef TC_APPLICATION_H
#define TC_APPLICATION_H

#include <QObject>
#include <QProcess>
#include <QQuickItem>

class TCApplication : public QObject
{
	Q_OBJECT

	public:
	    explicit TCApplication(QObject *parent = 0);
	    ~TCApplication();
	    void Initialize(int id, QString path, QString imagePath, QString imageName, bool rear, QString caption, QStringList args);
		void Launch();
		void Exit();
		QProcess::ProcessState GetStatus() const;
		int GetID() const;
		bool IsRear() const;
		bool IsPreemptKeyEvent() const;
		void SetIndex(unsigned int index);
		void SetLanguage(const char *language);
		void SetPreemptKeyEvent(bool set);
		unsigned int GetIndex() const;
		QString &GetCaption();
		QStringList GetIconNames(void) const;
		void SetActive(bool active);
		bool getActive() const;

	private slots:
		void OnStateChanged(QProcess::ProcessState state);
		void OnStarted();
		void OnFinished(int exitCode, QProcess::ExitStatus exitStatus);
		void OnError(QProcess::ProcessError err);

	signals:
		void AppStarting();
		void AppStarted();
		void AppFinished();
		void AppErrorOccurred(QProcess::ProcessError err);

	private:
		QProcess::ProcessState _status;
		unsigned int _index;
		int _id;
		bool _rear;
		bool _exist;
		bool _preemptKey;
		bool _active;
		QString _path;
		QString _caption;
		QString _imagePath;
		QString _imageName;
		QStringList _arguments;
		QProcess _process;
		QProcessEnvironment _env;
		QStringList _iconNames;
};

#endif // TC_APPLICATION_H
