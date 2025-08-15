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

#ifndef FREETUXTV_UTILS_H
#define FREETUXTV_UTILS_H

#include <QString>
#include <QDateTime>

class Application;

class Utils
{
public:
    // Time formatting functions
    static QString formatTime(int seconds);
    static QString formatTime2(int seconds);
    
    // Size formatting functions
    static QString formatSize(qint64 size);
    
    // Recording options
    static QString getRecordingOptions(Application *app, const QString &baseFilename, bool fromPreview, QString *outFilename = nullptr);
    
    // Date/time utility functions
    static qint64 dateTimeToInt64(const QDateTime &dateTime);
    static QString dateTimeToString(const QDateTime &dateTime, const QString &format);
    static QDateTime stringToDateTime(const QString &strTime, const QString &format);
    static QDateTime addSeconds(const QDateTime &dateTime, int seconds);
    
    // String utility functions
    static QString removeDiacritics(const QString &str);
    
    // Debug functions
    static void printDateTime(const QDateTime &dateTime);

private:
    Utils(); // Static class, no instantiation
};

#endif // FREETUXTV_UTILS_H