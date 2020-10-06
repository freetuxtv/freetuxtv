//
// Created by ebeuque on 06/10/2020.
//


#include <QVBoxLayout>
#include <QLabel>
#include <QToolButton>
#include <QSlider>
#include <QtGui/QIconDragEvent>

#include "QCtrlBarView.h"

QCtrlBarView::QCtrlBarView(QWidget * parent)
{
	QBoxLayout* pMainLayout;

	pMainLayout = new QVBoxLayout();
	pMainLayout->setContentsMargins(0,0,0,0);
	setLayout(pMainLayout);

	QWidget* pTmpWidget;

	pTmpWidget = createTimeRow(parent);
	pMainLayout->addWidget(pTmpWidget);

	pTmpWidget = createCtrlRow(parent);
	pMainLayout->addWidget(pTmpWidget);
}

QCtrlBarView::~QCtrlBarView()
{

}

QWidget* QCtrlBarView::createTimeRow(QWidget * parent)
{
	QWidget* pMainWidget = new QWidget(parent);

	QBoxLayout* pMainLayout = new QHBoxLayout();
	pMainLayout->setContentsMargins(0,0,0,0);
	pMainWidget->setLayout(pMainLayout);

	pMainLayout->addWidget(new QLabel(tr("00:00:00")));

	m_pSliderTime = new QSlider();
	m_pSliderTime->setOrientation(Qt::Horizontal);
	pMainLayout->addWidget(m_pSliderTime);

	pMainLayout->addWidget(new QLabel(tr("00:00:00")));

	return pMainWidget;
}

QWidget* QCtrlBarView::createCtrlRow(QWidget * parent)
{
	QWidget* pMainWidget = new QWidget(parent);

	QBoxLayout* pMainLayout = new QHBoxLayout();
	pMainLayout->setContentsMargins(0,0,0,0);
	pMainWidget->setLayout(pMainLayout);

	m_pButtonBackward = new QToolButton(pMainWidget);
	m_pButtonBackward->setIcon(QIcon::fromTheme("media-seek-backward"));
	pMainLayout->addWidget(m_pButtonBackward);

	m_pButtonPlay = new QToolButton(pMainWidget);
	m_pButtonPlay->setIcon(QIcon::fromTheme("media-playback-start"));
	pMainLayout->addWidget(m_pButtonPlay);

	m_pButtonStop = new QToolButton(pMainWidget);
	m_pButtonStop->setIcon(QIcon::fromTheme("media-playback-stop"));
	pMainLayout->addWidget(m_pButtonStop);

	m_pButtonRecord = new QToolButton(pMainWidget);
	m_pButtonRecord->setIcon(QIcon::fromTheme("media-record"));
	pMainLayout->addWidget(m_pButtonRecord);

	m_pButtonForward = new QToolButton(pMainWidget);
	m_pButtonForward->setIcon(QIcon::fromTheme("media-seek-forward"));
	pMainLayout->addWidget(m_pButtonForward);

	pMainLayout->addWidget(new QLabel(tr("Vol: -")));

	m_pSliderVolume = new QSlider();
	m_pSliderVolume->setOrientation(Qt::Horizontal);
	pMainLayout->addWidget(m_pSliderVolume);

	pMainLayout->addWidget(new QLabel(tr("+")));

	m_pButtonFullScreen = new QToolButton(pMainWidget);
	m_pButtonFullScreen->setIcon(QIcon::fromTheme("view-fullscreen"));
	pMainLayout->addWidget(m_pButtonFullScreen);

	m_pButtonModeMini = new QToolButton(pMainWidget);
	m_pButtonModeMini->setIcon(QIcon::fromTheme("view-restore"));
	pMainLayout->addWidget(m_pButtonModeMini);

	pMainLayout->addStretch();


	return pMainWidget;
}

QSlider* QCtrlBarView::getSliderTime() const
{
	return m_pSliderTime;
}

QToolButton* QCtrlBarView::getButtonBackward() const
{
	return m_pButtonBackward;
}

QToolButton* QCtrlBarView::getButtonPlay() const
{
	return m_pButtonPlay;
}

QToolButton* QCtrlBarView::getButtonStop() const
{
	return m_pButtonStop;
}

QToolButton* QCtrlBarView::getButtonRecord() const
{
	return m_pButtonRecord;
}

QToolButton* QCtrlBarView::getButtonForward() const
{
	return m_pButtonForward;
}

QSlider* QCtrlBarView::getSliderVolume() const
{
	return m_pSliderVolume;
}

QToolButton* QCtrlBarView::getButtonFullScreen() const
{
	return m_pButtonFullScreen;
}

QToolButton* QCtrlBarView::getButtonModeMini() const
{
	return m_pButtonModeMini;
}