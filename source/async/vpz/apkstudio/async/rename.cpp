#include "rename.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Rename::Rename(const QString &device, const QMap<QString, QString> &files, QObject *parent) :
    Task(parent), device(device), files(files)
{
}

void Rename::start()
{
    int failed = 0;
    int successful = 0;
    QStringList renamed;
    foreach(const QString &file, files.keys()) {
        if (ADB::instance()->move(device, file, files[file])) {
            renamed << file;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), renamed);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
