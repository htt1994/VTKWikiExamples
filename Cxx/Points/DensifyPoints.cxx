#include <vtkSmartPointer.h>
#include <vtkDensifyPointCloudFilter.h>
#include <vtkXMLPolyDataReader.h>

#include <vtkSphereSource.h>
#include <vtkGlyph3D.h>

#include <vtkPolyDataMapper.h>
#include <vtkProperty.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkCamera.h>

int main (int argc, char *argv[])
{
  if (argc < 2)
  {
    std::cout << "Usage: " << argv[0] << " file.vtp" << std::endl;
    return EXIT_FAILURE;
  }
  vtkSmartPointer<vtkXMLPolyDataReader> reader =
    vtkSmartPointer<vtkXMLPolyDataReader>::New();
  reader->SetFileName (argv[1]);
  reader->Update();

  double bounds[6];
  reader->GetOutput()->GetBounds(bounds);
  double range[3];
  for (int i = 0; i < 3; ++i)
  {
    range[i] = bounds[2*i + 1] - bounds[2*i];
  }
  std::cout << "Range: "
            << range[0] << ", "
            << range[1] << ", "
            << range[2] << std::endl;
  std::cout << "# of original points: " << reader->GetOutput()->GetNumberOfPoints() << std::endl;
  vtkSmartPointer<vtkDensifyPointCloudFilter> densify =
    vtkSmartPointer<vtkDensifyPointCloudFilter>::New();
  densify->SetInputConnection(reader->GetOutputPort());
  densify->SetMaximumNumberOfIterations(5);
  densify->SetTargetDistance(.01);
  densify->SetTargetDistance(range[0] * .03);
  densify->SetNumberOfClosestPoints(10);
  densify->Update();
  std::cout << "# of densified points: " << densify->GetOutput()->GetNumberOfPoints() << std::endl;

///
  double radius = range[0] * .01;
  vtkSmartPointer<vtkSphereSource> sphereSource1 =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource1->SetRadius(radius);

  vtkSmartPointer<vtkGlyph3D> glyph3D1 =
    vtkSmartPointer<vtkGlyph3D>::New();
  glyph3D1->SetInputConnection(reader->GetOutputPort());
  glyph3D1->SetSourceConnection(sphereSource1->GetOutputPort());
  glyph3D1->ScalingOff();
  glyph3D1->Update();
  vtkSmartPointer<vtkPolyDataMapper> glyph3DMapper1 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  glyph3DMapper1->SetInputConnection(glyph3D1->GetOutputPort());
  glyph3DMapper1->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> glyph3DActor1 =
    vtkSmartPointer<vtkActor>::New();
  glyph3DActor1->SetMapper(glyph3DMapper1);
  glyph3DActor1->GetProperty()->SetColor(0.8900, 0.8100, 0.3400);

////
  vtkSmartPointer<vtkSphereSource> sphereSource2 =
    vtkSmartPointer<vtkSphereSource>::New();
  sphereSource2->SetRadius(radius * .75);

  vtkSmartPointer<vtkGlyph3D> glyph3D2 =
    vtkSmartPointer<vtkGlyph3D>::New();
  glyph3D2->SetInputConnection(densify->GetOutputPort());
  glyph3D2->SetSourceConnection(sphereSource2->GetOutputPort());
  glyph3D2->ScalingOff();
  glyph3D2->Update();

  vtkSmartPointer<vtkPolyDataMapper> glyph3DMapper2 =
    vtkSmartPointer<vtkPolyDataMapper>::New();
  glyph3DMapper2->SetInputConnection(glyph3D2->GetOutputPort());
  glyph3DMapper2->ScalarVisibilityOff();

  vtkSmartPointer<vtkActor> glyph3DActor2 =
    vtkSmartPointer<vtkActor>::New();
  glyph3DActor2->SetMapper(glyph3DMapper2);
  glyph3DActor2->GetProperty()->SetColor(1.0000, 0.4900, 0.2500);

  // Create graphics stuff
  //
  vtkSmartPointer<vtkRenderer> ren1 =
    vtkSmartPointer<vtkRenderer>::New();
  ren1->SetBackground(.3, .4, .6);

  vtkSmartPointer<vtkRenderWindow> renWin =
    vtkSmartPointer<vtkRenderWindow>::New();
  renWin->AddRenderer(ren1);
  renWin->SetSize(512,512);

  vtkSmartPointer<vtkRenderWindowInteractor> iren =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
  iren->SetRenderWindow(renWin);
  
  // Add the actors to the renderer, set the background and size
  //
  ren1->AddActor(glyph3DActor1);
  ren1->AddActor(glyph3DActor2);

  // Generate an interesting view
  //
  ren1->ResetCamera();
  ren1->GetActiveCamera()->SetPosition (1, 0, 0);
  ren1->GetActiveCamera()->SetFocalPoint (0, 1, 0);
  ren1->GetActiveCamera()->SetViewUp (0, 0, 1);
  ren1->ResetCamera();
  ren1->GetActiveCamera()->Dolly(1.0);
  ren1->ResetCameraClippingRange();

  iren->Initialize();
  iren->Start();

  return EXIT_SUCCESS;
}
