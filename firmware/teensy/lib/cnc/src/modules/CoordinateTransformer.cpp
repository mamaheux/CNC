#include <cnc/modules/CoordinateTransformer.h>

#include <cnc/modules/ModuleKernel.h>

CoordinateSystem::CoordinateSystem() : offset(0.f, 0.f, 0.f), rotation(0.f), rotationInv(0.f) {
}


CoordinateTransformer::CoordinateTransformer() :
  m_isIncrementalMode(false),
  m_scale(1.f),
  m_currentCoordinateSystemIndex(0),
  m_globalOffset(0.f, 0.f, 0.f) {
}

void CoordinateTransformer::configure(const ConfigItem& item) {
}

void CoordinateTransformer::begin() {
  m_kernel->registerToEvent(ModuleEventType::GCODE_COMMAND, this);
}

CommandResult CoordinateTransformer::onGCodeCommandReceived(const GCode& gcode, uint32_t commandId) {
  if (gcode.code() == 10 && gcode.l() == 2u) {
    return setCoordinateSystemL2(gcode);
  }
  else if (gcode.code() == 10 && gcode.l() == 20u) {
    return setCoordinateSystemL20(gcode);
  }
  else if (gcode.code() == 20) {
    m_scale = INCH_TO_MM_SCALE;
  }
  else if (gcode.code() == 21) {
    m_scale = 1.f;
  }
  else if (gcode.code() == 54) {
    m_currentCoordinateSystemIndex = 0;
  }
  else if (gcode.code() == 55) {
    m_currentCoordinateSystemIndex = 1;
  }
  else if (gcode.code() == 56) {
    m_currentCoordinateSystemIndex = 2;
  }
  else if (gcode.code() == 57) {
    m_currentCoordinateSystemIndex = 3;
  }
  else if (gcode.code() == 58) {
    m_currentCoordinateSystemIndex = 4;
  }
  else if (gcode.code() == 59 && gcode.subcode() == tl::nullopt) {
    m_currentCoordinateSystemIndex = 5;
  }
  else if (gcode.code() == 59 && gcode.subcode() == 1u) {
    m_currentCoordinateSystemIndex = 6;
  }
  else if (gcode.code() == 59 && gcode.subcode() == 2u) {
    m_currentCoordinateSystemIndex = 7;
  }
  else if (gcode.code() == 59 && gcode.subcode() == 3u) {
    m_currentCoordinateSystemIndex = 8;
  }
  else if (gcode.code() == 90) {
    m_isIncrementalMode = false;
  }
  else if (gcode.code() == 91) {
    m_isIncrementalMode = true;
  }
  else if (gcode.code() == 92 && gcode.subcode() == tl::nullopt) {
    return setGlobalOffset(gcode);
  }
  else if (gcode.code() == 92 && gcode.subcode() == 1u) {
    m_globalOffset = Vector3<float>(0.f, 0.f, 0.f);
  }
  else {
    return CommandResult::NOT_HANDLED;
  }

  return CommandResult::OK;
}

void CoordinateTransformer::onTargetPositionChanged(const Vector3<float>& machinePosition) {
  m_targetMachinePosition = machinePosition;
}

Vector3<float> CoordinateTransformer::gcodeCoordinateToMachineCoordinate(const Vector3<float> v) {
  CoordinateSystem& coordinateSystem = m_coordinateSystems[m_currentCoordinateSystemIndex];
  Vector3<float> rotatedV = coordinateSystem.rotation.rotate(v * m_scale);
  if (m_isIncrementalMode) {
    return rotatedV + m_targetMachinePosition;
  } else {
    return rotatedV + coordinateSystem.offset + m_globalOffset;
  }
}

Vector3<float> CoordinateTransformer::machineCoordinateToUserCurentCoordinate(const Vector3<float> v) {
  CoordinateSystem& coordinateSystem = m_coordinateSystems[m_currentCoordinateSystemIndex];
  return coordinateSystem.rotationInv.rotate(v - m_globalOffset - coordinateSystem.offset) / m_scale;
}

Vector3<float> CoordinateTransformer::machineCoordinateToUserMachineCoordinate(const Vector3<float> v) {
  return v / m_scale;
}

CommandResult CoordinateTransformer::setCoordinateSystemL2(const GCode& gcode) {
  tl::optional<size_t> coordinateSystemIndex = getCoordinateSystemIndex(gcode);
  if (!coordinateSystemIndex.has_value()) {
    return CommandResult::ERROR;
  }

  CoordinateSystem& coordinateSystem = m_coordinateSystems[*coordinateSystemIndex];
  if (gcode.x().has_value()) {
    coordinateSystem.offset.x = *gcode.x() * m_scale;
  }
  if (gcode.y().has_value()) {
    coordinateSystem.offset.y = *gcode.y() * m_scale;
  }
  if (gcode.z().has_value()) {
    coordinateSystem.offset.z = *gcode.z() * m_scale;
  }
  if (gcode.r().has_value()) {
    float rad = degToRad(*gcode.r());
    coordinateSystem.rotation.setAngleRad(rad);
    coordinateSystem.rotationInv.setAngleRad(-rad);
  }

  return CommandResult::OK;
}

CommandResult CoordinateTransformer::setCoordinateSystemL20(const GCode& gcode) {
  tl::optional<size_t> coordinateSystemIndex = getCoordinateSystemIndex(gcode);
  if (!coordinateSystemIndex.has_value()) {
    return CommandResult::ERROR;
  }

  CoordinateSystem& coordinateSystem = m_coordinateSystems[*coordinateSystemIndex];
  if (gcode.x().has_value()) {
    coordinateSystem.offset.x = m_targetMachinePosition.x - m_globalOffset.x - (*gcode.x() * m_scale);
  }
  if (gcode.y().has_value()) {
    coordinateSystem.offset.y = m_targetMachinePosition.y - m_globalOffset.y - (*gcode.y() * m_scale);
  }
  if (gcode.z().has_value()) {
    coordinateSystem.offset.z = m_targetMachinePosition.z - m_globalOffset.z - (*gcode.z() * m_scale);
  }

  return CommandResult::OK;
}

tl::optional<size_t> CoordinateTransformer::getCoordinateSystemIndex(const GCode& gcode) {
  if (!gcode.p().has_value()) {
    return tl::nullopt;
  }
  size_t p = static_cast<size_t>(*gcode.p());

  if (p == 0) {
    return m_currentCoordinateSystemIndex;
  }
  else if (1 <= p && p <= 9) {
    return p - 1;
  }

  return tl::nullopt;
}

CommandResult CoordinateTransformer::setGlobalOffset(const GCode& gcode) {
  if (gcode.x() == tl::nullopt && gcode.y() == tl::nullopt && gcode.z() == tl::nullopt) {
    return CommandResult::ERROR;
  }

  if (gcode.x().has_value()) {
    m_globalOffset.x = m_targetMachinePosition.x - (*gcode.x() * m_scale);
  }
  else {
    m_globalOffset.x = 0;
  }

  if (gcode.y().has_value()) {
    m_globalOffset.y = m_targetMachinePosition.y - (*gcode.y() * m_scale);
  }
  else {
    m_globalOffset.y = 0;
  }

  if (gcode.z().has_value()) {
    m_globalOffset.z = m_targetMachinePosition.z - (*gcode.z() * m_scale);
  }
  else {
    m_globalOffset.z = 0;
  }

  return CommandResult::OK;
}
