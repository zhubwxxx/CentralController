#ifndef QTBASE_H
#define QTBASE_H

#include <QtCore>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <error.h>
#include <termios.h>
#include <fcntl.h>
#include <errno.h>
#include <assert.h>

#include "util.h"

#define debug(fmt, arg...) qDebug(fmt, ##arg)
#define Debug qDebug()
#define MY_TAG_DEBUG {             \
    qDebug() << __FUNCTION__ << __LINE__; \
}

#define ASSERT(x)  assert(x)

#define MEMORY_FREE(a)              \
        do {                        \
             if(a)                  \
             {                      \
                delete a;           \
                a = NULL;           \
             }                      \
        } while(0)

#define DEVICE_A_NAME        "APF"
#define DEVICE_B_NAME        "battery"
enum DataSignEnum{
    G_UNSIGNED_S,
    G_SIGNED_S,
};
enum DataTypeEnum{
    G_INI_T,
    G_FLOAT_1BIT_T,
    G_FLOAT_2BIT_T,
    G_FLOAT_3BIT_T,

};

/*********************************************************************/

/*
 * For create thread-safe singleton we must first write our call_once() implementation:
 */
namespace CallOnce {
    enum ECallOnce {
        CO_Request,
        CO_InProgress,
        CO_Finished
    };

    Q_GLOBAL_STATIC(QThreadStorage<QAtomicInt*>, once_flag)
}

template <class Function>
inline static void qCallOnce(Function func, QBasicAtomicInt& flag)
{
    using namespace CallOnce;

#if QT_VERSION < 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag);
#elif QT_VERSION >= 0x050000
    int protectFlag = flag.fetchAndStoreAcquire(flag.load());
#endif

    if (protectFlag == CO_Finished)
        return;
    if (protectFlag == CO_Request && flag.testAndSetRelaxed(protectFlag,
                                                           CO_InProgress)) {
        func();
        flag.fetchAndStoreRelease(CO_Finished);
    }
    else {
        do {
            QThread::yieldCurrentThread();
        }
        while (!flag.testAndSetAcquire(CO_Finished, CO_Finished));
    }
}

template <class Function>
inline static void qCallOncePerThread(Function func)
{
    using namespace CallOnce;
    if (!once_flag()->hasLocalData()) {
        once_flag()->setLocalData(new QAtomicInt(CO_Request));
        qCallOnce(func, *once_flag()->localData());
    }
}
/* For create thread-safe singleton we must first write our call_once() implementation:*/



/*单例*/
template <class T>
class Singleton
{
public:
    static T& instance()
    {
        qCallOnce(init, flag);
        return *tptr;
    }
    static void init()
    {
        tptr.reset(new T);
    }
private:
    Singleton() {}
    ~Singleton() {}
    Q_DISABLE_COPY(Singleton)
    static QScopedPointer<T> tptr;
    static QBasicAtomicInt flag;
};
template<class T> QScopedPointer<T> Singleton<T>::tptr(0);
template<class T> QBasicAtomicInt Singleton<T>::flag = Q_BASIC_ATOMIC_INITIALIZER(CallOnce::CO_Request);
/*单例*/


/*工作线程类*/
namespace task {

template<class T>
class Worker : public QThread
{
public:
    typedef void (T::*callBack)();

    Worker(T* object, callBack fun): pointT(object),
                                         _fun(fun),
                                         _running(false),
                                         _stopped(false),
                                         _mutex(QMutex::NonRecursive)
    {

    }
    ~Worker()
    {

    }

    void msleep(unsigned long microseconds)
    {
        QThread::msleep(microseconds);
    }

    void sleep(unsigned long sec)
    {
        QThread::sleep(sec);
    }

    bool isRunning() const
    {
        return _running;
    }

    bool isStopped() const
    {
        return _stopped;
    }

    void start()
    {
        QMutexLocker locker(&_mutex);
        if (!_running) {
            try {
                QThread::start();
                _running = true;
                _stopped = false;
            }
            catch(...) {
                _running = false;
                throw;
            }
        }
    }

    void stop()
    {
        _stopped = true;
        _running = false;
        this->wait(2000);
        QMutexLocker locker(&_mutex);
    }

protected:

    void run()
    {
        QMutexLocker locker(&_mutex);
        //_fun();
        try {
            (pointT->*_fun)();
        }
        catch (...){
            _running = false;
            throw;
        }
        _running = false;
    }

private:

    T* pointT;
    callBack _fun;

