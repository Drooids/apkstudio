#include "explorer.hpp"

using namespace VPZ::APKStudio::Components;
using namespace VPZ::APKStudio::Helpers;

namespace VPZ {
namespace APKStudio {
namespace Windows {

Explorer::Explorer(const QString &device, QWidget *parent) :
    Dialog(parent), device(device)
{
    QHBoxLayout *content = new QHBoxLayout();
    QVBoxLayout *layout = new QVBoxLayout(this);
    content->setContentsMargins(0, 0, 0, 0);
    content->setSpacing(2);
    setLayout(layout);
    createTree(content);
    createTabs(content);
    createToolbar();
    layout->addLayout(content);
    layout->setContentsMargins(2, 2, 2, 2);
    layout->setSpacing(0);
    setWindowIcon(::icon("folder_stand"));
    setWindowTitle(translate("title_window").arg(device));
}

void Explorer::createTabs(QLayout *layout)
{
    QSizePolicy size(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tabs = new QTabWidget(this);
    size.setHorizontalStretch(2);
    tabs->setSizePolicy(size);
    tabs->setTabsClosable(true);
    layout->addWidget(tabs);
}

void Explorer::createToolbar()
{
    QToolBar *tool_bar = new QToolBar(this);
    QAction *refresh = new QAction(::icon("arrow_circle_double"), translate("label_refresh"), tool_bar);
    QActionGroup *applications = new QActionGroup(tool_bar);
    QActionGroup *music = new QActionGroup(tool_bar);
    QActionGroup *partitions = new QActionGroup(tool_bar);
    QActionGroup *photos = new QActionGroup(tool_bar);
    QActionGroup *storage = new QActionGroup(tool_bar);
    QActionGroup *videos = new QActionGroup(tool_bar);
    applications->addAction(::icon("details"), translate("label_details"));
    applications->addAction(::icon("disc_arrow"), translate("label_install"));
    applications->addAction(::icon("disc_minus"), translate("label_uninstall"));
    applications->addAction(::icon("tick"), translate("label_enable"));
    applications->addAction(::icon("cross"), translate("label_disable"));
    applications->addAction(::icon("arrow_top_left"), translate("label_pull"));
    applications->addAction(::icon("arrow_push"), translate("label_push"));
    applications->setVisible(false);
    music->addAction(::icon("details"), translate("label_details"));
    music->addAction(::icon("scissors"), translate("label_move"));
    music->addAction(::icon("files"), translate("label_copy"));
    music->addAction(::icon("arrow_top_left"), translate("label_pull"));
    music->addAction(::icon("rename"), translate("label_rename"));
    music->addAction(::icon("bin_empty"), translate("label_remove"));
    music->setVisible(false);
    partitions->addAction(::icon("drive_arrow"), translate("label_remount"));
    partitions->addAction(::icon("drive_minus"), translate("label_unmount"));
    partitions->setVisible(false);
    photos->addAction(::icon("details"), translate("label_details"));
    photos->addAction(::icon("scissors"), translate("label_move"));
    photos->addAction(::icon("files"), translate("label_copy"));
    photos->addAction(::icon("arrow_top_left"), translate("label_pull"));
    photos->addAction(::icon("rename"), translate("label_rename"));
    photos->addAction(::icon("bin_empty"), translate("label_remove"));
    photos->setVisible(false);
    storage->addAction(::icon("folder_plus"), translate("label_create"));
    storage->addAction(::icon("details"), translate("label_details"));
    storage->addAction(::icon("scissors"), translate("label_move"));
    storage->addAction(::icon("files"), translate("label_copy"));
    storage->addAction(::icon("arrow_top_left"), translate("label_pull"));
    storage->addAction(::icon("arrow_bottom_right"), translate("label_push"));
    storage->addAction(::icon("key_solid"), translate("label_chmod"));
    storage->addAction(::icon("rename"), translate("label_rename"));
    storage->addAction(::icon("bin_empty"), translate("label_remove"));
    storage->setVisible(false);
    videos->addAction(::icon("details"), translate("label_details"));
    videos->addAction(::icon("scissors"), translate("label_move"));
    videos->addAction(::icon("files"), translate("label_copy"));
    videos->addAction(::icon("arrow_top_left"), translate("label_pull"));
    videos->addAction(::icon("rename"), translate("label_rename"));
    videos->addAction(::icon("bin_empty"), translate("label_remove"));
    videos->setVisible(false);
    tool_bar->addAction(refresh);
    tool_bar->addSeparator();
    tool_bar->addActions(applications->actions());
    tool_bar->addActions(storage->actions());
    tool_bar->addActions(music->actions());
    tool_bar->addActions(partitions->actions());
    tool_bar->addActions(photos->actions());
    tool_bar->addActions(videos->actions());
    layout()->addWidget(tool_bar);
    connections.append(connect(tabs, static_cast<void(QTabWidget::*)(int)>(&QTabWidget::currentChanged), [ applications, music, partitions, photos, refresh, storage, videos, this ] (int index) {
        applications->setVisible(false);
        music->setVisible(false);
        partitions->setVisible(false);
        photos->setVisible(false);
        refresh->setEnabled(false);
        storage->setVisible(false);
        videos->setVisible(false);
        if (index < 0)
            return;
        QWidget *widget = this->tabs->currentWidget();
        if (!widget)
            return;
        refresh->setEnabled(true);
        if (widget->inherits(Applications::staticMetaObject.className()))
            applications->setVisible(true);
        else if (widget->inherits(Music::staticMetaObject.className()))
            music->setVisible(true);
        else if (widget->inherits(Partitions::staticMetaObject.className()))
            partitions->setVisible(true);
        else if (widget->inherits(Photos::staticMetaObject.className()))
            photos->setVisible(true);
        else if (widget->inherits(Storage::staticMetaObject.className()))
            storage->setVisible(true);
        else if (widget->inherits(Videos::staticMetaObject.className()))
            videos->setVisible(true);
        widget->setFocus();
    }));
    connections.append(connect(tabs, static_cast<void(QTabWidget::*)(int)>(&QTabWidget::tabCloseRequested), [ this ] (int index) {
        this->tabs->removeTab(index);
    }));
    refresh->setEnabled(false);
}

void Explorer::createTree(QLayout *layout)
{
    QSizePolicy size(QSizePolicy::Preferred, QSizePolicy::Preferred);
    tree = new QTreeWidget(this);
    size.setHorizontalStretch(1);
    tree->header()->hide();
    tree->setContextMenuPolicy(Qt::CustomContextMenu);
    tree->setEditTriggers(QAbstractItemView::NoEditTriggers);
    tree->setSelectionBehavior(QAbstractItemView::SelectItems);
    tree->setSelectionMode(QAbstractItemView::SingleSelection);
    tree->setSortingEnabled(false);
    tree->setSizePolicy(size);
    connections.append(connect(tree, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(onNodeClicked(QModelIndex))));
    layout->addWidget(tree);
}

void Explorer::onInitComplete()
{
    QTreeWidgetItem *bookmarks = new QTreeWidgetItem(tree);
    QTreeWidgetItem *system = new QTreeWidgetItem(tree);
    QTreeWidgetItem *applications = new QTreeWidgetItem(system);
    QTreeWidgetItem *information = new QTreeWidgetItem(system);
    QTreeWidgetItem *music = new QTreeWidgetItem(bookmarks);
    QTreeWidgetItem *partitions = new QTreeWidgetItem(system);
    QTreeWidgetItem *photos = new QTreeWidgetItem(bookmarks);
    QTreeWidgetItem *storage = new QTreeWidgetItem(system);
    QTreeWidgetItem *videos = new QTreeWidgetItem(bookmarks);
    applications->setIcon(0, ::icon("apk"));
    applications->setText(0, translate("navigation_applications"));
    applications->setData(0, ROLE_TYPE, NAVIGATION_APPLICATIONS);
    bookmarks->addChild(music);
    bookmarks->addChild(photos);
    bookmarks->addChild(videos);
    bookmarks->setIcon(0, ::icon("bookmark"));
    bookmarks->setText(0, translate("navigation_bookmarks"));
    information->setIcon(0, ::icon("system_monitor"));
    information->setText(0, translate("navigation_information"));
    information->setData(0, ROLE_TYPE, NAVIGATION_INFORMATION);
    music->setIcon(0, ::icon("music_beam"));
    music->setText(0, translate("navigation_music"));
    music->setData(0, ROLE_TYPE, NAVIGATION_MUSIC);
    partitions->setIcon(0, ::icon("drive"));
    partitions->setText(0, translate("navigation_partitions"));
    partitions->setData(0, ROLE_TYPE, NAVIGATION_PARTITIONS);
    photos->setIcon(0, ::icon("photo_album"));
    photos->setText(0, translate("navigation_photos"));
    photos->setData(0, ROLE_TYPE, NAVIGATION_PHOTOS);
    system->addChild(applications);
    system->addChild(information);
    system->addChild(partitions);
    system->addChild(storage);
    system->setIcon(0, ::icon("board_game"));
    system->setText(0, translate("navigation_system"));
    storage->setIcon(0, ::icon("sd_memory_card"));
    storage->setText(0, translate("navigation_storage"));
    storage->setData(0, ROLE_TYPE, NAVIGATION_STORAGE);
    videos->setIcon(0, ::icon("films"));
    videos->setText(0, translate("navigation_videos"));
    videos->setData(0, ROLE_TYPE, NAVIGATION_VIDEOS);
    tree->addTopLevelItem(bookmarks);
    tree->addTopLevelItem(system);
    tree->expandAll();
}

void Explorer::onNodeClicked(const QModelIndex &)
{
    QList<QTreeWidgetItem *> items = tree->selectedItems();
    if (items.count() != 1)
        return;
    QTreeWidgetItem *item = items.first();
    int type = item->data(0, ROLE_TYPE).value<int>();
    if (type < NAVIGATION_APPLICATIONS)
        return;
    switch (type) {
    case NAVIGATION_APPLICATIONS: {
        Applications *application = new Applications(device, tabs);
        tabs->addTab(application, item->icon(0), item->text(0));
        QTimer::singleShot(0, application, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_INFORMATION: {
        Information *information = new Information(device, tabs);
        tabs->addTab(information, item->icon(0), item->text(0));
        QTimer::singleShot(0, information, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_MUSIC: {
        Music *music = new Music(device, tabs);
        tabs->addTab(music, item->icon(0), item->text(0));
        QTimer::singleShot(0, music, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_PARTITIONS: {
        Partitions *partitions = new Partitions(device, tabs);
        tabs->addTab(partitions, item->icon(0), item->text(0));
        QTimer::singleShot(0, partitions, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_PHOTOS: {
        Photos *photos = new Photos(device, tabs);
        tabs->addTab(photos, item->icon(0), item->text(0));
        QTimer::singleShot(0, photos, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_STORAGE: {
        Storage *storage = new Storage(device, tabs);
        tabs->addTab(storage, item->icon(0), item->text(0));
        QTimer::singleShot(0, storage, SLOT(onInitComplete()));
        break;
    }
    case NAVIGATION_VIDEOS: {
        Videos *videos = new Videos(device, tabs);
        tabs->addTab(videos, item->icon(0), item->text(0));
        QTimer::singleShot(0, videos, SLOT(onInitComplete()));
        break;
    }
    default:
        break;
    }
}

Explorer::~Explorer()
{
    foreach (QMetaObject::Connection connection, connections)
        disconnect(connection);
}

} // namespace Windows
} // namespace APKStudio
} // namespace VPZ