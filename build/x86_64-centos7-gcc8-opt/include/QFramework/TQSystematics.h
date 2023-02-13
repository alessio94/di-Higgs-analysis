//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef TQSYSTEMATICS
#define TQSYSTEMATICS

namespace TQSystematics {
  enum Status {
    UNKNOWN=0,
    ACTIVE=1,
    FIXED=2,
    ZERO=3,
    DROPPED=4,
    OVERSHOOT=5,
    ONESIDED=6,
    INSIGNIFICANT=7,
    SYMMETRIZED=8,
    SMOOTHED=9,
  };
  const char* name(Status s);
  const char* name(int s);
}

#endif
