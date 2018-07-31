#include "MainWindow.h"
#include <QApplication>
#include <QCommandLineParser>
#include <QFileDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("RAR Streamer");
    app.setApplicationVersion(RARSTREAMER_VERSION);

    QCommandLineParser parser;
    parser.setApplicationDescription("Streams content from incomplete archives.");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addPositionalArgument("archive", "The first archive.");

    QCommandLineOption autoDirectory(QStringList() << "d" << "auto-directory", "Extracts to a rarstream subdirectory");
    parser.addOption(autoDirectory);
    QCommandLineOption sameDirectory(QStringList() << "s" << "same-directory", "Extracts to the directory of the archive");
    parser.addOption(sameDirectory);
    QCommandLineOption outputDirectory(QStringList() << "o" << "output", "Sets the output directory", "dir");
    parser.addOption(outputDirectory);

    QCommandLineOption passwordParameter(QStringList() << "p" << "password", "Sets an extraction password", "password");
    parser.addOption(passwordParameter);

    parser.process(app);

    const QStringList args = parser.positionalArguments();

    // Create window
    MainWindow w;
    w.show();

    // Compute input file
    QString inputFile;
    if (args.size() > 0)
        inputFile = args.at(0);
    else
        inputFile = QFileDialog::getOpenFileName(&w, "Open Archive", QString(), "RAR Archives (*.rar);;All Files (*.*)");

    if (inputFile.isEmpty())
        exit(1);

    // Compute output directory
    QString extractDirectory;
    if (parser.isSet(outputDirectory))
    {
        extractDirectory = parser.value(outputDirectory);
    }
    else if (parser.isSet(sameDirectory))
    {
        QFileInfo file(inputFile);
        extractDirectory = file.dir().path();
    }
    else if (parser.isSet(autoDirectory))
    {
        QFileInfo file(inputFile);
        QDir directory = file.dir();
        extractDirectory = directory.filePath("rarstream");
    }
    else
    {
        QFileInfo file(inputFile);
        extractDirectory = QFileDialog::getExistingDirectory(&w, "Choose output directory", file.dir().path());
    }

    if (extractDirectory.isEmpty())
        exit(1);

    // Compute password
    QString password;
    if (parser.isSet(passwordParameter))
    {
        password = parser.value(passwordParameter);
    }

    // Start extraction
    w.extract(inputFile, extractDirectory, password);

    exit(app.exec());
}
