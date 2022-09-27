#include "editwindow.h"
#include "ui_editwindow.h"

EditWindow::EditWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EditWindow)
{
    ui->setupUi(this);
    reader = vtkSmartPointer<vtkDICOMImageReader>::New();
    viewer = vtkSmartPointer<vtkImageViewer2>::New();
}

EditWindow::~EditWindow()
{
    delete ui;
}

EditWindow::EditWindow(QString& filepath){
    QFile file (filepath);
    QFileInfo fileInfo(file.fileName());
    QString filename(fileInfo.fileName());
    QString name = filename;
    currentDicomPath = filepath;
}

void EditWindow::on_pushButton_clicked()
{
     currentDicomPath =  EditWindow::windowTitle();

    qDebug() <<"Hello9" << currentDicomPath;
    std::string fileDICOM = currentDicomPath.toUtf8().constData();

          reader->SetFileName(fileDICOM.c_str());
          reader->Update();

          vtkSmartPointer<vtkImageViewer2> imageViewer =
            vtkSmartPointer<vtkImageViewer2>::New();
          imageViewer->SetInputConnection(reader->GetOutputPort());
          imageViewer->SetRenderWindow(ui->original->GetRenderWindow());
          imageViewer->Render();

          viewer->SetInputConnection(reader->GetOutputPort());
          viewer->SetRenderWindow(ui->edit->GetRenderWindow());
          viewer->Render();

          minSlice = -100;
           maxSlice =viewer->GetColorLevel() + 200;

           // Actualiza labels de la GUI
           ui->horizontalSlider->setMinimum(minSlice);
           ui->horizontalSlider->setMaximum(maxSlice);

}


void EditWindow::on_horizontalSlider_sliderMoved(int position)
{
    viewer->SetColorLevel(position);
    viewer->Render();
}



void EditWindow::on_rRotate_clicked()
{
    vtkNew<vtkNamedColors> colors;
        if(angle == 360){
            angle  = 0;
        }else;
      angle+=90;

      qDebug() <<angle<<"angle";

      // Read file
      vtkNew<vtkImageReader2Factory> readerFactory;
      std::string fileDICOM = currentDicomPath.toUtf8().constData();


     // reader.TakeReference(readerFactory->CreateImageReader2(fileDICOM.c_str()));
       reader->SetFileName(fileDICOM.c_str());
      reader->Update();
      double bounds[6];
      reader->GetOutput()->GetBounds(bounds);

      // Rotate about the center of the image
      vtkNew<vtkTransform> transform;

      // Compute the center of the image
      double center[3];
      center[0] = (bounds[1] + bounds[0]) / 2.0;
      center[1] = (bounds[3] + bounds[2]) / 2.0;
      center[2] = (bounds[5] + bounds[4]) / 2.0;

      // Rotate about the center
      transform->Translate(center[0], center[1], center[2]);
      transform->RotateWXYZ(angle, 0, 0, 1);
      transform->Translate(-center[0], -center[1], -center[2]);

      // Reslice does all of the work
      vtkNew<vtkImageReslice> reslice;
      reslice->SetInputConnection(reader->GetOutputPort());
      reslice->SetResliceTransform(transform);
      reslice->SetInterpolationModeToCubic();
      reslice->SetOutputSpacing(reader->GetOutput()->GetSpacing()[0],
                                reader->GetOutput()->GetSpacing()[1],
                                reader->GetOutput()->GetSpacing()[2]);
      reslice->SetOutputOrigin(reader->GetOutput()->GetOrigin()[0],
                               reader->GetOutput()->GetOrigin()[1],
                               reader->GetOutput()->GetOrigin()[2]);
      reslice->SetOutputExtent(
          reader->GetOutput()->GetExtent()); // Use a larger extent than the
                                             // original image's to prevent clipping
      viewer->SetInputConnection(reslice->GetOutputPort());
      vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
      viewer->SetupInteractor(renderWindowInteractor);
      viewer->GetRenderWindow()->SetWindowName("ImageRotate");
      viewer->Render();
      viewer->GetRenderer()->ResetCamera();
      viewer->Render();
      renderWindowInteractor->Start();
      viewer->Render();
}


