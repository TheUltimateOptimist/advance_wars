/**
* LevelScene.cpp
*
* @date 28.01.2025
* @author Jonathan Dueck (jonathan.dueck@informatik.hs-fulda.de)
* @author Nils Jonathan Friedrich Eckardt implemented advanced placement
*/

#include "LevelScene.hpp"

#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPoint>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>


#include "highfive/H5File.hpp"
#include "SpriteProvider.hpp"

namespace editor
{

LevelScene::LevelScene(
    const std::string& name, int width, int height, std::vector<uint8_t> tile_ids,
    const std::string& file_path, QWidget* parent)
    : QGraphicsScene(parent), m_name(name), m_width(width), m_height(height), m_tile_ids(tile_ids),
      m_file_path(file_path), m_selected_tile_id(2)
{
    setSceneRect(0, 0, m_width * 16, m_height * 16);
    m_advanced_tile_placement = false;
    m_tile_occupants = {};
    m_tile_occupants.reserve(tile_ids.size());
    for (int index = 0; index < tile_ids.size(); index++)
    {
        int   x = (index % m_width) * 16;
        int   y = (index / m_width) * 16;
        Tile* tile = new Tile(index, is_border(index) ? tile_ids[index] : 0);
        addItem(tile);
        tile->setZValue(0);
        tile->setPos(x, y);
        if (!is_border(index) && tile_ids[index] > 0)
        {
            m_tile_occupants.push_back(occupy_tile(index, tile_ids[index]));
        }
        else
        {
            m_tile_occupants.push_back(nullptr);
        }
    }
}

LevelScene* LevelScene::empty(const std::string& name, int width, int height, QWidget* parent)
{
    // + 2 because of surrounding the map with cliffs
    std::vector<uint8_t> tile_ids;
    tile_ids.reserve(width*height);

    // create top row with cliffs
    tile_ids.push_back(22); // cliff corner top left 
    for (int i = 0; i < width - 2; i++) {
        tile_ids.push_back(19); // cliff bottom
    }
    tile_ids.push_back(23); // cliff corner top right 

    // create main rows with cliff at start and end
    for (int i = 0; i < height - 2; i++) {
        tile_ids.push_back(21); // cliff right
        for (int j = 0; j < width - 2; j++) {
            tile_ids.push_back(0); // pleins
        }
        tile_ids.push_back(20); // cliff left
    }

    // create bottom row with cliffs
    tile_ids.push_back(24); // cliff corner bottom left 
    for (int i = 0; i < width - 2; i++) {
        tile_ids.push_back(18); // cliff top
    }
    tile_ids.push_back(25); // cliff corner bottom right 

    return new LevelScene(name, width, height, tile_ids, "../res/level_new.h5", parent);
}

LevelScene* LevelScene::fromFile(const std::string& file_path, QWidget* parent)
{
    HighFive::File file(file_path, HighFive::File::ReadOnly);

    // read level metadata
    std::string level_metadata;
    file.getDataSet("metadata").read(level_metadata);

    // read tilesarray
    std::vector<uint8_t> level_tilesarray;
    file.getDataSet("tilesarray").read(level_tilesarray);

    // extract metadata from xml
    std::istringstream          xmlStream(level_metadata);
    boost::property_tree::ptree pt;
    boost::property_tree::read_xml(xmlStream, pt);
    int         width = pt.get<int>("level.width");
    int         height = pt.get<int>("level.height");
    std::string name = pt.get<std::string>("level.name");

    return new LevelScene(name, width, height, level_tilesarray, file_path, parent);
}

std::string LevelScene::getName()
{
    return m_name;
}

int LevelScene::getWidth()
{
    return m_width;
}

int LevelScene::getHeight()
{
    return m_height;
}

bool LevelScene::is_border(int index)
{
    return (index / m_width) == 0 
    || (index / m_width) == (m_height -1)
    || (index % m_width) == 0 
    || (index % m_width) == (m_width - 1);
}

bool LevelScene::is_water_tile(uint8_t id)
{
    if (id == 1) 
    {
        return true;
    }
    if (id >= 17 && id <= 29)
    {
        return true;
    }
    return false;
}

void LevelScene::onLevelNameUpdated(std::string new_name)
{
    m_name = new_name;
}

void LevelScene::onLevelWriteRequested(QString file_path)
{
    boost::property_tree::ptree pt;

    // Add data to the property tree
    pt.put("level.width", m_width);
    pt.put("level.height", m_height);
    pt.put("level.name", m_name);

    // convert property tree to xml string
    std::ostringstream xmlStream;
    boost::property_tree::write_xml(xmlStream, pt);
    std::string xml_data = xmlStream.str();

    // write level to hdf5
    HighFive::File file(file_path.toStdString(), HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream))
        .write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(m_tile_ids))
        .write(m_tile_ids);
}

