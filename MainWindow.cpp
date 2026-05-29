#include "MainWindow.h"

#include <QMimeData>
#include "ui_MainWindow.h"


const double MainWindow::MAX_IMAGE_PIXELS = 3000000;
double MainWindow::MAX_IMAGE_DIMENSION = 3000;
double MainWindow::FRAME_NUMBER = 222428526746345;
double MainWindow::FRAME_NUMBER_MIN = 0;
double MainWindow::G_ACCELERATION = 9.8;
const QString MainWindow::appVersion = "1";

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {


    ui->setupUi(this);

    //setWindowTitle("blur" + appVersion);
    resize(1000, 700);

    helpDialog = new HelpDialog(this);

    imageLabel = new QLabel;
    imageLabel->setBackgroundRole(QPalette::Base);
    imageLabel->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    imageLabel->setScaledContents(true);

    ui->scrollArea->setBackgroundRole(QPalette::Dark);
    ui->scrollArea->setWidget(imageLabel);

    radius = 9;
    quality = 30;

    workerThread = new WorkerThread();

    inputImage = NULL;
    outputImage = NULL;
    fileNameCSV = "";

    createActions();
    initControls();
    updateZoomControls();

    progressBar = new QProgressBar();
    progressBar->setStyleSheet("QProgressBar { border: 2px solid grey;  border-radius: 5px; text-align: center; }");
    progressBar->setValue(0);
    progressBar->setVisible(false);
    lblDeconvolutionTime = new QLabel();
    lblThreadsCount = new QLabel();
    lblImageSize = new QLabel();
    ui->statusBar->addWidget(lblThreadsCount);
    ui->statusBar->addWidget(lblImageSize);
    ui->statusBar->addWidget(lblDeconvolutionTime);
    ui->statusBar->addWidget(progressBar);

    workerThread->start();

    if (helpDialog->isShowOnStartup()) {
        // Delay start because we need to wait while main window will be initialized
        startupTimer = new QTimer();
        startupTimer->setSingleShot(true);
        connect(startupTimer, SIGNAL(timeout()), SLOT(help()));
        startupTimer->start(500);
    }

    //dsd checkUpdatesThread = new CheckUpdatesThread();
    //checkUpdatesThread->start();
}


MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::updatePreviewImage(int deconvolutionTime) {
    progressBar->setValue(0);
    progressBar->setVisible(false);
    // Hack to force update resized pixmap
    imageLabel->setPixmap(QPixmap::fromImage(*outputImage));

    //lblDeconvolutionTime->setText(" Last operation time: " + QString::number(deconvolutionTime) + " ms ");
}

void MainWindow::updateProgress(int value, QString text) {
    progressBar->setFormat(QString("%1: %p%").arg(text));
    progressBar->setVisible(true);
    progressBar->setValue(value);
    if (value==progressBar->maximum()) {
        progressBar->setVisible(false);
    }

}

Blur *MainWindow::generateBlurInfo(bool previewMode) {
    Blur* blur;
    QImage* kernelImage;
        MotionBlur* motionBlur = new MotionBlur();
        motionBlur->radius = motionLength;
        motionBlur->angle = motionAngle;
        motionBlur->vectorXYb = vectorXY;
        kernelImage = ImageUtils::buildKernelImage(motionBlur);
        //kernelImage = ImageUtils::buildKernelImage(motionBlur,&vectorXY);
        blur = motionBlur;

    blur->smooth = quality;
    blur->mode = previewMode ? PREVIEW_GRAY : HIGH_QUALITY;
    // Update kernel preview
    ui->labelKernelPreview->setPixmap(QPixmap::fromImage(kernelImage->scaled(ui->labelKernelPreview->size())));
    delete(kernelImage);

    return blur;
}


void MainWindow::updatePreviewDeconvolution() {
    Blur* blur = generateBlurInfo(true);
    if (inputImage) {
        workerThread->deconvolutionRequest(inputImage, outputImage, blur);
    }
}


