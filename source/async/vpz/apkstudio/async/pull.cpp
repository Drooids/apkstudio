#include "pull.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Pull::Pull(const QString &device, const QMap<QString, bool> &files, const QDir &destination, QObject *parent)
    : Task(parent), files(files), destination(destination), device(device)
{
}

void Pull::start()
{
    int failed = 0;
    int successful = 0;
    foreach(const QString &path, files.keys()) {
        if (ADB::instance()->pull(device, path, destination.absolutePath())) {
            if (files[path])
                successful++;
            else if (destination.exists(path.section('/', -1, -1)))
                successful++;
            else
                failed++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
