#ifndef VPZ_APKSTUDIO_WINDOWS_SETTINGS_HPP
#define VPZ_APKSTUDIO_WINDOWS_SETTINGS_HPP

#include <QBoxLayout>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QFileDialog>
#include <QFontDatabase>
#include <QFormLayout>
#include <QHeaderView>
#include <QLabel>
#include <QLineEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include "components/framework.hpp"
#include "components/tasks.hpp"
#include "helpers/settings.hpp"
#include "helpers/text.hpp"
#include "helpers/ui.hpp"
#include "dialog.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

class Settings : public Dialog
{
    Q_OBJECT
private:
    QListWidget *list;
    QStackedWidget *stack;
private:
    void createApktoolTab();
    void createEditorTab();
    void createGeneralTab();
    void createJavaTab();
    void createViewerTab();
    void fixButtonSize(QPushButton *);
    static QString translate(const char *key) {
        return Helpers::Text::translate("settings", key);
    }
public:
    explicit Settings(QWidget * = 0);
public slots:
    void onInitComplete();
};

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_WINDOWS_SETTINGS_HPP
