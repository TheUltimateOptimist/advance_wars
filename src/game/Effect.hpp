/**
 * Effect.hpp
 *
 * @author
 */

#pragma once

#include "Engine.hpp"

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

/**
 * Representation of an effect animation
 */
class Effect
{
    public:
        /**
         * Contructor
         *
         * @param x tile-based x-position
         * @param y tile-based y-position
         * @param id The kind of effect
         * @param repeat Enable looping
         */
        Effect(int x, int y, EffectId id, bool repeat);

        /**
         * Render this object.
         *
         * @param engine An engine object with valid rendering context
         * @param scale The factor to scale this object with
         */
        void render(Engine& engine, int scale);

        /**
         * Gets the state of the effect.
         *
         * @return ```true``` if effect rendered one full cycle or should loop,
         * otherwise ```false```
         */
        bool is_finished(Engine& engine);

    private:
        int      m_x;  // The tile-based x-position
        int      m_y;  // The tile-based y-position
        EffectId m_id; // The kind of effect

        bool m_repeat; // Flag if the effect should loop
        int  m_start;  // Starting point of the effect relevant for animation
};

} // namespace advanced_wars