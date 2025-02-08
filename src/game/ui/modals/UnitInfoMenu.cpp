#include "UnitInfoMenu.hpp"

#include <SDL_ttf.h>
#include <iostream>

namespace advanced_wars
{

// UnitInfoMenu::UnitInfoMenu() : m_currentUnit(nullptr), m_isVisible(false) {}

UnitInfoMenu::UnitInfoMenu() : m_currentUnit(nullptr) {}

std::string UnitInfoMenu::getMovementTypeString(MovementType type)
{
    switch (type)
    {
    case MovementType::FOOT:
        return "Foot";
    case MovementType::WHEELED:
        return "Wheeled";
    case MovementType::TREAD:
        return "Tracked";
    case MovementType::AIR:
        return "Aircraft";
    case MovementType::SEA:
        return "Ship";
    case MovementType::LANDER:
        return "Lander";
    default:
        return "Unknown";
    }
}

std::unordered_map<UnitId, std::string> unitDescriptions = {
    {                UnitId::INFANTERY,"Infanterie   Kostenguenstig   und   vielseitig   einsetzbar"                                       },
    {     UnitId::MECHANIZED_INFANTERY,
     "Mech-Infanterie   Stark   gegen   Panzer   langsam   aber   effizient"                                   },
    {                    UnitId::RECON,       "Aufklaerung   Schnell   und   ideal   für   frühe   Aufklaerung"},
    {              UnitId::MEDIUM_TANK,    "Mittlerer   Panzer   Guter   Allrounder   stark   und   ausgewogen"},
    {               UnitId::HEAVY_TANK, "Schwerer   Panzer   Langsam   aber   sehr   stark   und   beschützend"},
    {                 UnitId::NEO_TANK,             "Neo Tank   Einer der besten Panzer   stark und vielseitig"},
    {                      UnitId::APC,         "Transporter   Traeger fuer Infanterie   keine Offensivkraefte"},
    {            UnitId::ANTI_AIR_TANK,                         "FlugabwehrPanzer   Ideal zur Luftverteidigung"},
    {                UnitId::ARTILLERY,  "Artillerie   Kann aus Distanz zufuegen   aber verletzbar im Nahkampf"},
    {         UnitId::ROCKET_ARTILLERY,
     "Raketenartillerie   Grosse Reichweite   ideal fuer defensive Taktiken"                                   },
    {UnitId::ANTI_AIR_MISSILE_LAUNCHER,
     "Raketenwerfer   Kann Flugeinheiten auf grosse Distanz angreifen"                                         },
    {                  UnitId::FIGHTER,                                "Jaeger   Ideal fuer Luftueberlegenheit"},
    {                   UnitId::BOMBER,
     "Bomber   Stark gegen Boden- und Seeziele   aber verletzbar gegen Luft-und Flak"                          },
    {        UnitId::BATTLE_HELICOPTER,         "Kampfhubschrauber   Stark gegen Bodenfahrzeuge und Infanterie"},
    {     UnitId::TRANSPORT_HELICOPTER,         "Transporthubschrauber   Kann Einheiten schnell transportieren"},
    {               UnitId::BATTLESHIP,              "Schlachtschiff   Langreichweitenangriff auf See und Land"},
    {                  UnitId::CRUISER,                "Kreuzer   Verteidigung gegen Luft und U-Boot-Einheiten"},
    {                   UnitId::LANDER,                             "Landungsschiff   Transport und Versorgung"},
    {                UnitId::SUBMARINE,           "U-Boot   Versteckt sich und kann Ueberwasserziele angreifen"}
};

void UnitInfoMenu::handleEvent(Engine& engine, SDL_Event& event)
{
    // Hier kannst du den Code hinzufügen, um die Ereignisse für das UnitInfoMenu zu behandeln
    // Wenn keine spezifische Ereignisbehandlung erforderlich ist, kann diese Methode auch leer
    // bleiben.
}

void UnitInfoMenu::setUnit(Unit& unit)
{
    m_currentUnit = &unit;
    m_isVisible = true;
}

void UnitInfoMenu::render(Engine& engine)
{
    if (!m_currentUnit || !m_isVisible)
        return;

    // TTF Initialisierung
    if (TTF_Init() == -1)
    {
        std::cerr << "TTF konnte nicht initialisiert werden: " << TTF_GetError() << std::endl;
        return;
    }

    std::string basePath = SDL_GetBasePath();
    std::string fullPath = basePath + "res/ARCADECLASSIC.TTF";
    TTF_Font*   font = TTF_OpenFont(fullPath.c_str(), 16);

    if (!font)
    {
        std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
        return;
    }

    SDL_Color yellow = {255, 255, 0, 255}; // Gelb für den Text
    int       spacing = 10;                // Abstand zwischen den Textzeilen
    UnitId    unitId = m_currentUnit->getId();

    // Textzeilen, einschließlich der Beschreibung
    std::vector<std::string> info_lines = {
        "HP   " + std::to_string(m_currentUnit->getHealth()),
        "Movement   " + std::to_string(m_currentUnit->getMovementPoints()),
        "Ammo   " + std::to_string(m_currentUnit->getAmmo()),
        "Movement   Type   " + getMovementTypeString(m_currentUnit->getMovementType()),
        "Cost   " + std::to_string(m_currentUnit->getCost()),
        unitDescriptions[unitId] // Beschreibung einfügen
    };

    int                       max_text_width = 0;
    int                       total_height = 0;
    std::vector<SDL_Texture*> textures;

    for (const auto& line : info_lines)
    {
        SDL_Surface* textSurface = TTF_RenderText_Solid(font, line.c_str(), yellow);
        if (!textSurface)
            continue;

        max_text_width = std::max(max_text_width, textSurface->w);
        total_height += textSurface->h + spacing;
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(engine.renderer(), textSurface);
        textures.push_back(textTexture);
        SDL_FreeSurface(textSurface);
    }

    int width = std::max(max_text_width + 20 * RENDERING_SCALE, 16 * RENDERING_SCALE + 20);
    int height =
        total_height + 30 * RENDERING_SCALE; // Die Höhe anpassen, um alle Textzeilen zu integrieren
    SDL_Rect box = {m_x, m_y, width, height};

    SDL_SetRenderDrawColor(engine.renderer(), 75, 87, 219, 255); // Schwarzes Hintergrundrechteck
    SDL_RenderFillRect(engine.renderer(), &box);

    SDL_SetRenderDrawColor(engine.renderer(), 255, 255, 255, 255); // Weißer Rahmen
    SDL_RenderDrawRect(engine.renderer(), &box);

    // Render unit sprite
    Spritesheet* spritesheet = engine.getSpritesheet();
    SDL_Texture* unit_texture = spritesheet->getUnitTextures()
                                    .at(static_cast<int>(m_currentUnit->getFaction()))
                                    .at(static_cast<int>(unitId))
                                    .at(static_cast<int>(UnitState::IDLE))
                                    .first;

    SDL_Rect sprite_rect = {m_x + 10, m_y + 10, 16 * RENDERING_SCALE, 16 * RENDERING_SCALE};
    SDL_Rect source_rect = {0, 0, 16, 16};
    SDL_RenderCopy(engine.renderer(), unit_texture, &source_rect, &sprite_rect);

    // Text zeichnen
    int text_y = m_y + 20 * RENDERING_SCALE; // Starte etwas unterhalb des Sprites
    for (auto* texture : textures)
    {
        int w, h;
        SDL_QueryTexture(texture, nullptr, nullptr, &w, &h);
        SDL_Rect textRect = {m_x + 10, text_y, w, h};
        SDL_RenderCopy(engine.renderer(), texture, nullptr, &textRect);
        SDL_DestroyTexture(texture);
        text_y += (h + spacing);
    }

    TTF_CloseFont(font);
    TTF_Quit();
}

void UnitInfoMenu::update(int x, int y)
{
    m_x = x;
    m_y = y;
}

} // namespace advanced_wars