    volatile bool _running;
    volatile bool _stopped;

    QMutex _mutex;
};
}
/*工作线程类*/

#if 0
#include <tr1/functional>
#include <tr1/memory>

#if __cplusplus <= 199711L
  #error This library needs at least a C++11 compliant compiler
#endif

using namespace std;
using namespace tr1;

typedef enum {
    TH_SUCCESS,
    TH_ATTR_INIT_ERR,
    TH_ATTR_SET_ERR,
    TH_CREATE_ERR,

} THreadErrStu;

#if 0
template<class T>
class Thread
{
public:
    typedef void (T::*callBack)();
    Thread(T* object, callBack fun): pointT(object),
        _fun(fun),
        _isRuning(false) {}

public:
    void stop()
    {
        if (!_isRuning) {
            return;
        }

        _isRuning = false;
        pthread_join(_tid, NULL);
    }
    int start()
    {
        int ret;
        pthread_attr_t attr;

        if (_isRuning)
            ret = false;
        else {
            ret = pthread_attr_init(&attr);
            if (ret != 0) {
                ret = TH_ATTR_INIT_ERR;
                goto err;
            }

            ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            if (ret != 0) {
                ret = TH_ATTR_SET_ERR;
                goto err;
            }

            ret = pthread_create(&_tid, &attr, threadFunc, this);
            if (ret != 0) {
                ret = TH_CREATE_ERR;
                goto err;
            }
        }

        return ret;
    err:
        perror("Thread::start():");
        return ret;
    }

private:
    static void* threadFunc(void* arg)
    {
        Thread* thread = static_cast<Thread*> (arg);
        T* t = thread->pointT;
        thread->_isRuning = true;
        while (thread->_isRuning) {
            (t->*_fun)();
        }
        thread->_isRuning = false;
        return 0;
    }

private:
    T* pointT;
    callBack _fun;

    pthread_t _tid;
    volatile bool _isRuning;
};
//class Task
//{
//public:
//    Task() : worker(this, &Task::doWork)
//    { worker.start();}

//private:
//    Thread<Task> worker;
//    void doWork();
//};
#else
class Thread
{
public:
    typedef tr1::function<void(void)> callBack;
    Thread(callBack fun):
        _fun(fun),
        _isRuning(false) {}
public:
/*    void stop()
    {
        if (!_isRuning) {
            return;
        }

        _isRuning = false;
        pthread_join(_tid, NULL);
    }
*/
    int start()
    {
        int ret;
        pthread_attr_t attr;

        if (_isRuning)
            ret = false;
        else {
            ret = pthread_attr_init(&attr);
            if (ret != 0) {
                ret = TH_ATTR_INIT_ERR;
                goto err;
            }

            ret = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
            if (ret != 0) {
                ret = TH_ATTR_SET_ERR;
                goto err;
            }

            ret = pthread_create(&_tid, &attr, threadFunc, this);
            if (ret != 0) {
                ret = TH_CREATE_ERR;
                goto err;
            }
        }

        return ret;
    err:
        perror("Thread::start():");
        return ret;
    }
private:
    static void* threadFunc(void* arg)
    {
        Thread* thread = static_cast<Thread*> (arg);
        thread->_isRuning = true;
        thread->_fun();
        thread->_isRuning = false;
        return 0;
    }
private:
    callBack _fun;
    pthread_t _tid;
    volatile bool _isRuning;
};
#endif
#endif



/*********************************************************************/
const QString print_level[] = {
    "DEBUG    ",
    "INFO     ",
    "WARNING  ",
    "ERROR    ",
    "NONE     "
};
const QString print_level_temp[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR",
    "NONE"
};
class Loger
{

public:
    enum PRINT_LEVEL {
        DEBUG_LOG,
        INFO_LOG,
        WARNING_LOG,
        ERROR_LOG,
        NONE_LOG
    };

    Loger(): log_level(DEBUG_LOG)
    {}
    ~Loger(){}

    int log_level;
    static Loger& instance()
    {
        return Singleton<Loger>::instance();
    }

    void console(QString msg, PRINT_LEVEL level = DEBUG_LOG)
    {
        if (log_level <= level) {
            QDateTime dateTime = QDateTime::currentDateTime();
            QString date = dateTime.toString("yyyy-MM-dd hh:mm:ss ");
            QString strType = print_level[level];
            QString str = date+strType;

            str.append(msg);
            Debug << str;
        }
    }
};
/*********************************************************************/


#endif // QTBASE_H
