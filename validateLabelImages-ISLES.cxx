
// Executable for computing image metrics
// Usage: <prog> truth.nii[.gz] test.nii[.gz]
//
// Metrics returned (semicolon-separated):
//   1. Average symmetric surface distance (ASSD)
//   2. Dice's Coefficient (DC)
//   3. Hausdorff Distance (HD)
//	 4. Precision = positive predictive value (precision)
// 	 5. Recall = true positive rate = sensitivity (recall)
//
// Error return codes:
//	 0: success
//  -1: wrong number of input parameters
//  -2: failed to read input image(s)
//  -3: incompatible input images
//	-4: exception thrown during metric computation
//
// prastawa@sci.utah.edu
// modified stefan.bauer@istb.unibe.ch
// adapted for www.isles-challenge.org by maier@imi.uni-luebeck.de

#include "itkImage.h"
#include "itkImageFileReader.h"

#include "ImageToImageValidator.h"

#include "AverageDistanceImageToImageMetric.h"
#include "HausdorffDistanceImageToImageMetric.h"
#include "DiceOverlapImageToImageMetric.h"
#include "SensitivityImageToImageMetric.h"
#include "PositivePredictiveValueImageToImageMetric.h"

#include <exception>
#include <iostream>
#include <string>

typedef itk::Image<unsigned short, 3> ImageType;

int
validateLabelImage(ImageType::Pointer fixedImage, ImageType::Pointer movingImage)
{
  // input assumes to be binary and valid

  // Metrics for binary data
  typedef AverageDistanceImageToImageMetric<ImageType, ImageType>
	  AverageDistanceMetricType;
  typedef DiceOverlapImageToImageMetric<ImageType, ImageType>
  	  DiceMetricType;
  typedef HausdorffDistanceImageToImageMetric<ImageType, ImageType>
  	  HausdorffDistanceMetricType;
  typedef SensitivityImageToImageMetric<ImageType, ImageType>
  	  SensitivityMetricType;
  typedef PositivePredictiveValueImageToImageMetric<ImageType, ImageType>
  	  PPVMetricType;

  AverageDistanceMetricType::Pointer assd = AverageDistanceMetricType::New();
  assd->SetFixedImage(fixedImage);
  assd->SetMovingImage(movingImage);
  std::cout << assd->GetValue() << ";";

  DiceMetricType::Pointer dc = DiceMetricType::New();
  dc->SetFixedImage(fixedImage);
  dc->SetMovingImage(movingImage);
  std::cout << dc->GetValue() << ";";

  HausdorffDistanceMetricType::Pointer hd = HausdorffDistanceMetricType::New();
  hd->SetFixedImage(fixedImage);
  hd->SetMovingImage(movingImage);
  hd->SetPercentile(1.0);
  std::cout << hd->GetValue() << ";";

  PPVMetricType::Pointer precision = PPVMetricType::New();
  precision->SetFixedImage(fixedImage);
  precision->SetMovingImage(movingImage);
  std::cout << precision->GetValue() << ";";

  SensitivityMetricType::Pointer recall = SensitivityMetricType::New();
  recall->SetFixedImage(fixedImage);
  recall->SetMovingImage(movingImage);
  std::cout << recall->GetValue() << std::endl;
  
  return 0;
}

int
main(int argc, char** argv)
{
  if (argc != 3)
  {
	std::cerr << "computes assd, dc, hd, precision and recall between two binary images" << std::endl;
    std::cerr << "usage: " << argv[0] << " truth-image test-image" << std::endl;
    return -1;
  }

  // read images
  typedef itk::ImageFileReader<ImageType> ReaderType;

  // read truth image
  ReaderType::Pointer freader = ReaderType::New();
  freader->SetFileName(argv[1]);
  try
  {
	freader->Update();
  }
  catch ( itk::ExceptionObject &exception )
  {
	std::cerr << "Exception caught while reading truth-image!" << std::endl;
	std::cerr << &exception << std::endl;
	return -2;
  }
  ImageType::Pointer truthImage = freader->GetOutput();

  // read test image
  ReaderType::Pointer mreader = ReaderType::New();
  mreader->SetFileName(argv[2]);
  try
  {
	mreader->Update();
  }
  catch ( itk::ExceptionObject &exception )
  {
	std::cerr << "Exception caught while reading!" << std::endl;
	std::cerr << &exception << std::endl;
	return -2;
  }
  ImageType::Pointer testImage = mreader->GetOutput();

  // validate image compatibility
  typedef ImageToImageValidator<ImageType, ImageType>
  	  ImageToImageValidatorType;
  ImageToImageValidatorType::Pointer validator = ImageToImageValidatorType::New();
  validator->SetFixedImage(truthImage);
  validator->SetMovingImage(testImage);
  if (!(0 == validator->GetValue()))
  {
	  std::cerr << "Incompatible input images (according to origin, spacing, direction or largest possible region)! " << std::endl;
	  return -3;
  }

  // compute metrics
  try
  {
	  validateLabelImage(truthImage, testImage);
  } 
  catch ( itk::ExceptionObject& e )
  {
    std::cerr << e << std::endl;
    return -4;
  }
  catch ( std::exception& e )
  {
    std::cerr << "Exception: " << e.what() << std::endl;
    return -4;
  }
  catch ( std::string& s )
  {
    std::cerr << "Exception: " << s << std::endl;
    return -4;
  }
  catch (...)
  {
    std::cerr << "Unknown exception" << std::endl;
    return -4;
  }

  return 0;
}
