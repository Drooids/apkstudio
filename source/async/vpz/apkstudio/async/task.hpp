#ifndef VPZ_APKSTUDIO_ASYNC_TASK_HPP
#define VPZ_APKSTUDIO_ASYNC_TASK_HPP

#include <QObject>
#include <QRunnable>
#include <QVariant>

namespace VPZ {
namespace APKStudio {
namespace Async {

class Task : public QObject, public QRunnable
{
    Q_OBJECT
public:
    explicit Task(QObject * = 0);
    void run();
    virtual void start() = 0;
signals:
    void started();
    void stopped();
};


} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_TASK_HPP
