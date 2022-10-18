#include "callsaverthread.h"
#include <QDir>
#include <QFileInfo>
#include <QDebug>

CallSaverThread::CallSaverThread(QObject *parent) : QThread(parent)
{
}

void CallSaverThread::run()
{
    QDir dir("/run/media/defaultuser/9016-4EF8", "*.ogg", QDir::Reversed);
    QStringList files;
    for (const QFileInfo& info : dir.entryInfoList()) {
        emit file(info.absoluteFilePath());
    }
    exit();
}
