//
// Created by ebeuque on 08/02/25.
//

#ifndef FREETUXTV_RECORDINGINFOS_H
#define FREETUXTV_RECORDINGINFOS_H

#include <QString>
#include <QDateTime>

class RecordingInfos {
public:
    enum Status {
        NotSet = 0,
        Waiting,
        Processing,
        Finished,
        Skipped,
        Error
    };

public:
    RecordingInfos();
    RecordingInfos(const QString& title, const QDateTime& beginTime, const QDateTime& endTime, int channelId);
    virtual ~RecordingInfos();

    void setId(int id);
    int getId() const;

    void setTitle(const QString& title);
    const QString& getTitle() const;

    void setStatus(Status status);
    Status getStatus() const;

    void setBeginTime(const QDateTime& beginTime);
    const QDateTime& getBeginTime() const;

    void setEndTime(const QDateTime& endTime);
    const QDateTime& getEndTime() const;

    void setFileName(const QString& fileName);
    const QString& getFileName() const;

    void setChannelId(int channelId);
    int getChannelId() const;

    bool hasTime(const QDateTime& refTime) const;
    bool isTimeGreater(const QDateTime& refTime) const;

private:
    int m_id;
    QString m_title;
    Status m_status;
    QDateTime m_beginTime;
    QDateTime m_endTime;
    QString m_fileName;
    int m_channelId;
};

#endif //FREETUXTV_RECORDINGINFOS_H
