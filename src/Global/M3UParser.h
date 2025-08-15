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

#ifndef FREETUXTV_M3UPARSER_H
#define FREETUXTV_M3UPARSER_H

#include <QString>
#include <QStringList>
#include <functional>

class M3UParser
{
public:
    enum Error {
        OK = 0,
        INVALID_CALLBACK = -1,
        FILE_NOT_FOUND = -2,
        EXTINFO_NOT_FOUND = -3,
        EXTDATA_NOT_FOUND = -4,
        CALLBACK_RETURN_ERROR = -5,
        INVALID_PARAM = -6
    };

    struct ExtInfo {
        QString time;
        QString title;
    };

    struct ExtData {
        QString name;
        QStringList data;
    };

    // Callback function type: url, num, args, user_data -> return error code
    using ParseCallback = std::function<int(const QString &url, int num, const QStringList &args, void *userData)>;

public:
    static int parse(const QString &file, ParseCallback callback, void *userData = nullptr);
    
    static int getExtInfo(const QStringList &args, ExtInfo &extInfo);
    
    static int getExtData(const QStringList &args, const QString &extName, ExtData &extData);
    
    static QString errorMessage(int error);

private:
    M3UParser(); // Static class, no instantiation
};

#endif // FREETUXTV_M3UPARSER_H