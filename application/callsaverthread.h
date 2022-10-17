#ifndef CALLSAVERTHREAD_H
#define CALLSAVERTHREAD_H

#include <QThread>
#include <QString>

class QMediaPlayer;

class CallSaverThread : public QThread
{
    Q_OBJECT
public:
    explicit CallSaverThread(QObject *parent = nullptr);
    virtual void run();

signals:
    void file(const QString& name);

private:
};

#endif // CALLSAVERTHREAD_H
