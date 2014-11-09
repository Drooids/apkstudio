#ifndef VPZ_APKSTUDIO_ASYNC_RENAME_HPP
#define VPZ_APKSTUDIO_ASYNC_RENAME_HPP

#include "helpers/adb.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Rename : public Task
{
    Q_OBJECT
private:
    QString device;
    QMap<QString, QString> files;
public:
    explicit Rename(const QString &, const QMap<QString, QString> &, QObject * = 0);
    void start();
signals:
    void finished(QVariant, QStringList);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_RENAME_HPP
