#ifndef VPZ_APKSTUDIO_ASYNC_ENABLE_HPP
#define VPZ_APKSTUDIO_ASYNC_ENABLE_HPP

#include "helpers/adb.hpp"
#include "resources/variant.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Enable : public Task
{
    Q_OBJECT
private:
    QVector<Resources::Application> applications;
    QString device;
    bool state;
public:
    explicit Enable(const QString &, const QVector<Resources::Application> &, const bool, QObject * = 0);
    void start();
signals:
    void finished(QVariant, QStringList);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_ENABLE_HPP
