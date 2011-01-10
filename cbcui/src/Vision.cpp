/**************************************************************************
 *  Copyright 2008,2009 KISS Institute for Practical Robotics             *
 *                                                                        *
 *  This file is part of CBC Firmware.                                    *
 *                                                                        *
 *  CBC Firmware is free software: you can redistribute it and/or modify  *
 *  it under the terms of the GNU General Public License as published by  *
 *  the Free Software Foundation, either version 2 of the License, or     *
 *  (at your option) any later version.                                   *
 *                                                                        *
 *  CBC Firmware is distributed in the hope that it will be useful,       *
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *  GNU General Public License for more details.                          *
 *                                                                        *
 *  You should have received a copy of the GNU General Public License     *
 *  along with this copy of CBC Firmware.  Check the LICENSE file         *
 *  in the project root.  If not, see <http://www.gnu.org/licenses/>.     *
 **************************************************************************/

#include "Vision.h"

// System includes
#include "ctdebug.h"

// Qt includes
#include <QApplication>

// Local includes
#include "MainWindow.h"
#include "test.h"
#include "SimulatedCamera.h"
#include "MicrodiaCamera.h"
#include "Pixel565toHSV.h"

Vision::Vision() : m_colorTracker(Vision::NUM_CHANNELS)
{
    ctassert(sizeof(Pixel565) == 2);
    ctassert(sizeof(ushort) == 2);
    ctassert(sizeof(short) == 2);

    Pixel565toHSV::init();              // initialize the HSV color map

    m_camera=NULL;

#ifdef HAS_MICRODIA_CAMERA
    bool use_simulated_camera = false;  // change to true to use simulated cam on chumby
#else
    bool use_simulated_camera = true;
#endif

    if (use_simulated_camera) {
        SimulatedCamera *sc = new SimulatedCamera(160, 120);
        //Image simulatedImage(120, 160);
        //simulatedImage.loadRaw("/mnt/usb/crashframe.raw");
        QImage simulatedImage("simulated.png");
        if (!simulatedImage.width()) {
            fprintf(stderr, "Can't load simulated.png");
            exit(1);
        }
        sc->loadSimulatedImage(simulatedImage);
        m_camera = sc;
    }
    else {
#ifdef HAS_MICRODIA_CAMERA
        m_camera = new MicrodiaCamera();
#else
        ctassert(0);
#endif
    }

    m_colorTracker.shareResults("/tmp/color_tracking_results");

    ctassert(m_camera);
    m_camera->addFrameHandler(&m_colorTracker);
    m_camera->addFrameHandler(&m_rawCameraView);
    m_camera->requestContinuousFrames();
}

