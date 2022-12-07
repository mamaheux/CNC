#ifndef CNC_PARSING_GCODE_H
#define CNC_PARSING_GCODE_H

#include <cnc/math/Vector3.h>
#include <cnc/parsing/ParsingResult.h>

#include <tl/optional.hpp>

#include <cstdlib>
#include <cstdint>

constexpr size_t GCODE_LINE_BUFFER_SIZE = 256;

class GCodeParser;

class GCode
{
    tl::optional<float> m_x;
    tl::optional<float> m_y;
    tl::optional<float> m_z;
    tl::optional<float> m_f;
    tl::optional<float> m_i;
    tl::optional<float> m_j;
    tl::optional<float> m_k;
    tl::optional<float> m_s;
    tl::optional<float> m_p;
    tl::optional<float> m_r;
    tl::optional<uint32_t> m_l;

    uint32_t m_code;
    tl::optional<uint32_t> m_subcode;

    bool m_isMachineCoordinateSystem;  // G53

public:
    GCode();

    tl::optional<float> x() const;
    tl::optional<float> y() const;
    tl::optional<float> z() const;
    tl::optional<float> f() const;
    tl::optional<float> i() const;
    tl::optional<float> j() const;
    tl::optional<float> k() const;
    tl::optional<float> s() const;
    tl::optional<float> p() const;
    tl::optional<float> r() const;
    tl::optional<uint32_t> l() const;

    uint32_t code() const;
    tl::optional<uint32_t> subcode() const;

    bool isMachineCoordinateSystem() const;

    static GCode g1(const Vector3<float>& position, tl::optional<float> f, bool isMachineCoordinateSystem);

private:
    void clear();

    friend GCodeParser;
};

inline tl::optional<float> GCode::x() const
{
    return m_x;
}

inline tl::optional<float> GCode::y() const
{
    return m_y;
}

inline tl::optional<float> GCode::z() const
{
    return m_z;
}

inline tl::optional<float> GCode::f() const
{
    return m_f;
}

inline tl::optional<float> GCode::i() const
{
    return m_i;
}

inline tl::optional<float> GCode::j() const
{
    return m_j;
}

inline tl::optional<float> GCode::k() const
{
    return m_k;
}

inline tl::optional<float> GCode::s() const
{
    return m_s;
}

inline tl::optional<float> GCode::p() const
{
    return m_p;
}

inline tl::optional<float> GCode::r() const
{
    return m_r;
}

inline tl::optional<uint32_t> GCode::l() const
{
    return m_l;
}

inline uint32_t GCode::code() const
{
    return m_code;
}

inline tl::optional<uint32_t> GCode::subcode() const
{
    return m_subcode;
}

inline bool GCode::isMachineCoordinateSystem() const
{
    return m_isMachineCoordinateSystem;
}

inline GCode GCode::g1(const Vector3<float>& position, tl::optional<float> f, bool isMachineCoordinateSystem)
{
    GCode gcode;
    gcode.m_code = 1;
    gcode.m_x = position.x;
    gcode.m_y = position.y;
    gcode.m_z = position.z;
    gcode.m_f = f;
    gcode.m_isMachineCoordinateSystem = isMachineCoordinateSystem;
    return gcode;
}

class GCodeParser
{
    char m_lineBuffer[GCODE_LINE_BUFFER_SIZE];

    tl::optional<uint32_t> m_modalMoveCode;  // G0, G1, G2 or G3
    bool m_isMachineCoordinateSystem;  // G53

public:
    GCodeParser();

    ParsingResult parse(const char* line, GCode& gcode);

private:
    ParsingResult parseModalMove(GCode& gcode);
    ParsingResult parseG53(GCode& gcode, size_t lineSize);
    ParsingResult parseG53FollowingLine(GCode& gcode);
    ParsingResult parseNormalGCode(char* start, GCode& gcode);
    ParsingResult parseParameters(char* start, GCode& gcode);
};

#endif
