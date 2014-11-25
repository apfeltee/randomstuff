
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <cstdio>

/* windows headers */
#include <windows.h>
#include <imagehlp.h>

namespace DLLHelpers
{

    struct Error: public std::runtime_error
    {
        Error(const std::string& msg="Error"): std::runtime_error(msg)
        {}
    };

    struct DLLLoadError: public Error
    {
        DLLLoadError(const std::string& msg="DLLLoadError"):
            Error(msg)
        {}
    };

    struct DLLExportError: public Error
    {
        DLLExportError(const std::string& msg="DLLExportError"):
            Error(msg)
        {}
    };

    typedef std::vector<std::string> SymbolVec;

    std::string UnDecorate(const std::string& sym)
    {
        std::string undec;
        DWORD clen;
        DWORD buflen;
        // first check if it's a decorated symbol by
        // making sure it starts with a question mark.
        // see http://msdn.microsoft.com/library/ms681400.aspx
        if((sym.size() > 1) && (sym[0] == '?'))
        {
            // how do we know how big the buffer should be?
            // for now this seems to work, but it's hardly the
            // best solution...
            buflen = (sym.size() + 200);
            CHAR* buf = new CHAR[buflen];
            clen = UnDecorateSymbolName(sym.c_str(), buf, buflen, UNDNAME_COMPLETE);
            if(clen > 0)
            {
                undec.append(buf);
                delete[] buf;
                return undec;
            }
            delete[] buf;
            return sym;
        }
        return sym;
    }

    void ListDLLFunctions(const std::string& dllname,
                          SymbolVec& funcvec,
                          USHORT what=IMAGE_DIRECTORY_ENTRY_EXPORT)
    {
        size_t i;
        unsigned long cDirSize;
        DWORD* dNameRVAs = 0;
        _IMAGE_EXPORT_DIRECTORY* ImageExportDirectory;
        _LOADED_IMAGE LoadedImage;
        std::string sName;
        funcvec.clear();
        if (MapAndLoad(dllname.c_str(), NULL, &LoadedImage, true, true))
        {
            ImageExportDirectory = (_IMAGE_EXPORT_DIRECTORY*)
                ImageDirectoryEntryToData(
                    LoadedImage.MappedAddress,
                    false,
                    what,
                    &cDirSize);
            if (ImageExportDirectory != NULL)
            {
                dNameRVAs = (DWORD*)ImageRvaToVa(
                    LoadedImage.FileHeader,
                    LoadedImage.MappedAddress,
                    ImageExportDirectory->AddressOfNames,
                    NULL);
                for(i=0; i<(ImageExportDirectory->NumberOfNames); i++)
                {
                    sName = (const char*)ImageRvaToVa(
                        LoadedImage.FileHeader,
                        LoadedImage.MappedAddress,
                        dNameRVAs[i],
                        NULL);
                    sName = UnDecorate(sName);
                    funcvec.push_back(sName);
                }
            }
            else
            {
                UnMapAndLoad(&LoadedImage);
                throw DLLExportError();
                return;
            }
            UnMapAndLoad(&LoadedImage);
        }
        else
        {
            throw DLLLoadError();
            return;
        }
    }
}

int main(int argc, char* argv[])
{
    int i;
    std::string dllname;
    std::string sym;
    DLLHelpers::SymbolVec names;
    if(argc > 1)
    {
        dllname = argv[1];
        try
        {
            DLLHelpers::ListDLLFunctions(dllname, names);
            for(i=0; i<names.size(); i++)
            {
                std::string sym = names[i];
                fprintf(stdout, "[%04d]:  %s\n", i, sym.c_str());
            }
        }
        catch(DLLHelpers::DLLLoadError& err)
        {
            fprintf(stderr, "Failed to load '%s'\n", dllname.c_str());
            return 1;
        }
        catch(DLLHelpers::DLLExportError& err)
        {
            fprintf(stderr, "Failed to export symbols from '%s'\n", dllname.c_str());
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "usage: %s <dll>\n", argv[0]);
        return 1;
    }
    return 0;
}
