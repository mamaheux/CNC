#ifndef MCU_MODULES_MCU_PLANNER_H
#define MCU_MODULES_MCU_PLANNER_H

#include <cnc/modules/Planner.h>

constexpr size_t MAX_MCU_PLANNER_RESPONSE_SIZE = 128;

class McuPlanner : public Planner
{
    char m_response[MAX_MCU_PLANNER_RESPONSE_SIZE];

public:
    McuPlanner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter);
    ~McuPlanner() override = default;

protected:
    void sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position) override;
    void sendOkWithS(CommandSource source, uint32_t commandId, float sValue) override;
    void sendOkWithP(CommandSource source, uint32_t commandId, float pValue) override;
};

#endif
