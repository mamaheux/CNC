#include "mcu/modules/McuPlanner.h"
#include "mcu/utils/StringPrint.h"

#include <cnc/modules/ModuleKernel.h>

McuPlanner::McuPlanner(CoordinateTransformer* coordinateTransformer, ArcConverter* arcConverter)
    : Planner(coordinateTransformer, arcConverter)
{
}

void McuPlanner::sendPosition(CommandSource source, uint32_t commandId, const Vector3<float>& position)
{
    StringPrint stringPrint(m_response, MAX_MCU_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" X");
    stringPrint.print(position.x);
    stringPrint.print(" Y");
    stringPrint.print(position.y);
    stringPrint.print(" Z");
    stringPrint.print(position.z);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void McuPlanner::sendOkWithS(CommandSource source, uint32_t commandId, float sValue)
{
    StringPrint stringPrint(m_response, MAX_MCU_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" S");
    stringPrint.print(sValue);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}

void McuPlanner::sendOkWithP(CommandSource source, uint32_t commandId, float pValue)
{
    StringPrint stringPrint(m_response, MAX_MCU_PLANNER_RESPONSE_SIZE);
    stringPrint.print(OK_COMMAND_RESPONSE);
    stringPrint.print(" P");
    stringPrint.print(pValue);
    stringPrint.finish();

    m_kernel->sendCommandResponse(m_response, source, commandId);
}
