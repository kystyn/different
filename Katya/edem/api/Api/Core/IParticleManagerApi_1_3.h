#ifndef IPARTICLEMANAGERAPI_1_3_H
#define IPARTICLEMANAGERAPI_1_3_H

#include "IParticleManagerApi_1_2.h"
#include "NExternalForceTypesV3_0_0.h"

namespace NApiCore
{
    /**
     * The IParticleManagerApi_1_3 interface provides basic access to the
     * particles.
     *
     * NAME:     Particle Manager
     * ID:       EApiId::eParticleManager (9)
     * VERSION:  1.3
     */
    class IParticleManagerApi_1_3 : public IParticleManagerApi_1_2
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IParticleManagerApi_1_3() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IParticleManagerApi_1_3() {};

        /**
         * Get particle data.
         * @param particleId Particle ID.
         * @return particle data.
         */
        virtual const NExternalForceTypesV3_0_0::SParticle getParticleData(int particleId) const = 0;

        /**
         * Get particle custom property value.
         * @param particleId Particle ID.
         * @param propertyIndex Property index.
         * @return Property element value array.
         */
        virtual const double* getCustomPropertyValue(int particleId, unsigned int propertyIndex) const = 0;

        /**
         * Mark External Force model particle for additional cpu processing.
         * @param particleId Particle ID.
         */
        virtual void markParticleOfInterest(int particleId) = 0;
    };
};

#endif // IPARTICLEMANAGERAPI_1_2_H
