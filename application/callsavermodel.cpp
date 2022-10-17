#include "callsavermodel.h"
#include "callsaverthread.h"
#include <QMediaPlayer>
#include <QDebug>

CallSaverModel::CallSaverModel(QObject *parent) : QAbstractListModel(parent), m_player(new QMediaPlayer(this))
{
    qDebug() << Q_FUNC_INFO;
    CallSaverThread* thread = new CallSaverThread(this);
    connect(thread, &CallSaverThread::file, [this](const QString& name) {
        qDebug() << name;
        m_files.append(name);
        insertRow(m_files.count() - 1);
    });
    connect(thread, &CallSaverThread::finished, [this]() {
        qDebug() << m_files;
        if (!m_files.isEmpty())
            m_player->setMedia(QUrl::fromLocalFile(m_files.at(0)));
    });
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [this]() {
        qDebug() << m_player->mediaStatus();
        switch(m_player->mediaStatus()) {
        case QMediaPlayer::LoadedMedia: {
            qDebug() << m_player->duration();
            int index = m_files.indexOf(m_player->media().canonicalUrl().toLocalFile());
            qDebug() << index;
            if (index != -1 && index < m_files.count() - 1)
                m_player->setMedia(QUrl::fromLocalFile(m_files.at(index + 1)));
            break;
            }
        case QMediaPlayer::InvalidMedia: {
            int index = m_files.indexOf(m_player->media().canonicalUrl().toLocalFile());
            qDebug() << index;
            beginRemoveRows(QModelIndex(), index, index);
            m_files.removeAt(index);
            endRemoveRows();
            break;
            }
        }
    });
    thread->start();
}

int CallSaverModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_files.count();
}

QVariant CallSaverModel::data(const QModelIndex &index, int role) const
{
    qDebug() << Q_FUNC_INFO << index << role;
    return m_files.at(index.row());
}

QHash<int, QByteArray> CallSaverModel::roleNames() const
{
    qDebug() << Q_FUNC_INFO;
    return {{NumberRole, "number"}};
}
