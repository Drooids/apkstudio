#ifndef VPZ_APKSTUDIO_ASYNC_REMOVE_HPP
#define VPZ_APKSTUDIO_ASYNC_REMOVE_HPP

#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Remove : public Task
{
    Q_OBJECT
private:
    QMap<QString, bool> files;
    QString device;
public:
    explicit Remove(const QString &, const QMap<QString, bool> &, QObject * = 0);
    void start();
signals:
    void finished(QVariant, QStringList);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_REMOVE_HPP
