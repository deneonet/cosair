#pragma once

namespace cosair {

class Timestep {
 public:
  Timestep(float time = 0) : time_(time) {}

  operator float() const { return time_; }

  inline float GetSeconds() const { return time_; }
  inline float GetMilliseconds() const { return time_ * 1000; }

 private:
  float time_;
};

}  // namespace cosair