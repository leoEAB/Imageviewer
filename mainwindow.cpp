#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , isModified(false)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->label->setFrameStyle(QFrame::Panel | QFrame::Sunken);
    ui->label->setAlignment(Qt::AlignLeft | Qt::AlignRight);
    ui->scrollArea->ensureWidgetVisible(ui->label, 50,50);      //Klappt leider nicht. Sollte es erlauben ein Bild zu scrollen falls es größer als das Fenster ist.
    MainWindow::createStatusBar();
    MainWindow::updateStatusBar("Ready", statusPanel);
//    connect(
//        ui->scrollArea(),
//        SIGNAL(modificationChanged(bool)),
//        this,
//        SLOT(setModified(bool))
//    );
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::maybeSave()
{
    if( this->isModified ) {

        auto ret = QMessageBox::warning(
            this,
            "Editor",
            "The file was modified\nDo you want to save it?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel
        );

        if( ret==QMessageBox::Save ) {

            save();

        } else if( ret==QMessageBox::Discard ) {

            return true;

        } else { // QMessageBox::Cancel

            return false;

        }
    }
    return true;
}

void MainWindow::save()
{


    imagePath = QFileDialog::getSaveFileName(this,tr("Save File"),"/home/leonard/Pictures",tr("Image Files (*.png *.jpg *.bmp)"));
                                                                      //  ^^^^^^^^^^^^^^^^^^^^^^^
    //-------------------------------------------------------------------Kann später entfernt werden-------------------------------------
    *imageObject = image.toImage();
    imageObject->save(imagePath);
}

void MainWindow::on_actionE_xit_triggered()
{
    if(!maybeSave()){
        return;
    }
    QApplication::quit();
}

void MainWindow::on_action_Open_triggered()
{
    if( !maybeSave() ) {
        return;
    }

    imagePath = QFileDialog::getOpenFileName(this, tr("Open Image"), "/home/leonard/Pictures", tr("Image Files (*.png *.jpg *.bmp)"));
                                                                         //  ^^^^^^^^^^^^^^^^^^^^^^^
    //----------------------------------------------------------------------Kann später entfernt werden-------------------------------------


    imageObject = new QImage();
    imageObject->load(imagePath);

    image = QPixmap::fromImage(*imageObject);

    ui->label->setAlignment(Qt::AlignLeft);


    if(image.load(imagePath)){

        image = image.scaled(ui->label->size(),Qt::KeepAspectRatio);
        ui->label->setPixmap(image);
    }


    //When File is opened, activate save as icon
    ui->action_copy->setEnabled(true);
    ui->actionSave_as->setEnabled(true);
    ui->actionZoom_in->setEnabled(true);
    ui->actionZoom_out->setEnabled(true);
    ui->actionZoom_100->setEnabled(true);
}

void MainWindow::setModified(bool modified)
{
    this->isModified = modified;
    //Activate save Icon when something has been changed
    ui->actionSave->setEnabled(true);
}


void MainWindow::on_action_New_triggered()
{
    if( !maybeSave() ) {
        return;
    }
    ui->label->clear();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if( maybeSave() ) {
        event->accept();
    } else {
        event->ignore();
    }
}

void MainWindow::on_action_Print_triggered()
{
//    QPrinter printer;

//    QPrintDialog printDialog( &printer, this );

//    if( printDialog.exec() == QDialog::Rejected ) {
//        return;
//    }

//    ui->label->print(&printer);
}

void MainWindow::on_actionSave_triggered()
{
    *imageObject = image.toImage();
    imageObject->save(imagePath);
}

void MainWindow::on_actionSave_as_triggered()
{
    MainWindow::save();
}

void MainWindow::on_actionZoom_in_triggered()
{
    MainWindow::scaleImage(1);
}
void MainWindow::on_actionZoom_out_triggered()
{
    MainWindow::scaleImage(-1);
}
void MainWindow::on_actionZoom_100_triggered()
{
    MainWindow::scaleImage(0);
}

void MainWindow::scaleImage(int zoom_dir)
{
    ui->label->setScaledContents(false);
    if(zoom_dir == 0){
        scaleFactor = 1;
    }else if(fmod(scaleFactor,0.25) != 0){

    }
    else if(zoom_dir == 1 && scaleFactor < 4){
        if(fmod(scaleFactor,0.25) != 0){
               scaleFactor += fmod(scaleFactor,0.25);
        }else{
            scaleFactor += 0.25;
        }
    }else if(zoom_dir == -1 && scaleFactor > 0.25){
        if(fmod(scaleFactor,0.25) != 0)
        {
          scaleFactor -= fmod(scaleFactor,0.25);
        }else{
            scaleFactor -= 0.25;
        }
    }
    QPixmap scaled_image = image.scaled(image.width()*scaleFactor, image.height()*scaleFactor, Qt::KeepAspectRatio);

    ui->label->setPixmap(scaled_image);
    ui->label->resize(ui->label->pixmap()->size());
    ui->label->setAlignment(Qt::AlignLeft);
    ui->scrollArea->setWidgetResizable(true);
    ui->label->setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);
    QString s_zoomFactor = "Zoom: " + QString::number((scaleFactor)*100) + "%";
    MainWindow::updateStatusBar(s_zoomFactor, zoomIndication);

}

void MainWindow::updateStatusBar(QString message, QLabel * whichStatusBar)
{
      whichStatusBar->setText(message);
}

void MainWindow::createStatusBar(){
    statusBar()->addPermanentWidget(zoomIndication);
    statusBar()->addWidget(statusPanel);
}
void MainWindow::wheelEvent(QWheelEvent *event){
    int numDegrees = event->angleDelta().y();
    if(numDegrees > 0)
    {
        scaleImage(1);
    }
    else if(numDegrees < 0)
    {
        scaleImage(-1);

    }
}


void MainWindow::on_actionFit_to_Window_triggered()
{

      ui->label->setScaledContents(true);

      int w = ui->centralwidget->width();
      int h = ui->centralwidget->height();
      ui->label->resize(w,h);

      ui->label->setPixmap(image.scaled(w,h,Qt::KeepAspectRatio));
}

//void MainWindow::scaleImageQuality(){
//    ui->label->setScaledContents(true);
//    scaleFactor = 1.0;
//}

void MainWindow::on_action_Paste_triggered()
{
    const QClipboard *clipboard = QApplication::clipboard();
    const QMimeData *mimeData = clipboard->mimeData();

    if(mimeData->hasImage()){
        image = qvariant_cast<QPixmap>(mimeData->imageData());
        ui->label->setPixmap(image);
        updateStatusBar("Image pasted", statusPanel);
        ui->label->setAlignment(Qt::AlignLeft);
        ui->actionSave_as->setEnabled(true);
        ui->actionZoom_in->setEnabled(true);
        ui->actionZoom_out->setEnabled(true);
        ui->actionZoom_100->setEnabled(true);
    }else{
        MainWindow::updateStatusBar("Cannot Display data", statusPanel);
    }
}




void MainWindow::on_action_copy_triggered()
{
    QClipboard *clipboard = QApplication::clipboard();
    QMimeData *data = new QMimeData;
    data->setImageData(image);
    clipboard->setMimeData(data);

}
