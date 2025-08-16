//
// Created by ebeuque on 15/02/25.
//

#include "QRecordingsList.h"

#include <QHeaderView>
#include <QStandardItem>
#include <QDateTime>

QRecordingsList::QRecordingsList(QWidget* parent)
    : QWidget(parent)
    , m_tabWidget(nullptr)
    , m_inProgressView(nullptr)
    , m_terminatedView(nullptr)
    , m_model(nullptr)
    , m_inProgressFilter(nullptr)
    , m_terminatedFilter(nullptr)
    , m_dbInstance(nullptr)
{
    setupUI();
    setupModel();
}

QRecordingsList::~QRecordingsList()
{
}

void QRecordingsList::setupUI()
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    
    m_tabWidget = new QTabWidget(this);
    layout->addWidget(m_tabWidget);
    
    // In Progress tab
    m_inProgressView = new QTreeView();
    m_inProgressView->setRootIsDecorated(false);
    m_inProgressView->setAlternatingRowColors(true);
    m_inProgressView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_inProgressView->setSortingEnabled(true);
    m_tabWidget->addTab(m_inProgressView, tr("In Progress"));
    
    // Terminated tab
    m_terminatedView = new QTreeView();
    m_terminatedView->setRootIsDecorated(false);
    m_terminatedView->setAlternatingRowColors(true);
    m_terminatedView->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_terminatedView->setSortingEnabled(true);
    m_tabWidget->addTab(m_terminatedView, tr("Terminated"));
    
    // Connect signals
    connect(m_inProgressView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &QRecordingsList::onRecordingSelectionChanged);
    connect(m_terminatedView->selectionModel(), &QItemSelectionModel::currentChanged,
            this, &QRecordingsList::onRecordingSelectionChanged);
    
    connect(m_inProgressView, &QTreeView::doubleClicked,
            this, &QRecordingsList::onRecordingDoubleClicked);
    connect(m_terminatedView, &QTreeView::doubleClicked,
            this, &QRecordingsList::onRecordingDoubleClicked);
}

void QRecordingsList::setupModel()
{
    // Create main model
    m_model = new QStandardItemModel(this);
    m_model->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Status") 
                                                     << tr("Begin Time") << tr("End Time") 
                                                     << tr("Channel"));
    
    // Create filter models
    m_inProgressFilter = new QSortFilterProxyModel(this);
    m_inProgressFilter->setSourceModel(m_model);
    m_inProgressFilter->setFilterRole(Qt::UserRole + 1); // Status role
    m_inProgressFilter->setFilterFixedString("InProgress");
    
    m_terminatedFilter = new QSortFilterProxyModel(this);
    m_terminatedFilter->setSourceModel(m_model);
    m_terminatedFilter->setFilterRole(Qt::UserRole + 1); // Status role
    m_terminatedFilter->setFilterFixedString("Terminated");
    
    // Set models to views
    m_inProgressView->setModel(m_inProgressFilter);
    m_terminatedView->setModel(m_terminatedFilter);
    
    // Configure headers
    m_inProgressView->header()->setStretchLastSection(true);
    m_terminatedView->header()->setStretchLastSection(true);
}

void QRecordingsList::setDatabaseInstance(DatabaseInstance* dbInstance)
{
    m_dbInstance = dbInstance;
}

void QRecordingsList::loadRecordings(QError& error)
{
    // TODO: Implement database loading
    // For now, clear the model
    m_model->clear();
    m_model->setHorizontalHeaderLabels(QStringList() << tr("Title") << tr("Status") 
                                                     << tr("Begin Time") << tr("End Time") 
                                                     << tr("Channel"));
    m_recordings.clear();
}

void QRecordingsList::refreshRecordings()
{
    QError error;
    loadRecordings(error);
}

void QRecordingsList::updateRecordingStatus(QError& error)
{
    // TODO: Implement status update from database
}

void QRecordingsList::addRecording(const QSharedPointer<RecordingInfos>& recording, QError& error)
{
    if (!recording) {
        return;
    }
    
    m_recordings.append(recording);
    addRecordingToModel(recording);
}

void QRecordingsList::addRecordingToModel(const QSharedPointer<RecordingInfos>& recording)
{
    QList<QStandardItem*> items;
    
    // Title
    QStandardItem* titleItem = new QStandardItem(recording->getTitle());
    titleItem->setData(QVariant::fromValue(recording), Qt::UserRole); // Store recording pointer
    items.append(titleItem);
    
    // Status
    QString statusText;
    QString statusFilter;
    switch (recording->getStatus()) {
        case RecordingInfos::Waiting:
            statusText = tr("Waiting");
            statusFilter = "InProgress";
            break;
        case RecordingInfos::Processing:
            statusText = tr("Processing");
            statusFilter = "InProgress";
            break;
        case RecordingInfos::Finished:
            statusText = tr("Finished");
            statusFilter = "Terminated";
            break;
        case RecordingInfos::Skipped:
            statusText = tr("Skipped");
            statusFilter = "Terminated";
            break;
        case RecordingInfos::Error:
            statusText = tr("Error");
            statusFilter = "Terminated";
            break;
        default:
            statusText = tr("Unknown");
            statusFilter = "InProgress";
            break;
    }
    
    QStandardItem* statusItem = new QStandardItem(statusText);
    statusItem->setData(statusFilter, Qt::UserRole + 1); // Status filter role
    items.append(statusItem);
    
    // Begin Time
    QStandardItem* beginItem = new QStandardItem(recording->getBeginTime().toString("yyyy-MM-dd hh:mm"));
    items.append(beginItem);
    
    // End Time
    QStandardItem* endItem = new QStandardItem(recording->getEndTime().toString("yyyy-MM-dd hh:mm"));
    items.append(endItem);
    
    // Channel (placeholder)
    QStandardItem* channelItem = new QStandardItem(QString("Channel %1").arg(recording->getChannelId()));
    items.append(channelItem);
    
    m_model->appendRow(items);
}

QSharedPointer<RecordingInfos> QRecordingsList::getRecordingFromIndex(const QModelIndex& index)
{
    if (!index.isValid()) {
        return QSharedPointer<RecordingInfos>();
    }
    
    // Get the source index if this is from a proxy model
    QModelIndex sourceIndex = index;
    const QSortFilterProxyModel* proxyModel = qobject_cast<const QSortFilterProxyModel*>(index.model());
    if (proxyModel) {
        sourceIndex = proxyModel->mapToSource(index);
    }
    
    // Get the recording from the first column
    QModelIndex titleIndex = m_model->index(sourceIndex.row(), 0);
    QVariant data = m_model->data(titleIndex, Qt::UserRole);
    return data.value<QSharedPointer<RecordingInfos>>();
}

void QRecordingsList::onRecordingSelectionChanged()
{
    QTreeView* view = qobject_cast<QTreeView*>(sender()->parent());
    if (!view) {
        return;
    }
    
    QModelIndex current = view->currentIndex();
    QSharedPointer<RecordingInfos> recording = getRecordingFromIndex(current);
    
    if (recording) {
        emit recordingSelected(recording);
    }
}

void QRecordingsList::onRecordingDoubleClicked(const QModelIndex& index)
{
    QSharedPointer<RecordingInfos> recording = getRecordingFromIndex(index);
    
    if (recording) {
        emit recordingDoubleClicked(recording);
    }
}