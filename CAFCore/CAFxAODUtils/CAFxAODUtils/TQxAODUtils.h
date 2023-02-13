//this file looks like plain C, but it's actually -*- c++ -*-
#ifndef __TQxAODUtils__
#define __TQxAODUtils__

#include "xAODEventFormat/EventFormat.h"
#include "xAODRootAccess/TEvent.h"

namespace TQxAODUtils {
  const xAOD::EventFormat* getMetaEventFormat(xAOD::TEvent* evt);
}

#endif