void EditWindow::on_lRotate_clicked()
{
    vtkNew<vtkNamedColors> colors;
        if(angle == 0){
            angle  = 360;
        }else;
      angle-=90;

      qDebug() <<angle<<"angle";

      // Read file
      vtkNew<vtkImageReader2Factory> readerFactory;
      std::string fileDICOM = currentDicomPath.toUtf8().constData();


     // reader.TakeReference(readerFactory->CreateImageReader2(fileDICOM.c_str()));
       reader->SetFileName(fileDICOM.c_str());
      reader->Update();
      double bounds[6];
      reader->GetOutput()->GetBounds(bounds);

      // Rotate about the center of the image
      vtkNew<vtkTransform> transform;

      // Compute the center of the image
      double center[3];
      center[0] = (bounds[1] + bounds[0]) / 2.0;
      center[1] = (bounds[3] + bounds[2]) / 2.0;
      center[2] = (bounds[5] + bounds[4]) / 2.0;

      // Rotate about the center
      transform->Translate(center[0], center[1], center[2]);
      transform->RotateWXYZ(angle, 0, 0, 1);
      transform->Translate(-center[0], -center[1], -center[2]);

      // Reslice does all of the work
      vtkNew<vtkImageReslice> reslice;
      reslice->SetInputConnection(reader->GetOutputPort());
      reslice->SetResliceTransform(transform);
      reslice->SetInterpolationModeToCubic();
      reslice->SetOutputSpacing(reader->GetOutput()->GetSpacing()[0],
                                reader->GetOutput()->GetSpacing()[1],
                                reader->GetOutput()->GetSpacing()[2]);
      reslice->SetOutputOrigin(reader->GetOutput()->GetOrigin()[0],
                               reader->GetOutput()->GetOrigin()[1],
                               reader->GetOutput()->GetOrigin()[2]);
      reslice->SetOutputExtent(
          reader->GetOutput()->GetExtent()); // Use a larger extent than the
                                             // original image's to prevent clipping

      viewer->SetInputConnection(reslice->GetOutputPort());
      vtkNew<vtkRenderWindowInteractor> renderWindowInteractor;
      viewer->SetupInteractor(renderWindowInteractor);

      viewer->GetRenderWindow()->SetWindowName("ImageRotate");
      viewer->Render();
      viewer->GetRenderer()->ResetCamera();
      viewer->Render();

      renderWindowInteractor->Start();

      viewer->Render();
}



