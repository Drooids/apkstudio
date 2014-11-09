#include "enable.hpp"

using namespace VPZ::APKStudio::Helpers;
using namespace VPZ::APKStudio::Resources;

namespace VPZ {
namespace APKStudio {
namespace Async {

Enable::Enable(const QString &device, const QVector<Application> &applications, const bool state, QObject *parent) :
    Task(parent), applications(applications), device(device), state(state)
{
}

void Enable::start()
{
    int failed = 0;
    int successful = 0;
    QStringList toggled;
    foreach (const Application &application, applications) {
        if (ADB::instance()->enable(device, application.package, state)) {
            successful++;
            toggled << application.package;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), toggled);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