void MainWindow::updateFullDeconvolution() {
    Blur* blur = generateBlurInfo(false);
    if (inputImage) {
        workerThread->deconvolutionRequest(inputImage, outputImage, blur);
    }
}


void MainWindow::PSNRChanged() {
    // Non-linear transformation
    quality = ui->sliderPSNR->value();
    ui->labelPSNR->setText(QString::number(quality) + "%");
    updatePreviewDeconvolution();
}


void MainWindow::motionLengthChanged() {
    motionLength = ui->sliderMotionLength->value()/10.0;
    ui->labelMotionLength->setText(QString::number(motionLength));
    updatePreviewDeconvolution();

}



void MainWindow::defectTypeChanged(int type) {
    bool motionVisible = type == 1;
    int yMotion1 = 29;
    int yMotion3 = 69;

    bool gaussianVisible = type == 2;

    // Set visibility
    ui->labelMotionLengthCaption->setVisible(motionVisible);
    ui->labelMotionLength->setVisible(motionVisible);
    ui->sliderMotionLength->setVisible(motionVisible);


    // Move controls
    if (type == 1) {
        // Out of Focus Blur
        ui->labelMotionLengthCaption->move( ui->labelMotionLengthCaption->x(), yMotion1);
        ui->labelMotionLength->move( ui->labelMotionLength->x(), yMotion1);
        ui->sliderMotionLength->move( ui->sliderMotionLength->x(), yMotion1);


        ui->labelPSNRCaption->move( ui->labelPSNRCaption->x(), yMotion3);
        ui->labelPSNR->move( ui->labelPSNR->x(), yMotion3);
        ui->sliderPSNR->move( ui->sliderPSNR->x(), yMotion3);
    } else {
        // Motion Blur

//        ui->labelPSNRCaption->move( ui->labelPSNRCaption->x(), yFocus2);
//        ui->labelPSNR->move( ui->labelPSNR->x(), yFocus2);
//        ui->sliderPSNR->move( ui->sliderPSNR->x(), yFocus2);
    }

    updateFullDeconvolution();
}

void MainWindow::dragEnterEvent(QDragEnterEvent *event)
{
    event->acceptProposedAction();
}

void MainWindow::dragMoveEvent(QDragMoveEvent *event)
{
    event->acceptProposedAction();
}


void MainWindow::dropEvent(QDropEvent *event)
{
    QList<QUrl> urls = event->mimeData()->urls();
    if (urls.isEmpty()) {
        return;
    }

    QString fileName = urls.first().toLocalFile();
    if (fileName.isEmpty()) {
        return;
    }
    event->acceptProposedAction();
    openFile(fileName);

}

void MainWindow::openFile(QString fileName) {
    delete(inputImage);
    delete(outputImage);
    inputImage = new QImage(fileName);

    if (inputImage->isNull()) {
        QMessageBox::information(this, tr("blur"),
                                 tr("Не считать %1.").arg(fileName));
        return;
    }
    QString file = fileName.section("/",-1,-1);
    file = file.section(".",0,0);
    FRAME_NUMBER = file.toDouble();
    // Resize image if it's necessary
    int width = inputImage->width();
    int height = inputImage->height();

    if (width > MAX_IMAGE_DIMENSION || height > MAX_IMAGE_DIMENSION) {
        double resizeRatio = qMin(MAX_IMAGE_DIMENSION/width, MAX_IMAGE_DIMENSION/height);
        width = width*resizeRatio;
        height = height*resizeRatio;

        width += width % 2;
        height += height % 2;

        inputImage = new QImage(inputImage->scaled(
                                    width, height,
                                    Qt::IgnoreAspectRatio, Qt::SmoothTransformation));

        QMessageBox::information(this, tr("blur"),
                                 tr("Изображение уменьшено %1 * %2 для увеличения скорости обработки")
                                 .arg(width).arg(height));
    }

    // Crop image if sizes are odd
    if (width%2 != 0 || height%2 !=0) {
        width -= width % 2;
        height -= height % 2;
        inputImage = new QImage(inputImage->copy(0,0, width, height));
    }

    lblImageSize->setText(tr(" Кадр: %1 x %2 №: %3").arg(inputImage->width()).arg(inputImage->height()).arg(file));

    ui->btnSave->setEnabled(true);
    ui->btnShowOriginal->setEnabled(true);

    outputImage = new QImage(inputImage->width(), inputImage->height(), QImage::Format_RGB32);
    lblThreadsCount->setText(tr(" %1 ").arg(workerThread->initFFT(inputImage)));
    imageLabel->setPixmap(QPixmap::fromImage(*inputImage));
    // updateFullDeconvolution();
    //ui->checkBoxFitToWindow->setChecked(true);
    fitToWindow();
}

