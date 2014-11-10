#ifndef VPZ_APKSTUDIO_ASYNC_FRAMEWORK_HPP
#define VPZ_APKSTUDIO_ASYNC_FRAMEWORK_HPP

#include "helpers/apktool.hpp"
#include "task.hpp"

namespace VPZ {
namespace APKStudio {
namespace Async {

class Framework : public Task
{
    Q_OBJECT
private:
    QString apk;
    QString tag;
public:
    explicit Framework(const QString &, const QString &, QObject * = 0);
    void start();
signals:
    void finished(QVariant);
};

} // namespace Async
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_ASYNC_FRAMEWORK_HPP
