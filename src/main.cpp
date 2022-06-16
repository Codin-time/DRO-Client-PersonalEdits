#include "aoapplication.h"
#include "drmediatester.h"
#include "lobby.h"
#include "logger.h"

#include <QDebug>

int main(int argc, char *argv[])
{
#if defined(Q_OS_WINDOWS)
  qputenv("QT_MULTIMEDIA_PREFERRED_PLUGINS", "windowsmediafoundation");
#elif defined(Q_OS_MACOS)
  qputenv("QT_MAC_WANTS_LAYER", "1");
#endif

  qInstallMessageHandler(logger::log);
  qInfo() << "Starting Danganronpa Online...";

  bool l_dpi_scaling = false;
  for (int i = 0; i < argc; ++i)
  {
    const QString l_arg(argv[i]);

    if (l_arg == "-dpiscaling")
    {
      l_dpi_scaling = true;
    }
  }

  if (l_dpi_scaling)
  {
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
  }
  else
  {
    qputenv("QT_FONT_DPI", "96");
    qputenv("QT_SCALE_FACTOR", "1");
    qputenv("QT_AUTO_SCREEN_SCALE_FACTOR", "0");

    QCoreApplication::setAttribute(Qt::AA_DisableHighDpiScaling);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, false);
  }

  AOApplication app(argc, argv);
  DRMediaTester tester;

  app.load_fonts();
  app.construct_lobby();
  app.get_lobby()->show();

  const int code = app.exec();

  logger::shutdown();

  return code;
}
