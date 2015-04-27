
// absolute volume error
// Input: binary images

#ifndef _AbsoluteVolumeErrorImageToImageMetric_h
#define _AbsoluteVolumeErrorImageToImageMetric_h

#include "AbstractValidationMetric.h"

#include "itkImageToImageMetric.h"
#include "itkSmartPointer.h"

template <class TFixedImage, class TMovingImage>
class ITK_EXPORT AbsoluteVolumeErrorImageToImageMetric :
  public itk::ImageToImageMetric<TFixedImage, TMovingImage>, public AbstractValidationMetric
{

public:

  // Validation metric properties
  bool IsInputBinary() { return true; }
  bool IsSymmetric() { return true; }
  double GetBestScore() { return 1.0; }
  double GetWorstScore() { return 0.0; }

  /** Standard class typedefs. */
  typedef AbsoluteVolumeErrorImageToImageMetric           Self;
  typedef itk::ImageToImageMetric<TFixedImage, TMovingImage> Superclass;
  typedef itk::SmartPointer<Self>                            Pointer;
  typedef itk::SmartPointer<const Self>                      ConstPointer;

  typedef TFixedImage FixedImageType;
  typedef TMovingImage MovingImageType;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(AbsoluteVolumeErrorImageToImageMetric,
    itk::ImageToImageMetric);

  typedef typename Superclass::MeasureType MeasureType;
  typedef typename Superclass::TransformParametersType TransformParametersType;
  typedef typename Superclass::DerivativeType DerivativeType;

  /**  Get the value for single valued optimizers. */
  MeasureType GetValue() const;

  MeasureType GetValue(const TransformParametersType& p) const
  { // TODO: apply transform with nearest neighbor interpolation
      TransformParametersType pp = p;pp = TransformParametersType();
    return this->GetValue(); }

  void GetDerivative(const TransformParametersType& p, DerivativeType& dp) const
  {
      TransformParametersType pp = p;pp = TransformParametersType();dp.rms();
      itkExceptionMacro(<< "Not implemented"); }

  void GetValueAndDerivative(const TransformParametersType& p, MeasureType& v, DerivativeType& dp) const
  {
      TransformParametersType pp = p;pp = TransformParametersType();v=0;dp.rms();
      itkExceptionMacro(<< "Not implemented"); }

protected:

  AbsoluteVolumeErrorImageToImageMetric();
  virtual ~AbsoluteVolumeErrorImageToImageMetric();

// TODO: transform member, default to identity

};

#ifndef ITK_MANUAL_INSTANTIATION
#include "AbsoluteVolumeErrorImageToImageMetric.txx"
#endif

#endif
