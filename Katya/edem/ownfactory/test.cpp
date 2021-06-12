#include "factory.h"

class ApiMgr : public NApiCore::IApiManager_1_0
{
public:
    IApi* getApi(NApiCore::EApiId apiId,
                 NApi::tApiMajorVersion major,
                 NApi::tApiMinorVersion minor ) override
    {
        return new ApiMgr;
    }

    void release( IApi* apiInstance ) override
    {
        delete apiInstance;
    }

    void getApiVersion(NApi::tApiMajorVersion& major,
                                       NApi::tApiMinorVersion& minor) override
    {
        major = minor = 0;
    }

    NApiCore::EApiId getApiId() const override
    {
        return NApiCore::EApiId::eApiManager;
    }

    bool readOnly() const override
    {
        return false;
    }
};

int main()
{
    PTIIoffeFactory *factory = dynamic_cast<PTIIoffeFactory *>(GETFACTORYINSTANCE());
    ApiMgr mgr;
    factory->setup(mgr, "config.txt");
    bool success = true;
    bool additionalParticleRequired;
    char type[100];
    double scale;
    double posX, posY, posZ;
    double velX, velY, velZ;
    double angVelX, angVelY, angVelZ;
    double orientation[9];
    NApiCore::ICustomPropertyDataApi_1_0 *propData = nullptr;
    int cnt = 0;
    while (success)
    {
        factory->createParticle(0, success, additionalParticleRequired,
                                type, scale,
                                posX, posY, posZ,
                                velX, velY, velZ,
                                angVelX, angVelY, angVelZ,
                                orientation, propData);
        cnt += success;
    }

    return 0;
}
