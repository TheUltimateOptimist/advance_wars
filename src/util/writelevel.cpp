#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <highfive/H5File.hpp>

int main()
{
    boost::property_tree::ptree pt;

    // Add data to the property tree
    pt.put("level.width", "15");
    pt.put("level.height", "10");
    pt.put("level.name", "Alpha");

    std::ostringstream xmlStream;
    boost::property_tree::write_xml(xmlStream, pt);
    std::string          xml_data = xmlStream.str();
    std::vector<uint8_t> levelmap = {
        2,  3,  3, 3, 2,  2,  2, 3, 3, 3, 3, 1, 0, 0, 0,  0, 2, 3, 0, 0, 0,  3, 3, 3, 3,
        0,  4,  0, 0, 0,  0,  0, 0, 0, 0, 0, 0, 3, 3, 0,  0, 1, 0, 0, 2, 0,  0, 0, 0, 76,
        76, 51, 3, 0, 0,  0,  1, 0, 3, 3, 0, 0, 0, 0, 0,  0, 0, 3, 0, 0, 57, 1, 3, 0, 0,
        50, 0,  0, 0, 76, 77, 0, 3, 0, 0, 0, 1, 0, 0, 55, 0, 0, 0, 0, 0, 0,  0, 3, 3, 0,
        0,  1,  3, 0, 0,  0,  0, 0, 0, 0, 0, 0, 0, 3, 3,  3, 1, 0, 3, 3, 2,  3, 3, 0, 0,
        0,  0,  0, 0, 0,  0,  4, 0, 0, 2, 3, 3, 3, 3, 0,  3, 2, 2, 3, 2, 3,  1, 3, 2, 2,
    };

    HighFive::File file("level.h5", HighFive::File::Truncate);
    file.createDataSet<std::string>("metadata", HighFive::DataSpace::From(xmlStream))
        .write(xml_data);
    file.createDataSet<uint8_t>("tilesarray", HighFive::DataSpace::From(levelmap)).write(levelmap);
}
