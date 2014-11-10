#include "framework.hpp"

namespace VPZ {
namespace APKStudio {
namespace Components {

Framework::Framework(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *buttons = new QHBoxLayout;
    QVBoxLayout *layout = new QVBoxLayout(this);
    QPushButton *browse = new QPushButton(translate("button_browse"), this);
    QPushButton *remove = new QPushButton(translate("button_remove"), this);
    tree = new QTreeWidget(this);
    buttons->addWidget(browse);
    buttons->addWidget(remove);
    buttons->setAlignment(browse, Qt::AlignLeft);
    buttons->setAlignment(remove, Qt::AlignLeft);
    connections.append(connect(browse, &QPushButton::clicked, [ this ] () {
        QFileDialog dialog(this, translate("title_select"), Helpers::Settings::previousDirectory(), "Framework APK (*.apk)");
        dialog.setAcceptMode(QFileDialog::AcceptOpen);
        dialog.setFileMode(QFileDialog::ExistingFile);
        if (dialog.exec() != QFileDialog::Accepted)
            return;
        QStringList files = dialog.selectedFiles();
        if (files.isEmpty())
            return;
        Helpers::Settings::previousDirectory(dialog.directory().absolutePath());
        Async::Framework *framework = new Async::Framework(files.first(), Helpers::Text::random());
        connections.append(connect(framework, SIGNAL(finished(QVariant)), this, SLOT(onFrameworkInstalled(QVariant)), Qt::QueuedConnection));
        Tasks::instance()->add(translate("task_framework"), framework);
    }));
    connections.append(connect(remove, &QPushButton::clicked, [ this ] () {
        QModelIndexList selection = this->tree->selectionModel()->selectedRows(0);
        if (selection.count() != 1)
            return;
        QModelIndex selected = selection.first();
        QString path = selected.data(ROLE_PATH).toString();
        QFileInfo info(path);
        if (!info.exists() || !info.isFile())
            return;
        if (QMessageBox::Yes != QMessageBox::question(this, translate("title_confirm"), translate("message_framework_delete").arg(info.absoluteFilePath())))
            return;
        QFile::remove(info.absoluteFilePath());
        this->tree->model()->removeRow(selected.row());
    }));
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setRootIsDecorated(false);
    tree->setMaximumHeight(160);
    tree->setSelectionBehavior(QAbstractItemView::SelectRows);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setSortingEnabled(true);
    QStringList labels(translate("header_id"));
    labels << translate("header_tag");
    labels << translate("header_time");
    tree->setHeaderLabels(labels);
    layout->addWidget(tree);
    layout->addLayout(buttons);
    fixButtonSize(browse);
    fixButtonSize(remove);
    refreshFrameworks();
    setLayout(layout);
}

void Framework::fixButtonSize(QPushButton *button)
{
    QSize size = button->fontMetrics().size(Qt::TextShowMnemonic, button->text());
    size.setWidth(size.width() + 12);
    QStyleOptionButton option;
    option.initFrom(button);
    option.rect.setSize(size);
    button->setMaximumSize(button->style()->sizeFromContents(QStyle::CT_PushButton, &option, size, button));
}

void Framework::refreshFrameworks()
{
    QFileInfo info(Helpers::Settings::frameworkPath());
    if (!info.exists() || !info.isDir())
        return;
    if (tree->model()->hasChildren())
        tree->model()->removeRows(0, tree->model()->rowCount());
    QDirIterator iterator(info.absoluteFilePath(), QDirIterator::Subdirectories);
    while (iterator.hasNext()) {
        QFileInfo file(iterator.next());
        if (!file.isFile())
            continue;
        QString name = file.baseName();
        QStringList parts = name.split('-').mid(0, 2);
        if (parts.count() != 2)
            continue;
        QTreeWidgetItem *framework = new QTreeWidgetItem(tree);
        framework->setText(0, parts.at(0));
        framework->setText(1, name.section('-', 1));
        framework->setText(2, Helpers::Format::timestamp(file.lastModified()));
        framework->setToolTip(0, file.absoluteFilePath());
        for (int i = 0; i < 3; ++i)
            framework->setData(i, ROLE_PATH, file.absoluteFilePath());
        tree->addTopLevelItem(framework);
    }
    tree->sortByColumn(2, Qt::DescendingOrder);
}

void Framework::onFrameworkInstalled(const QVariant &result)
{
    if (result.toBool()) {
        refreshFrameworks();
        return;
    }
    QMessageBox::critical(this, translate("title_failure"), translate("message_framework_failed"), QMessageBox::Close);
}

Framework::~Framework()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Components
} // namespace APKStudio
} // namespace VPZ
