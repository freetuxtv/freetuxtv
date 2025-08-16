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

#ifndef QPROGRESSDIALOG_H
#define QPROGRESSDIALOG_H

#include <QDialog>

class QLabel;
class QProgressBar;
class QPushButton;
class QVBoxLayout;

class QProgressDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QProgressDialog(QWidget *parent = nullptr);
    virtual ~QProgressDialog();

    void setTitle(const QString &title);
    void setText(const QString &text);
    void setPercent(double percent);
    void pulse();
    void setButtonCloseEnabled(bool enabled);

private slots:
    void onCloseClicked();

private:
    void setupUI();

private:
    QLabel *m_titleLabel;
    QLabel *m_textLabel;
    QProgressBar *m_progressBar;
    QPushButton *m_closeButton;
    QVBoxLayout *m_layout;
};

#endif // QPROGRESSDIALOG_H