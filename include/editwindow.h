#ifndef EDITWINDOW_H
#define EDITWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <QDebug>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeWidgetItem>
#include <QMimeDatabase>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageReader2.h>
#include <vtkImageReader2Factory.h>
#include <vtkImageReslice.h>
#include <vtkImageViewer2.h>
#include <vtkInteractorStyleImage.h>
#include <vtkRenderer.h>
#include <vtkTransform.h>
#include <vtkCamera.h>
#include <vtkGeometryFilter.h>
#include <vtkImageCast.h>
#include <vtkImageConstantPad.h>
#include <vtkImageGaussianSmooth.h>
#include <vtkImageGradient.h>
#include <vtkImageLuminance.h>
#include <vtkImageMagnitude.h>
#include <vtkImageNonMaximumSuppression.h>
#include <vtkImageToStructuredPoints.h>
#include <vtkLinkEdgels.h>
#include <vtkNamedColors.h>
#include <vtkNew.h>
#include <vtkPNGReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkStripper.h>
#include <vtkStructuredPoints.h>
#include <vtkSubPixelPositionEdgels.h>
#include <vtkThreshold.h>


namespace Ui {
class EditWindow;
}

class EditWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit EditWindow(QWidget *parent = nullptr);
    EditWindow( QString& filepath  );

    ~EditWindow();

private slots:
    void on_pushButton_clicked();
    void on_horizontalSlider_sliderMoved(int position);
    void on_rRotate_clicked();
    void on_lRotate_clicked();



    void on_sharpnessSlider_sliderMoved(int position);

private:
    Ui::EditWindow *ui;
    vtkSmartPointer<vtkDICOMImageReader> reader;
    vtkSmartPointer<vtkImageViewer2> viewer;
    int minSlice;
    int maxSlice;
    QString currentDicomPath =  EditWindow::windowTitle();
    double angle = 0;
};

#endif // EDITWINDOW_H
