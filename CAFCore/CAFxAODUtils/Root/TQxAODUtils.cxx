#include "CAFxAODUtils/TQxAODUtils.h"

const xAOD::EventFormat* TQxAODUtils::getMetaEventFormat(xAOD::TEvent* evt) {
  if (!evt) return nullptr;
  const xAOD::EventFormat* ef = nullptr;
  evt->retrieveMetaInput(ef,"EventFormat").ignore();
  return ef;
}
