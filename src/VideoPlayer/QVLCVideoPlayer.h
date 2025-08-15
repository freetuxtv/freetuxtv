//
// Created by ebeuque on 15/02/25.
//

#ifndef FREETUXTV_VIDEOPLAYER_H
#define FREETUXTV_VIDEOPLAYER_H

#include <vlc/vlc.h>

#include <QWidget>
#ifdef Q_WS_X11
#include <QX11EmbedContainer>
#endif

class QVBoxLayout;
class QPushButton;
class QTimer;
class QFrame;
class QSlider;

class QVLCVideoPlayer : public QWidget
{
	Q_OBJECT
public:
	QVLCVideoPlayer(QWidget* parent);
	virtual ~QVLCVideoPlayer();

public:
	//void raise(libvlc_exception_t * ex);

public slots:
	void playMRL(const QString& szMrl);
	void updateInterface();
	void changeVolume(int newVolume);

private:
	QFrame* m_pVideoWidget;
	QTimer *poller;
	bool _isPlaying;
	//libvlc_exception_t _vlcexcep;
	libvlc_instance_t *_vlcinstance;
	libvlc_media_player_t *_mp;
	libvlc_media_t *_m;
};

#endif //FREETUXTV_VIDEOPLAYER_H
