# 🖥️ 1g4-panel

> **The sleek, modular taskbar for the OneG4 Desktop Environment.**

`1g4-panel` is the core desktop panel for OneG4, providing a customizable and feature-rich interface for managing your running applications, system tray, clock, and more.

---

## ✨ Overview

The **1g4-panel** is built around a flexible plugin architecture. While the source code organizes these as "plugins" (e.g., `plugin-mainmenu`), the configuration interface refers to them as **"Widgets"**.

### 🧩 Available Plugins (Widgets)

Here are some of the key plugins you can add to your panel:

#### 🛠️ Custom Command (`plugin-customcommand`)
Execute custom scripts or commands and display their output directly on the panel. Supports text or image output, and interactive click/scroll actions.

![Custom command plugin settings](customcommand.png)

#### 🌍 World Clock (`plugin-worldclock`)
Stay on time with a comprehensive clock and calendar.
- 🕒 Displays local time and date
- 🌐 Supports multiple time zones
- 📅 Integrated calendar view

#### 🚀 Quick Launch (`plugin-quicklaunch`)
Launch your favorite apps instantly!
- Drag & drop applications from the main menu to add them.
- Organize your most-used tools for easy access.

#### 🔔 Status Notifier / System Tray
Manage your background applications and notifications. We support both modern and legacy protocols:
- **`plugin-statusnotifier`**: Implements the modern [StatusNotifierItem (SNI)](https://www.freedesktop.org/wiki/Specifications/StatusNotifierItem) specification.
- **`plugin-tray`**: Supports the legacy [System Tray Protocol](https://www.freedesktop.org/wiki/Specifications/systemtray-spec).

> **Note:** Both plugins can run simultaneously to ensure compatibility with all your applications (Qt, GTK, etc.).

#### 🔊 Volume Control (`plugin-volume`)
Easily adjust your system volume.
- Supports **ALSA**, **OSS**, and **PulseAudio** backends.
- One-click access to advanced mixers like [1g4-mixer](https://github.com/oneg4/1g4-mixer).

---

## 📦 Installation

### 🔧 Compiling from Source
Please refer to the generic compilation instructions for **OneG4** components to build `1g4-panel` from source.

### 💿 Binary Packages
Official packages are available for most major Linux and BSD distributions. Simply search for `1g4-panel` in your package manager.

```bash
# Example (Debian/Ubuntu)
sudo apt install 1g4-panel
```

---

## ⚙️ Configuration & Usage

### 🚀 Launching
`1g4-panel` usually starts automatically with your OneG4 session. You can also launch it manually from the terminal.

### 🎨 Customizing
Make the panel your own!
- **Right-click** anywhere on the panel to access the context menu.
- **"Configure Panel"**: Adjust global settings like position, size, and theme.
- **"Manage Widgets"**: Add, remove, or reorder plugins.

> **Tip:** If a specific plugin's context menu blocks the main panel menu, look for the "handle" on the left side of the widget (in supported themes like `Frost`) or use the "Manage Widgets" dialog to move it.

---

<div align="center">
  <sub>Built with ❤️ by the OneG4 Team</sub>
</div>