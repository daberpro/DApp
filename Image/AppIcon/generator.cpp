#include <iostream>
#include <filesystem>
#define STB_IMAGE_IMPLEMENTATION
#include "../../Headers/stb_image.h"
#include <string>
#include <fstream>
#include <iomanip>

std::string ToHex(const std::string& s, bool upper_case,const std::string& filename)
{
    std::ostringstream ret;
    int length = s.length();
    ret << "{\n";
    for (std::string::size_type i = 0; i < length; ++i){
        ret << "0x" << std::hex << std::setw(2) << std::setfill('0') << (upper_case ? std::uppercase : std::nouppercase) << (s[i]&0xff);
        if(i != length - 1){
            ret << ",";
        }
        if(i % 12 == 0 && i != 0){
            ret << "\n";
        }
    }
    ret << "\n}\n";
    return ret.str();
}

void convertToBytes(const std::string &filePath, std::ofstream& result)
{
    std::ifstream::pos_type size;
    char *memblock;
    std::string filename = std::filesystem::path(filePath).stem().string();
    std::ifstream file(filePath, std::ios::in | std::ios::binary | std::ios::ate);
    if (file.is_open())
    {
        size = file.tellg();
        memblock = new char[size];
        file.seekg(0, std::ios::beg);
        file.read(memblock, size);
        file.close();
        std::string tohexed = ToHex(std::string(memblock, memblock+size), true, filename);
        result << tohexed;
    }
}

void renameFile(const std::string& path){

    if (std::filesystem::exists(path)){
        if(std::filesystem::is_directory(path)){
            uint32_t index = 1;
            for (const auto &file : std::filesystem::directory_iterator(path)){
                std::string ext = std::filesystem::path(file.path()).extension().string();
                if(ext == ".png" || ext == ".jpg" || ext == ".jpeg"){
                    std::ostringstream newName;
                    newName << path << "/icon" << index << ext;
                    if(!std::filesystem::exists(newName.str())){
                        std::filesystem::rename(file,newName.str());
                    }else{
                        std::cout << "file name " << newName.str() << " already exists\n";
                    }
                    index++;
                }
            }
        }else{
            std::ostringstream newName;
            std::string ext = std::filesystem::path(path).extension().string();
            if(ext == ".png" || ext == ".jpg" || ext == ".jpeg"){
                newName << path << "/icon" << ext;
                if(!std::filesystem::exists(newName.str())){
                    std::filesystem::rename(path,newName.str());
                }else{
                    std::cout << "file name " << newName.str() << " already exists\n";
                }
            }
        }
    }

}


int main(int argc, char *argv[])
{

    if (argc == 2)
    {
        std::string path = std::filesystem::current_path().string() + argv[1];
        if (!std::filesystem::exists(path))
        {
            std::cout << "No file or Dir in " << path << "\n";
            return EXIT_FAILURE;
        }

        renameFile(path);

        if (std::filesystem::is_directory(path))
        {
            std::string ext = "";
            std::ofstream result("include.h");
            int totalFiles = 0, index = 0;
            for (const auto &file : std::filesystem::directory_iterator(path))
            {
                ext = std::filesystem::path(file).extension().string();
                if (std::filesystem::path(file).has_extension() && (ext == ".png" || ext == ".jpg" || ext == ".jpeg")){
                    totalFiles++;
                }
            }
            result << "#pragma once" << "\n"; 
            result << "#define TOTAL_APP_ICONS " << totalFiles << "\n"; 
            result << "#include <vector> " << "\n"; 
            result << "std::vector<std::vector<uint8_t>> AppIcon{\n";
            for (const auto &file : std::filesystem::directory_iterator(path))
            {
                ext = std::filesystem::path(file).extension().string();
                if (std::filesystem::path(file).has_extension() && (ext == ".png" || ext == ".jpg" || ext == ".jpeg"))
                {
                    try{
                        convertToBytes(file.path().string(),result);
                        if(index < totalFiles - 1) result << ",\n";
                        index++;
                    }
                    catch (std::exception e){
                        std::cout << e.what() << "\n";
                    }
                }
            }
            result << "\n};\n";
            result.close();
        }
        else
        {
            std::ofstream result("include.h");
            std::string ext = std::filesystem::path(path).extension().string();
            if (std::filesystem::path(path).has_extension() && (ext == ".png" || ext == ".jpg" || ext == ".jpeg"))
            {
                try{
                    convertToBytes(path,result);
                }
                catch (std::exception e){
                    std::cout << e.what() << "\n";
                }
            }
            result.close();
        }

        return EXIT_SUCCESS;
    }
    std::cout << "Unknown argument -> ";
    for (int i = 1; i < argc; i++)
    {
        std::cout << argv[i] << " ";
    }
    std::cout << "\n";
    return EXIT_SUCCESS;
}