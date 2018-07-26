#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RARStreamer");
    app.setApplicationVersion("0.1");

    QCommandLineParser parser;
    parser.setApplicationDescription("Streams content from incomplete archives.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("archive", "The first archive.");

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    if (args.empty())
        parser.showHelp();

    MainWindow w;
    w.show();

    w.extract(args.at(0));

    exit(app.exec());
}
