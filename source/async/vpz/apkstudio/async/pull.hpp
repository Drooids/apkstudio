#ifndef VPZ_APKSTUDIO_ASYNC_PULL_HPP
#define VPZ_APKSTUDIO_ASYNC_PULL_HPP

#include <QDir>
#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Pull : public Task
{
    Q_OBJECT
private:
    QDir destination;
    QString device;
    QMap<QString, bool> files;
public:
    Pull(const QString &, const QMap<QString, bool> &, const QDir &, QObject * = 0);
    void start();
signals:
    void finished(QVariant);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_PULL_HPP