void MainWindow::open() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        openFile(fileName);
    }
}

void MainWindow::save() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    tr("Save File"), QDir::currentPath(),tr("Images (*.png *.jpg)"));
    if (!fileName.isEmpty()) {
        outputImage->save(fileName);
    }
}

void MainWindow::zoomIn() {
    //ui->checkBoxFitToWindow->setChecked(false);
    scaleImage(scaleFactor*2.0);
}

void MainWindow::zoomOut() {
    //ui->checkBoxFitToWindow->setChecked(false);
    scaleImage(scaleFactor*0.5);
}

void MainWindow::actualSize() {
    //ui->checkBoxFitToWindow->setChecked(false);
    scaleFactor = 1;
    scaleImage(scaleFactor);
}

void MainWindow::fitToWindow() {
    if (!imageLabel->pixmap()) {
        return;
    }

    double factor = qMin(
                ui->scrollArea->width()/((double)inputImage->width()),
                ui->scrollArea->height()/((double)inputImage->height()));
    if (factor > 1) {
        factor = 1;
    }
    scaleImage(factor);
}

void MainWindow::help() {
    helpDialog->exec();
    if (helpDialog->isOpenExampleAfterClose()) {
        openFile(":/SmartDeblur/Icons/BlurExample1.jpg");
    }
}

void MainWindow::showOriginalPressed() {
    imageLabel->setPixmap(QPixmap::fromImage(*inputImage));
}

void MainWindow::showOriginalReleased() {
    imageLabel->setPixmap(QPixmap::fromImage(*outputImage));
}


void MainWindow::scaleImage(double factor) {
    if (!imageLabel->pixmap()) {
        return;
    }

    scaleFactor =factor;
    imageLabel->resize(scaleFactor * imageLabel->pixmap()->size());

    adjustScrollBar(ui->scrollArea->horizontalScrollBar(), factor);
    adjustScrollBar(ui->scrollArea->verticalScrollBar(), factor);

    //ui->btnZoomIn->setEnabled(scaleFactor < 1.0);
    //ui->btnZoomOut->setEnabled(scaleFactor > 0.3);
}

void MainWindow::initControls() {
    ui->sliderPSNR->setValue(29);

    ui->sliderMotionLength->setValue(1);

    defectTypeChanged(1);

    ui->btnSave->setEnabled(false);
    ui->btnShowOriginal->setEnabled(false);
    ui->tableWidgetCSV->setEnabled(true);
    ui->doubleSpinBoxG->setValue(G_ACCELERATION);
    ui->doubleSpinBoxShutter->setValue(0.0166666);

    this->setAcceptDrops(true);
}

