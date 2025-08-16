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

#include "QDateTimePicker.h"

#include <QDateTimeEdit>
#include <QHBoxLayout>

QDateTimePicker::QDateTimePicker(QWidget *parent, const QString &dateFormat)
    : QWidget(parent)
    , m_dateTimeEdit(nullptr)
    , m_layout(nullptr)
    , m_dateFormat(dateFormat)
{
    setupUI();
}

QDateTimePicker::~QDateTimePicker()
{
}

void QDateTimePicker::setupUI()
{
    m_layout = new QHBoxLayout(this);
    m_layout->setContentsMargins(0, 0, 0, 0);
    
    m_dateTimeEdit = new QDateTimeEdit(this);
    m_dateTimeEdit->setCalendarPopup(true);
    m_dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    
    // Set display format if provided
    if (!m_dateFormat.isEmpty()) {
        m_dateTimeEdit->setDisplayFormat(m_dateFormat);
    } else {
        // Default format: "yyyy-MM-dd hh:mm:ss"
        m_dateTimeEdit->setDisplayFormat("yyyy-MM-dd hh:mm:ss");
    }
    
    connect(m_dateTimeEdit, &QDateTimeEdit::dateTimeChanged,
            this, &QDateTimePicker::onDateTimeChanged);
    
    m_layout->addWidget(m_dateTimeEdit);
}

void QDateTimePicker::setDateTime(const QDateTime &dateTime)
{
    m_dateTimeEdit->setDateTime(dateTime);
}

QDateTime QDateTimePicker::getDateTime() const
{
    return m_dateTimeEdit->dateTime();
}

void QDateTimePicker::onDateTimeChanged(const QDateTime &dateTime)
{
    emit dateTimeChanged(dateTime);
}