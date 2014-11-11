#ifndef VPZ_APKSTUDIO_WINDOWS_PROJECT_HPP
#define VPZ_APKSTUDIO_WINDOWS_PROJECT_HPP

#include <QBoxLayout>
#include <QCheckBox>
#include <QFileDialog>
#include <QPushButton>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include "components/framework.hpp"
#include "helpers/settings.hpp"
#include "helpers/text.hpp"
#include "helpers/ui.hpp"
#include "resources/embedded.hpp"
#include "dialog.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

class Project : public Dialog
{
    Q_OBJECT
private:
    QLineEdit *apk;
    Components::Framework *framework;
    QCheckBox *java;
    QLineEdit *project_name;
    QLineEdit *project_path;
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
