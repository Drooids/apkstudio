#include "project.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

Project::Project(const QString &apk, QWidget *parent) :
    Dialog(parent), apk(apk)
{
    setWindowIcon(::icon("create"));
    setWindowTitle(translate("title_window"));
}

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ
