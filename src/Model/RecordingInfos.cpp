//
// Created by ebeuque on 08/02/25.
//

#include "RecordingInfos.h"

RecordingInfos::RecordingInfos()
    : m_id(-1)
    , m_status(NotSet)
    , m_channelId(-1)
{
}

RecordingInfos::RecordingInfos(const QString& title, const QDateTime& beginTime, const QDateTime& endTime, int channelId)
    : m_id(-1)
    , m_title(title)
    , m_status(NotSet)
    , m_beginTime(beginTime)
    , m_endTime(endTime)
    , m_channelId(channelId)
{
}

RecordingInfos::~RecordingInfos()
{
}

void RecordingInfos::setId(int id)
{
    m_id = id;
}

int RecordingInfos::getId() const
{
    return m_id;
}

void RecordingInfos::setTitle(const QString& title)
{
    m_title = title;
}

const QString& RecordingInfos::getTitle() const
{
    return m_title;
}

void RecordingInfos::setStatus(Status status)
{
    m_status = status;
}

RecordingInfos::Status RecordingInfos::getStatus() const
{
    return m_status;
}

void RecordingInfos::setBeginTime(const QDateTime& beginTime)
{
    m_beginTime = beginTime;
}

const QDateTime& RecordingInfos::getBeginTime() const
{
    return m_beginTime;
}

void RecordingInfos::setEndTime(const QDateTime& endTime)
{
    m_endTime = endTime;
}

const QDateTime& RecordingInfos::getEndTime() const
{
    return m_endTime;
}

void RecordingInfos::setFileName(const QString& fileName)
{
    m_fileName = fileName;
}

const QString& RecordingInfos::getFileName() const
{
    return m_fileName;
}

void RecordingInfos::setChannelId(int channelId)
{
    m_channelId = channelId;
}

int RecordingInfos::getChannelId() const
{
    return m_channelId;
}

bool RecordingInfos::hasTime(const QDateTime& refTime) const
{
    return refTime >= m_beginTime && refTime <= m_endTime;
}

bool RecordingInfos::isTimeGreater(const QDateTime& refTime) const
{
    return m_beginTime > refTime;
}
