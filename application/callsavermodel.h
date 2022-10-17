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
        LengthRole
    };
    struct CallSaverType {
        int length;
    };

    explicit CallSaverModel(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    virtual QHash<int, QByteArray> roleNames() const;

signals:

private:
    QStringList m_files;
    QMediaPlayer* m_player;
    QHash<QString, CallSaverType> m_data;
};

#endif // CALLSAVERMODEL_H
