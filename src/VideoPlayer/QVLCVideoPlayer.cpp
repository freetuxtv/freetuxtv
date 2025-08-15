//
// Created by ebeuque on 15/02/25.
//

//#include <QX11EmbedContainer>
#include <QVBoxLayout>
#include <QPushButton>
#include <QSlider>
#include <QTimer>
#include <QFrame>

#include "QVLCVideoPlayer.h"

#define POSITION_RESOLUTION 10000

QVLCVideoPlayer::QVLCVideoPlayer(QWidget* parent) : QWidget(parent)
{
	setStyleSheet("background-color:black;");
	//preparation of the vlc command
	const char * const vlc_args[] = {
			"--verbose=2", //be much more verbose then normal for debugging purpose
	};

	m_pVideoWidget= new QFrame(this);
	// [20101215 JG] If KDE is used like unique desktop environment, only use _videoWidget=new QFrame(this);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addWidget(m_pVideoWidget);
	setLayout(layout);

	_isPlaying=false;
	poller=new QTimer(this);

	//Initialize an instance of vlc
	//a structure for the exception is neede for this initalization
	//libvlc_exception_init(&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.

	//create a new libvlc instance
	_vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args);  //tricky calculation of the char space used
	//_vlcinstance=libvlc_new(sizeof(vlc_args) / sizeof(vlc_args[0]), vlc_args,&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	//raise (&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.

	// Create a media player playing environement
	_mp = libvlc_media_player_new (_vlcinstance);
	//_mp = libvlc_media_player_new (_vlcinstance, &_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	//raise (&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.

	//connect the two sliders to the corresponding slots (uses Qt's signal / slots technology)
	connect(poller, SIGNAL(timeout()), this, SLOT(updateInterface()));

	poller->start(100); //start timer to trigger every 100 ms the updateInterface slot
}

QVLCVideoPlayer::~QVLCVideoPlayer()
{
	libvlc_media_player_stop (_mp);

	libvlc_media_player_release (_mp);

	libvlc_release (_vlcinstance);
}

void QVLCVideoPlayer::playMRL(const QString& szMrl)
{
	qDebug("[VideoPlayer] Playing %s", qPrintable(szMrl));

	/* Create a new LibVLC media descriptor */
	_m = libvlc_media_new_location(_vlcinstance, szMrl.toLatin1());
	//_m = libvlc_media_new (_vlcinstance, file.toAscii(), &_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	//raise(&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.

	libvlc_media_player_set_media (_mp, _m);
	//libvlc_media_player_set_media (_mp, _m, &_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	//raise(&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.

	// /!\ Please note /!\
    //
	// passing the widget to the lib shows vlc at which position it should show up
	// vlc automatically resizes the video to the ´given size of the widget
	// and it even resizes it, if the size changes at the playing

	// Get our media instance to use our window
#if defined(Q_OS_WIN)
	libvlc_media_player_set_drawable(_mp, reinterpret_cast<unsigned int>(_videoWidget->winId()));
#elif defined(Q_OS_MAC)
	libvlc_media_player_set_drawable(_mp, _videoWidget->winId());
#else //Linux
	int windid = m_pVideoWidget->winId();
	libvlc_media_player_set_xwindow (_mp, windid);
#endif

	libvlc_media_player_play (_mp);

	_isPlaying=true;
}

void QVLCVideoPlayer::changeVolume(int newVolume)
{
	libvlc_audio_set_volume (_mp,newVolume);
}

void QVLCVideoPlayer::updateInterface()
{
	if(!_isPlaying)
		return;

	// It's possible that the vlc doesn't play anything
	// so check before
	libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp);
	//libvlc_media_t *curMedia = libvlc_media_player_get_media (_mp, &_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	//libvlc_exception_clear(&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	if (curMedia == NULL)
		return;

	int volume = libvlc_audio_get_volume (_mp);
	//int volume=libvlc_audio_get_volume (_vlcinstance,&_vlcexcep); // [20101215 JG] Used for versions prior to VLC 1.2.0.
	
	// Emit signal to update volume on interface
	emit volumeChanged(volume);
}