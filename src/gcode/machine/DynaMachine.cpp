/***************************************************
 * file: CAMotics/src/gcode/machine/DynaMachine.cpp
 *
 * @file    DynaMachine.cpp
 * @author  Eric L. Hernes
 * @version V1.0
 * @born_on   Tuesday, November 26, 2019
 * @copyright (C) Copyright Eric L. Hernes 2019
 * @copyright (C) Copyright Q, Inc. 2019
 *
 * @brief   An Eric L. Hernes Signature Series C++ module
 *
 */

#include "DynaMachine.h"

#include <gcode/Plane.h>

#include <cbang/Exception.h>
#include <cbang/Math.h>
#include <cbang/log/Logger.h>

#include <limits>

using namespace std;
using namespace GCode;

/*
DynaMachine::DynaMachine(const cb::SmartPointer<std::ostream> &stream, Units units) : stream(stream), units(units) {
}
*/

void
DynaMachine::beginLine() {
  *_stream << __func__ << endl;
}

void
DynaMachine::start() {
  *_stream << __func__ << endl;
}

void
DynaMachine::end() {
  *_stream << __func__ << endl;
}

void
DynaMachine::setFeed(double feed) {
  *_stream << __func__ << ": " << feed << endl;
}

void
DynaMachine::setFeedMode(feed_mode_t mode) {
  *_stream << __func__ << ": " << mode << endl;
}

void
DynaMachine::setSpeed(double speed) {
  *_stream << __func__ << ": " << speed << endl;
}

void
DynaMachine::setSpinMode(spin_mode_t mode, double max) {
  *_stream << __func__ << ": " << mode <<  " / " << max << endl;
}

void
DynaMachine::setPathMode(path_mode_t mode, double motionBlending, double naiveCAM) {
  *_stream << __func__ << ": " << mode <<  " / " << motionBlending << " / " << naiveCAM << endl;
}

void
DynaMachine::changeTool(unsigned tool) {
  *_stream << __func__ << ": " << tool <<  endl;
}

void
DynaMachine::input(port_t port, input_mode_t mode, double timeout) {
  *_stream << __func__ << ": " << port <<  " / " << mode << " / " << timeout << endl;
}

void
DynaMachine::seek(port_t port, bool active, bool error) {
  *_stream << __func__ << ": " << port <<  " / " << active << " / " << error << endl;
}

void
DynaMachine::output(port_t port, double value) {
  *_stream << __func__ << ": " << port <<  " / " << value << endl;
}

void
DynaMachine::setPosition(const Axes &position) {
  *_stream << __func__ << ": " << position << endl;
}

void
DynaMachine::dwell(double seconds) {
  *_stream << __func__ << ": " << seconds << endl;
}

void
DynaMachine::move(const Axes &position, int axes, bool rapid) {
  *_stream << __func__ << "(position " << position << ") (axes " << axes << ") (rapid " << rapid << ")" << endl;
}

void
DynaMachine::arc(const cb::Vector3D &offset, const cb::Vector3D &target,
			double angle, plane_t plane) {
  *_stream << __func__ << "(offset " << offset << ") (target " << target << ") (angle " << angle << ") (plane " << plane << ")" << endl;
}

void
DynaMachine::pause(pause_t pause) {
  *_stream << __func__ << ": " << pause << endl;
}

void
DynaMachine::comment(const std::string &s) const {
  *_stream << __func__ << ": " << s << endl;
}

void
DynaMachine::message(const std::string &s) {
  *_stream << __func__ << ": " << s << endl;
}



/* end of CAMotics/src/gcode/machine/DynaMachine.cpp */
