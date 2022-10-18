#ifndef CALLSAVERMODEL_H
#define CALLSAVERMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QMediaPlayer>
#include <QHash>

class CallSaverModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(CallSaverModelRoles)

public:
    enum CallSaverModelRoles {
        NumberRole = Qt::UserRole + 1,
        LengthRole,
        DateRole
    };
    struct CallSaverType {
        int length;
        QString date;
    };

    explicit CallSaverModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

    Q_INVOKABLE void play(int index);

signals:

private:
    QStringList m_files;
    QMediaPlayer* m_player;
    QHash<QString, CallSaverType> m_data;
    int m_playindex;
};

#endif // CALLSAVERMODEL_H
