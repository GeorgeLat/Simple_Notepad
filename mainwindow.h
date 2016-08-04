#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QPlainTextEdit>
#include <QMainWindow>
#include <QObject>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QAction *newAct;
    QAction *aboutAct;
    QAction *saveAct;
    QAction *openAct;
    QPlainTextEdit *txt;
    QAction *quit;
    QString curFile;
    bool maybeSave();
    bool saveAs();
    void setCurrentFile(const QString &fileName);
    bool savefile(const QString &fileName);
    void loadFile(const QString &fileName);
private slots:
    void newFile();
    void openFile();
    void saveFile();
    void aboutFile();
    void documentWasModified();
public:
    MainWindow();
    void createMenus();
    void createActions();
};

#endif // MAINWINDOW_H
