#include "mainwindow.h"
#include <QMenu>
#include <QMenuBar>
#include <QAction>
#include <QtWidgets>

MainWindow :: MainWindow()
{
    txt = new QPlainTextEdit;
    setCentralWidget(txt);
    connect(txt->document(), SIGNAL(contentsChanged()),
            this, SLOT(documentWasModified()));
    setCurrentFile("");
    createActions();
    createMenus();
}

void MainWindow::createMenus()
{
    QMenu *fileMenu,*aboutMenu;
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(quit);
    aboutMenu = menuBar()->addMenu(tr("&About"));
    aboutMenu->addAction(aboutAct);
}

void MainWindow:: createActions()
{
    newAct = new QAction(tr("&New"), this);
    connect(newAct, SIGNAL(triggered()),this,SLOT(newFile()));
    openAct = new QAction(tr("&Open"), this);
    connect(openAct, SIGNAL(triggered()), this, SLOT(openFile()));
    saveAct = new QAction(tr("&Save"),this);
    connect(saveAct,SIGNAL(triggered()),this,SLOT(saveFile()));
    aboutAct = new QAction(tr("&About"), this);
    connect(aboutAct,SIGNAL(triggered()),this,SLOT(aboutFile()));
    quit = new QAction("&Quit",this);
    connect(quit, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::newFile()
{
    if (maybeSave()) {
        txt->clear();
        setCurrentFile("");
    }
}

void MainWindow::openFile()
{
    if (maybeSave()) {
        QString fileName = QFileDialog::getOpenFileName(this);
        if (!fileName.isEmpty())
            loadFile(fileName);
    }
}

void MainWindow::saveFile()
{
     if (curFile.isEmpty()) {
           saveAs();
     } else {
           savefile(curFile);
     }
}

bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;

    return savefile(files.at(0));
}

void MainWindow::aboutFile()
{
    QMessageBox::about(this, tr("About Application"),
             tr("Η εφαρμογή είναι ένας επεξαργαστής κειμένου με τις βασικές "
                "λειτουργίες (Νέο φύλλο εργασίας, Άνοιγμα, Αποθήκευση)"
                "\n\n"
                "ΛΑΤΜΟΣ ΓΕΩΡΓΙΟΣ"));
}

bool MainWindow::maybeSave()
{
    if (txt->document()->isModified()) {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Application"),
                     tr("The document has been modified.\n"
                        "Do you want to save your changes?"),
                     QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
              saveFile();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    txt->document()->setModified(false);
    setWindowModified(false);

    QString shownName = curFile;
    if (curFile.isEmpty())
        shownName = "untitled.txt";
        setWindowFilePath(shownName);
}

void MainWindow::documentWasModified()
{
    setWindowModified(txt->document()->isModified());
}

void MainWindow::loadFile(const QString &fileName)
//! [42] //! [43]
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    txt->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded"), 2000);
}

bool MainWindow::savefile(const QString &fileName)
//! [44] //! [45]
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, tr("Application"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << txt->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);
    return true;
}
