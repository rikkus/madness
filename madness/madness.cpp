/*
 * madness.cpp
 *
 * Copyright (C) 2000 Rik Hemsley <rik@kde.org>
 */

#include <qpushbutton.h>
#include <qtimer.h>
#include <kwin.h>
#include <kwinmodule.h>
#include <kapp.h>
#include <kiconloader.h>

#include "madness.h"

#include <X11/Xlib.h>

Madness::Madness()
  : KSystemTray(
    0,
    "Madness !"
)
{
  wm_ = new KWinModule(this);
  timer_ = new QTimer(this);

  setPixmap(SmallIcon("madness"));

  connect(wm_,    SIGNAL(currentDesktopChanged(int)), SLOT(update()));
  connect(wm_,    SIGNAL(windowAdded(WId)),           SLOT(update()));
  connect(wm_,    SIGNAL(windowRemoved(WId)),         SLOT(update()));
  connect(wm_,    SIGNAL(strutChanged()),             SLOT(update()));
  connect(timer_, SIGNAL(timeout()),                  SLOT(getmad()));

  update();

  timer_->start(10, true);
}

  void
Madness::update()
{
  windowList_ = wm_->windows();
  workArea_ = wm_->workArea();

  velocityMap_.clear();

  QValueList<WId>::ConstIterator it(windowList_.begin());

  int delta = 2;

  for (; it != windowList_.end(); ++it)
  {
    if (!originalPositions_.contains(*it))
      originalPositions_.insert(*it, KWin::info(*it).frameGeometry.topLeft());

    velocityMap_.insert(*it, QPoint(delta, -delta));
    delta = -delta;
  }
}

  void
Madness::getmad()
{
  QValueList<WId>::ConstIterator it(windowList_.begin());

  for (; it != windowList_.end(); ++it)
  {
    KWin::Info i(KWin::info(*it));

    if (
        (winId() == i.win)
        ||
        (NET::Visible != i.mappingState)
        ||
        (
          (NET::Unknown != i.windowType) &&
          (NET::Normal  != i.windowType) &&
          (NET::Tool    != i.windowType) &&
          (NET::Menu    != i.windowType) &&
          (NET::Dialog  != i.windowType)
        )
        ||
        (NET::Max & i.state)
        ||
        (NET::Shaded & i.state)
        ||
        (wm_->currentDesktop() != i.desktop)
    )
      continue;

    QRect r = i.frameGeometry;

    int x = r.x();
    int y = r.y();

    int dx = velocityMap_[i.win].x();
    int dy = velocityMap_[i.win].y();

    if (dx < 0 && (x - dx < workArea_.left()))
      dx = -dx;

    else if (dx > 0 && (x + dx + r.width() > workArea_.right()))
      dx = -dx;

    if (dy < 0 && (y - dy < workArea_.top()))
      dy = -dy;

    else if (dy > 0 && (y + dy + r.height() > workArea_.bottom()))
      dy = -dy;

    velocityMap_.replace(*it, QPoint(dx, dy));

    XMoveWindow(
        qt_xdisplay(),
        i.win,
        x + dx,
        y + dy
    );
  }

  timer_->start(10, true);
}

  void
Madness::mousePressEvent(QMouseEvent *e)
{
  if (e->button() != LeftButton)
    return;

  QMap<WId, QPoint>::ConstIterator it(originalPositions_.begin());

  for (; it != originalPositions_.end(); ++it)
    XMoveWindow(qt_xdisplay(), it.key(), (*it).x(), (*it).y());

  kapp->quit();
}

