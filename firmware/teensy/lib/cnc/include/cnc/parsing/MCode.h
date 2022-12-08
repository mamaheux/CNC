#ifndef CNC_PARSING_MCODE_H
#define CNC_PARSING_MCODE_H

#include <cnc/parsing/ParsingResult.h>

#include <tl/optional.hpp>

#include <cstdlib>
#include <cstdint>

constexpr size_t MCODE_LINE_BUFFER_SIZE = 256;

class MCodeParser;

class MCode
{
    tl::optional<float> m_s;
    tl::optional<float> m_p;
    tl::optional<float> m_i;
    tl::optional<float> m_d;
    const char* m_path;

    uint32_t m_code;
    tl::optional<uint32_t> m_subcode;

public:
    MCode();

    tl::optional<float> s() const;
    tl::optional<float> p() const;
    tl::optional<float> i() const;
    tl::optional<float> d() const;
    const char* path() const;

    uint32_t code() const;
    tl::optional<uint32_t> subcode() const;

    void clear();

    friend MCodeParser;
};

inline tl::optional<float> MCode::s() const
{
    return m_s;
}

inline tl::optional<float> MCode::p() const
{
    return m_p;
}

inline tl::optional<float> MCode::i() const
{
    return m_i;
}

inline tl::optional<float> MCode::d() const
{
    return m_d;
}

inline const char* MCode::path() const
{
    return m_path;
}

inline uint32_t MCode::code() const
{
    return m_code;
}

inline tl::optional<uint32_t> MCode::subcode() const
{
    return m_subcode;
}

class MCodeParser
{
    char m_lineBuffer[MCODE_LINE_BUFFER_SIZE];

public:
    MCodeParser();

    ParsingResult parse(const char* line, MCode& mcode);

private:
    ParsingResult parseNormalMCode(char* start, MCode& mcode);
    ParsingResult parseParameters(char* start, MCode& mcode);
    static ParsingResult parsePath(char* start, MCode& mcode);
};

#endif
