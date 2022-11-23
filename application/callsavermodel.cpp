#include "callsavermodel.h"
#include "callsaverthread.h"
#include <QMediaPlayer>
#include <QDir>
#include <QDebug>

CallSaverModel::CallSaverModel(QObject *parent) : QAbstractListModel(parent), m_player(new QMediaPlayer(this)), m_playindex(-1), m_playPosition(0)
{
    connect(m_player, &QMediaPlayer::mediaStatusChanged, [this]() {
        QString file = m_player->media().canonicalUrl().toLocalFile();
        int i = m_files.indexOf(file);
        qDebug() << Q_FUNC_INFO << m_player->mediaStatus() << i << file;

        switch(m_player->mediaStatus()) {
        case QMediaPlayer::LoadedMedia: {
            if (m_playindex != -1) {
                qDebug() << Q_FUNC_INFO;
                m_player->play();
            }
            CallSaverType data;
            data.length = m_player->duration();
            m_data[file] = data;
            emit dataChanged(index(i), index(i));
            [[fallthrough]];
        }
        case QMediaPlayer::InvalidMedia:
            if (m_playindex != -1) {
                qDebug() << Q_FUNC_INFO;
                m_playindex = -1;
                return;
            }
            if (i < m_files.count() - 1)
                m_player->setMedia(QUrl::fromLocalFile(m_files.at(i + 1)));
            break;
        default:
            break;
        }
    });
    connect(m_player, static_cast<void(QMediaPlayer::*)(QMediaPlayer::Error )>(&QMediaPlayer::error), [this]() {
        if (m_playindex != -1) {
            qDebug() << Q_FUNC_INFO;
            m_playindex = -1;
            return;
        }
        QString file = m_player->media().canonicalUrl().toLocalFile();
        int i = m_files.indexOf(file);
        if (i < m_files.count() - 1)
            m_player->setMedia(QUrl::fromLocalFile(m_files.at(i + 1)));
    });
    connect(m_player, &QMediaPlayer::stateChanged, [this]() {
        qDebug() << Q_FUNC_INFO << m_player->state();
    });
    connect(m_player, &QMediaPlayer::positionChanged, [this](qint64 position) {
        int percentage = 0;
        if (position > 0)
            percentage = ((float)position / (float)m_player->duration()) * 100.0;
        if ( percentage != m_playPosition) {
            m_playPosition = percentage;
            emit playPositionChanged();
        }
    });
    QDir dir("/run/media/defaultuser/9016-4EF8", "*.ogg", QDir::Reversed);
    QStringList files;
    for (const QFileInfo& info : dir.entryInfoList()) {
        m_files.append(info.absoluteFilePath());
    }
    if (!m_files.isEmpty())
        m_player->setMedia(QUrl::fromLocalFile(m_files.at(0)));
}

int CallSaverModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return m_files.count();
}

QVariant CallSaverModel::data(const QModelIndex &index, int role) const
{
    switch(role) {
    case NumberRole: {
        QString file = m_files.at(index.row());
        QString basename = file.mid(file.lastIndexOf('/') + 1);
        return basename.mid(16, basename.length() - 20);
        break;
    }
    case LengthRole: {
        int secs = m_data[m_files.at(index.row())].length / 1000;
        int mins = secs / 60;
        return QString("%1:%2").arg(mins).arg(secs - 60 * mins, 2, 10, QLatin1Char('0'));
        break;
    }
    case DateRole: {
        QString file = m_files.at(index.row());
        QString basename = file.mid(file.lastIndexOf('/') + 1);
        return QString("%1.%2.%3 %4:%5").arg(basename.mid(6, 2)).arg(basename.mid(4, 2)).arg(basename.mid(0, 4)).arg(basename.mid(9, 2)).arg(basename.mid(11, 2));
    }
    default:
        return m_files.at(index.row());
    }
}

QHash<int, QByteArray> CallSaverModel::roleNames() const
{
    return {{NumberRole, "number"}, {LengthRole, "length"}, {DateRole, "date"}};
}

void CallSaverModel::play(int index)
{
    m_playindex = index;
    m_player->setMedia(QUrl::fromLocalFile(m_files.at(index)));
}

void CallSaverModel::pauseToggle()
{
    if (m_player->state() == QMediaPlayer::PlayingState)
        m_player->pause();
    else
        m_player->play();
}

void CallSaverModel::seek(int percentage)
{
    qDebug() << Q_FUNC_INFO << percentage;
}

int CallSaverModel::playPosition()
{
    return m_playPosition;
}
