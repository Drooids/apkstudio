#ifndef VPZ_APKSTUDIO_WINDOWS_PROJECT_HPP
#define VPZ_APKSTUDIO_WINDOWS_PROJECT_HPP

#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "dialog.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

class Project : public Dialog
{
    Q_OBJECT
private:
    QString apk;
private:
    static QString translate(const char *key) {
        return Helpers::Text::translate("project", key);
    }
public:
    explicit Project(const QString &, QWidget * = 0);
};

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_WINDOWS_PROJECT_HPP
