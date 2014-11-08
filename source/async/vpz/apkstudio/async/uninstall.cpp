#include "uninstall.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Uninstall::Uninstall(const QString &device, const QStringList &packages, QObject *parent)
    : Task(parent), packages(packages), device(device)
{
}

void Uninstall::start()
{
    int failed = 0;
    int successful = 0;
    QStringList uninstalled;
    foreach (const QString &package, packages) {
        if (ADB::instance()->uninstall(device, package)) {
            uninstalled << package;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(failed, successful)), uninstalled);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
