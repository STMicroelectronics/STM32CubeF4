/**
  ******************************************************************************
  * This file is part of the TouchGFX 4.10.0 distribution.
  *
  * <h2><center>&copy; Copyright (c) 2018 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
  

#ifndef EXTENDED_ZOOM_ANIMATION_IMAGE_HPP
#define EXTENDED_ZOOM_ANIMATION_IMAGE_HPP

#include <touchgfx/containers/ZoomAnimationImage.hpp>

using namespace touchgfx;

/**
 * @class ExtendedZoomAnimationImage
 *
 * @brief This widget is part of the TouchGFX Open Widget Repository.
 *        
 *        Extends the ZoomAnimationImage with an additional bitmap size.
 *
 * @sa ZoomAnimationImage
 */
class ExtendedZoomAnimationImage : public ZoomAnimationImage
{
public:
    /**
     * @fn ExtendedZoomAnimationImage::ExtendedZoomAnimationImage()
     *
     * @brief Default constructor.
     */
    ExtendedZoomAnimationImage() : 
        ZoomAnimationImage()
    {
    
    }

    /**
     * @fn ExtendedZoomAnimationImage::~ExtendedZoomAnimationImage()
     *
     * @brief Destructor.
     */
    virtual ~ExtendedZoomAnimationImage()
    {
    }

    /**
     * @fn void ExtendedZoomAnimationImage::setBitmaps(const Bitmap& smallBitmap, const Bitmap& mediumBitmap, const Bitmap& largeBitmap)
     *
     * @brief Initializes the bitmap of the image to be used. The bitmaps should represent the same
     *        image in the three needed static resolutions. Note that it is possible to scale the
     *        image beyond the sizes of these bitmaps.
     *
     * @param smallBitmap  The image in the smallest resolution.
     * @param mediumBitmap The image in the medium resolution.
     * @param largeBitmap  The image in the largest resolution.
     */
    void setBitmaps(const Bitmap& smallBitmap, const Bitmap& mediumBitmap, const Bitmap& largeBitmap)
    {
        ZoomAnimationImage::setBitmaps(smallBitmap, largeBitmap);
        mediumBmp = mediumBitmap;
       
    }

    /**
     * @fn void ExtendedZoomAnimationImage::updateRenderingMethod()
     *
     * @brief Handles the rendering of the medium bitmap
     */
    virtual void updateRenderingMethod()
    {
        if ((mediumBmp.getWidth() == getWidth()) && (mediumBmp.getHeight() == getHeight()))
        {
            image.setVisible(true);
            scalableImage.setVisible(false);
            image.setBitmap(mediumBmp);
            image.invalidate();
            scalableImage.invalidate();
        }
        else
        {
            ZoomAnimationImage::updateRenderingMethod();
        }
    }

private:

    touchgfx::Bitmap mediumBmp; ///< The bitmap representing the medium image
};

#endif /* EXTENDED_ZOOM_ANIMATION_IMAGE_HPP */
