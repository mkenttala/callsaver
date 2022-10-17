#include "daemon.h"

#include <QDBusInterface>
#include <QUrl>
#include <QDateTime>
#include <QDebug>

daemon::daemon(QObject *parent) : QObject(parent), m_bus(QDBusConnection::sessionBus()), m_recorder(nullptr)
{
    qDebug() << Q_FUNC_INFO;
    m_bus.connect("org.nemomobile.voicecall", "/calls/active", "org.nemomobile.voicecall.VoiceCall", "statusChanged", this, SLOT(statusChanged(QDBusMessage)));
    m_bus.connect("org.nemomobile.voicecall", "/calls/active", "org.nemomobile.voicecall.VoiceCall", "lineIdChanged", this, SLOT(lineIdChanged(QDBusMessage)));
    m_bus.connect("org.nemomobile.voicecall", "/", "org.nemomobile.voicecall.VoiceCallManager", "activeVoiceCallChanged", this, SLOT(activeVoiceCallChanged()));
}

void daemon::statusChanged(const QDBusMessage& msg)
{
    qDebug() << Q_FUNC_INFO << msg;
    switch (msg.arguments().at(0).toUInt()) {
    case 1:
        if (!m_recorder) {
            m_recorder = new QAudioRecorder(this);
            QAudioEncoderSettings settings;
            settings.setCodec("audio/vorbis");
            settings.setQuality(QMultimedia::LowQuality);
            m_recorder->setAudioSettings(settings);
            m_recorder->setContainerFormat("ogg");
        }
        if (m_recorder->status() != QMediaRecorder::RecordingStatus) {
            QString file("/run/media/defaultuser/9016-4EF8/");
            file.append(QDateTime::currentDateTime().toString("yyyyMMdd_HHmmss_"));
            file.append(m_number);
            file.append(".ogg");
            m_recorder->setOutputLocation(QUrl::fromLocalFile(file));
            m_recorder->record();
        }
        break;
    case 7:
        if (m_recorder && m_recorder->status() == QMediaRecorder::RecordingStatus)
            m_recorder->stop();
        break;
    }
}

void daemon::lineIdChanged(const QDBusMessage& msg)
{
    qDebug() << Q_FUNC_INFO << msg;
    m_number = msg.arguments().at(0).toString();
    //QString callingNr = a.arguments().at(0).toString();
    /*QSqlQuery cq(db::Instance()->getContactsDb() );
        cq.prepare("SELECT contactId FROM PhoneNumbers WHERE normalizedNumber = ?");
        cq.addBindValue(phonenumber::process(callingNr));
        if(!cq.exec()) {
            qDebug() << "error " << cq.lastError().text();
        }

        while(cq.next()) {
            qDebug() << "found existing contact with id " << cq.value("contactId").toInt();
            return ;
        }

    m_db_contacts = QSqlDatabase::addDatabase("QSQLITE", "db_contacts");
    m_db_contacts.setDatabaseName("/home/nemo/.local/share/system/Contacts/qtcontacts-sqlite/contacts.db");
    m_db_contacts.open();
    */
}

void daemon::activeVoiceCallChanged()
{
    qDebug() << Q_FUNC_INFO;
    if (m_recorder && m_recorder->status() == QMediaRecorder::RecordingStatus)
        m_recorder->stop();
}