void MainWindow::createActions() {
    //connect(ui->btnZoomIn, SIGNAL(clicked()), this, SLOT(zoomIn()));
    //connect(ui->btnZoomOut, SIGNAL(clicked()), this, SLOT(zoomOut()));
    //connect(ui->btnZoomNormal, SIGNAL(clicked()), this, SLOT(actualSize()));
    //connect(ui->checkBoxFitToWindow, SIGNAL(stateChanged(int)), SLOT(updateZoomControls()));

    connect(ui->btnOpen, SIGNAL(clicked()), this, SLOT(open()));
    connect(ui->btnSave, SIGNAL(clicked()), this, SLOT(save()));
    //connect(ui->btnAbout, SIGNAL(clicked()), this, SLOT(help()));

    connect(ui->sliderMotionLength, SIGNAL(valueChanged(int)), this, SLOT(motionLengthChanged()));
    connect(ui->sliderPSNR, SIGNAL(valueChanged(int)), this, SLOT(PSNRChanged()));

    connect(ui->sliderPSNR, SIGNAL(sliderReleased()), this, SLOT(updateFullDeconvolution()));
    connect(ui->sliderMotionLength, SIGNAL(sliderReleased()), this, SLOT(updateFullDeconvolution()));

    connect(ui->btnShowOriginal, SIGNAL(pressed()), this, SLOT(showOriginalPressed()));
    connect(ui->btnShowOriginal, SIGNAL(released()), this, SLOT(showOriginalReleased()));

    //connect(ui->, SIGNAL())

    connect(workerThread, SIGNAL(deconvolutionFinished(int)), SLOT(updatePreviewImage(int)));
    connect(workerThread->getDeconvolutionTool(), SIGNAL(progressEvent(int, QString)), this, SLOT(updateProgress(int, QString)));


    connect(ui->btnOpen_AccCSV, SIGNAL(clicked()), this, SLOT(openAccCSV()));
    connect(ui->doubleSpinBoxG, SIGNAL(valueChanged(double)), SLOT(gAccelerationChanged(double)));
    connect(ui->doubleSpinBoxShutter, SIGNAL(valueChanged(double)), SLOT(shutterChanged(double)));

    connect(ui->tvIterationsCountSpinBox, SIGNAL(valueChanged(int)), SLOT(tvIterationsCountChanged(int)));
    connect(ui->previewMethodComboBox, SIGNAL(currentIndexChanged(int)), SLOT(previewMethodChanged(int)));
}

void MainWindow::adjustScrollBar(QScrollBar *scrollBar, double factor) {
    scrollBar->setValue(int(factor * scrollBar->value()
                            + ((factor - 1) * scrollBar->pageStep()/2)));
}

void MainWindow::resizeEvent(QResizeEvent *resizeEvent) {
    int w = resizeEvent->size().width();
//    if (ui->checkBoxFitToWindow->isChecked()) {
//        fitToWindow();
//        w += w;
//    }
}

void MainWindow::updateZoomControls() {
//    bool fitChecked = ui->checkBoxFitToWindow->isChecked();

//    ui->scrollArea->setVerticalScrollBarPolicy(!fitChecked ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);
//    ui->scrollArea->setHorizontalScrollBarPolicy(!fitChecked ? Qt::ScrollBarAsNeeded : Qt::ScrollBarAlwaysOff);

//    if (fitChecked) {
//        fitToWindow();
//    }
}
const QString MainWindow::allFileToString(QFile &aFile)
{
    if (!aFile.open(QFile::ReadOnly | QFile::Text)) {
        return NULL;
    }
    QTextStream in(&aFile);
    return in.readAll();
}
//void MainWindow::setDataToVectorXY(QVector< QVector <double> > &aVector)
//{

