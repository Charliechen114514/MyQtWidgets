#ifndef CMAKESELFINFO_H
#define CMAKESELFINFO_H
#include <QString>

class CMakeSelfInfo
{
public:
    const short DEFAULT_CMAKE_MAJOR_VERSION = 3;
    const short DEFAULT_CMAKE_MINOR_VERSION = 20;
    CMakeSelfInfo();
    CMakeSelfInfo(const CMakeSelfInfo& other){
        this->CMake_Major_Version = other.CMake_Major_Version;
        this->CMake_Minor_Version = other.CMake_Minor_Version;
    };
    const CMakeSelfInfo& operator=(const CMakeSelfInfo&) = delete;
    void setCMakeVersion(short major, short minor)
    {
        CMake_Major_Version = major;
        CMake_Minor_Version = minor;
    }

    inline QString CMake_Version_String(){
        return QString::number(CMake_Major_Version) + "." + QString::number(CMake_Minor_Version);
    }



private:
    short   CMake_Major_Version = DEFAULT_CMAKE_MAJOR_VERSION;
    short   CMake_Minor_Version = DEFAULT_CMAKE_MINOR_VERSION;
};

#endif // CMAKESELFINFO_H
