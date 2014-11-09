#include "copy.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Copy::Copy(const QString &device, const QMap<QString, bool> &files, const QString &destination, QObject *parent) :
    Task(parent), destination(destination), device(device), files(files)
{
}

void Copy::start()
{
    int failed = 0;
    int successful = 0;
    foreach(const QString &path, files.keys()) {
        if (ADB::instance()->copy(device, path, destination, files[path]))
            successful++;
        else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