void LevelScene::onTileEntered(int index)
{
    if (m_selected_tile_id == m_tile_ids[index])
    {
        return;
    }
    if (is_border(index) && !is_water_tile(m_selected_tile_id))
    {
        return;
    }
    if (m_tile_occupants[index] != nullptr)
    {
        removeItem(m_tile_occupants[index]);
        delete m_tile_occupants[index];
        m_tile_occupants[index] = nullptr;
    }
    if (!is_border(index) && m_selected_tile_id > 0 || is_border(index))
    {
        m_tile_occupants[index] = occupy_tile(index, m_selected_tile_id);
    }
}

void LevelScene::onTileExited(int index)
{
    if (m_selected_tile_id == m_tile_ids[index])
    {
        return;
    }
    if (m_tile_occupants[index] != nullptr)
    {
        removeItem(m_tile_occupants[index]);
        delete m_tile_occupants[index];
        m_tile_occupants[index] = nullptr;
    }
    if (m_tile_ids[index] > 0)
    {
        m_tile_occupants[index] = occupy_tile(index, m_tile_ids[index]);
    }
}

void LevelScene::setTile(int index, uint8_t id)
{
    m_tile_ids[index]=id;
    if (m_tile_occupants[index] != nullptr)
    {
        removeItem(m_tile_occupants[index]);
        delete m_tile_occupants[index];
        m_tile_occupants[index] = nullptr;
    }
    if (m_tile_ids[index] > 0)
    {
        m_tile_occupants[index] = occupy_tile(index, id);
    }
}

void LevelScene::onTileClicked(int index)
{
    /*
    m_tile_ids[index] = m_selected_tile_id;
    if(!(m_advanced_tile_placement)) return;
    */

	if (is_border(index) && !is_water_tile(m_selected_tile_id)) {
		return;
	}
	std::cout << m_advanced_tile_placement << std::endl;
	if (!m_advanced_tile_placement) {
    	m_tile_ids[index] = m_selected_tile_id;
		return;
	}
	if(m_selected_tile_id > 5 && m_selected_tile_id < 17){	//Straße plaziert
		placeRoad(index, true);
		return;
	}
	if((m_selected_tile_id > 16 && m_selected_tile_id < 30) || m_selected_tile_id == 1){	//Wasser plaziert
		if(m_selected_tile_id == 17){
			setTile(index,17);
		} else {
			setTile(index,1);
		}
		placeCliff(false, index);
		return;
	}
	if(m_selected_tile_id == 0 || m_selected_tile_id == 2 || m_selected_tile_id == 3){	//Land plaziert
		setTile(index,m_selected_tile_id);
		placeCliff(true, index);
		return;
	}
}

void LevelScene::onNewTileIdSelected(uint8_t tile_id)
{
    m_selected_tile_id = tile_id;
}

void LevelScene::onCheckBoxToggled()
{
    m_advanced_tile_placement = !m_advanced_tile_placement;
}

QGraphicsPixmapItem* LevelScene::occupy_tile(int index, uint8_t tile_id)
{
    int                  x = (index % m_width) * 16;
    int                  y = (index / m_width) * 16;
    QPixmap              tile_occupant = SpriteProvider::get_sprite(tile_id);
    QGraphicsPixmapItem* tile_occupant_item = addPixmap(tile_occupant);
    tile_occupant_item->setZValue(tile_id < 50 ? 1 : 2 + index);
    tile_occupant_item->setPos(x, tile_id < 50 ? y : y - 16);
    return tile_occupant_item;
}















