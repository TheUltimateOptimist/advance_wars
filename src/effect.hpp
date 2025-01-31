#pragma once

#include "engine.hpp"

namespace advanced_wars
{

enum class EffectId
{
    LAND_EXPLOSION = 0,
    AIR_EXPLOSION = 1,
    NAVAL_EXPLOSION = 2,
    SUBMARINE_HIDE = 3,
    SUBMARINE_APPEAR = 4
};

class Effect
{
    public:
        Effect(int x, int y, EffectId id, bool repeat);

        void render(Engine& engine, int scale);

        bool is_finished(Engine& engine);

        int      m_x;
        int      m_y;
        EffectId m_id;
        bool     m_repeat;
        int      m_start;
};

} // namespace advanced_wars