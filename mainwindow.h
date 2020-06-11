#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include <QtCore>
#include <QtGui>
#include <QtWidgets>
#include <QtPrintSupport>
//------Selbst hinzugefuegte include
#include <QSaveFile>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

void save();

bool maybeSave();

void closeEvent(QCloseEvent *event);

private slots:
    void on_actionE_xit_triggered();

    void on_action_Open_triggered();

    void setModified(bool modified);

    void on_action_New_triggered();

    void on_action_Print_triggered();

    void on_actionSave_triggered();

    void on_actionSave_as_triggered();

    void on_actionZoom_in_triggered();

    void scaleImage(int zoom_dir);

    void updateStatusBar(QString message, QLabel * whichStatusBar);

    void createStatusBar();

    void on_actionZoom_out_triggered();

    void on_actionZoom_100_triggered();

    void wheelEvent(QWheelEvent *event);
    

    void on_actionFit_to_Window_triggered();

    void on_action_Paste_triggered();

    void on_action_copy_triggered();

private:

    QLabel *zoomIndication = new QLabel;
    QLabel *statusPanel = new QLabel;
    QPixmap image;
    QImage *imageObject;
    QString imagePath;
    double scaleFactor = 1;
    bool isModified;
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
