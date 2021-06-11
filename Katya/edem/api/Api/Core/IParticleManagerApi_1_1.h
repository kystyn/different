#ifndef IPARTICLEMANAGERAPI_1_1_H
#define IPARTICLEMANAGERAPI_1_1_H

#include "IParticleManagerApi_1_0.h"

namespace NApiCore
{
    /**
     * The IParticleManagerApi_1_1 interface provides basic access to the
     * particles.
     *
     * NAME:     Particle Manager
     * ID:       EApiId::eParticleManager (9)
     * VERSION:  1.1
     */
    class IParticleManagerApi_1_1 : public IParticleManagerApi_1_0
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IParticleManagerApi_1_1() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IParticleManagerApi_1_1() {};

        /**
        * Gets the total number of particles of a type.
        * @param type The particle type.
        * @return The total number of particles of a type in the simulation.
        */
        virtual int getTotalNumberParticlesPerType(const char type[NApi::API_BASIC_STRING_LENGTH]) = 0;
    };
};

#endif // IPARTICLEMANAGERAPI_1_1_H
