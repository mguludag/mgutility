#ifndef DARKLIGHTDETECTOR_HPP
#define DARKLIGHTDETECTOR_HPP
#include <QtGlobal>
#ifdef Q_OS_WINDOWS

#include <QSettings>
#include <QBasicTimer>

class DarkLightDetector : public QObject
{
    Q_OBJECT
public:
    DarkLightDetector();
    ~DarkLightDetector();

signals:
    void notify(bool isDark);

protected:
    void timerEvent(QTimerEvent *event) override;

private:
    const QString mRegLocation;
    bool mIsDark;
    QBasicTimer mTimer;
    QSettings mSettings;
};

inline DarkLightDetector::DarkLightDetector() :
    mRegLocation("HKEY_CURRENT_USER\\SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Themes\\Personalize"),
    mIsDark(false), mSettings(mRegLocation, QSettings::NativeFormat)
{
    mTimer.start(500, Qt::VeryCoarseTimer, this);
}

inline DarkLightDetector::~DarkLightDetector()
{
    mTimer.stop();
}

inline void DarkLightDetector::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    auto result = mSettings.value("AppsUseLightTheme", true).toBool();
    if(result == mIsDark){
        mIsDark = !result;
        emit notify(mIsDark);
    }
}

#endif // Q_OS_WINDOWS
#endif // DARKLIGHTDETECTOR_HPP
