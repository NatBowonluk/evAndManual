/*
 * BarryKalmanFilter - a Kalman Filter implementation for single variable models.
 * Created by Denys Sene, January, 1, 2017.
 * Released under MIT License - see LICENSE file for details.
 */

#include "Arduino.h"
#include "BarryKalmanFilter.h"
#include <math.h>

BarryKalmanFilter::BarryKalmanFilter(float mea_e, float est_e, float q)
{
  _err_measure=mea_e;
  _err_estimate=est_e;
  _q = q;
}

float BarryKalmanFilter::updateEstimate(float mea)
{
  _kalman_gain = _err_estimate/(_err_estimate + _err_measure);
  _current_estimate = _last_estimate + _kalman_gain * (mea - _last_estimate);
  _err_estimate =  (1.0 - _kalman_gain)*_err_estimate + fabs(_last_estimate-_current_estimate)*_q;
  _last_estimate=_current_estimate;

  return _current_estimate;
}

void BarryKalmanFilter::setMeasurementError(float mea_e)
{
  _err_measure=mea_e;
}

void BarryKalmanFilter::setEstimateError(float est_e)
{
  _err_estimate=est_e;
}

void BarryKalmanFilter::setProcessNoise(float q)
{
  _q=q;
}

float BarryKalmanFilter::getKalmanGain() {
  return _kalman_gain;
}

float BarryKalmanFilter::getEstimateError() {
  return _err_estimate;
}
