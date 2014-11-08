#ifndef VPZ_APKSTUDIO_HELPERS_ASYNC_HPP
#define VPZ_APKSTUDIO_HELPERS_ASYNC_HPP

#include <QThread>
#include <QVariant>
#include "resources/variant.hpp"

namespace VPZ {
namespace APKStudio {
namespace Helpers {

class Async : public QThread
{
    Q_OBJECT
private:
    Resources::Runnable runnable;
protected:
    void run();
public:
    explicit Async(QObject *parent = 0);
    void start(Resources::Runnable);
signals:
    void finished(QVariant);
};

} // namespace Helpers
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_HELPERS_ASYNC_HPP
