#ifndef CALLSAVERMODEL_H
#define CALLSAVERMODEL_H

#include <QAbstractListModel>
#include <QStringList>
#include <QMediaPlayer>

class CallSaverModel : public QAbstractListModel
{
    Q_OBJECT
    Q_ENUMS(CallSaverModelRoles)

public:
    enum CallSaverModelRoles {
        NumberRole = Qt::UserRole + 1
    };

    explicit CallSaverModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:

private:
    QStringList m_files;
    QMediaPlayer* m_player;
};

#endif // CALLSAVERMODEL_H
