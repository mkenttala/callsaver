#ifndef DAEMON_H
#define DAEMON_H

#include <QObject>
#include <QDBusConnection>
#include <QString>
#include <QAudioRecorder>

class daemon : public QObject
{
    Q_OBJECT
public:
    explicit daemon(QObject *parent = nullptr);

public slots:
    void statusChanged(const QDBusMessage& msg);
    void lineIdChanged(const QDBusMessage& msg);
    void activeVoiceCallChanged();

signals:

private:
    QDBusConnection m_bus;
    QString m_number;
    QAudioRecorder* m_recorder;
};

#endif // DAEMON_H
