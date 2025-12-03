/* plugin-statusnotifier/statusnotifierconfiguration.cpp
 * Configuration implementation for plugin-statusnotifier
 */

#include "statusnotifierconfiguration.h"
#include "ui_statusnotifierconfiguration.h"

#include <QAbstractButton>
#include <QAbstractSpinBox>
#include <QCheckBox>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QPushButton>
#include <QTableWidget>
#include <QTableWidgetItem>

StatusNotifierConfiguration::StatusNotifierConfiguration(PluginSettings* settings, QWidget* parent)
    : OneG4PanelPluginConfigDialog(settings, parent),
      ui(new Ui::StatusNotifierConfiguration) {
  setAttribute(Qt::WA_DeleteOnClose);
  setObjectName(QStringLiteral("StatusNotifierConfigurationWindow"));

  ui->setupUi(this);

  if (QPushButton* closeBtn = ui->buttons->button(QDialogButtonBox::Close))
    closeBtn->setDefault(true);

  connect(ui->buttons, &QDialogButtonBox::clicked, this, &StatusNotifierConfiguration::dialogButtonsAction);

  ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
  ui->tableWidget->horizontalHeader()->setSectionsClickable(false);
  ui->tableWidget->sortByColumn(0, Qt::AscendingOrder);

  loadSettings();

  connect(ui->orderCB, &QCheckBox::toggled, this, &StatusNotifierConfiguration::saveSettings);
  connect(ui->attentionSB, &QAbstractSpinBox::editingFinished, this, &StatusNotifierConfiguration::saveSettings);
}

StatusNotifierConfiguration::~StatusNotifierConfiguration() {
  delete ui;
}

void StatusNotifierConfiguration::loadSettings() {
  ui->orderCB->setChecked(settings().value(QStringLiteral("reverseOrder"), false).toBool());
  ui->attentionSB->setValue(settings().value(QStringLiteral("attentionPeriod"), 5).toInt());
  mAutoHideList = settings().value(QStringLiteral("autoHideList")).toStringList();
  mHideList = settings().value(QStringLiteral("hideList")).toStringList();
}

void StatusNotifierConfiguration::saveSettings() {
  settings().setValue(QStringLiteral("reverseOrder"), ui->orderCB->isChecked());
  settings().setValue(QStringLiteral("attentionPeriod"), ui->attentionSB->value());
  settings().setValue(QStringLiteral("autoHideList"), mAutoHideList);
  settings().setValue(QStringLiteral("hideList"), mHideList);
}

void StatusNotifierConfiguration::addItems(const QStringList& items) {
  ui->tableWidget->clearContents();
  ui->tableWidget->setRowCount(items.size());
  ui->tableWidget->setSortingEnabled(false);

  int row = 0;
  for (const QString& item : items) {
    auto* widgetItem = new QTableWidgetItem(item);
    widgetItem->setFlags(widgetItem->flags() & ~Qt::ItemIsEditable & ~Qt::ItemIsSelectable);
    ui->tableWidget->setItem(row, 0, widgetItem);

    auto* cb = new QComboBox(ui->tableWidget);
    cb->addItems(QStringList() << tr("Always show") << tr("Auto-hide") << tr("Always hide"));

    if (mAutoHideList.contains(item))
      cb->setCurrentIndex(1);
    else if (mHideList.contains(item))
      cb->setCurrentIndex(2);
    else
      cb->setCurrentIndex(0);

    connect(cb, &QComboBox::currentIndexChanged, this, [this, item](int index) {
      switch (index) {
        case 0:
          mAutoHideList.removeAll(item);
          mHideList.removeAll(item);
          break;
        case 1:
          mHideList.removeAll(item);
          if (!mAutoHideList.contains(item))
            mAutoHideList << item;
          break;
        case 2:
          mAutoHideList.removeAll(item);
          if (!mHideList.contains(item))
            mHideList << item;
          break;
      }
      saveSettings();
    });

    ui->tableWidget->setCellWidget(row, 1, cb);
    ++row;
  }

  ui->tableWidget->setSortingEnabled(true);
  ui->tableWidget->horizontalHeader()->setSortIndicatorShown(false);
  if (ui->tableWidget->rowCount() > 0)
    ui->tableWidget->setCurrentCell(0, 1);
}

void StatusNotifierConfiguration::dialogButtonsAction(QAbstractButton* btn) {
  OneG4PanelPluginConfigDialog::dialogButtonsAction(btn);

  auto* box = qobject_cast<QDialogButtonBox*>(btn->parent());
  if (!box || box->buttonRole(btn) != QDialogButtonBox::ResetRole)
    return;

  for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
    auto* cb = qobject_cast<QComboBox*>(ui->tableWidget->cellWidget(i, 1));
    QTableWidgetItem* widgetItem = ui->tableWidget->item(i, 0);
    if (!cb || !widgetItem)
      continue;

    const QString name = widgetItem->text();
    cb->blockSignals(true);
    if (mAutoHideList.contains(name))
      cb->setCurrentIndex(1);
    else if (mHideList.contains(name))
      cb->setCurrentIndex(2);
    else
      cb->setCurrentIndex(0);
    cb->blockSignals(false);
  }
}
