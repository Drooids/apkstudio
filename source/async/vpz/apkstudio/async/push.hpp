#ifndef VPZ_APKSTUDIO_ASYNC_PUSH_HPP
#define VPZ_APKSTUDIO_ASYNC_PUSH_HPP

#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Push : public Task
{
    Q_OBJECT
private:
    QStringList files;
    QString destination;
    QString device;
public:
    explicit Push(const QString &, const QStringList &, const QString &, QObject * = 0);
    void start();
signals:
    void finished(QVariant);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_PUSH_HPP
