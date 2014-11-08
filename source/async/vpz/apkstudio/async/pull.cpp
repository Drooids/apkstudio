#include "pull.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Pull::Pull(const QString &device, const QStringList &paths, const QDir &destination, QObject *parent)
    : Task(parent), paths(paths), destination(destination), device(device)
{
}

void Pull::start()
{
    int failed = 0;
    int successful = 0;
    foreach (const QString &path, paths) {
        if (ADB::instance()->pull(device, path, destination.absolutePath())) {
            if (QFile::exists(destination.absoluteFilePath(path.section('/', -1, -1))))
                successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(failed, successful)));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
