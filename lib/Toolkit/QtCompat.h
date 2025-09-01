/*
 * QtCompat.h
 *
 *  Created on: 21 oct. 2014
 *      Author: ebeuque
 */

#ifndef TOOLKIT_QTCORE_QTCOMPAT_H_
#define TOOLKIT_QTCORE_QTCOMPAT_H_

#include <QtGlobal>

// Added in Qt 5.0.0
#if QT_VERSION >= QT_VERSION_CHECK(5,0,0)
#define USE_QTRANSLATORCONTEXT
#define USE_QTMESSAGEHANDLER
#define USE_QTENGINEDEFAULT
#define USE_QTURLQUERY
#define USE_CUSTOM_HTTHEADERS
#define USE_QTCONCURRENT_HEADER
#define USE_HEADER_VIEW_SECTION_RESIZE_MODE
#define USE_COMPAT_QFTP
#define USE_NOT_TRANSLUCENT_BG
#define USE_QWINDOW
#define USE_CLEAR_ACCESS_CACHE
#define USE_TABWIDGET_TABBAR
#define USE_ANGLE_DELTA
#define USE_SUBJECTINFOLIST
#define USE_ISSELFSIGNED
#define USE_GUIAPPLICATION
#define USE_QJSONDOCUMENT

#define fromAscii fromLatin1
#define toAscii toLatin1

#define qsocketdesc qintptr

#else // Qt 5.0.0

#define qsocketdesc int

#endif // QT_VERSION

#if QT_VERSION >= QT_VERSION_CHECK(5,1,0)
#define USE_MENU_ADD_SECTION
#define USE_CERT_CHAIN
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,2,0)
#define USE_QMESSAGEBOX_CHECKBOX
#endif

// Added in Qt 5.5.0
#if QT_VERSION >= QT_VERSION_CHECK(5,5,0)
#define USE_QTASPRINTF
#define USE_QTINFOMSG
#define USE_QSENATIVEERRORCODE
#endif

// Added in Qt 5.9.0
#if QT_VERSION >= QT_VERSION_CHECK(5,9,0)
#define USE_QCA_SENDPOSTEDEVENTS
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5,10,0)
#define USE_QPTETABSTOPDISTANCE
#define USE_QFILEINFO_BIRTHTIME
#endif

// Added in Qt 5.11
#if QT_VERSION >= QT_VERSION_CHECK(5,11,0)
#define USE_QTFMHORIZONTALADVANCE
#define USE_QUUID_STRING_FORMAT
#endif

// Added in Qt 5.14.0
#if QT_VERSION >= QT_VERSION_CHECK(5,14,0)
#define USE_NO_QSORT
#define USE_NO_QSTRING_ENUM
#define USE_WHEEL_POSTIION
#define USE_QDATE_STARTOFDAY
#define USE_QTENDL
#endif

// Added in Qt 5.15.0
#if QT_VERSION >= QT_VERSION_CHECK(5,15,0)
#define USE_QRANDOMGENERATOR
#define USE_QWHEEL_NEW
#define USE_QNO_BY_STR_APPEND
#define USE_NO_QTLINKEDLIST
#define USE_QSQLERROR_NATIVEERRORCODE
#define USE_QFILEDIALOG_SETOPTION
#define USE_QPRINTERSETPAGESIZE
#define USE_QPAGELAYOUT_PAINTRECTPIXELS
#define USE_QTNETWORKREQUEST_TRANSFERT_TIMEOUT
#define USE_NO_SYSTEM_LOCALE_SHORT_DATE
#endif

// Added in Qt 6.0.0
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define USE_NO_QREGEXP
#define USE_NO_QIODEVICE_OPEN_MODE
#define USE_NO_QTEXTSTREAM_SET_CODEC
#define USE_NO_QDATETIME_TIME_T
#define USE_NO_QSTRINGREF
#define USE_QCHECKSUM_QBYTEARRAYVIEW
#define USE_QSIZETYPE
#define USE_QMETATYPE
#define USE_QICONENGINEV2_MERGED
#define USE_QICONENGINE_LESS_HOOKS // qt5 -> https://doc.qt.io/qt-5/qiconengine.html#IconEngineHook-enum
								   // vs
								   // qt6 -> https://doc.qt.io/qt-6/qiconengine.html#IconEngineHook-enum
#define USE_QLAYOUT_CONTENTS_MARGINS
#define USE_QMOUSE_POSITION
#define USE_QHOVER_POSITION
#define USE_NO_X11_EXTRA
#define USE_NO_VIEW_OPTIONS
#define USE_QRECURSIVEMUTEX
#define USE_NO_QDESKTOPWIDGET // Deprecated since 5.0.0 and removed from 6.0.0
#define USE_NEW_AUDIO_API
#define USE_NEW_OPENGL_API
#define USE_QLIBRARYINFO_PATH
#define USE_QENTEREVENT
#define USE_NO_INICODEC
#endif


#if QT_VERSION >= QT_VERSION_CHECK(6,5,0)
#define USE_PARSE_RESULT
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6,4,0)
#define USE_QCOLOR_FROMSTRING
#endif

// Rename open mode enum
#ifdef USE_NO_QIODEVICE_OPEN_MODE
#define QIODeviceDefault QIODeviceBase
#else
#define QIODeviceDefault QIODevice
#endif

#ifdef USE_QTASPRINTF
#define QStringASPrintf(str, format, ...) str = QString::asprintf(format, __VA_ARGS__)
#else
#define QStringASPrintf(str, format, ...) str.sprintf(format, __VA_ARGS__)
#endif


#endif /* TOOLKIT_QTCORE_QTCOMPAT_H_ */
