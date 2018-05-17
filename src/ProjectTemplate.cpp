#include "ProjectTemplate.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <memory>
#include <algorithm>

#include "pugixml/pugiconfig.hpp"
#include "pugixml/pugixml.hpp"


ProjectTemplate::ProjectTemplate()
{

}

ProjectTemplate ProjectTemplate::FromFolder(std::string folderFileName)
{
    ProjectTemplate pt;

    boost::filesystem::recursive_directory_iterator dir(folderFileName);
    for(auto elem: dir)
    {
        if(boost::filesystem::is_directory( elem.path() ) && !boost::filesystem::is_empty( elem.path() ))
            continue;

        if(boost::filesystem::is_directory( elem.path() ))
        {
            pt.files[elem.path()] = File("", true);
            continue;
        }
        std::ifstream ifs(elem.path().string());
        std::stringstream ss;
        ss << ifs.rdbuf();
        pt.files[elem.path()] = File(ss.str());
    }
    return pt;
}

ProjectTemplate ProjectTemplate::FromXML(std::string appFileName, std::string templateName)
{
    ProjectTemplate pt;
    bool found = false;

    pugi::xml_document doc;
    doc.load_file(((boost::filesystem::path(appFileName)).parent_path().wstring()+L"\\Templates.xml").c_str());
    pugi::xml_node root = doc.first_child();
    for(auto projectTemplateNode: root)
    {
        if(projectTemplateNode.attribute("name").as_string()==templateName)
        {
            found=true;
            for(auto fileNode: projectTemplateNode)
            {
                if(std::string(fileNode.name())==std::string("file"))
                    pt.files[boost::filesystem::path(fileNode.attribute("path").as_string())] = File(fileNode.text().as_string());
                else
                    pt.files[boost::filesystem::path(fileNode.attribute("path").as_string())] = File("", true);
            }
            break;
        }
    }

    if(!found)
        throw TemplateNotFoundException();

    return pt;
}

void ProjectTemplate::SaveXML(std::string appFileName,std::string folderFileName, std::string templateName)
{
    pugi::xml_document doc;
    doc.load_file(((boost::filesystem::path(appFileName)).parent_path().wstring()+L"\\Templates.xml").c_str());
    pugi::xml_node root = doc.first_child();
    pugi::xml_node projectTemplateNode = root.append_child("template");
    projectTemplateNode.append_attribute("name") = templateName.c_str();

    for(auto file: files)
    {
        auto filePathString = (boost::filesystem::relative(file.first, boost::filesystem::path(folderFileName))).string();

        pugi::xml_node fileNode;
        if(boost::filesystem::is_directory(file.first))
            fileNode = projectTemplateNode.append_child("folder");
        else
        {
            fileNode = projectTemplateNode.append_child("file");
            fileNode.append_child(pugi::node_pcdata).set_value(file.second.Data.c_str());
        }

        fileNode.append_attribute("path") = filePathString.c_str();

    }

    doc.save_file((
        (boost::filesystem::path(appFileName)).parent_path().wstring()
        +std::wstring(ProjectTemplate::Slash.begin(),ProjectTemplate::Slash.end())
        +L"\\Templates.xml"
        ).c_str());
}

void ProjectTemplate::SaveFolder(std::string folderFileName)
{
    boost::filesystem::create_directories(folderFileName.c_str());

    for(auto file: files)
    {
        if(file.second.IsDirectory)
        {
            boost::filesystem::create_directories(folderFileName+ProjectTemplate::Slash+file.first.string());
        }
        else
        {
            boost::filesystem::create_directories(folderFileName+ProjectTemplate::Slash+file.first.parent_path().string());

            std::ofstream ofs(folderFileName+ProjectTemplate::Slash+file.first.string());
            ofs<<file.second.Data;
            ofs.close();
        }
    }
}

void ProjectTemplate::DeleteTemplate(std::string appFileName, std::string templateName)
{
    bool found = false;

    pugi::xml_document doc;
    doc.load_file((
        (boost::filesystem::path(appFileName)).parent_path().wstring()
        +std::wstring(ProjectTemplate::Slash.begin(),ProjectTemplate::Slash.end())
        +L"\\Templates.xml"
        ).c_str());
    pugi::xml_node root = doc.first_child();


    for(auto projectTemplateNode: root)
    {
        if(projectTemplateNode.attribute("name").as_string()==templateName)
        {
            found=true;
            root.remove_child(projectTemplateNode);
            break;
        }
    }

    if(!found)
        throw TemplateNotFoundException();

    doc.save_file((
        (boost::filesystem::path(appFileName)).parent_path().wstring()
        +std::wstring(ProjectTemplate::Slash.begin(),ProjectTemplate::Slash.end())
        +L"\\Templates.xml"
        ).c_str());
}

#ifdef _WIN32
std::string ProjectTemplate::Slash = "\\";
#else
std::string ProjectTemplate::Slash = "/";
#endif

ProjectTemplate::File::File()
{

}

ProjectTemplate::File::File(std::string data, bool isDirectory) : Data(data), IsDirectory(isDirectory)
{

}
