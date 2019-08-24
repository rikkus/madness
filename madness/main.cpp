#include "madness.h"
#include <kapp.h>
#include <kaboutdata.h>
#include <kcmdlineargs.h>
#include <klocale.h>

static const char *description = I18N_NOOP("Return of Madness");

static const char *version = "v1.2";

  int
main(int argc, char **argv)
{
  KAboutData about(
      "madness",
      I18N_NOOP("Madness"),
      version,
      description,
      KAboutData::License_GPL,
      "(C) 2000 Rik Hemsley"
  );

  about.addAuthor(
      "Rik Hemsley",
      "code",
      "rik@kde.org",
      "http://www.geoid.clara.net/rik"
  );

  about.addAuthor(
      "Malte Starostik",
      "patch",
      "Malte.Starostik@t-online.de"
  );

  KCmdLineArgs::init(argc, argv, &about);

  KApplication app;

  (new Madness)->show();

  return app.exec();
}
