#include "Map.h"
#include <fstream>
#include <ostream>
#include <filesystem>


void testLoadMap(){
    cout << endl;

    std::cout << "------------------TESTING MAP-----------------" << std::endl;
    cout << endl;

    std::vector<string> fullPaths;

    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Anatomy.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Asia.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Brazil.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Disconnected_continent.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Disconnected_map.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Invalid_map.map");
    fullPaths.push_back("C:\\Users\\Krish\\Documents\\COMP345Warzone\\Map\\map_files\\Territory_More_Than_One_Cont.map");

    //Loop through each file, create maps and validate them, print their info out
    for(string paths: fullPaths){
        cout << "Testing: " << paths << endl;
        Map* testMap = MapLoader().loadMap(paths);
        if(testMap->validate()){
            cout << "MAP IS VALID: " << endl;
            cout << testMap << endl;
        }else {
            cout << "MAP IS NOT!! VALID: " << endl;
            cout << testMap << endl;
        }
    }
    std::cout << "---------------end: TESTING MAP---------------" << std::endl;
    cout << endl;


}

//int main(){
//    testLoadMap();
//    return 0;
//}