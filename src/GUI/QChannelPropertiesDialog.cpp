//
// Created by ebeuque on 15/02/25.
//

#include "QChannelPropertiesDialog.h"

#include <QDialogButtonBox>
#include <QFileDialog>
#include <QMessageBox>
#include <QPixmap>
#include <QStandardPaths>

QChannelPropertiesDialog::QChannelPropertiesDialog(QWidget* parent)
    : QDialog(parent)
    , m_nameEdit(nullptr)
    , m_uriEdit(nullptr)
    , m_descriptionEdit(nullptr)
    , m_logoLabel(nullptr)
    , m_selectLogoButton(nullptr)
    , m_clearLogoButton(nullptr)
    , m_modified(false)
{
    setupUI();
    setWindowTitle(tr("Channel Properties"));
    setModal(true);
    resize(500, 400);
}

QChannelPropertiesDialog::~QChannelPropertiesDialog()
{
}

void QChannelPropertiesDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Channel information group
    QGroupBox* infoGroup = new QGroupBox(tr("Channel Information"), this);
    QFormLayout* infoLayout = new QFormLayout(infoGroup);
    
    m_nameEdit = new QLineEdit(this);
    infoLayout->addRow(tr("Name:"), m_nameEdit);
    
    m_uriEdit = new QLineEdit(this);
    infoLayout->addRow(tr("URI:"), m_uriEdit);
    
    m_descriptionEdit = new QTextEdit(this);
    m_descriptionEdit->setMaximumHeight(100);
    infoLayout->addRow(tr("Description:"), m_descriptionEdit);
    
    mainLayout->addWidget(infoGroup);
    
    // Logo group
    QGroupBox* logoGroup = new QGroupBox(tr("Channel Logo"), this);
    QVBoxLayout* logoLayout = new QVBoxLayout(logoGroup);
    
    m_logoLabel = new QLabel(this);
    m_logoLabel->setMinimumSize(64, 64);
    m_logoLabel->setMaximumSize(128, 128);
    m_logoLabel->setScaledContents(true);
    m_logoLabel->setStyleSheet("border: 1px solid gray;");
    m_logoLabel->setAlignment(Qt::AlignCenter);
    m_logoLabel->setText(tr("No Logo"));
    
    QHBoxLayout* logoButtonsLayout = new QHBoxLayout();
    m_selectLogoButton = new QPushButton(tr("Select Logo..."), this);
    m_clearLogoButton = new QPushButton(tr("Clear Logo"), this);
    logoButtonsLayout->addWidget(m_selectLogoButton);
    logoButtonsLayout->addWidget(m_clearLogoButton);
    logoButtonsLayout->addStretch();
    
    logoLayout->addWidget(m_logoLabel, 0, Qt::AlignCenter);
    logoLayout->addLayout(logoButtonsLayout);
    
    mainLayout->addWidget(logoGroup);
    
    // Button box
    QDialogButtonBox* buttonBox = new QDialogButtonBox(
        QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    mainLayout->addWidget(buttonBox);
    
    // Connect signals
    connect(m_nameEdit, &QLineEdit::textChanged, this, &QChannelPropertiesDialog::onDataChanged);
    connect(m_uriEdit, &QLineEdit::textChanged, this, &QChannelPropertiesDialog::onDataChanged);
    connect(m_descriptionEdit, &QTextEdit::textChanged, this, &QChannelPropertiesDialog::onDataChanged);
    connect(m_selectLogoButton, &QPushButton::clicked, this, &QChannelPropertiesDialog::onSelectLogo);
    connect(m_clearLogoButton, &QPushButton::clicked, this, &QChannelPropertiesDialog::onClearLogo);
    
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QChannelPropertiesDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
}

void QChannelPropertiesDialog::setChannelInfos(const QSharedPointer<ChannelInfos>& channelInfos)
{
    m_channelInfos = channelInfos;
    m_modified = false;
    loadChannelData();
}

QSharedPointer<ChannelInfos> QChannelPropertiesDialog::getChannelInfos() const
{
    return m_channelInfos;
}

bool QChannelPropertiesDialog::isModified() const
{
    return m_modified;
}

void QChannelPropertiesDialog::loadChannelData()
{
    if (!m_channelInfos) {
        m_nameEdit->clear();
        m_uriEdit->clear();
        m_descriptionEdit->clear();
        m_logoPath.clear();
        updateLogoDisplay();
        return;
    }
    
    // Block signals to avoid triggering onDataChanged
    m_nameEdit->blockSignals(true);
    m_uriEdit->blockSignals(true);
    m_descriptionEdit->blockSignals(true);
    
    m_nameEdit->setText(m_channelInfos->getName());
    m_uriEdit->setText(m_channelInfos->getUrl());
    m_descriptionEdit->setPlainText(""); // Description not available in current model
    
    // TODO: Load logo path from channel infos
    m_logoPath.clear();
    updateLogoDisplay();
    
    // Restore signals
    m_nameEdit->blockSignals(false);
    m_uriEdit->blockSignals(false);
    m_descriptionEdit->blockSignals(false);
    
    m_modified = false;
}

void QChannelPropertiesDialog::updateChannelInfos()
{
    if (!m_channelInfos) {
        return;
    }
    
    m_channelInfos->setName(m_nameEdit->text());
    m_channelInfos->setUrl(m_uriEdit->text());
    // TODO: Set description when available in model
    // TODO: Set logo path when available in model
}

void QChannelPropertiesDialog::updateLogoDisplay()
{
    if (m_logoPath.isEmpty()) {
        m_logoLabel->setText(tr("No Logo"));
        m_logoLabel->setPixmap(QPixmap());
        m_clearLogoButton->setEnabled(false);
    } else {
        QPixmap pixmap(m_logoPath);
        if (!pixmap.isNull()) {
            m_logoLabel->setPixmap(pixmap);
            m_logoLabel->setText("");
            m_clearLogoButton->setEnabled(true);
        } else {
            m_logoLabel->setText(tr("Invalid Logo"));
            m_logoLabel->setPixmap(QPixmap());
            m_clearLogoButton->setEnabled(true);
        }
    }
}

void QChannelPropertiesDialog::onDataChanged()
{
    m_modified = true;
}

void QChannelPropertiesDialog::onSelectLogo()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("Select Channel Logo"),
        QStandardPaths::writableLocation(QStandardPaths::PicturesLocation),
        tr("Image Files (*.png *.jpg *.jpeg *.gif *.bmp *.svg)")
    );
    
    if (!fileName.isEmpty()) {
        m_logoPath = fileName;
        updateLogoDisplay();
        m_modified = true;
    }
}

void QChannelPropertiesDialog::onClearLogo()
{
    m_logoPath.clear();
    updateLogoDisplay();
    m_modified = true;
}

void QChannelPropertiesDialog::accept()
{
    // Validate input
    if (m_nameEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Channel name cannot be empty."));
        m_nameEdit->setFocus();
        return;
    }
    
    if (m_uriEdit->text().trimmed().isEmpty()) {
        QMessageBox::warning(this, tr("Invalid Input"), tr("Channel URI cannot be empty."));
        m_uriEdit->setFocus();
        return;
    }
    
    // Update channel infos
    updateChannelInfos();
    
    QDialog::accept();
}