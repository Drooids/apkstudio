#include "project.hpp"

using namespace VPZ::APKStudio::Components;

namespace VPZ {
namespace APKStudio {
namespace Windows {

Project::Project(const QString &apk, QWidget *parent) :
    Dialog(parent)
{
    this->apk = new QLineEdit(apk, this);
    framework = new Framework(this);
    java = new QCheckBox(this);
    project_name = new QLineEdit(this);
    project_path = new QLineEdit(this);
    QPushButton *browse = new QPushButton(translate("button_browse"), this);
    QDialogButtonBox *buttons = new QDialogButtonBox(this);
    QFormLayout *form = new QFormLayout;
    QVBoxLayout *layout = new QVBoxLayout(this);
    this->apk->setReadOnly(true);
    buttons->addButton(translate("button_decompile"), QDialogButtonBox::AcceptRole);
    buttons->addButton(translate("button_cancel"), QDialogButtonBox::RejectRole);
    connections.append(connect(browse, &QPushButton::clicked, [ this ] () {
        QString path = QFileDialog::getExistingDirectory(this, translate("title_browse"), Helpers::Settings::previousDirectory());
        if (path.isEmpty())
            return;
        Helpers::Settings::previousDirectory(path);
        this->project_path->setText(path);
    }));
    form->addRow(translate("label_apk"), this->apk);
    form->addRow(translate("label_project_name"), project_name);
    form->addRow(translate("label_project_path"), project_path);
    form->addRow("", browse);
    form->addRow(translate("label_java"), java);
    form->addRow(translate("label_framework"), framework);
    layout->addLayout(form);
    layout->addWidget(buttons);
    setLayout(layout);
    setWindowIcon(::icon("create"));
    setWindowTitle(translate("title_window"));
}

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ
