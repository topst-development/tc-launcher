/****************************************************************************************
 *   FileName    : NowPlayRadio.h
 *   Description : NowPlayRadio.h
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

#ifndef NOWPLAY_RADIO_H
#define NOWPLAY_RADIO_H

#include <QObject>
#include <QQuickItem>
#include "MediaPlayerType.h"

#define PRESET_COUNT	6

class NowPlayRadio : public QQuickItem
{
	Q_OBJECT

	Q_PROPERTY(QString frequency MEMBER _frequency NOTIFY changedFrequency)
	Q_PROPERTY(int type READ type NOTIFY changedType)
	Q_PROPERTY(bool hdRadio MEMBER _hdRadio NOTIFY changedHDRadio)
	Q_PROPERTY(QStringList presets READ presets NOTIFY changedPresets)

	public:
		NowPlayRadio(QQuickItem *parent = 0);
		void Initialize(void);
		void ResetInformation(void);
		void TurnOnRadio(void);
		bool SetRadioType(int type);
		void SetRadioFrequency(int freq);
		void SetRadioPreset(int type, int freq, int idx);
		void SetRadioService(QString service);
		void SetRadioServicePreset(int type, QString service, int idx);
		void SetHDRadio(int status);

		int type() const;
		QStringList presets() const;

		Q_INVOKABLE void OnClickedRadioType(void);
		Q_INVOKABLE void OnClickedPreset(int idx);

	private:
		void InitializeSignalSlots(void);
		QString GetTimeString(unsigned char hour, unsigned char min, unsigned char sec);

	signals:
		void ChangeRadioType(void);
		void SelectRadioPreset(int idx);
		void RadioTurnOn(void);

		void changedFrequency(void);
		void changedType(void);
		void changedHDRadio(void);
		void changedPresets(void);

	private:
		QString				_frequency;
		int					_channel;
		bool				_hdRadio;
		RadioTypes			_type;
		QStringList			_presets[TotalRadioTypes];
};

#endif // NOWPLAY_RADIO_H
