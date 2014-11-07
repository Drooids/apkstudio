#ifndef VPZ_APKSTUDIO_WINDOWS_APPLICATION_HPP
#define VPZ_APKSTUDIO_WINDOWS_APPLICATION_HPP

#include <QBoxLayout>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QGroupBox>
#include <QLabel>
#include <QLineEdit>
#include "helpers/adb.hpp"
#include "helpers/text.hpp"
#include "resources/embedded.hpp"
#include "dialog.hpp"

namespace VPZ {
namespace APKStudio {
namespace Windows {

class Application : public Dialog
{
    Q_OBJECT
private:
    QString package;
    QString device;
private:
    static QString translate(const char *key) {
        return Helpers::Text::translate("application", key);
    }
public:
    explicit Application(const QString &, const QString &, QWidget * = 0);
public slots:
    void onInitComplete();
signals:
    void detailsReceived(QMap<QString, QString>);
};

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ

#endif // VPZ_APKSTUDIO_WINDOWS_APPLICATION_HPP