void LevelScene::placeCliff(bool placedLand, int index){
	const int16_t IdToSum[12] = {12,3,9,6,2,1,4,8,13,14,11,7};
	const int16_t SumToId[16] = {1,23,22,19,24,24,21,29,25,20,25,28,18,26,27,0};

	int16_t surroundingIDs[8] = {-2,-2,-2,-2,-2,-2,-2,-2};	//-2 = uninitialisiert, -1 = kartenrand
	bool marchingSquares[8][4] = {false};
	
	//Überprüfe Kartenränder, setze diese auf -1
	if(index < m_width){					//oberer Kartenrand
		surroundingIDs[7] = -1;
		surroundingIDs[0] = -1;
		surroundingIDs[1] = -1;
	}
	if((index + 1) % m_width == 0){			//rechter Kartenrand
		surroundingIDs[1] = -1;
		surroundingIDs[2] = -1;
		surroundingIDs[3] = -1;
	}
	if(index > m_width * (m_height - 1)){	//unterer Kartenrand
		surroundingIDs[3] = -1;
		surroundingIDs[4] = -1;
		surroundingIDs[5] = -1;
	}
	if(index % m_width == 0){				//linker Kartenrand
		surroundingIDs[5] = -1;
		surroundingIDs[6] = -1;
		surroundingIDs[7] = -1;
	}

	//Erhalte IDs von den umliegenden 8 Feldern, die kein Kartenrand sind
	//startet in der mitte Oben, gehe im Uhrzeigersinn
	if(surroundingIDs[0] == -2) surroundingIDs[0] = (int16_t) m_tile_ids[index - m_width];
	if(surroundingIDs[1] == -2) surroundingIDs[1] = (int16_t) m_tile_ids[index - m_width + 1];
	if(surroundingIDs[2] == -2) surroundingIDs[2] = (int16_t) m_tile_ids[index + 1];
	if(surroundingIDs[3] == -2) surroundingIDs[3] = (int16_t) m_tile_ids[index + m_width + 1];
	if(surroundingIDs[4] == -2) surroundingIDs[4] = (int16_t) m_tile_ids[index + m_width];
	if(surroundingIDs[5] == -2) surroundingIDs[5] = (int16_t) m_tile_ids[index + m_width - 1];
	if(surroundingIDs[6] == -2) surroundingIDs[6] = (int16_t) m_tile_ids[index - 1];
	if(surroundingIDs[7] == -2) surroundingIDs[7] = (int16_t) m_tile_ids[index - m_width - 1];

	//Ids können nicht lesbare Tiles enthalten, diese Herausfiltern
	for(int i = 0; i < 8; i++){
		if(surroundingIDs[i] == 17)                          surroundingIDs[i] = 1;		//Riff als Wasser angesehen
		if(surroundingIDs[i] == 2 || surroundingIDs[i] == 3) surroundingIDs[i] = 0;		//Wald, Gebirge als Land angesehen
		if(surroundingIDs[i] > 3 && surroundingIDs[i] < 17)  surroundingIDs[i] = -1;	//Straßen werden nicht verändert
		if(surroundingIDs[i] > 29)                           surroundingIDs[i] = 0;	//Gebäude werden als Land angesehen
	}

	//ID remapping um damit arbeiten zu können
	for(int i = 0; i < 8; i++){
        if(surroundingIDs[i] == -1){
        } else if(surroundingIDs[i] == 0){
			surroundingIDs[i] = 15;
		} else if(surroundingIDs[i] == 1){
			surroundingIDs[i] = 0;
		} else{
			surroundingIDs[i]= IdToSum[surroundingIDs[i] - 18];
		}
	}
	
	//Berechne Marching Squares aus der remapped Id
	for(int i = 0; i < 8; i++){
		if(surroundingIDs[i] / 8 == 1){
			marchingSquares[i][0] = true;	//top left
			surroundingIDs[i] -= 8;
		}
		if(surroundingIDs[i] / 4 == 1){
			marchingSquares[i][1] = true;	//top right
			surroundingIDs[i] -= 4;
		}
		if(surroundingIDs[i] / 2 == 1){
			marchingSquares[i][2] = true;	//bottom right
			surroundingIDs[i] -= 2;
		}
		if(surroundingIDs[i] == 1){
			marchingSquares[i][3] = true;	//bottom left
		}
	}
	
	//Einfügen/Abziehen des Landes
	marchingSquares[0][2] = placedLand;	//oben
	marchingSquares[0][3] = placedLand;
	marchingSquares[1][3] = placedLand;	//oben rechts
	marchingSquares[2][3] = placedLand;	//rechts
	marchingSquares[2][0] = placedLand;
	marchingSquares[3][0] = placedLand;	//untern rechts
	marchingSquares[4][0] = placedLand;	//unten
	marchingSquares[4][1] = placedLand;
	marchingSquares[5][1] = placedLand;	//unten links
	marchingSquares[6][1] = placedLand;	//links
	marchingSquares[6][2] = placedLand;
	marchingSquares[7][2] = placedLand;	//oben links
	
	//Berechne remapped ID aus Marching Squares
	for(int i = 0; i < 8; i++){
		if(surroundingIDs[i] > -1){
            surroundingIDs[i] = 0;
		    if(marchingSquares[i][0]) surroundingIDs[i] += 8;
	    	if(marchingSquares[i][1]) surroundingIDs[i] += 4;
		    if(marchingSquares[i][2]) surroundingIDs[i] += 2;
    		if(marchingSquares[i][3]) surroundingIDs[i] += 1;
        }
    }
	
	//Remappe ID für setzbare Tiles
	for(int i = 0; i < 8; i++){
		if(surroundingIDs[i] > -1) surroundingIDs[i] = SumToId[ surroundingIDs[i] ];
	}
	
	//Plaziere Tiles
    for(int i = 0; i < 8; i++){
	    if(isntIdentical(surroundingIDs[i], calcDir(i, index)))
            setTile( calcDir(i, index),(uint8_t)surroundingIDs[i] );
    }
}

