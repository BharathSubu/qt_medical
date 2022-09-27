#include "dicomwidget.h"
#include "ui_dicomwidget.h"



DicomWidget::DicomWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DicomWidget)
{
    ui->setupUi(this);

}

void DicomWidget::setImage(const QString &text , const QString label) {
   // ui->diWidget->setImage(text);
    std::string fileDICOM = text.toUtf8().constData();
      ui->diLabel->setText(label);
      ui->diWidget->setWhatsThis(text);
    qDebug()<<"My path"<< text;
    vtkSmartPointer<vtkDICOMImageReader> reader =
        vtkSmartPointer<vtkDICOMImageReader>::New();
      reader->SetFileName(fileDICOM.c_str());
      reader->Update();

      vtkSmartPointer<vtkImageViewer2> imageViewer =
        vtkSmartPointer<vtkImageViewer2>::New();
      imageViewer->SetInputConnection(reader->GetOutputPort());
      imageViewer->SetRenderWindow(ui->diWidget->GetRenderWindow());
      imageViewer->Render();
}

void DicomWidget::mouseDoubleClickEvent(QMouseEvent *event){
    qDebug() << ui->diWidget->whatsThis();
    QString path = ui->diWidget->whatsThis();
    EditWindow * editWindow = new EditWindow(this);
    editWindow->setWindowTitle(path);
    editWindow->setWhatsThis(path);
    editWindow->show();
}


QString DicomWidget::getText() {
    return ui->diWidget->whatsThis();
}

DicomWidget::~DicomWidget()
{
    delete ui;
}
