/****************************************************************************************
 *   FileName    : PassengerScreen.cpp
 *   Description : PassengerScreen.cpp
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
#include <QApplication>
#include <QDesktopWidget>
#include "LauncherIFManager.h"
#include "PassengerScreen.h"

extern int g_debug;

#define DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF(format, arg...) \
	if (true) \
	{ \
		fprintf(stderr, "[LAUNCHER PASSENGER SCREEN] %s: " format "", __FUNCTION__, ##arg); \
	}

static const char *_iconAreaImageNames = ":/images/linux_avn_main_icon_area.png";
static const char *_timeImageNames[10] = {
	":/images/linux_avn_main_time_0.png",
	":/images/linux_avn_main_time_1.png",
	":/images/linux_avn_main_time_2.png",
	":/images/linux_avn_main_time_3.png",
	":/images/linux_avn_main_time_4.png",
	":/images/linux_avn_main_time_5.png",
	":/images/linux_avn_main_time_6.png",
	":/images/linux_avn_main_time_7.png",
	":/images/linux_avn_main_time_8.png",
	":/images/linux_avn_main_time_9.png"
};

static const char *_timeColonImageNames = ":/images/linux_avn_main_time_colon.png";
static const char *_timeAMImageNames = ":/images/linux_avn_main_time_am.png";
static const char *_timePMImageNames = ":/images/linux_avn_main_time_pm.png";
static const char *_backgroundImageNames = ":/images/linux_avn_background.png";

static const char *_btnFVCImageNames[4] = {
	":/images/passenger_screen/fvc_n.png", ":/images/passenger_screen/fvc_p.png",
	":/images/passenger_screen/fvc_f.png", ":/images/passenger_screen/fvc_d.png",
};

static const char *_btnSVMImageNames[4] = {
	":/images/passenger_screen/svm_n.png", ":/images/passenger_screen/svm_p.png",
	":/images/passenger_screen/svm_f.png", ":/images/passenger_screen/svm_d.png",
};

static const char *_btnExitImageNames[2] = {
	":/images/passenger_screen/exit_n.png", ":/images/passenger_screen/exit_p.png",
};

#define DEFAULT_CONFIG_FILE_PATH		"/usr/share/tcc_launcher/config.xml"
#define APP_BUTTON_X_OFFSET				30
#define APP_BUTTON_Y_OFFSET				220
#define APP_BUTTON_WIDTH				132
#define APP_BUTTON_HEIGHT				167
#define APP_AREA_WIDTH					720
#define SCREEN0_NUM						1
#define SCREEN1_NUM						0
#define REAR_Y_OFFSET					(-70)
#define MULTI_LINE_Y_OFFSET				(-80)
#define MULTI_2ND_LINE_Y_OFFSET			170
#define RELEASEDISPLAY					1
#define MAX_ICON_COUNT					2

static const int _timeH1Rect[4] = {
	265, 80, 42, 68
};

static const int _timeH2Rect[4] = {
	315, 80, 42, 68
};

static const int _timeM1Rect[4] = {
	385, 80, 42, 68
};

static const int _timeM2Rect[4] = {
	435, 80, 42, 68
};

static const int _timeColonRect[4] = {
	364, 80, 14, 68
};

static const int _timeAMRect[4] = {
	485, 80, 63, 68
};

static const int _iconAreaRect[4] = {
	0, 292, 800, 80
};

static const int _btnExitRect[4] = {
	10, 10, 96, 96
};

PassengerScreen::PassengerScreen(QWidget *parent) :
	QWidget(parent)
{
	_lblBackground = new TCImageButton(this);
	_lblTimeH1 = new TCImageButton(_lblBackground);
	_lblTimeH2 = new TCImageButton(_lblBackground);
	_lblTimeM1 = new TCImageButton(_lblBackground);
	_lblTimeM2 = new TCImageButton(_lblBackground);
	_lblTimeColon = new TCImageButton(_lblBackground);
	_lblTimeAM = new TCImageButton(_lblBackground);
	_lblTimePM = new TCImageButton(_lblBackground);
	_lblIconArea = new TCImageButton(_lblBackground);
	_btnSVM = new TCImageButton(_lblBackground);
	_btnFVC = new TCImageButton(_lblBackground);
	_btnExitSVM = new TCImageButton(this);
	_btnExitFVC = new TCImageButton(this);
	
	_btnManager = new TCImageButtonManager();
	_btnManager->ClearButtons();
	_btnManager->AddButton(_btnSVM);
	_btnManager->AddButton(_btnFVC);
	ReplaceIconPosition();
}

PassengerScreen::~PassengerScreen()
{
}

bool PassengerScreen::Initialize(int x, int y, int width, int height)
{
	bool initialized = false;

	QDesktopWidget *desktop = QApplication::desktop();
	if (desktop != NULL)
	{
		this->setWindowFlags(Qt::FramelessWindowHint);
		this->setStyleSheet("background:transparent;");

		if (width > 800)
		{
			_deltaWidth = (width - 800);
			_deltaX = _deltaWidth / 2;
			_deltaY = (height - 480) / 2;
		}
		else
		{
			_deltaX = 0;
			_deltaY = 0;
		}

		this->setGeometry(x, y, width, height);

		UpdateImages();
		UpdateGeometry();
		InitializeSignalSlots();

		_currentTime.SetCurrentTime();
		UpdateDisplay();
		_timer.setInterval(1000);

		ReplaceIconPosition();

		initialized = true;
		_timer.start();
	}
	else
	{
		fprintf(stderr, "Can not get desktop\n");
	}

	return initialized;
}

void PassengerScreen::UpdateImages(void)
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");

	_lblIconArea->SetImage(_iconAreaImageNames);
	_lblIconArea->SetEnableClick(false);

	for (int i = 0; i < 10; i++)
	{
		_timeImages[i].load(_timeImageNames[i]);
	}
	_lblTimeH1->SetEnableClick(false);
	_lblTimeH2->SetEnableClick(false);
	_lblTimeM1->SetEnableClick(false);
	_lblTimeM2->SetEnableClick(false);

	_lblTimeColon->SetImage(_timeColonImageNames);
	_lblTimeColon->SetEnableClick(false);

	_lblTimeAM->SetImage(_timeAMImageNames);
	_lblTimeAM->SetEnableClick(false);

	_lblTimePM->SetImage(_timePMImageNames);
	_lblTimePM->SetEnableClick(false);

	_lblBackground->SetImage(_backgroundImageNames);
	_lblBackground->SetEnableClick(false);

	_btnSVM->SetImage(_btnSVMImageNames[0], _btnSVMImageNames[1],
					  _btnSVMImageNames[2], _btnSVMImageNames[3]);
	_btnFVC->SetImage(_btnFVCImageNames[0], _btnFVCImageNames[1],
					  _btnFVCImageNames[2], _btnFVCImageNames[3]);
	_btnExitSVM->SetImage(_btnExitImageNames[0], _btnExitImageNames[1]);
	_btnExitFVC->SetImage(_btnExitImageNames[0], _btnExitImageNames[1]);
}

void PassengerScreen::UpdateGeometry()
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");

	_lblTimeH1->setGeometry(_deltaX + _timeH1Rect[0], _deltaY + _timeH1Rect[1],
								 _timeH1Rect[2], _timeH1Rect[3]);
	_lblTimeH2->setGeometry(_deltaX + _timeH2Rect[0], _deltaY + _timeH2Rect[1],
								 _timeH2Rect[2], _timeH2Rect[3]);
	_lblTimeColon->setGeometry(_deltaX + _timeColonRect[0], _deltaY + _timeColonRect[1],
									_timeColonRect[2], _timeColonRect[3]);
	_lblTimeM1->setGeometry(_deltaX + _timeM1Rect[0], _deltaY + _timeM1Rect[1],
								 _timeM1Rect[2], _timeM1Rect[3]);
	_lblTimeM2->setGeometry(_deltaX + _timeM2Rect[0], _deltaY + _timeM2Rect[1],
								 _timeM2Rect[2], _timeM2Rect[3]);
	_lblTimeAM->setGeometry(_deltaX + _timeAMRect[0], _deltaY + _timeAMRect[1],
								 _timeAMRect[2], _timeAMRect[3]);
	_lblTimePM->setGeometry(_lblTimeAM->geometry());
	_lblTimePM->hide();

	_lblIconArea->setGeometry(_iconAreaRect[0], _deltaY * 1.7 + _iconAreaRect[1],
								   _iconAreaRect[2] + _deltaWidth, _iconAreaRect[3]);
	_lblIconArea->lower();

	_lblBackground->setGeometry(QRect(0, 0, this->width(), this->height()));
	_lblBackground->lower();
	_lblBackground->show();
	
	_btnExitSVM->setGeometry(_btnExitRect[0], _btnExitRect[1], _btnExitRect[2], _btnExitRect[3]);
	_btnExitFVC->setGeometry(_btnExitRect[0], _btnExitRect[1], _btnExitRect[2], _btnExitRect[3]);
	_btnExitSVM->hide();
	_btnExitFVC->hide();
}

void PassengerScreen::InitializeSignalSlots(void)
{
	Qt::ConnectionType dbusConnectionType = Qt::QueuedConnection;
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");

	connect(&_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
	connect(_btnSVM, SIGNAL(mouseClicked()),
			this, SLOT(OnClickedSVM()), dbusConnectionType);
	connect(_btnFVC, SIGNAL(mouseClicked()),
			this, SLOT(OnClickedFVC()), dbusConnectionType);
	connect(_btnExitSVM, SIGNAL(mouseClicked()),
			this, SLOT(OnClickedExitSVM()), dbusConnectionType);
	connect(_btnExitFVC, SIGNAL(mouseClicked()),
			this, SLOT(OnClickedExitFVC()), dbusConnectionType);
	
	connect(this, SIGNAL(ShowSVM(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnSVMOn(void)), dbusConnectionType);
	connect(this, SIGNAL(HideSVM(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnSVMOff(void)), dbusConnectionType);
	connect(this, SIGNAL(ShowFVC(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnFVCOn(void)), dbusConnectionType);
	connect(this, SIGNAL(HideFVC(void)),
			LAUNCHER_IF_MANAGER, SLOT(OnFVCOff(void)), dbusConnectionType);
}

void PassengerScreen::UpdateDisplay()
{
	int hour = _currentTime.GetHour();
	int min = _currentTime.GetMinute();
	int index;

	if (hour >= 12)
	{
		_lblTimePM->show();
		_lblTimeAM->hide();
		hour -= 12;
	}
	else
	{
		_lblTimePM->hide();
		_lblTimeAM->show();
	}

	index = hour / 10;
	_lblTimeH1->SetImage(_timeImages[index]);

	index = hour % 10;
	_lblTimeH2->SetImage(_timeImages[index]);

	index = min / 10;
	_lblTimeM1->SetImage(_timeImages[index]);

	index = min % 10;
	_lblTimeM2->SetImage(_timeImages[index]);
	this->repaint();
}

void PassengerScreen::ReplaceIconPosition(void)
{
	TCImageButton *button;
	int regionX, regionWidth;
	int x_offset;
	int count, index;
	int x, y, width, height;

	regionWidth = (APP_AREA_WIDTH + _deltaWidth) / MAX_ICON_COUNT;
	regionX = (regionWidth - APP_BUTTON_WIDTH) / 2;
	x_offset = APP_BUTTON_X_OFFSET;

	count = _btnManager->GetTotalCount();
	index = 0;
	for (int i = 0; i < count; i++)
	{
		button = _btnManager->GetButton(i);
		if (button != NULL)
		{
			x = x_offset + (index % MAX_ICON_COUNT) * regionWidth + regionX;
			y = _deltaY * 1.7 + APP_BUTTON_Y_OFFSET + MULTI_LINE_Y_OFFSET + (MULTI_2ND_LINE_Y_OFFSET * (index / MAX_ICON_COUNT));
			width = APP_BUTTON_WIDTH;
			height = APP_BUTTON_HEIGHT;
			button->setGeometry(x, y, width, height);
			index++;
		}
	}

	_lblIconArea->hide();
}

void PassengerScreen::OnClickedSVM()
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");
	_lblBackground->hide();
	_btnExitSVM->show();
	emit ShowSVM();
}

void PassengerScreen::OnClickedFVC()
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");
	_lblBackground->hide();
	_btnExitFVC->show();
	emit ShowFVC();
}

void PassengerScreen::OnClickedExitSVM()
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");
	_lblBackground->show();
	_btnExitSVM->hide();
	emit HideSVM();
}

void PassengerScreen::OnClickedExitFVC()
{
	DEBUG_LAUNCHER_PASSENGER_SCREEN_PRINTF("\n");
	_lblBackground->show();
	_btnExitFVC->hide();
	emit HideFVC();
}

void PassengerScreen::OnTimer()
{
	_currentTime.SetCurrentTime();
	UpdateDisplay();
}
