#ifndef VPZ_APKSTUDIO_ASYNC_UNINSTALL_HPP
#define VPZ_APKSTUDIO_ASYNC_UNINSTALL_HPP

#include <QObject>
#include <QVariant>
#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Uninstall : public Task
{
    Q_OBJECT
private:
    QStringList packages;
    QString device;
public:
    Uninstall(const QString &, const QStringList &, QObject * = 0);
    void start();
signals:
    void finished(QVariant, QStringList);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_UNINSTALL_HPP
