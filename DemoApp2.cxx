#pragma execution_character_set("utf-8")
#include "myMainWindow.h"
#include <pqPVApplicationCore.h>
#include <QApplication>
#include <QTranslator>
#include <QLocale>

int main(int argc, char** argv)
{

    QApplication app(argc, argv);
    pqPVApplicationCore appCore(argc, argv);

    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for (const QString &locale : uiLanguages) {
        const QString baseName = "Demo2_1_" + QLocale(locale).name();
        if (translator.load(":/i18n/" + baseName)) {
            app.installTranslator(&translator);
            break;
        }
    }

    // Create the window which initialize all paraview behaviors
    myMainWindow window;
    window.setWindowTitle("Demo App");

    // Load a configuration XML in order to have access to filters and readers
    appCore.loadConfiguration(qApp->applicationDirPath() + "/../ParaViewFilters.xml");

    window.show();
    return app.exec();

}
