#ifndef IPARTICLEMANAGERAPI_1_2_H
#define IPARTICLEMANAGERAPI_1_2_H

#include "IParticleManagerApi_1_1.h"

namespace NApiCore
{
    /**
     * The IParticleManagerApi_1_2 interface provides basic access to the
     * particles.
     *
     * NAME:     Particle Manager
     * ID:       EApiId::eParticleManager (9)
     * VERSION:  1.2
     */
    class IParticleManagerApi_1_2 : public IParticleManagerApi_1_1
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IParticleManagerApi_1_2() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IParticleManagerApi_1_2() {};

        using IParticleManagerApi_1_1::resetCustomProperty;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param particleName The particle type name.
         * @param propIndex The index of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char particleName[NApi::API_BASIC_STRING_LENGTH], unsigned int propIndex, double value) = 0;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param particleName The particle type name.
         * @param propIndex The index of the target custom property.
         * @param elementIndex The target index for a multi element property
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char particleName[NApi::API_BASIC_STRING_LENGTH], unsigned int propIndex, int elementIndex, double value) = 0;
    };
};

#endif // IPARTICLEMANAGERAPI_1_2_H
