/****************************************************************************************
 *   FileName    : PassengerScreen.h
 *   Description : PassengerScreen.h
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

#ifndef PASSENGER_SCREEN_H
#define PASSENGER_SCREEN_H

#include <QWidget>
#include <QTimer>
#include <QList>
#include <QRect>
#include "TCImageButton.h"
#include "TCImageButtonManager.h"
#include "TCTime.h"
#include "TCApplication.h"

class PassengerScreen : public QWidget
{
    Q_OBJECT
	public:
		PassengerScreen(QWidget *parent = 0);
		~PassengerScreen();
		bool Initialize(int x = 0, int y = 0, int width = 1920, int height = 720);

	private:
		void UpdateImages(void);
		void UpdateGeometry();
		void InitializeSignalSlots(void);
		void UpdateDisplay(void);
		void ReplaceIconPosition(void);

	private slots:
		void OnClickedSVM();
		void OnClickedFVC();
		void OnClickedExitSVM();
		void OnClickedExitFVC();
		void OnTimer();
		
	signals:
		void ShowSVM();
		void HideSVM();
		void ShowFVC();
		void HideFVC();

	private:
		TCImageButton			*_lblBackground;
		TCImageButton			*_lblTimeH1;
		TCImageButton			*_lblTimeH2;
		TCImageButton			*_lblTimeM1;
		TCImageButton			*_lblTimeM2;
		TCImageButton			*_lblTimeColon;
		TCImageButton			*_lblTimeAM;
		TCImageButton			*_lblTimePM;
		TCImageButton			*_lblIconArea;
		TCImageButton			*_btnSVM;
		TCImageButton			*_btnFVC;
		TCImageButton			*_btnExitSVM;
		TCImageButton			*_btnExitFVC;
		TCImageButtonManager	*_btnManager;
		TCTime					_currentTime;
		QTimer					_timer;
		QImage					_timeImages[10];

		int						_deltaX;
		int						_deltaY;
		int						_deltaWidth;
};

#endif // PASSENGER_SCREEN_H
