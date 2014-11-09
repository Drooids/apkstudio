#include "push.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Push::Push(const QString &device, const QStringList &files, const QString &destination, QObject *parent) :
    Task(parent), destination(destination), device(device), files(files)
{
}

void Push::start()
{
    int failed = 0;
    int successful = 0;
    foreach(const QString &file, files) {
        if (ADB::instance()->push(device, file, destination))
            successful++;
        else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
