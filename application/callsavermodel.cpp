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
        emit beginInsertRows(index(m_files.count()), m_files.count(), m_files.count());
        emit endInsertRows();
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
            QString file = m_player->media().canonicalUrl().toLocalFile();
            qDebug() << file << m_player->duration();
            CallSaverType data;
            data.length = m_player->duration();
            m_data[file] = data;

            int i = m_files.indexOf(file);
            qDebug() << i;
            emit dataChanged(index(i), index(i));

            if (i != -1 && i < m_files.count() - 1)
                m_player->setMedia(QUrl::fromLocalFile(m_files.at(i + 1)));
            break;
            }
        case QMediaPlayer::InvalidMedia: {
            int i = m_files.indexOf(m_player->media().canonicalUrl().toLocalFile());
            qDebug() << i << m_files.count();
            m_files.removeAt(i);
            beginRemoveRows(index(i), i, i);
            endRemoveRows();
            qDebug() << m_files.count();
            break;
            }
        }
    });
    thread->start();
}

int CallSaverModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    qDebug() << Q_FUNC_INFO << m_files.count();
    return m_files.count();
}

QVariant CallSaverModel::data(const QModelIndex &index, int role) const
{
    qDebug() << Q_FUNC_INFO << index << role;
    switch(role) {
    case NumberRole: {
        QString file = m_files.at(index.row());
        QString basename = file.mid(file.lastIndexOf('/'));
        return basename.mid(17, basename.length() - 21);
        break;
    }
    case LengthRole: {
        int secs = m_data[m_files.at(index.row())].length / 1000;
        int mins = secs / 60;
        return QString("%1:%2").arg(mins).arg(secs - 60 * mins, 2, 10, QLatin1Char('0'));
        break;
    }
    default:
        return m_files.at(index.row());
    }
}

QHash<int, QByteArray> CallSaverModel::roleNames() const
{
    qDebug() << Q_FUNC_INFO;
    return {{NumberRole, "number"}, {LengthRole, "length"}};
}
