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

#include "QProgressDialog.h"

#include <QLabel>
#include <QProgressBar>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

QProgressDialog::QProgressDialog(QWidget *parent)
    : QDialog(parent)
    , m_titleLabel(nullptr)
    , m_textLabel(nullptr)
    , m_progressBar(nullptr)
    , m_closeButton(nullptr)
    , m_layout(nullptr)
{
    setupUI();
    setModal(true);
    setWindowTitle(tr("Progress"));
    resize(400, 150);
}

QProgressDialog::~QProgressDialog()
{
}

void QProgressDialog::setupUI()
{
    m_layout = new QVBoxLayout(this);
    
    // Title label
    m_titleLabel = new QLabel(this);
    m_titleLabel->setAlignment(Qt::AlignCenter);
    QFont titleFont = m_titleLabel->font();
    titleFont.setBold(true);
    titleFont.setPointSize(titleFont.pointSize() + 2);
    m_titleLabel->setFont(titleFont);
    m_layout->addWidget(m_titleLabel);
    
    // Text label
    m_textLabel = new QLabel(this);
    m_textLabel->setAlignment(Qt::AlignCenter);
    m_textLabel->setWordWrap(true);
    m_layout->addWidget(m_textLabel);
    
    // Progress bar
    m_progressBar = new QProgressBar(this);
    m_progressBar->setRange(0, 100);
    m_progressBar->setValue(0);
    m_layout->addWidget(m_progressBar);
    
    // Button layout
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    
    m_closeButton = new QPushButton(tr("Close"), this);
    m_closeButton->setEnabled(false);
    connect(m_closeButton, &QPushButton::clicked, this, &QProgressDialog::onCloseClicked);
    buttonLayout->addWidget(m_closeButton);
    
    buttonLayout->addStretch();
    m_layout->addLayout(buttonLayout);
}

void QProgressDialog::setTitle(const QString &title)
{
    m_titleLabel->setText(title);
    setWindowTitle(title);
}

void QProgressDialog::setText(const QString &text)
{
    m_textLabel->setText(text);
}

void QProgressDialog::setPercent(double percent)
{
    int value = static_cast<int>(percent * 100);
    m_progressBar->setValue(value);
    
    // If progress is complete, enable close button
    if (value >= 100) {
        setButtonCloseEnabled(true);
    }
}

void QProgressDialog::pulse()
{
    // For indeterminate progress, set range to 0,0 to show busy indicator
    if (m_progressBar->minimum() != 0 || m_progressBar->maximum() != 0) {
        m_progressBar->setRange(0, 0);
    }
}

void QProgressDialog::setButtonCloseEnabled(bool enabled)
{
    m_closeButton->setEnabled(enabled);
}

void QProgressDialog::onCloseClicked()
{
    accept();
}