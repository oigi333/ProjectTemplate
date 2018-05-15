#include <boost/filesystem.hpp>
#include <vector>
#include <map>
#include <iostream>
#include <functional>
#include "ProjectTemplate.h"


void help(std::string appFileName)
{
    std::cout
    <<"Usage: "<<appFileName<<" <Option> <Option Parameter> "<< std::endl
    <<"Possible options are listed below:"<< std::endl
    <<"newTemplate <Folder FileName> <Template Name>: create new/ override existing template"<< std::endl
    <<"deleteTemplate <Template Name>: delete existing template"<< std::endl
    <<"makeProject <Project(Folder) Name>"<< std::endl;

}

int main(int argc, char** argv)
{
    std::string appFileName = boost::filesystem::absolute(boost::filesystem::path(argv[0])).string();
    
    if(argc<2)
    {
        help(appFileName);
        return 1;
    }
    std::string option = argv[1];
    
    if(option=="newTemplate" && argc>3)
    {
        std::string folderFileName = argv[2];
        std::string templateName = argv[3];
        try
        {
            ProjectTemplate::FromXML(appFileName, templateName);
            ProjectTemplate::DeleteTemplate(appFileName,templateName);
        }
        catch(ProjectTemplate::TemplateNotFindedException e) {}
        
        auto Pt = ProjectTemplate::FromFolder(folderFileName);
        Pt.SaveXML(appFileName, folderFileName, templateName);
        
    }
    else if(option=="deleteTemplate")
    {
        std::string templateName = argv[2];
        try
        {
            ProjectTemplate::DeleteTemplate(appFileName,templateName);
        }
        catch(ProjectTemplate::TemplateNotFindedException e)
        {
            std::cout<<"Template haven't been finded. It may be caused by type error."<<std::endl;
            return 2;
        }
    }
    else if(option=="makeProject" && argc>3)
    {
        std::string folderFileName = argv[2];
        std::string templateName = argv[3];

        ProjectTemplate Pt;
        try
        {
            Pt = ProjectTemplate::FromXML(appFileName, templateName);
        }
        catch(ProjectTemplate::TemplateNotFindedException e)
        {
            std::cout<<"Template haven't been finded. It may be caused by type error."<<std::endl;
            return 2;
        }

        Pt.SaveFolder(folderFileName);
    }
    else
    {
        help(appFileName);
        return 1;
    }

    return 0;
}