
#ifndef _AbsoluteVolumeErrorImageToImageMetric_txx
#define _AbsoluteVolumeErrorImageToImageMetric_txx

#include "AbsoluteVolumeErrorImageToImageMetric.h"

#include "itkImageRegionIterator.h"

template <class TFixedImage, class TMovingImage>
AbsoluteVolumeErrorImageToImageMetric<TFixedImage, TMovingImage>
::AbsoluteVolumeErrorImageToImageMetric()
{

}

template <class TFixedImage, class TMovingImage>
AbsoluteVolumeErrorImageToImageMetric<TFixedImage, TMovingImage>
::~AbsoluteVolumeErrorImageToImageMetric()
{

}

template <class TFixedImage, class TMovingImage>
typename AbsoluteVolumeErrorImageToImageMetric<TFixedImage,TMovingImage>::MeasureType
AbsoluteVolumeErrorImageToImageMetric<TFixedImage, TMovingImage>
::GetValue() const
{
  if (Superclass::m_FixedImage.IsNull() || Superclass::m_MovingImage.IsNull())
    itkExceptionMacro(<< "Need two input classification images");

  // Define iterators
  typedef itk::ImageRegionConstIterator<FixedImageType> FixedIteratorType;
  typedef itk::ImageRegionConstIterator<MovingImageType> MovingIteratorType;

  FixedIteratorType fixedIt(Superclass::m_FixedImage, Superclass::m_FixedImage->GetRequestedRegion());
  MovingIteratorType movingIt(Superclass::m_MovingImage, Superclass::m_MovingImage->GetRequestedRegion());

  // Get intersection and individual set sizes
  unsigned int numA = 0;
  unsigned int numB = 0;

  fixedIt.GoToBegin();
  movingIt.GoToBegin();
  while (!fixedIt.IsAtEnd() && !movingIt.IsAtEnd())
  {
    unsigned int r = (unsigned int)fixedIt.Get();
    unsigned int c = (unsigned int)movingIt.Get();

    bool a = (r != 0);
    bool b = (c != 0);
      
    if (a)
      numA++;

    if (b)
      numB++;

    ++fixedIt;
    ++movingIt;
  }

  float spacing[3];

  spacing[0] = Superclass::m_FixedImage->GetSpacing()[0];
  spacing[1] = Superclass::m_FixedImage->GetSpacing()[1];
  spacing[2] = Superclass::m_FixedImage->GetSpacing()[2];

  float volumeFixed;
  float volumeMoving;
 
  volumeFixed = numA * spacing[0]*spacing[1]*spacing[2];
  volumeMoving = numB * spacing[0]*spacing[1]*spacing[2];

  double absVolErr = volumeMoving - volumeFixed;

  return absVolErr;
}

#endif
