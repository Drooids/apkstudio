#include "move.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Move::Move(const QString &device, const QMap<QString, QString> &files, QObject *parent) :
    Task(parent), device(device), files(files)
{
}

void Move::start()
{
    int failed = 0;
    int successful = 0;
    QStringList moved;
    foreach(const QString &source, files.keys()) {
        if (ADB::instance()->move(device, QStringList(source), files[source])) {
            moved << source;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), moved);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
