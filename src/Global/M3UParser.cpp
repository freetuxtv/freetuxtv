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

#include "M3UParser.h"

#include <QFile>
#include <QTextStream>
#include <QRegularExpression>
#include <QDebug>

int M3UParser::parse(const QString &file, ParseCallback callback, void *userData)
{
    if (!callback) {
        return INVALID_CALLBACK;
    }

    QFile m3uFile(file);
    if (!m3uFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return FILE_NOT_FOUND;
    }

    QTextStream in(&m3uFile);
    QString line;
    QStringList args;
    int num = 0;

    while (!in.atEnd()) {
        line = in.readLine().trimmed();
        
        if (line.isEmpty()) {
            continue;
        }

        if (line.startsWith("#EXTM3U")) {
            // M3U header, continue
            continue;
        } else if (line.startsWith("#EXTINF:")) {
            // Extended info line
            args.clear();
            args.append(line);
        } else if (line.startsWith("#")) {
            // Other extended data
            if (!args.isEmpty()) {
                args.append(line);
            }
        } else {
            // URL line
            if (!line.startsWith("http://") && !line.startsWith("https://") && 
                !line.startsWith("rtmp://") && !line.startsWith("rtsp://") &&
                !line.startsWith("udp://") && !line.startsWith("rtp://") &&
                !line.startsWith("file://")) {
                // Skip non-URL lines
                continue;
            }

            int result = callback(line, num, args, userData);
            if (result != 0) {
                return CALLBACK_RETURN_ERROR;
            }
            
            args.clear();
            num++;
        }
    }

    return OK;
}

int M3UParser::getExtInfo(const QStringList &args, ExtInfo &extInfo)
{
    for (const QString &arg : args) {
        if (arg.startsWith("#EXTINF:")) {
            // Parse #EXTINF:duration,title
            QRegularExpression re(R"(#EXTINF:\s*([^,]*),\s*(.*))");
            QRegularExpressionMatch match = re.match(arg);
            
            if (match.hasMatch()) {
                extInfo.time = match.captured(1).trimmed();
                extInfo.title = match.captured(2).trimmed();
                return OK;
            }
        }
    }
    
    return EXTINFO_NOT_FOUND;
}

int M3UParser::getExtData(const QStringList &args, const QString &extName, ExtData &extData)
{
    extData.name = extName;
    extData.data.clear();

    QString searchPattern = "#" + extName + ":";
    
    for (const QString &arg : args) {
        if (arg.startsWith(searchPattern)) {
            QString data = arg.mid(searchPattern.length()).trimmed();
            extData.data.append(data);
        }
    }

    if (extData.data.isEmpty()) {
        return EXTDATA_NOT_FOUND;
    }

    return OK;
}

QString M3UParser::errorMessage(int error)
{
    switch (error) {
        case OK:
            return "No error";
        case INVALID_CALLBACK:
            return "Invalid callback function";
        case FILE_NOT_FOUND:
            return "File not found";
        case EXTINFO_NOT_FOUND:
            return "Extended info not found";
        case EXTDATA_NOT_FOUND:
            return "Extended data not found";
        case CALLBACK_RETURN_ERROR:
            return "Callback function returned error";
        case INVALID_PARAM:
            return "Invalid parameter";
        default:
            return "Unknown error";
    }
}