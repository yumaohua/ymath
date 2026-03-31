#include "TimeIntegrator.h"
#include <fstream>
#include <filesystem>
void Output(const std::vector<std::vector<double>>& vec2,const std::string& outputpath,std::ios_base::openmode mode){
    std::filesystem::path filepath(outputpath);
    std::filesystem::path dir = filepath.parent_path();
    if (!dir.empty() && !std::filesystem::exists(dir)) {
        if (!std::filesystem::create_directories(dir)) {
            std::cerr << "Could not create directories: " << dir << std::endl;
            return;
        }
    }
    std::ofstream outFile(outputpath,mode);
    if (!outFile) {
        std::cerr << "Could not open the file for writing: " << outputpath << std::endl;
        return;
    }
    for (const auto& row : vec2) {
        for (const auto& elem : row) {
            outFile << elem << " ";
        }
        outFile << std::endl;
    }
    outFile.close();  
}