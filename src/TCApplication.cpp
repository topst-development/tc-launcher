/****************************************************************************************
 *   FileName    : TCApplication.cpp
 *   Description : TCApplication.cpp
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
#include "TCApplication.h"

#define INVALID_INDEX		(unsigned int)(-1)

TCApplication::TCApplication(QObject *parent) :
	QObject(parent),
	_status(QProcess::NotRunning),
	_index(INVALID_INDEX),
	_id(-1),
	_rear(false),
	_exist(false),
	_preemptKey(false),
	_active(false),
	_path(""),
	_caption(""),
	_imagePath(""),
	_imageName("")
{
}

TCApplication::~TCApplication()
{
}

void TCApplication::Initialize(int id, QString path, QString imagePath, QString imageName, bool rear, QString caption, QStringList args)
{
	_id = id;
	_rear = rear;
	_exist = false;
	_preemptKey = false;
	_active = false;
	_path = path;
	_caption = caption;
	_imagePath = imagePath;
	_imageName = imageName;
	_arguments = args;

	if (!imagePath.isEmpty())
	{
		_iconNames.append(QString("%1/%2_d.png").arg(imagePath).arg(imageName));
		_iconNames.append(QString("%1/%2_n.png").arg(imagePath).arg(imageName));
		_iconNames.append(QString("%1/%2_p.png").arg(imagePath).arg(imageName));
		_iconNames.append(QString("%1/%2_f.png").arg(imagePath).arg(imageName));
	}

    _process.setProcessChannelMode(QProcess::ForwardedChannels);
	_arguments.append(QString("--id=%1").arg(_id));
	if (rear)
	{
		_arguments.append(QString("--rear").arg(_id));
	}


    connect(&_process, SIGNAL(stateChanged(QProcess::ProcessState)),
			this, SLOT(OnStateChanged(QProcess::ProcessState)));
	connect(&_process, SIGNAL(started()),
			 this, SLOT(OnStarted()));
    connect(&_process, SIGNAL(finished(int,QProcess::ExitStatus)),
			this, SLOT(OnFinished(int,QProcess::ExitStatus)));
    connect(&_process, SIGNAL(error(QProcess::ProcessError)),
			this, SLOT(OnError(QProcess::ProcessError)));

	int err;

	err = access(_path.toLatin1().data(), F_OK);
	if (err == 0)
	{
		_exist = true;
		_env = QProcessEnvironment::systemEnvironment();
		_process.setProcessEnvironment(_env);
	}
	else
	{
		fprintf(stderr, "check exist file(%s) failed:", _path.toLatin1().data());
		perror("");
	}
}

void TCApplication::Launch()
{
	if (_exist)
	{
		_process.start(_path, _arguments);
	}
	else
	{
		fprintf(stderr, "%s: %s not exist\n", __PRETTY_FUNCTION__, _path.toLatin1().data());
	}
}

void TCApplication::Exit()
{
	_process.terminate();
	if (!_process.waitForFinished(5000))
	{
		_process.kill();
	}
}

QProcess::ProcessState TCApplication::GetStatus() const
{
	return _status;
}

int TCApplication::GetID() const
{
	return _id;
}

bool TCApplication::IsRear() const
{
	return _rear;
}

bool TCApplication::IsPreemptKeyEvent() const
{
	return _preemptKey;
}

void TCApplication::SetIndex(unsigned int index)
{
	_index = index;
}

void TCApplication::SetLanguage(const char *language)
{
	_env.insert("LANG", language);
	_process.setProcessEnvironment(_env);
}

void TCApplication::SetPreemptKeyEvent(bool set)
{
	_preemptKey = set;
}

unsigned int TCApplication::GetIndex() const
{
	return _index;
}

QString &TCApplication::GetCaption()
{
	return _caption;
}

QStringList TCApplication::GetIconNames(void) const
{
	return _iconNames;
}

void TCApplication::SetActive(bool active)
{
	_active = active;
}

bool TCApplication::getActive() const
{
	return _active;
}

void TCApplication::OnStateChanged(QProcess::ProcessState state)
{
	if (state == QProcess::Starting)
	{
		emit AppStarting();
	}

	_status = state;
}

void TCApplication::OnStarted()
{
	emit AppStarted();
}

void TCApplication::OnFinished(int exitCode, QProcess::ExitStatus exitStatus)
{
    Q_UNUSED(exitCode);
    Q_UNUSED(exitStatus);

    emit AppFinished();
}

void TCApplication::OnError(QProcess::ProcessError err)
{
    if (err == QProcess::Crashed)
	{
        emit AppFinished();
	}
	else
	{
		emit AppErrorOccurred(err);
	}
}

