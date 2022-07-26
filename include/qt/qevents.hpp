#ifndef QEVENTS_HPP
#define QEVENTS_HPP
#include <QEvent>

template <typename Ret = void, typename ...Args>
class QCallableEvent : public QEvent
{
public:
    QCallableEvent() : QEvent(QCallableEvent::type())  {}
    static QEvent::Type type(){
        static int generatedType = QEvent::registerEventType();
        return static_cast<QEvent::Type>(generatedType);
    }
    virtual Ret operator()(Args... args) = 0;
};

template <typename Data>
class QDataEvent : public QEvent
{
public:
    QDataEvent(const Data& data) :
        QEvent(QDataEvent::type()),
        mData(data)
    {}
    static QEvent::Type type(){
        static int generatedType = QEvent::registerEventType();
        return static_cast<QEvent::Type>(generatedType);
    }
    const Data& data() const{ return mData; }

private:
    Data mData;
};


#define CUSTOM_QEVENT(EventName)                                  \
    class EventName : public QEvent                               \
{                                                                 \
    public:                                                       \
    EventName() : QEvent(EventName::type()) {}                    \
    static QEvent::Type type(){                                   \
    static int generatedType = QEvent::registerEventType();       \
    return static_cast<QEvent::Type>(generatedType);              \
    }                                                             \
    };


#define CUSTOM_QEVENT_T(EventName, T)                             \
    class EventName : public QDataEvent<T>                        \
{                                                                 \
    public:                                                       \
    EventName(const T& data) : QDataEvent<T>(data) {}             \
    };


#endif // QEVENTS_HPP
