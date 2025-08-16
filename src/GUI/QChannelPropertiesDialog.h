//
// Created by ebeuque on 15/02/25.
//

#ifndef FREETUXTV_GUI_QCHANNELPROPERTIESDIALOG_H
#define FREETUXTV_GUI_QCHANNELPROPERTIESDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QTextEdit>
#include <QLabel>
#include <QPixmap>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QGroupBox>
#include <QSharedPointer>

#include "Model/ChannelInfos.h"
#include "Global/QError.h"

class QChannelPropertiesDialog : public QDialog
{
    Q_OBJECT

public:
    explicit QChannelPropertiesDialog(QWidget* parent = nullptr);
    virtual ~QChannelPropertiesDialog();

    void setChannelInfos(const QSharedPointer<ChannelInfos>& channelInfos);
    QSharedPointer<ChannelInfos> getChannelInfos() const;

    bool isModified() const;

public slots:
    void accept() override;

private slots:
    void onDataChanged();
    void onSelectLogo();
    void onClearLogo();

private:
    void setupUI();
    void updateChannelInfos();
    void loadChannelData();
    void updateLogoDisplay();

private:
    // UI elements
    QLineEdit* m_nameEdit;
    QLineEdit* m_uriEdit;
    QTextEdit* m_descriptionEdit;
    QLabel* m_logoLabel;
    QPushButton* m_selectLogoButton;
    QPushButton* m_clearLogoButton;
    
    // Data
    QSharedPointer<ChannelInfos> m_channelInfos;
    QString m_logoPath;
    bool m_modified;
};

#endif //FREETUXTV_GUI_QCHANNELPROPERTIESDIALOG_H