void EditWindow::on_sharpnessSlider_sliderMoved(int position)
{
    vtkNew<vtkNamedColors> colors;




     // Define viewport ranges
     // (xmin, ymin, xmax, ymax)
     double originalViewport[4] = {0.0, 0.0, 0.5, 1.0};
     double edgeViewport[4] = {0.5, 0.0, 1.0, 1.0};

     vtkNew<vtkRenderer> originalRenderer;
     originalRenderer->SetViewport(originalViewport);
     originalRenderer->SetBackground(colors->GetColor3d("SlateGray").GetData());
     vtkNew<vtkRenderer> edgeRenderer;
     edgeRenderer->SetViewport(edgeViewport);
     edgeRenderer->SetBackground(colors->GetColor3d("LightSlateGray").GetData());

     vtkNew<vtkRenderWindow> renderWindow;
     renderWindow->SetSize(600, 300);
     renderWindow->SetMultiSamples(0);
     renderWindow->AddRenderer(originalRenderer);
     renderWindow->AddRenderer(edgeRenderer);
     renderWindow->SetWindowName("CannyEdgeDetector");

     vtkNew<vtkRenderWindowInteractor> interactor;
     interactor->SetRenderWindow(renderWindow);

     vtkNew<vtkPNGReader> imageIn;
        std::string fileDICOM = currentDicomPath.toUtf8().constData();
     imageIn->SetFileName(fileDICOM.c_str());
     imageIn->Update();

     vtkNew<vtkImageActor> imageActor;
     imageActor->SetInputData(imageIn->GetOutput());

     originalRenderer->AddActor(imageActor);

     vtkNew<vtkImageLuminance> il;
     il->SetInputConnection(imageIn->GetOutputPort());

     vtkNew<vtkImageCast> ic;
     ic->SetOutputScalarTypeToFloat();
     ic->SetInputConnection(il->GetOutputPort());

     // Smooth the image
     vtkNew<vtkImageGaussianSmooth> gs;
     gs->SetInputConnection(ic->GetOutputPort());
     gs->SetDimensionality(2);
     gs->SetRadiusFactors(1, 1, 0);

     // Gradient the image
     vtkNew<vtkImageGradient> imgGradient;
     imgGradient->SetInputConnection(gs->GetOutputPort());
     imgGradient->SetDimensionality(2);

     vtkNew<vtkImageMagnitude> imgMagnitude;
     imgMagnitude->SetInputConnection(imgGradient->GetOutputPort());

     // Non maximum suppression
     vtkNew<vtkImageNonMaximumSuppression> nonMax;
     imgMagnitude->Update();
     nonMax->SetMagnitudeInputData(imgMagnitude->GetOutput());
     imgGradient->Update();
     nonMax->SetVectorInputData(imgGradient->GetOutput());
     nonMax->SetDimensionality(2);

     vtkNew<vtkImageConstantPad> pad;
     pad->SetInputConnection(imgGradient->GetOutputPort());
     pad->SetOutputNumberOfScalarComponents(3);
     pad->SetConstant(0);

     vtkNew<vtkImageToStructuredPoints> i2sp1;
     i2sp1->SetInputConnection(nonMax->GetOutputPort());
     pad->Update();
     i2sp1->SetVectorInputData(pad->GetOutput());

     // Link edgles
     vtkNew<vtkLinkEdgels> imgLink;
     imgLink->SetInputConnection(i2sp1->GetOutputPort());
     imgLink->SetGradientThreshold(2);

     // Threshold links
     vtkNew<vtkThreshold> thresholdEdges;
     thresholdEdges->SetInputConnection(imgLink->GetOutputPort());
     thresholdEdges->ThresholdByUpper(10);
//     thresholdEdges->SetThresholdFunction(vtkThreshold::THRESHOLD_UPPER);
     thresholdEdges->AllScalarsOff();

     vtkNew<vtkGeometryFilter> gf;
     gf->SetInputConnection(thresholdEdges->GetOutputPort());

     vtkNew<vtkImageToStructuredPoints> i2sp;
     i2sp->SetInputConnection(imgMagnitude->GetOutputPort());
     pad->Update();
     i2sp->SetVectorInputData(pad->GetOutput());

     // Subpixel them
     vtkNew<vtkSubPixelPositionEdgels> spe;
     spe->SetInputConnection(gf->GetOutputPort());
     i2sp->Update();
     spe->SetGradMapsData(i2sp->GetStructuredPointsOutput());

     vtkNew<vtkStripper> strip;
     strip->SetInputConnection(spe->GetOutputPort());

     vtkNew<vtkPolyDataMapper> dsm;
     dsm->SetInputConnection(strip->GetOutputPort());
     dsm->ScalarVisibilityOff();

     vtkNew<vtkActor> planeActor;
     planeActor->SetMapper(dsm);
     planeActor->GetProperty()->SetAmbient(1.0);
     planeActor->GetProperty()->SetDiffuse(0.0);
     planeActor->GetProperty()->SetColor(
         colors->GetColor3d("GhostWhite").GetData());

     // Add the actors to the renderer, set the background and size
     edgeRenderer->AddActor(planeActor);

     // Render the image
     interactor->Initialize();
     renderWindow->Render();
     renderWindow->Render();

     interactor->Start();




}

