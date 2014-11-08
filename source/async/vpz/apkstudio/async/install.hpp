#ifndef VPZ_APKSTUDIO_ASYNC_INSTALL_HPP
#define VPZ_APKSTUDIO_ASYNC_INSTALL_HPP

#include <QObject>
#include <QVariant>
#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Install : public Task
{
    Q_OBJECT
private:
    QStringList apks;
    QString device;
public:
    Install(const QString &, const QStringList &, QObject * = 0);
    void start();
signals:
    void finished(QVariant);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_INSTALL_HPP
