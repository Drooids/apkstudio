#include "uninstall.hpp"

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Async {

Uninstall::Uninstall(const QString &device, const QVector<Application> &applications, QObject *parent)
    : Task(parent), applications(applications), device(device)
{
}

void Uninstall::start()
{
    int failed = 0;
    int successful = 0;
    QStringList uninstalled;
    foreach (const Application &application, applications) {
        if (ADB::instance()->uninstall(device, application.package)) {
            uninstalled << application.package;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), uninstalled);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
