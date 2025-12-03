/* OneG4/PluginInfo.cpp
 * PluginInfo class implementation
 */

#include "PluginInfo.h"

#include <QDir>
#include <QFileInfo>
#include <QIcon>
#include <QSettings>
#include <QStringList>

#include "XdgIcon.h"

namespace {

QStringList normalizedServiceTypes(const QString& raw) {
  QStringList list = raw.split(QLatin1Char(';'), Qt::SkipEmptyParts);
  for (QString& entry : list)
    entry = entry.trimmed();
  return list;
}

QString normalizedPattern(const QString& pattern) {
  if (pattern.isEmpty())
    return QStringLiteral("*.desktop");
  return pattern;
}

}  // namespace

namespace OneG4 {

PluginInfo::PluginInfo(const QString& filePath)
    : mFilePath(filePath),
      mIsValid(false) {
  QSettings desktopFile(filePath, QSettings::IniFormat);
  if (!desktopFile.childGroups().contains(QStringLiteral("Desktop Entry")))
    return;

  desktopFile.beginGroup(QStringLiteral("Desktop Entry"));
  const auto keys = desktopFile.childKeys();
  for (const QString& key : keys)
    mValues.insert(key, desktopFile.value(key));
  desktopFile.endGroup();

  mServiceTypes = normalizedServiceTypes(mValues.value(QStringLiteral("ServiceTypes")).toString());
  mIsValid = true;
}

QString PluginInfo::id() const {
  if (!mFilePath.isEmpty())
    return QFileInfo(mFilePath).completeBaseName();
  return QString();
}

QString PluginInfo::name() const {
  QString name = mValues.value(QStringLiteral("Name")).toString();
  if (name.isEmpty())
    name = id();
  return name;
}

QString PluginInfo::comment() const {
  return mValues.value(QStringLiteral("Comment")).toString();
}

QIcon PluginInfo::icon(const QIcon& fallback) const {
  const QString iconName = mValues.value(QStringLiteral("Icon")).toString();
  if (iconName.isEmpty())
    return fallback;

  const QIcon icon = XdgIcon::fromTheme(iconName, fallback);
  return icon.isNull() ? fallback : icon;
}

QVariant PluginInfo::value(const QString& key) const {
  return mValues.value(key);
}

QStringList PluginInfo::serviceTypes() const {
  return mServiceTypes;
}

QList<PluginInfo> PluginInfo::search(const QStringList& directories,
                                     const QString& serviceType,
                                     const QString& pattern) {
  QList<PluginInfo> plugins;
  const QString filePattern = normalizedPattern(pattern);

  for (const QString& dirName : directories) {
    const QDir dir(dirName);
    if (!dir.exists())
      continue;

    const QStringList files = dir.entryList(QStringList(filePattern), QDir::Files);
    for (const QString& file : files) {
      const QString absPath = dir.absoluteFilePath(file);
      PluginInfo info(absPath);
      if (!info.isValid())
        continue;
      if (!serviceType.isEmpty() && !info.serviceTypes().contains(serviceType))
        continue;
      plugins.append(info);
    }
  }

  return plugins;
}

}  // namespace OneG4
