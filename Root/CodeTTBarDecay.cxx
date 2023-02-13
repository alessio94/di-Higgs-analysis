#include "CAFbbll/CodeTTBarDecay.h"

// uncomment the following line to enable debug printouts
// #define _DEBUG_
// be careful to not move the _DEBUG_ flag behind the following line
// otherwise, it will show no effect
#include "QFramework/TQLibrary.h"

#include "xAODEventInfo/EventInfo.h"

double CodeTTBarDecay::getValue() const {
    bool isMC = false;
    fSample->getTag("~isMC", isMC);

    if (isMC) {
        xAOD::EventInfo const* event_info = nullptr;
        if (!get_xaod_eventinfo(fEvent, "Nominal", event_info)) {
            return -1;
        }
        return event_info->auxdataConst<int>("codeTTBarDecay");
    } else {
        return -1;
    }
}
