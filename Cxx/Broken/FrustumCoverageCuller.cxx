#include <vtkSphereSource.h>
#include <vtkPolyData.h>
#include <vtkSmartPointer.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>

#include <vtkCullerCollection.h>
#include <vtkFrustumCoverageCuller.h>
 
int main(int argc, char *argv[])
{
  //Create a sphere
  vtkSmartPointer<vtkSphereSource> sphereSource = 
      vtkSmartPointer<vtkSphereSource>::New();
  sphereSource->SetCenter(0.0, 0.0, 0.0);
  sphereSource->SetRadius(5.0);
  sphereSource->Update();
 
  //Create a mapper and actor
  vtkSmartPointer<vtkPolyDataMapper> mapper = 
      vtkSmartPointer<vtkPolyDataMapper>::New();
  mapper->SetInputConnection(sphereSource->GetOutputPort());
  vtkSmartPointer<vtkActor> actor = 
      vtkSmartPointer<vtkActor>::New();
  actor->SetMapper(mapper);
 
  //Create a renderer, render window, and interactor
  vtkSmartPointer<vtkRenderer> renderer = 
      vtkSmartPointer<vtkRenderer>::New();
  vtkSmartPointer<vtkRenderWindow> renderWindow = 
      vtkSmartPointer<vtkRenderWindow>::New();
  renderWindow->AddRenderer(renderer);
  vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor = 
      vtkSmartPointer<vtkRenderWindowInteractor>::New();
  renderWindowInteractor->SetRenderWindow(renderWindow);
 
  static_cast<vtkFrustumCoverageCuller*>(renderer->GetCullers()->GetLastItem())->SetSortingStyleToBackToFront();
  
  //Add the actor to the scene
  renderer->AddActor(actor);
  renderer->SetBackground(1,1,1); // Background color white
 
  //Render and interact
  renderWindow->Render();
  renderWindowInteractor->Start();
 
  return EXIT_SUCCESS;
}
