#ifndef CNC_MODULES_COORDINATE_TRANSFORMER_H
#define CNC_MODULES_COORDINATE_TRANSFORMER_H

#include <cnc/math/Vector3.h>
#include <cnc/math/ZRotation.h>
#include <cnc/modules/Module.h>

#include <cstdlib>

constexpr size_t COORDINATE_SYSTEM_COUNT = 9;
constexpr float INCH_TO_MM_SCALE = 25.4;

class CoordinateTransformer;

class CoordinateSystem {
public:
  Vector3<float> offset;
  ZRotation<float> rotation;
  ZRotation<float> rotationInv;

  CoordinateSystem();
};

// TODO Test
class CoordinateTransformer : public Module {
  bool m_isIncrementalMode;
  float m_scale;
  CoordinateSystem m_coordinateSystems[COORDINATE_SYSTEM_COUNT];
  size_t m_currentCoordinateSystemIndex;
  Vector3<float> m_globalOffset;

  Vector3<float> m_targetMachinePosition; // Machine Coordinate

public:
  CoordinateTransformer();
  ~CoordinateTransformer() override = default;

  void configure(const ConfigItem& item) override;
  void begin() override;

  CommandResult onGCodeCommandReceived(const GCode& gcode, uint32_t commandId) override;
  void onTargetPositionChanged(const Vector3<float>& machinePosition) override;

  Vector3<float> gcodeCoordinateToMachineCoordinate(const Vector3<float> v);
  Vector3<float> machineCoordinateToUserCurentCoordinate(const Vector3<float> v);
  Vector3<float> machineCoordinateToUserMachineCoordinate(const Vector3<float> v);

private:
  CommandResult setCoordinateSystemL2(const GCode& gcode);
  CommandResult setCoordinateSystemL20(const GCode& gcode);
  tl::optional<size_t> getCoordinateSystemIndex(const GCode& gcode);

  CommandResult setGlobalOffset(const GCode& gcode);
};

#endif
