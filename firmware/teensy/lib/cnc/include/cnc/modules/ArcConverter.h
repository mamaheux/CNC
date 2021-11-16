#ifndef CNC_MODULES_ARC_CONVERTER_H
#define CNC_MODULES_ARC_CONVERTER_H

#include <cnc/math/Vector2.h>
#include <cnc/math/Vector3.h>
#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>

#include <utility>

enum class ArcConverterPlan {
  XY, XZ, YZ
};

class ArcConverter : public Module {
  tl::optional<float> m_feedrate;
  Vector2<float> m_startPoint;
  Vector2<float> m_endPoint;
  Vector2<float> m_centerPoint;

  float m_startOtherAxis;
  float m_endOtherAxis;
  float m_currentOtherAxis;
  float m_otherAxisStep;

  float m_radius;
  float m_currentAngle; // Rad
  float m_angleStep; // Rad
  size_t m_segmentCount;
  size_t m_segmentIndex;

  float m_maxError;

  CoordinateTransformer* m_coordinateTransformer;
  bool m_isIncrementalArcDistanceMode;
  ArcConverterPlan m_currentPlan;
  Vector3<float> m_previousMachinePosition;
  Vector3<float> m_previousGcodePosition;

public:
  ArcConverter(CoordinateTransformer* coordinateTransformer);

  DECLARE_NOT_COPYABLE(ArcConverter);
  DECLARE_NOT_MOVABLE(ArcConverter);

  void configure(const ConfigItem& item) override;
  void begin() override;

  CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
  void onTargetPositionChanged(const Vector3<float>& machinePosition) override;

  CommandResult setArc(const GCode& gcode);
  bool getNextSegment(GCode& gcode);
  bool isFinished() const;

private:
  void setStartPoint();
  void setEndPoint(const GCode& gcode);
  bool setCenterPoint(const GCode& gcode);
  void setCenterPointFromOffset(const GCode& gcode);
  bool setRadius();
  bool calculateSegments(const GCode& gcode);

  bool isClockwise(const GCode& gcode);

  Vector3<float> gcodePositionToMachinePosition(const tl::optional<float>& x,
      const tl::optional<float>& y,
      const tl::optional<float>& z,
      bool isMachineCoordinateSystem);

  std::pair<Vector2<float>, float> toPlan(const Vector3<float>& position);
  Vector3<float> fromPlan(Vector2<float> position, float otherAxis);
};

inline bool ArcConverter::isFinished() const {
  return m_segmentIndex == m_segmentCount;
}

float atan2Pos(float a, float b);
Vector2<double> getCenterPointFromRadius(double x1, double y1, double x2, double y2, float radius, bool isClockwise);

#endif