void LevelScene::placeRoad(int index, bool updateFlag){
	const int16_t IdToSum[] = {5,10,13,14,11,7,15,6,3,12,9};
	const uint8_t SumToId[] = {6,6,7,14,6,6,13,11,7,16,7,10,15,8,9,12};
	
	bool tileDirections[] = {false,false,false,false};
	int16_t surroundingIDs[] = {-1,-1,-1,-1};		//-1 = kartenrand oder keine strasse
	uint8_t placedID = 0;
	
	//Erhalte IDs der umliegenden Felder - im Uhrzeigersinn, start: oben
	//Vorgehen: überprüfe, ob die obere Zelle im Kartenrand ist. Wenn nicht erhalte index der oberen Zelle
	if(!(index < m_width))                surroundingIDs[0] = (int16_t) m_tile_ids[index - m_width];
	if(!((index + 1) % m_width == 0))     surroundingIDs[1] = (int16_t) m_tile_ids[index + 1];
	if(!(index > m_width * (m_height-1))) surroundingIDs[2] = (int16_t) m_tile_ids[index + m_width];
	if(!(index % m_width == 0))           surroundingIDs[3] = (int16_t) m_tile_ids[index - 1];
	
	//Umformen der IDs oder ID ist keine Straße
	for(int i = 0; i < 4; i++){
		if(surroundingIDs[i] < 6 || surroundingIDs[i] > 16){
			surroundingIDs[i] = -1;			//ID ist keine Strasse
		} else {
			surroundingIDs[i] = IdToSum[ surroundingIDs[i] - 6];	//id remapping
		}
	}

	//Berechne in welche Richtungen das neue Tile Zeigt
	for(int i = 0; i < 4; i++){
		if(surroundingIDs[i] > -1) tileDirections[i] = true;
	}
	
	//Berechne die Remapped ID
	placedID = 0;
	if(tileDirections[0]) placedID += 8;
	if(tileDirections[1]) placedID += 4;
	if(tileDirections[2]) placedID += 2;
	if(tileDirections[3]) placedID += 1;
	
	if(placedID == 0) updateFlag = false; 	//Umliegende Tiles müssen nicht geupdated werden, da keine Strassen
	placedID = SumToId[placedID];			//Berechnete Summe in Valide ID umformen
	setTile(index, placedID);	            //Tile setzen
	
	//Update umliegende Tiles
	if(updateFlag){	
		if(tileDirections[0]) placeRoad((index - m_width), false);	//update oben
		if(tileDirections[1]) placeRoad((index + 1),       false);	//update rechts
		if(tileDirections[2]) placeRoad((index + m_width), false);	//update unten
		if(tileDirections[3]) placeRoad((index - 1),       false);	//update links
	}
}

int LevelScene::calcDir(int i, int index){
    if(i==0) return (index - m_width);
    if(i==1) return (index - m_width + 1);
    if(i==2) return (index + 1);
    if(i==3) return (index + m_width + 1);
    if(i==4) return (index + m_width);
    if(i==5) return (index + m_width - 1);
    if(i==6) return (index - 1);
    if(i==7) return (index - m_width - 1);
}

bool LevelScene::isntIdentical(int16_t id, int index){
    bool flag  = (!(id == -1 ||               //kartenrand
    (id == 0 && m_tile_ids[index]==2) ||   //wald
    (id == 0 && m_tile_ids[index]==3) ||   //gebirge
    (id == 1 && m_tile_ids[index]==17)) );  //riff
    return flag;
}

} // namespace editor