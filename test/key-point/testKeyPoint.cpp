/****************************************************************************
 *
 * $Id: testKeyPoint.cpp 5202 2015-01-24 09:29:06Z fspindle $
 *
 * This file is part of the ViSP software.
 * Copyright (C) 2005 - 2014 by INRIA. All rights reserved.
 * 
 * This software is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * ("GPL") version 2 as published by the Free Software Foundation.
 * See the file LICENSE.txt at the root directory of this source
 * distribution for additional information about the GNU GPL.
 *
 * For using ViSP with software that can not be combined with the GNU
 * GPL, please contact INRIA about acquiring a ViSP Professional 
 * Edition License.
 *
 * See http://www.irisa.fr/lagadic/visp/visp.html for more information.
 * 
 * This software was developed at:
 * INRIA Rennes - Bretagne Atlantique
 * Campus Universitaire de Beaulieu
 * 35042 Rennes Cedex
 * France
 * http://www.irisa.fr/lagadic
 *
 * If you have questions regarding the use of this file, please contact
 * INRIA at visp@inria.fr
 * 
 * This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
 * WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 *
 * Description:
 * Test keypoint matching.
 *
 * Authors:
 * Souriya Trinh
 *
 *****************************************************************************/

#include <visp/vpConfig.h>
#include <iostream>
#if ((defined (VISP_HAVE_X11) || defined(VISP_HAVE_GTK) || defined(VISP_HAVE_GDI)) && defined(VISP_HAVE_OPENCV))

#include <visp/vpKeyPoint.h>
#include <visp/vpImage.h>
#include <visp/vpImageIo.h>
#include <visp/vpDisplayX.h>
#include <visp/vpDisplayGTK.h>
#include <visp/vpDisplayGDI.h>
#include <visp/vpVideoReader.h>
#include <visp/vpIoTools.h>
#include <visp/vpParseArgv.h>

// List of allowed command line options
#define GETOPTARGS	"cdi:h"

void usage(const char *name, const char *badparam);
bool getOptions(int argc, const char **argv, bool &click_allowed, bool &display);

/*!

  Print the program options.

  \param name : Program name.
  \param badparam : Bad parameter name.
  \param ipath: Input image path.

*/
void usage(const char *name, const char *badparam)
{
  fprintf(stdout, "\n\
Test key points matching.\n\
\n\
SYNOPSIS\n\
  %s [-c] [-d] [-h]\n", name);

  fprintf(stdout, "\n\
OPTIONS:                                               \n\
\n\
  -c\n\
     Disable the mouse click. Useful to automaze the \n\
     execution of this program without humain intervention.\n\
\n\
  -d \n\
     Turn off the display.\n\
\n\
  -h\n\
     Print the help.\n");

  if (badparam)
    fprintf(stdout, "\nERROR: Bad parameter [%s]\n", badparam);
}

/*!

  Set the program options.

  \param argc : Command line number of parameters.
  \param argv : Array of command line parameters.
  \param click_allowed : Mouse click activation.
  \param display : Display activation.
  \return false if the program has to be stopped, true otherwise.

*/
bool getOptions(int argc, const char **argv, bool &click_allowed, bool &display)
{
  const char *optarg_;
  int	c;
  while ((c = vpParseArgv::parse(argc, argv, GETOPTARGS, &optarg_)) > 1) {

    switch (c) {
    case 'c': click_allowed = false; break;
    case 'd': display = false; break;
    case 'h': usage(argv[0], NULL); return false; break;

    default:
      usage(argv[0], optarg_);
      return false; break;
    }
  }

  if ((c == 1) || (c == -1)) {
    // standalone param or error
    usage(argv[0], NULL);
    std::cerr << "ERROR: " << std::endl;
    std::cerr << "  Bad argument " << optarg_ << std::endl << std::endl;
    return false;
  }

  return true;
}

/*!
  \example testKeyPoint.cpp

  \brief   Test keypoint matching.
*/
int main(int argc, const char ** argv) {
  try {
    std::string env_ipath;
    bool opt_click_allowed = true;
    bool opt_display = true;

    // Read the command line options
    if (getOptions(argc, argv, opt_click_allowed, opt_display) == false) {
      exit (-1);
    }

    // Get the visp-images-data package path or VISP_INPUT_IMAGE_PATH environment variable value
    env_ipath = vpIoTools::getViSPImagesDataPath();

    //Get the visp-images-data package path or VISP_INPUT_IMAGE_PATH environment variable value
    env_ipath = vpIoTools::getViSPImagesDataPath();

    if(env_ipath.empty()) {
      std::cerr << "Please get the visp-images-data package path or set the VISP_INPUT_IMAGE_PATH "
          "environment variable value." << std::endl;
      return -1;
    }

    vpImage<unsigned char> Iref, Icur, Imatch;

    //Set the path location of the image sequence
    std::string dirname = vpIoTools::createFilePath(env_ipath, "ViSP-images/mbt/cube");

    //Build the name of the image files
    std::string filenameRef = vpIoTools::createFilePath(dirname, "image0000.pgm");
    vpImageIo::read(Iref, filenameRef);
    std::string filenameCur = vpIoTools::createFilePath(dirname, "image%04d.pgm");

    //Init keypoints
    vpKeyPoint keypoints("ORB", "ORB");
    std::cout << "Build " << keypoints.buildReference(Iref) << " reference points." << std::endl;


    vpVideoReader g;
    g.setFileName(filenameCur);
    g.open(Icur);
    g.acquire(Icur);

    Imatch.resize(Icur.getHeight(), 2*Icur.getWidth());
    Imatch.insert(Iref, vpImagePoint(0,0));

#if defined VISP_HAVE_X11
    vpDisplayX display;
#elif defined VISP_HAVE_GTK
    vpDisplayGTK display;
#elif defined VISP_HAVE_GDI
    vpDisplayGDI display;
#else
    std::cerr << "No display available." << std::endl;
    return 0;
#endif

    if (opt_display)
      display.init(Imatch, 0, 0, "ORB keypoints matching");

    bool opt_click = false;
    vpMouseButton::vpMouseButtonType button;
    while(!g.end()) {
      g.acquire(Icur);
      Imatch.insert(Icur, vpImagePoint(0, Icur.getWidth()));

      vpDisplay::display(Imatch);

      //Match keypoints
      keypoints.matchPoint(Icur);
      //Display image with keypoints matched
      keypoints.displayMatching(Iref, Imatch);

      vpDisplay::flush(Imatch);

      //Click requested to process next image
      if (opt_click_allowed && opt_display) {
        if(opt_click) {
          vpDisplay::getClick(Imatch, button, true);
          if(button == vpMouseButton::button3) {
            opt_click = false;
          }
        } else {
          //Use right click to enable/disable step by step tracking
          if(vpDisplay::getClick(Imatch, button, false)) {
            if (button == vpMouseButton::button3) {
              opt_click = true;
            }
            else if(button == vpMouseButton::button1) {
              break;
            }
          }
        }
      }
    }

  } catch(vpException &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }

  return 0;
}
#else
int main() {
#if ( !(defined (VISP_HAVE_X11) || defined(VISP_HAVE_GTK) || defined(VISP_HAVE_GDI)) )
  std::cerr << "You do not have X11, GTK or GDI display functionalities." << std::endl;
#else
  std::cerr << "You need OpenCV library." << std::endl;
#endif

  return -1;
}

#endif
