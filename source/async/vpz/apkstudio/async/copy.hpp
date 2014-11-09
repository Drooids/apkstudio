#ifndef VPZ_APKSTUDIO_ASYNC_COPY_HPP
#define VPZ_APKSTUDIO_ASYNC_COPY_HPP

#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Copy : public Task
{
    Q_OBJECT
private:
    QMap<QString, bool> files;
    QString destination;
    QString device;
public:
    explicit Copy(const QString &, const QMap<QString, bool> &, const QString &, QObject * = 0);
    void start();
signals:
    void finished(QVariant);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_COPY_HPP
