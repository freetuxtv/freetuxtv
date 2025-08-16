//
// Created by ebeuque on 15/02/25.
//

#ifndef FREETUXTV_GUI_QRECORDINGSLIST_H
#define FREETUXTV_GUI_QRECORDINGSLIST_H

#include <QWidget>
#include <QTreeView>
#include <QStandardItemModel>
#include <QSortFilterProxyModel>
#include <QTabWidget>
#include <QVBoxLayout>
#include <QSharedPointer>

#include "Model/RecordingInfos.h"
#include "Database/DatabaseInstance.h"
#include "Global/QError.h"

Q_DECLARE_METATYPE(QSharedPointer<RecordingInfos>)

class QRecordingsList : public QWidget
{
    Q_OBJECT

public:
    explicit QRecordingsList(QWidget* parent = nullptr);
    virtual ~QRecordingsList();

    void setDatabaseInstance(DatabaseInstance* dbInstance);
    void loadRecordings(QError& error);
    void refreshRecordings();
    void updateRecordingStatus(QError& error);
    void addRecording(const QSharedPointer<RecordingInfos>& recording, QError& error);

signals:
    void recordingSelected(const QSharedPointer<RecordingInfos>& recording);
    void recordingDoubleClicked(const QSharedPointer<RecordingInfos>& recording);

private slots:
    void onRecordingSelectionChanged();
    void onRecordingDoubleClicked(const QModelIndex& index);

private:
    void setupUI();
    void setupModel();
    QSharedPointer<RecordingInfos> getRecordingFromIndex(const QModelIndex& index);
    void addRecordingToModel(const QSharedPointer<RecordingInfos>& recording);

private:
    QTabWidget* m_tabWidget;
    QTreeView* m_inProgressView;
    QTreeView* m_terminatedView;
    
    QStandardItemModel* m_model;
    QSortFilterProxyModel* m_inProgressFilter;
    QSortFilterProxyModel* m_terminatedFilter;
    
    DatabaseInstance* m_dbInstance;
    QList<QSharedPointer<RecordingInfos>> m_recordings;
};

#endif //FREETUXTV_GUI_QRECORDINGSLIST_H