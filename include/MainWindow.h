#ifndef MainWindow_H
#define MainWindow_H

#include <QMainWindow>
#include <vtkSmartPointer.h>
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <QItemSelectionModel>
#include <QFileDialog>
#include <QFileSystemModel>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QDebug>
#include "dicomwidget.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void openDICOMFolder();
    void drawDICOMSeries(std::string folderDICOM);


    void on_pushButton_clicked();
    void currentChanged(const QModelIndex & );
    void on_listWidget_doubleClicked(const QModelIndex &index);

    void on_pushButton_2_clicked();

    void on_pushButton_3_clicked();

private:
    Ui::MainWindow *ui;
    vtkSmartPointer<vtkDICOMImageReader> reader;
    vtkSmartPointer<vtkImageViewer2> viewer;
    QString currentFolder = "";
    QString currentFileDI = "";

};

#endif // MainWindow_H
