#include "install.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Install::Install(const QString &device, const QStringList &apks, QObject *parent)
    : Task(parent), apks(apks), device(device)
{
}

void Install::start()
{
    int failed = 0;
    int successful = 0;
    foreach (const QString &apk, apks) {
        if (ADB::instance()->install(device, apk))
            successful++;
        else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(failed, successful)));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
