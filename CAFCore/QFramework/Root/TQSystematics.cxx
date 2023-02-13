
#include <string>
#include <map>

#include "QFramework/TQSystematics.h"

namespace{
  std::map<TQSystematics::Status,std::string> names;

  bool populate(){
    names[TQSystematics::UNKNOWN]    = "unknown";            
    names[TQSystematics::ACTIVE]    = "OK";
    names[TQSystematics::ZERO]      = "zero";
    names[TQSystematics::DROPPED]   = "dropped";
    names[TQSystematics::FIXED]     = "fixed";
    names[TQSystematics::OVERSHOOT] = "overshoot";
    names[TQSystematics::ONESIDED] = "one-sided";
    names[TQSystematics::SYMMETRIZED] = "symmetrized";
    names[TQSystematics::INSIGNIFICANT] = "insignificant";
    names[TQSystematics::SMOOTHED] = "smoothed";
    return true;
  }
  
  bool _init = populate();
}

const char* TQSystematics::name(Status s){
  return ::names[s].c_str();
}

const char* TQSystematics::name(int s){
  return TQSystematics::name((Status)s);
}

  
      
