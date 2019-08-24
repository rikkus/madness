#ifndef MADNESS_H
#define MADNESS_H

#include <qwindowdefs.h>
#include <qmap.h>
#include <qvaluelist.h>

#include <ksystemtray.h>

class QTimer;
class KWinModule;

class Madness : public KSystemTray
{
  Q_OBJECT

  public:

    Madness();

  protected:

    virtual void mousePressEvent(QMouseEvent *);

  protected slots:

    void update();
    void getmad();

  private:

    QMap<WId, QPoint> originalPositions_;
    QValueList<WId> windowList_;
    QMap<WId, QPoint> velocityMap_;
    QRect workArea_;
    QTimer * timer_;
    KWinModule * wm_;
};

#endif // MADNESS_H
