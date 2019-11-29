/***************************************************
 * file: CAMotics/src/gcode/machine/DynaMachine.h
 *
 * @file    DynaMachine.h
 * @author  Eric L. Hernes
 * @version V1.0
 * @born_on   Tuesday, November 26, 2019
 * @copyright (C) Copyright Eric L. Hernes 2019
 * @copyright (C) Copyright Q, Inc. 2019
 *
 * @brief   An Eric L. Hernes Signature Series C/C++ header
 *
 * $Id$
 */

#ifndef DYNAMACHINE_H
#define DYNAMACHINE_H

#include "MachineAdapter.h"

#include <gcode/Units.h>

#include <ostream>

namespace GCode {
  class DynaMachine : public MachineAdapter {
    cb::SmartPointer<std::ostream> _stream;
    Axes _position;
    Units _units;
    cb::FileLocation _location;
    int _axisSeen = 0;

  public:
    DynaMachine(const cb::SmartPointer<std::ostream> &stream, Units units) : _stream(stream), _units(units) {
    };

    void beginLine();

    // From MachineInterface
    void start();
    void end();

    void setFeed(double feed);
    void setFeedMode(feed_mode_t mode);
    void setSpeed(double speed);
    void setSpinMode(spin_mode_t mode, double max);
    void setPathMode(path_mode_t mode, double motionBlending, double naiveCAM);
    void changeTool(unsigned tool);

    void input(port_t port, input_mode_t mode, double timeout);
    void seek(port_t port, bool active, bool error);
    void output(port_t port, double value);

    void setPosition(const Axes &position);

    void dwell(double seconds);
    void move(const Axes &position, int axes, bool rapid);
    void arc(const cb::Vector3D &offset, const cb::Vector3D &target,
             double angle, plane_t plane);
    void pause(pause_t pause);

    void comment(const std::string &s) const;
    void message(const std::string &s);

    // higher level canned functions:
    void drill(int nPeck, double zHeight, double zDepth, std::vector<Axes> position);
    void boltCircle(int nPeck, double zHeight, double zDepth, const Axes &center, double alpha, int n, double radius);
    void rectPocket();
    void rectFrame();
    void circPocket();
    void mill();
  };
}


#endif /* DYNAMACHINE_H */

/* end of CAMotics/src/gcode/machine/DynaMachine.h */
