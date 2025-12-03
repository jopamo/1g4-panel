/* plugin-spacer/spacer.cpp
 * Spacer plugin implementation
 */

#include "spacer.h"
#include "spacerconfiguration.h"

#include <QApplication>
#include <QDialog>
#include <QEvent>
#include <QSizePolicy>

void SpacerWidget::setType(const QString& type) {
  if (type == mType)
    return;

  mType = type;
  QEvent e(QEvent::ThemeChange);
  QApplication::sendEvent(this, &e);
}

void SpacerWidget::setOrientation(const QString& orientation) {
  if (orientation == mOrientation)
    return;

  mOrientation = orientation;
  QEvent e(QEvent::ThemeChange);
  QApplication::sendEvent(this, &e);
}

/************************************************

 ************************************************/
Spacer::Spacer(const IOneG4PanelPluginStartupInfo& startupInfo)
    : QObject(),
      IOneG4PanelPlugin(startupInfo),
      mSize(8),
      mExpandable(false) {
  settingsChanged();
}

/************************************************

 ************************************************/
void Spacer::settingsChanged() {
  mSize = settings()->value(QStringLiteral("size"), 8).toInt();
  const bool oldExpandable = mExpandable;
  mExpandable = settings()->value(QStringLiteral("expandable"), false).toBool();

  mSpacer.setType(settings()
                      ->value(QStringLiteral("spaceType"), SpacerConfiguration::msTypes[0])
                      .toString());

  setSizes();

  if (oldExpandable != mExpandable)
    pluginFlagsChanged();
}

/************************************************

 ************************************************/
QDialog* Spacer::configureDialog() {
  return new SpacerConfiguration(settings());
}

/************************************************

 ************************************************/
void Spacer::setSizes() {
  if (mExpandable) {
    mSpacer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mSpacer.setMinimumSize(1, 1);
    mSpacer.setMaximumSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    mSpacer.setOrientation(panel()->isHorizontal() ? QStringLiteral("horizontal")
                                                   : QStringLiteral("vertical"));
  }
  else {
    if (panel()->isHorizontal()) {
      mSpacer.setOrientation(QStringLiteral("horizontal"));
      mSpacer.setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Expanding);
      mSpacer.setFixedWidth(mSize);
      mSpacer.setMinimumHeight(0);
      mSpacer.setMaximumHeight(QWIDGETSIZE_MAX);
    }
    else {
      mSpacer.setOrientation(QStringLiteral("vertical"));
      mSpacer.setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
      mSpacer.setFixedHeight(mSize);
      mSpacer.setMinimumWidth(0);
      mSpacer.setMaximumWidth(QWIDGETSIZE_MAX);
    }
  }
}

/************************************************

 ************************************************/
void Spacer::realign() {
  setSizes();
}
