#pragma once

// --------- Qt ---------- //
#include <QApplication>
#include <QDir>
#include <QFile>
#include <QHBoxLayout>
#include <QList>
#include <QMap>
#include <QProcess>
#include <QPushButton>
#include <QString>
#include <QTextStream>
#include <QVariant>
#include <QWidget>

#include <QtCore>

// --------- X11 ---------- //
#define X11_NO_MIT_SHM
#define X11_NO_XINERAMA
#define X11_NO_XRANDR
#define X11_NO_XRENDER

#define Bool XBool
#define Status XStatus
#define None XNone
#define CursorShape XCursorShape
#define KeyPress XKeyPress
#define KeyRelease XKeyRelease

#include <QX11Info>
#include <X11/Xatom.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#undef Bool
#undef Status
#undef None
#undef CursorShape
#undef KeyPress
#undef KeyRelease

// ----------- std ---------- //
#include <cstdlib>
#include <iostream>