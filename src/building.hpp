#ifndef BUILDING_HPP
#define BUILDING_HPP

#include <vector>
#include <string>
#include <map>

enum class BuildingFaction {
    None = 0,    
    Player1,     
    Player2,     
    Player3,     
    Player4      
};

enum class BuildingType {
    Capital = 0,
    City,
    Factory,
    Airport,
    Seaport
};

class Building{
    public:
    Building(int x, int y, BuildingFaction bFaction, int buildingHealth, BuildingType bType);
    virtual ~Building() = default;

    // Getters/Setters
    int getX() const { return x; }
    int getY() const { return y; }
    // ...
    // ...

    private:
        // Position
        int x;
        int y;

        int buildingHealth;
        BuildingType bType;

        // for capturing
        bool capturable;
        int captureProgress;// Fortschritt der Eroberung (0,1,2) 
        int captureThreshold;
        BuildingFaction bFaction;

        /*
        Get the capturable parameter
        To differentiate between 
        */
        virtual bool isCapturable();

        /*
        Get the required unit type
        Only Infantry should be able to cap
        */
        virtual bool getUnitType(); // to check if the unit is inf or any other viable type

        /*
        This returns the capture of the building
        Might not be necessary if we issue the command that it takes exactly one turn for each building
        But could be cool if the capital (win-con) takes 2 turns
        */
        int getCaptureProgress() const { return captureProgress; }

        /*
        Start capturing: 
        Initialize the new Faction, and calculate caputurePoints
        This should be started when an opposing team moves onto a building and thus starts capturing
        */
        virtual void capture(BuildingFaction newFaction, int capturePoints);

        /*
        Simply set the affected stats of the building back to base if the capturing unit is neutralized
        */
        virtual void resetCapture();//reset health and stuff
        
        /*
        Checks if the building is clicked and calls the buildmenus
        */
        virtual void on_Click();

        /*
        Called only if the building can build something
        Should show an overview of all Units that could be build and renders their price
        */
        virtual void showBuildMenu();

        /*
        Hides the build menu when no longer needed
        */
        virtual void hideBuildMenu();

        //for recruiting

        /*
        This checks if the player can recruit the chosen unit 
        */
        virtual bool canRecruitUnit(const std::string& unitName) const;

        /*
        Checks if the spawnposition is free from units
        */
        virtual bool isSpawnPositionClear() const;

        /*
        Checks if the player has enough resources to recruit the unit
        */
        virtual bool hasEnoughResources(int cost) const;

        /*
        Must get the cost of the unit that has to be paid
        could use a std::map with unitnames as keys and with prices as values
        */
        virtual int getUnitCost(const std::string& unitName) const;
        
        /*
        Instantiates a new unit from the chosen type and spawns it on the map
        Transfers ownership to the recruiting player
        */
        virtual bool recruit_Unit(const std::string& name);
        std::vector<std::string> recruitable_units; // buildable Units
        std::map<std::string, int> unit_costs;
};
#endif