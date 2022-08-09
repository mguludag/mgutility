#ifndef QASYNCOBJECT_HPP
#define QASYNCOBJECT_HPP

#include <QObject>
#include <QThread>
#include <QEvent>

class QAsyncObject : public QObject
{
    Q_OBJECT
public:
    explicit QAsyncObject(QObject *parent = nullptr);
    virtual ~QAsyncObject();

    // QObject interface
    bool event(QEvent *event) override;

private slots:
    virtual void init();

private:
    QThread mThread;
};

inline QAsyncObject::QAsyncObject(QObject *parent) : QObject(parent)
{
    moveToThread(&mThread);
}

inline QAsyncObject::~QAsyncObject(){
    mThread.quit();
    mThread.wait();
}

inline void QAsyncObject::init(){}

inline bool QAsyncObject::event(QEvent *event)
{
    switch (event->type()) {
    case QEvent::ThreadChange:
        connect(&mThread, &QThread::started, this, &QAsyncObject::init);
        mThread.start();
        return true;
    default:
        return QObject::event(event);
    }
}

#endif // QASYNCOBJECT_HPP
