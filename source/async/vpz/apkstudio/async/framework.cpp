#include "framework.hpp"

using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Async {

Framework::Framework(const QString &apk, const QString &tag, QObject *parent) :
    Task(parent), apk(apk), tag(tag)
{
}

void Framework::start()
{
    emit finished(apktool::instance()->install(apk, tag));
}

} // namespace Async
} // namespace APKStudio
} // namespace VPZ
