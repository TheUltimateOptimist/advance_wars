/**
* EventHandler.cpp
*
* @date 29.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
* @author Nils Jonathan Friedrich Eckardt onCheckBoxToggle added
*/

#pragma once

#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <functional>
#include <QString>

namespace editor
{

// forward declaration of Tile
class Tile;

/**
 * EventHandler is the backbone in the architecture that makes interaction possible.
 * 
 * Classes that want to act on specific LevelEditor Events can subclass EventHandler
 * and overwrite the methods for the events they want to handle. If they don't overwrite
 * an event method the default implemenatation (doing nothing) is provided by EventHandler.
 * 
 * EventHandler stores all instances of EventHandler subclasses in a static variable.
 * Events can be emitted from anywhere in the program by using the static send method.
 * Emitting an event can be achieved by providing a callback to the send method that
 * receives an EventHandler instance as an argument and invokes the desired event method
 * on it. The static send method will invoke that event method for every EventHandler instance
 * from the static instances variable.
 */
class EventHandler {
public:
    /**
     * Saves a pointer to itself in the instances variable at construction.
     */
    EventHandler();

    /**
     * Removes the pointer to itself from the instances variable at deconstruction.
     */
    virtual ~EventHandler();

    /**
     * Executes the provided callback for every EventHandler instance from the static instances variable.
     * @param callback The callback that executes the desired event method.  
     *                 It will be executed for every EventHandler instance.
     */
    static void send(std::function<void(EventHandler*)> callback);

    /**
     * Overwrite this event method to handle the dispatch of a new level name 
     * by the level name edit field.
     * @param new_name The new level name.
     */
    virtual void onLevelNameUpdated(std::string){};

    /**
     * Overwrite this event method to handle the dispatch of a request to write 
     * the level to disk by the save button.
     * @param file_path The path to the file into which the level should be written.
     */
    virtual void onLevelWriteRequested(QString){};

    /**
     * Overwrite this event method to handle the dispatch of a new tile index due
     * to the mouse entering its boundaries.
     * @param index The index of the entered tile.
     */
    virtual void onTileEntered(int){};

    /**
     * Overwrite this event method to handle the dispatch of a new tile index due
     * to the mouse leaving its boundaries.
     * @param index The index of the exited tile.
     */
    virtual void onTileExited(int){};

    /**
     * Overwrite this event method to handle the dispatch of a new tile index due
     * to the user clicking it.
     * @param index The index of the clicked tile.
     */
    virtual void onTileClicked(int){};

    /**
     * Overwrite this event method to handle the dispatch of a new tile_id due to 
     * the user selecting a different tile type in the TileSelector on the right.
     * @param tile_id Id of the tile to use when an existing tile is clicked on the levelmap.
     */
    virtual void onNewTileIdSelected(uint8_t){};

    /**
     * Overwrite this event method to handle the change of tile placement method due
     * to the user selecting automatic tile placement assisstance.
     */
    virtual void onCheckBoxToggled(){};

    /**
     * Overwrite this event method to handle the dispatch of a new delta due to 
     * the user pressing the zoom in or zoom out button.
     * @param delta Amount to zoom in or out, e.g 0.2 => 20% in, -0.2 => 20% out.
     */
    virtual void onZoomed(double){}

private:
    /**
     * Stores all existing EventHandler instances so that the send method can 
     * invoke the emiited event method on every existing instance.
     */
    static std::vector<EventHandler*> instances ;
};

} // namespace editor