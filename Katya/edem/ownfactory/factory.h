#pragma once

#include <string>
#include <vector>
#include <Api/Core/ApiTypes.h>
#include <Api/Core/IApiManager_1_0.h>
#include <Api/Core/ICustomPropertyDataApi_1_0.h>
#include <Api/Factories/IPluginParticleFactoryV2_0_0.h>
#include <Api/Factories/PluginParticleFactoryCore.h>

struct Point
{
    Point( double x, double y, double z ) : x(x), y(y), z(z)
    {}

    double x, y, z;
};

class PTIIoffeFactory : public NApiFactory::IPluginParticleFactoryV2_0_0
{
public:
    void getPreferenceFileName( char prefFileName[NApi::FILE_PATH_MAX_LENGTH] ) override;
    bool setup( NApiCore::IApiManager_1_0& apiManager,
               const char prefFile[] ) override;

    NApi::ECalculateResult createParticle(
                                   double  time,
                                   bool&   particleCreated,
                                   bool&   additionalParticleRequired,
                                   char    type[NApi::API_BASIC_STRING_LENGTH],
                                   double& scale,
                                   double& posX,
                                   double& posY,
                                   double& posZ,
                                   double& velX,
                                   double& velY,
                                   double& velZ,
                                   double& angVelX,
                                   double& angVelY,
                                   double& angVelZ,
                                   double  orientation[9],
                                   NApiCore::ICustomPropertyDataApi_1_0* propData) override;
private:
    bool readCenters( std::string const& fileName );
    bool readRadiuses( std::string const& fileName );

    char configFileName[NApi::FILE_PATH_MAX_LENGTH];

    std::vector<double> radiuses;
    std::vector<Point> centers;

    size_t curno = 0;
};

EXPORT_MACRO NApiFactory::IPluginParticleFactory* GETFACTORYINSTANCE();

/**
 * This method should de-allocate the supplied instance.
 *
 * A plugin will only ever be supplied instances to
 * de-allocate that it created via it's GETFACTORYINSTANCE() method.
 *
 * TO BE IMPLEMENTED BY END USER IN EACH PARTICLE BODY FORCE PLUGIN
 *
 * @param instance The instance to be released
 */
EXPORT_MACRO void RELEASEFACTORYINSTANCE(NApiFactory::IPluginParticleFactory* instance);

/**
 * Returns the version of the plugin interface implemented
 * by this plugin file.
 *
 * TO BE IMPLEMENTED BY END USER
 *
 * The version number must be packed in to a single
 * 32 bit value.  Consider the following example code:
 *
 *  static const int INTERFACE_VERSION_MAJOR = 0x02;
 *  static const int INTERFACE_VERSION_MINOR = 0x00;
 *  static const int INTERFACE_VERSION_PATCH = 0x00;
 *
 *  return (INTERFACE_VERSION_MAJOR << 16 |
 *          INTERFACE_VERSION_MINOR << 8 |
 *          INTERFACE_VERSION_PATCH);
 *
 * @return Interface version packed in to a single 32bit value
 */
EXPORT_MACRO int GETFACTINTERFACEVERSION();
