namespace advanced_wars
{

constexpr float PIXELS_PER_METER = 16.0f;

/// Tile → Pixel
inline int tileToPixel(int tile)
{
    return tile * 16;
}

/// Pixel → Tile
inline int pixelToTile(int pixel)
{
    return pixel / 16;
}

/// Box2D (Meter) → Pixel
inline float worldToPixel(float world)
{
    return world * PIXELS_PER_METER;
}

/// Pixel → Box2D (Meter)
inline float pixelToWorld(float pixel)
{
    return pixel / PIXELS_PER_METER;
}

/// Tile → Box2D (Meter) (Oberer linker Punkt des Tiles)
inline float tileToWorld(int tile)
{
    return pixelToWorld(tileToPixel(tile));
}

/// Box2D (Meter) → Tile (Berechnung über Pixel)
inline int worldToTile(float world)
{
    return pixelToTile(worldToPixel(world));
}

}