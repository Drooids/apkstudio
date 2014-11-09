#include "move.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Move::Move(const QString &device, const QStringList &files, const QString &destination, QObject *parent) :
    Task(parent), destination(destination), device(device), files(files)
{
}

void Move::start()
{
    int failed = 0;
    int successful = 0;
    QStringList moved;
    foreach(const QString &file, files) {
        if (ADB::instance()->move(device, file, destination)) {
            moved << file;
            successful++;
        } else
            failed++;
    }
    emit finished(QVariant::fromValue(QPair<int, int>(successful, failed)), moved);
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
