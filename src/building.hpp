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
        int captureProgress;     // Fortschritt der Eroberung (0,1,2) 
        int captureThreshold;
        BuildingFaction bFaction;
        virtual bool isCapturable();
        virtual bool getUnitType(); // to check if the unit is inf or any other viable type
        int getCaptureProgress() const { return captureProgress; }
        virtual void capture(BuildingFaction newFaction, int capturePoints); //start capture: HP of unit is 
        virtual void resetCapture();//reset health and stuff

        // when clicked, start Interface stuff
        virtual void on_Click();
        virtual void showBuildMenu();
        virtual void hideBuildMenu();

        //for recruiting
        //check to see if enough monies and space available
        virtual bool canRecruitUnit(const std::string& unitName) const;
        virtual bool isSpawnPositionClear() const;
        virtual bool hasEnoughResources(int cost) const;
        virtual int getUnitCost(const std::string& unitName) const;
        
        virtual bool recruit_Unit(const std::string& name);
        std::vector<std::string> recruitable_units; // buildable Units
        std::map<std::string, int> unit_costs;
};
#endif