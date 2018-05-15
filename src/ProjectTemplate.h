#pragma once
#include <boost/filesystem.hpp>
#include <map>
#include <string>
#include <exception>

class ProjectTemplate
{
    struct File
    {
        std::string Data;
        bool IsDirectory = false;

        File(std::string data, bool isDirectory=false);
        File();
    };

    std::map<boost::filesystem::path, File> files;

    static std::string Slash;

    public:
    ProjectTemplate();

    void SaveXML(std::string appFileName,std::string folderFileName, std::string templateName);
    void SaveFolder(std::string folderFileName);
        
    static ProjectTemplate FromFolder(std::string folderFileName);
    static ProjectTemplate FromXML(std::string appFileName, std::string templateName);
    static void DeleteTemplate(std::string appFileName, std::string templateName);

    class TemplateNotFindedException: std::exception {};
};