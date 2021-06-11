#ifndef IPLUGINPARTICLEFACTORY_H
#define IPLUGINPARTICLEFACTORY_H

#include <Api/Core/ApiTypes.h>

/**
 * The NApiFactory namespace contains all interfaces related
 * to factory plugins.
 *
 * Pick one of the versioned interfaces (e.g. IPluginParticleFactoryV2_0_0)
 * and implement all of its methods as required.  Also implement the
 * methods declared in PluginParticleFactoryCore.h to enable EDEM to load
 * and access the plugin instance.
 */
namespace NApiFactory
{
    /**
     * Base class for all plugin particle factories.  This class
     * lets us abstractly group all plugin particle factories together
     * and determine their type by dynamic_cast operators.
     */
    class IPluginParticleFactory
    {
    public:
        /**
         * Constructor, does nothing
         */
        IPluginParticleFactory() {}

        /**
         * Destructor, does nothing
         */
        virtual ~IPluginParticleFactory() {}
    };
};

#endif // IPLUGINPARTICLEFACTORY_H

