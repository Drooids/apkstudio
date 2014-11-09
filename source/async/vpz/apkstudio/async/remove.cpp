#include "remove.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Remove::Remove(const QString &device, const QMap<QString, bool> &files, QObject *parent) :
    Task(parent), device(device), files(files)
{
}

void Remove::start()
{
    int failed = 0;
    int successful = 0;
    QStringList removed;
    foreach(const QString &file, files.keys()) {
        if (ADB::instance()->remove(device, file, files[file])) {
            removed << file;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), removed);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
