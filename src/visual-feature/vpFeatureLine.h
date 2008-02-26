/****************************************************************************
 *
 * $Id: vpFeatureLine.h,v 1.10 2008-02-26 10:32:11 asaunier Exp $
 *
 * Copyright (C) 1998-2006 Inria. All rights reserved.
 *
 * This software was developed at:
 * IRISA/INRIA Rennes
 * Projet Lagadic
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * http://www.irisa.fr/lagadic
 *
 * This file is part of the ViSP toolkit.
 *
 * This file may be distributed under the terms of the Q Public License
 * as defined by Trolltech AS of Norway and appearing in the file
 * LICENSE included in the packaging of this file.
 *
 * Licensees holding valid ViSP Professional Edition licenses may
 * use this file in accordance with the ViSP Commercial License
 * Agreement provided with the Software.
 *
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Contact visp@irisa.fr if any conditions of this licensing are
 * not clear to you.
 *
 * Description:
 * 2D line visual feature.
 *
 * Authors:
 * Eric Marchand
 *
 *****************************************************************************/


#ifndef vpFeatureLine_H
#define vpFeatureLine_H

/*!
  \file vpFeatureLine.h
  \brief Class that defines 2D line visual feature
*/

#include <visp/vpConfig.h>
#include <visp/vpMatrix.h>
#include <visp/vpBasicFeature.h>


#include <visp/vpHomogeneousMatrix.h>
#include <visp/vpRGBa.h>


/*!
  \class vpFeatureLine
  \brief Class that defines 2D line visual feature
*/
class VISP_EXPORT vpFeatureLine : public vpBasicFeature
{
  /*!
    attributes and members directly related to the vpBasicFeature needs
    other functionalities ar usefull but not mandatory
  */
private:
  //! FeatureLine depth (required to compute the interaction matrix)
  //!  equation of a plane
  double A,B,C,D ;

public:
  //! basic construction
  void init() ;
  //! basic constructor
  vpFeatureLine() ;
  //! destructor
  virtual ~vpFeatureLine() { ; }

public:
  /*!
    \section Set coordinates
  */

  //! set the point xy and Z-coordinates
  void setRhoTheta(const double rho, const double theta) ;
  void setABCD(const double A, const double B,
	       const double C, const double D) ;

  //  void buildFrom(const vpLine &l) ;
  //  void buildFrom(const vpCylinder &c, const int l) ;
  void buildFrom(const double rho, const double theta) ;
  void buildFrom(const double rho, const double theta,
		 const double A, const double B,
		 const double C, const double D) ;

  //! Get feature

  double getRho() const  { return s[0] ; }
  double getTheta() const { return s[1] ; }
public:
  /*!
    vpBasicFeature method instantiation
  */

  //! feature selection
  inline static int selectRho()  { return FEATURE_LINE[0] ; }
  inline static int selectTheta()  { return FEATURE_LINE[1] ; }
  //! compute the interaction matrix from a subset a the possible features
  vpMatrix  interaction(const int select = FEATURE_ALL) const;
  //! compute the error between two visual features from a subset
  //! a the possible features
  vpColVector error(const vpBasicFeature &s_star,
		    const int select = FEATURE_ALL)  ;
  //! compute the error between a visual features and zero
  vpColVector error(const int select = FEATURE_ALL)  ;
  //! print the name of the feature
  void print(const int select = FEATURE_ALL ) const ;

  //! feature duplication
  vpFeatureLine *duplicate() const ;


public:
  void display(const vpCameraParameters &cam,
	       vpImage<unsigned char> &I,
	       vpColor::vpColorType color=vpColor::green) const ;
  void display(const vpCameraParameters &cam,
               vpImage<vpRGBa> &I,
               vpColor::vpColorType color=vpColor::green) const ;

} ;



#endif

/*
 * Local variables:
 * c-basic-offset: 2
 * End:
 */
