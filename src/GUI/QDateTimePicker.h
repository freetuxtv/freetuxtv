/* -*- Mode: C++; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */
/*
 * FreetuxTV
 * Copyright (C) Eric Beuque 2011 <eric.beuque@gmail.com>
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

#ifndef QDATETIMEPICKER_H
#define QDATETIMEPICKER_H

#include <QWidget>
#include <QDateTime>

class QDateTimeEdit;
class QHBoxLayout;

class QDateTimePicker : public QWidget
{
    Q_OBJECT

public:
    explicit QDateTimePicker(QWidget *parent = nullptr, const QString &dateFormat = QString());
    virtual ~QDateTimePicker();

    void setDateTime(const QDateTime &dateTime);
    QDateTime getDateTime() const;

signals:
    void dateTimeChanged(const QDateTime &dateTime);

private slots:
    void onDateTimeChanged(const QDateTime &dateTime);

private:
    void setupUI();

private:
    QDateTimeEdit *m_dateTimeEdit;
    QHBoxLayout *m_layout;
    QString m_dateFormat;
};

#endif // QDATETIMEPICKER_H