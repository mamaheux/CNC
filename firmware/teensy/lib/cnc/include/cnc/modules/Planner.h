#ifndef CNC_MODULES_PLANNER_H
#define CNC_MODULES_PLANNER_H

#include <cnc/LinearBlock.h>
#include <cnc/collections/BoundedQueue.h>
#include <cnc/math/Range.h>
#include <cnc/modules/Module.h>
#include <cnc/modules/CoordinateTransformer.h>
#include <cnc/modules/ArcConverter.h>

#include <tl/optional.hpp>

constexpr size_t PLANNER_QUEUE_SIZE = 128;

class Planner : public Module {
  CoordinateTransformer* m_coordinateTransformer;
  ArcConverter* m_arcConverter;

  tl::optional<float> m_xStepCountPerMm;
  tl::optional<float> m_yStepCountPerMm;
  tl::optional<float> m_zStepCountPerMm;
  tl::optional<float> m_accelerationInMmPerSS;
  tl::optional<float> m_junctionDeviation;

  tl::optional<CommandSource> m_currentSource;
  BoundedQueue<LinearBlock, PLANNER_QUEUE_SIZE> m_blockQueue;

  InclusiveRange3<float> m_machineRange;
  Vector3<int32_t> m_lastRequestedPosition;

public:
  Planner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter);
  ~Planner() override = default;

  DECLARE_NOT_COPYABLE(Planner);
  DECLARE_NOT_MOVABLE(Planner);

  void configure(const ConfigItem& item) override;
  void checkConfigErrors(std::function<void(const char*, const char*, const char*)> onMissingConfigItem) override;
  void begin() override;

  CommandResult onGCodeCommandReceived(const GCode& gcode, CommandSource source, uint32_t commandId) override;
  CommandResult onMCodeCommandReceived(const MCode& gcode, CommandSource source, uint32_t commandId) override;

  void update() override;

  void reset(const InclusiveRange3<float>& range);
  float xStepCountPerMm() const;
  float yStepCountPerMm() const;
  float zStepCountPerMm() const;
};

inline float Planner::xStepCountPerMm() const {
  return *m_xStepCountPerMm;
}

inline float Planner::yStepCountPerMm() const {
  return *m_yStepCountPerMm;
}

inline float Planner::zStepCountPerMm() const {
  return *m_zStepCountPerMm;
}

#endif
