#include "sbne/ne_core.h"

using namespace sbne;

int main () {
    
    // set filename value to the absolute path of the SBML (.xml) file.
    const std::string filename = "Absolute path to the sbml (.xml) file";
    
    // set elementid value to the id of a network element.
    const std::string elementid = "Id of a network element";
    
    // set color value to a desired color for the element.
    const std::string color = "Desired color for the element";
    
    // create an infolist which is used by set function to set the stroke color of a network element
    std::unordered_map<std::string, std::string> infolistset({
        {"filename", filename},
        {"id",elementid},
        {"stroke",color}
        });
    
    // create an infolist which is used by get function to get the stroke color of a network element
    std::unordered_map<std::string, std::string> infolistget({
        {"filename", filename},
        {"id",elementid},
        {"key","stroke"}
        });
    
    // set the stroke color of the network element
    if (!ne_set(infolistset))
        std::cout << "The desired value is set\n";
    else
        std::cout << "Failed to set the desired value\n";
    
    // get the stroke color of the network element
    std::string value = ne_get(infolistget);
    if (!value.empty())
        std::cout << "The desired value is: " << value << "\n";
    else
        std::cout << "Failed to get the desired value\n";
}
