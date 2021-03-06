#include <iomanip>
#include "itkImageFileReader.h"
#include "itkImageFileWriter.h"
#include "itkCommand.h"
#include "itkSimpleFilterWatcher.h"

#include "itkParabolicCloseImageFilter.h"
#include "itkTimeProbe.h"
#include "itkMultiThreader.h"

int main(int, char *argv[])
{
  //itk::MultiThreader::SetGlobalMaximumNumberOfThreads(1);
  constexpr int dim = 2;

  //using PType = unsigned char;
  using PType = float;
  using IType = itk::Image< PType, dim >;

  using ReaderType = itk::ImageFileReader< IType >;
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);

  using FilterType = itk::ParabolicCloseImageFilter< IType, IType >;

  FilterType::Pointer filter = FilterType::New();

  filter->SetInput( reader->GetOutput() );
  filter->SetSafeBorder(true);
  FilterType::RadiusType scale;
  //scale[0]=1;
  //scale[1]=0.5;
  scale.Fill( std::stod(argv[3]) );
  filter->SetScale(scale);
//   itk::SimpleFilterWatcher watcher(filter, "filter");
  itk::TimeProbe NewTime;

  for ( unsigned i = 0; i < 1; i++ )
    {
    filter->Modified();
    NewTime.Start();
    filter->Update();
    NewTime.Stop();
    }

  using WriterType = itk::ImageFileWriter< IType >;
  WriterType::Pointer writer = WriterType::New();
  writer->SetInput( filter->GetOutput() );
  writer->SetFileName(argv[2]);
  writer->Update();
  std::cout << std::setprecision(3)
            << NewTime.GetMean() << std::endl;

  return EXIT_SUCCESS;
}
