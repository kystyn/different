#include <cstring>
#include <fstream>
#include <sstream>

#include "factory.h"

void PTIIoffeFactory::getPreferenceFileName(char prefFileName[])
{
    strncpy(prefFileName, configFileName, NApi::FILE_PATH_MAX_LENGTH);
}

bool PTIIoffeFactory::setup( NApiCore::IApiManager_1_0 &apiManager, const char prefFile[] )
{
    if (strlen(prefFile) > NApi::FILE_PATH_MAX_LENGTH)
        return false;

    strncpy(configFileName, prefFile, NApi::FILE_PATH_MAX_LENGTH);

    std::ifstream config(configFileName);

    if (!config)
        return false;

    std::string centerConfig, radConfig;
    std::getline(config, centerConfig);
    std::getline(config, radConfig);

    readCenters(centerConfig);
    readRadiuses(radConfig);

    return radiuses.size() == centers.size();
}

NApi::ECalculateResult PTIIoffeFactory::createParticle(
        double time, bool &particleCreated, bool &additionalParticleRequired,
        char type[], double &scale, double &posX, double &posY, double &posZ,
        double &velX, double &velY, double &velZ,
        double &angVelX, double &angVelY, double &angVelZ,
        double orientation[], NApiCore::ICustomPropertyDataApi_1_0 *propData )
{
    particleCreated = curno < centers.size();
    if (!particleCreated)
        return NApi::ECalculateResult::eSuccess;

    additionalParticleRequired = true;
    strcpy(type, "Katya");
    scale = radiuses[curno];
    Point &pt = centers[curno];
    posX = pt.x;
    posY = pt.y;
    posZ = pt.z;
    velX = velY = velZ = 0;
    angVelX = angVelY = angVelZ = 0;

    for (unsigned int i = 0; i < 9; i++)     {         orientation[i] = 0.0;     }

    curno = std::min(curno + 1, centers.size());

    return NApi::ECalculateResult::eSuccess;
}

bool PTIIoffeFactory::readCenters(const std::string &fileName)
{
    std::ifstream ifs(fileName);

    if (!ifs)
        return false;

    std::string line;

    while (std::getline(ifs, line, '\n'))
    {
        std::istringstream iss(line);
        std::string num, x, y, z;
        std::getline(iss, num, ',');
        std::getline(iss, x, ',');
        std::getline(iss, y, ',');
        std::getline(iss, z, ','); // or \n?
        auto pt = Point(std::stod(x), std::stod(y), std::stod(z));
        centers.push_back(pt);
    }
    return true;
}

bool PTIIoffeFactory::readRadiuses(const std::string &fileName)
{
    std::ifstream ifs(fileName);

    if (!ifs)
        return false;

    std::string line;

    while (std::getline(ifs, line, '\n'))
    {
        std::istringstream iss(line);
        std::string num, r;
        std::getline(iss, num, ',');
        std::getline(iss, r, ',');
        radiuses.push_back(std::stod(r));
    }

    return true;
}

EXPORT_MACRO NApiFactory::IPluginParticleFactory *GETFACTORYINSTANCE()
{
    return new PTIIoffeFactory;
}

EXPORT_MACRO void RELEASEFACTORYINSTANCE(NApiFactory::IPluginParticleFactory *instance)
{
    auto factory = dynamic_cast<PTIIoffeFactory *>(instance);
    if (factory != nullptr)
        delete factory;
}

EXPORT_MACRO int GETFACTINTERFACEVERSION()
{
    return 1;
}
