/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * FreetuxTV
 * Copyright (C) Eric Beuque 2010 <eric.beuque@gmail.com>
 * 
 * FreetuxTV is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * FreetuxTV is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "Utils.h"

#include <QObject>
#include <QDebug>
#include <QDir>
#include <QStandardPaths>

#include "Global/Application.h"

QString Utils::formatTime(int seconds)
{
    const int s = seconds % 60;
    const int m = ((seconds - s) / 60) % 60;
    const int h = (seconds - m * 60 - s) / (60 * 60);

    return QObject::tr("%1h%2m%3s")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

QString Utils::formatTime2(int seconds)
{
    const int s = seconds % 60;
    const int m = ((seconds - s) / 60) % 60;
    const int h = (seconds - m * 60 - s) / (60 * 60);

    return QObject::tr("%1:%2:%3")
        .arg(h, 2, 10, QChar('0'))
        .arg(m, 2, 10, QChar('0'))
        .arg(s, 2, 10, QChar('0'));
}

QString Utils::formatSize(qint64 size)
{
    if (size < 1) {
        return QObject::tr("%1 byte").arg(size);
    } else if (size < 1000) {
        return QObject::tr("%1 bytes").arg(size);
    } else if (size < 1000000) {
        return QObject::tr("%1 kB").arg(size / 1000.0, 0, 'f', 1);
    } else if (size < 1000000000) {
        return QObject::tr("%1 MB").arg(size / 1000000.0, 0, 'f', 1);
    } else {
        return QObject::tr("%1 GB").arg(size / 1000000000.0, 0, 'f', 1);
    }
}

QString Utils::getRecordingOptions(Application *app, const QString &baseFilename, 
                                 bool fromPreview, QString *outFilename)
{
    Q_UNUSED(app)
    Q_UNUSED(fromPreview)
    
    // TODO: Implement recording options based on preferences
    // This is a placeholder implementation
    QString dir = QStandardPaths::writableLocation(QStandardPaths::MoviesLocation);
    if (dir.isEmpty()) {
        dir = QDir::homePath();
    }
    
    QString filename = baseFilename;
    if (!filename.endsWith(".ts")) {
        filename += ".ts";
    }
    
    QString fullPath = QDir(dir).absoluteFilePath(filename);
    
    if (outFilename) {
        *outFilename = fullPath;
    }
    
    // Basic VLC recording options
    return QString("--intf dummy --sout '#std{access=file,mux=ts,dst=%1}'").arg(fullPath);
}

qint64 Utils::dateTimeToInt64(const QDateTime &dateTime)
{
    return dateTime.toMSecsSinceEpoch();
}

QString Utils::dateTimeToString(const QDateTime &dateTime, const QString &format)
{
    return dateTime.toString(format);
}

QDateTime Utils::stringToDateTime(const QString &strTime, const QString &format)
{
    return QDateTime::fromString(strTime, format);
}

QDateTime Utils::addSeconds(const QDateTime &dateTime, int seconds)
{
    return dateTime.addSecs(seconds);
}

QString Utils::removeDiacritics(const QString &str)
{
    // Basic implementation - could be enhanced with proper Unicode normalization
    QString result = str;
    
    // Simple character replacements for common diacritics
    result.replace(QChar(0x00C0), 'A'); // À
    result.replace(QChar(0x00C1), 'A'); // Á
    result.replace(QChar(0x00C2), 'A'); // Â
    result.replace(QChar(0x00C3), 'A'); // Ã
    result.replace(QChar(0x00C4), 'A'); // Ä
    result.replace(QChar(0x00C5), 'A'); // Å
    result.replace(QChar(0x00E0), 'a'); // à
    result.replace(QChar(0x00E1), 'a'); // á
    result.replace(QChar(0x00E2), 'a'); // â
    result.replace(QChar(0x00E3), 'a'); // ã
    result.replace(QChar(0x00E4), 'a'); // ä
    result.replace(QChar(0x00E5), 'a'); // å
    
    result.replace(QChar(0x00C8), 'E'); // È
    result.replace(QChar(0x00C9), 'E'); // É
    result.replace(QChar(0x00CA), 'E'); // Ê
    result.replace(QChar(0x00CB), 'E'); // Ë
    result.replace(QChar(0x00E8), 'e'); // è
    result.replace(QChar(0x00E9), 'e'); // é
    result.replace(QChar(0x00EA), 'e'); // ê
    result.replace(QChar(0x00EB), 'e'); // ë
    
    result.replace(QChar(0x00CC), 'I'); // Ì
    result.replace(QChar(0x00CD), 'I'); // Í
    result.replace(QChar(0x00CE), 'I'); // Î
    result.replace(QChar(0x00CF), 'I'); // Ï
    result.replace(QChar(0x00EC), 'i'); // ì
    result.replace(QChar(0x00ED), 'i'); // í
    result.replace(QChar(0x00EE), 'i'); // î
    result.replace(QChar(0x00EF), 'i'); // ï
    
    result.replace(QChar(0x00D2), 'O'); // Ò
    result.replace(QChar(0x00D3), 'O'); // Ó
    result.replace(QChar(0x00D4), 'O'); // Ô
    result.replace(QChar(0x00D5), 'O'); // Õ
    result.replace(QChar(0x00D6), 'O'); // Ö
    result.replace(QChar(0x00F2), 'o'); // ò
    result.replace(QChar(0x00F3), 'o'); // ó
    result.replace(QChar(0x00F4), 'o'); // ô
    result.replace(QChar(0x00F5), 'o'); // õ
    result.replace(QChar(0x00F6), 'o'); // ö
    
    result.replace(QChar(0x00D9), 'U'); // Ù
    result.replace(QChar(0x00DA), 'U'); // Ú
    result.replace(QChar(0x00DB), 'U'); // Û
    result.replace(QChar(0x00DC), 'U'); // Ü
    result.replace(QChar(0x00F9), 'u'); // ù
    result.replace(QChar(0x00FA), 'u'); // ú
    result.replace(QChar(0x00FB), 'u'); // û
    result.replace(QChar(0x00FC), 'u'); // ü
    
    result.replace(QChar(0x00C7), 'C'); // Ç
    result.replace(QChar(0x00E7), 'c'); // ç
    
    result.replace(QChar(0x00D1), 'N'); // Ñ
    result.replace(QChar(0x00F1), 'n'); // ñ
    
    return result;
}

void Utils::printDateTime(const QDateTime &dateTime)
{
    qDebug() << "DateTime:" << dateTime.toString(Qt::ISODate);
}