//}
void MainWindow::setDataToVector(const QStringList &aStringList,
                     QVector< QVector <double> > &aVector)
{
    size_t x = aStringList.size() - 1; // Count of line, 8
    size_t y = aStringList.at(0).count(",") + 1; // Count of digits in line, 6
//vector Acc
    for (size_t i = 0; i < x; ++i) {
        QVector<double> temp_vector;
        for (size_t j = 0; j < y; ++j) {
            if(j==0)
                temp_vector.push_back((aStringList.at(i).split(",").at(j).toDouble()-G_ACCELERATION)*(-1.0));
            else if(j==1 || j==2)
                temp_vector.push_back((aStringList.at(i).split(",").at(j).toDouble())*(-1.0));
            else
                temp_vector.push_back(aStringList.at(i).split(",").at(j).toDouble());
        }
        if(temp_vector.at(3) >= (FRAME_NUMBER - FRAME_NUMBER_MIN) && temp_vector.at(3) <= FRAME_NUMBER){
            temp_vector.push_back(0.0);//x
            temp_vector.push_back(0.0);//y
            aVector.push_back(temp_vector);
        }
    }

//vector X Y
    QVector< QVector <double> > vectorV;
    vectorV.push_back({0,0});
    for (int i = 1; i < aVector.size(); ++i) {

        QVector<double> temp_vectorV;
        double t = (aVector.at(i).at(3)-aVector.at(i-1).at(3))/1000000000;
        temp_vectorV.push_back(0.5*(aVector.at(i-1).at(0)+aVector.at(i).at(0))*t+vectorV.at(i-1).at(0));
        temp_vectorV.push_back(0.5*(aVector.at(i-1).at(1)+aVector.at(i).at(1))*t+vectorV.at(i-1).at(1));
        vectorV.push_back(temp_vectorV);
    }
    //QVector< QVector <double> > vectorXY;
    if(vectorXY.size()>0) vectorXY.erase(vectorXY.begin(),vectorXY.end());
    vectorXY.push_back({0,0});
    for (int i = 1; i < aVector.size(); ++i) {

        QVector<double> temp_vectorXY;
        double t = (aVector.at(i).at(3)-aVector.at(i-1).at(3))/1000000000;
        temp_vectorXY.push_back(0.25*(aVector.at(i-1).at(0)+aVector.at(i).at(0))*t*t+vectorV.at(i-1).at(0)+vectorXY.at(i-1).at(0));
        temp_vectorXY.push_back(0.25*(aVector.at(i-1).at(1)+aVector.at(i).at(1))*t*t+vectorV.at(i-1).at(1)+vectorXY.at(i-1).at(1));
        aVector[i][4]=temp_vectorXY.at(0);
        aVector[i][5]=temp_vectorXY.at(1);
        vectorXY.push_back(temp_vectorXY);
    }

}

void MainWindow::openFileAccCSV(QString fileName) {
    QVector< QVector <double> > vector;
    QFile file(fileName);
    setDataToVector(allFileToString(file).split("\n"), vector);
    ui->tableWidgetCSV->setRowCount(0);
    ui->tableWidgetCSV->setColumnCount(6);
    QHeaderView *verticalHeader = ui->tableWidgetCSV->verticalHeader();
    verticalHeader->setSectionResizeMode(QHeaderView::Fixed);
    verticalHeader->setDefaultSectionSize(16);
    for (int i = 0; i < vector.size(); ++i) {
        ui->tableWidgetCSV->setRowCount(ui->tableWidgetCSV->rowCount()+1);
        for (int j = 0; j < vector.at(0).size(); ++j) {
            QString cellStr;
            if(j==3)
                cellStr=QString("%1").arg(QString::number(vector.at(i).at(j), 'f', 0));
            else
                cellStr=QString("%1").arg(QString::number(vector.at(i).at(j), 'f', 10));
            QTableWidgetItem* ptwi =new QTableWidgetItem(cellStr);
            ui->tableWidgetCSV->setItem(i, j, ptwi);        //заполняется виждет таблицы всеми данными
        }
     }
}

void MainWindow::openAccCSV() {
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"), QDir::currentPath());
    if (!fileName.isEmpty()) {
        fileNameCSV = fileName;
        openFileAccCSV(fileNameCSV);
    }
}
void MainWindow::imageSizeLimitChanged(int value) {
    MAX_IMAGE_DIMENSION = value;
}
void MainWindow::gAccelerationChanged(double value) {
    G_ACCELERATION = value;
    if (!fileNameCSV.isEmpty()) {
        openFileAccCSV(fileNameCSV);
    }
}
void MainWindow::shutterChanged(double value) {
    FRAME_NUMBER_MIN = value*1000000000;
    if (!fileNameCSV.isEmpty()) {
        openFileAccCSV(fileNameCSV);
    }
}

void MainWindow::tvIterationsCountChanged(int value) {
    workerThread->getDeconvolutionTool()->setTVIterationsCount(value);
}

void MainWindow::previewMethodChanged(int value) {
    workerThread->getDeconvolutionTool()->setPreviewMethod(value);
}
