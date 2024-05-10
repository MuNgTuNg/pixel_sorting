#pragma once
#include <fstream>
#include <string>
#include <iostream>
#include <vector>
#include <stb_image.h>

//#include <windows.h>
namespace shb{


inline std::string readFile(const std::string& filePath) {
    std::string content;
    std::ifstream fileStream(filePath, std::ios::in);
    if(!fileStream.is_open()) {
        std::cerr << "Could not read file " << filePath << ". File does not exist." << std::endl;
        return "";
    }
    std::string line = "";
    while(!fileStream.eof()) {
        std::getline(fileStream, line);
        content.append(line + "\n");
    }
    fileStream.close();
    return content;
}
// Function to get the path of the executable
inline std::string getExecutablePath() {
    // char buffer[MAX_PATH];
    // GetModuleFileName(NULL, buffer, MAX_PATH);
    // std::string buf = std::string(buffer);
    // return buf;

}

// inline void printDetailsAboutEntry(std::filesystem::__cxx11::directory_entry entry){
//     std::cout 
//     <<"----------------------------------------\n"
//     << "exists?: " << entry.exists() << "\n"
//     << "File Size: " << entry.file_size() << "\n"
//     << "Is regular file?: " << entry.is_regular_file() << "\n"
//     << "Path: " << entry.path() << "\n"
//     << "Extension: " << entry.path().extension() << "\n"
//     << "filename: " << entry.path().filename() << "\n"
//     << "stem: " << entry.path().stem() << "\n"
//     << "parent_path: " << entry.path().parent_path() << "\n"
//     << "relative_path: " << entry.path().relative_path() << "\n"
//     << "root_path: " << entry.path().root_path() << "\n"
//     << "root_directory: " << entry.path().root_directory() << "\n"
//     << "root_name: " << entry.path().root_name() << "\n"
//     <<"----------------------------------------\n";
// }

/**
 * @brief This function takes in an extension to search for, a directory to search, and a vector to fill with results.
 *
 * 
 *
 * @param extension The extension to search for
 * @param fp The filepath
 * @return integer to indicate if operation was success or not
 */
// inline int readFilesInDirectoryFromExtensions(const std::string& extension, const std::string& fp,std::vector<std::filesystem::__cxx11::directory_entry>& fileNames ){
//     for (const auto& entry : std::filesystem::recursive_directory_iterator(std::filesystem::path{fp})) {
//         //printDetailsAboutEntry(entry);
//         if (std::filesystem::is_regular_file(entry) && entry.path().extension() == extension) {
//             std::cout << "Found file: " << entry.path().filename().stem() << " in directory: " << entry.path() << std::endl;
//             fileNames.push_back(entry);
//         }
//     }
//     return 0;
// }



}//namespace shb