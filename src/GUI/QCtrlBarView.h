//
// Created by ebeuque on 06/10/2020.
//

#ifndef FREETUXTV_QCTRLBARVIEW_H
#define FREETUXTV_QCTRLBARVIEW_H

#include <QWidget>

class QToolButton;
class QSlider;

class QCtrlBarView : public QWidget
{
public:
	QCtrlBarView(QWidget * parent = NULL);
	virtual ~QCtrlBarView();

public:
	// Time row
	QSlider* getSliderTime() const;

	// Ctrl row
	QToolButton* getButtonBackward() const;
	QToolButton* getButtonPlay() const;
	QToolButton* getButtonStop() const;
	QToolButton* getButtonRecord() const;
	QToolButton* getButtonForward() const;
	QSlider* getSliderVolume() const;
	QToolButton* getButtonFullScreen() const;
	QToolButton* getButtonModeMini() const;

private:
	QWidget* createTimeRow(QWidget * parent);
	QWidget* createCtrlRow(QWidget * parent);

private:
	// Time row
	QSlider* m_pSliderTime;

	// Ctrl row
	QToolButton* m_pButtonBackward;
	QToolButton* m_pButtonPlay;
	QToolButton* m_pButtonStop;
	QToolButton* m_pButtonRecord;
	QToolButton* m_pButtonForward;
	QSlider* m_pSliderVolume;
	QToolButton* m_pButtonFullScreen;
	QToolButton* m_pButtonModeMini;
};


#endif //FREETUXTV_QCTRLBARVIEW_H
