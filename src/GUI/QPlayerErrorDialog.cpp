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

#include "QPlayerErrorDialog.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIcon>
#include <QStyle>

#include "Global/Application.h"

QPlayerErrorDialog::QPlayerErrorDialog(QWidget *parent, Application *app)
    : QDialog(parent)
    , m_iconLabel(nullptr)
    , m_messageLabel(nullptr)
    , m_closeButton(nullptr)
    , m_layout(nullptr)
    , m_app(app)
{
    setupUI();
    setModal(true);
    setWindowTitle(tr("Player Error"));
    resize(400, 200);
}

QPlayerErrorDialog::~QPlayerErrorDialog()
{
}

void QPlayerErrorDialog::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    // Create horizontal layout for icon and message
    QHBoxLayout *contentLayout = new QHBoxLayout();
    
    // Error icon
    m_iconLabel = new QLabel(this);
    QIcon errorIcon = style()->standardIcon(QStyle::SP_MessageBoxCritical);
    m_iconLabel->setPixmap(errorIcon.pixmap(48, 48));
    m_iconLabel->setAlignment(Qt::AlignTop);
    contentLayout->addWidget(m_iconLabel);
    
    // Message label
    m_messageLabel = new QLabel(this);
    m_messageLabel->setWordWrap(true);
    m_messageLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    contentLayout->addWidget(m_messageLabel, 1);
    
    m_layout->addLayout(contentLayout);
    
    // Add some spacing
    m_layout->addStretch();
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_closeButton = new QPushButton(tr("Close"), this);
    connect(m_closeButton, &QPushButton::clicked, this, &QPlayerErrorDialog::onCloseClicked);
    buttonLayout->addWidget(m_closeButton);
    
    m_layout->addLayout(buttonLayout);
}

void QPlayerErrorDialog::setMessage(const QString &message)
{
    m_messageLabel->setText(message);
}

void QPlayerErrorDialog::onCloseClicked()
{
    accept();
}