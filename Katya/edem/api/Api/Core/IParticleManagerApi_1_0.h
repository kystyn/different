#ifndef IPARTICLEMANAGERAPI_1_0_H
#define IPARTICLEMANAGERAPI_1_0_H

#include "IApi.h"

namespace NApiCore
{
    /**
     * The IParticleManagerApi_1_0 interface provides basic access to the
     * particles.
     *
     * NAME:     Particle Manager
     * ID:       EApiId::eParticleManager (9)
     * VERSION:  1.0
     */
    class IParticleManagerApi_1_0 : public IApi
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IParticleManagerApi_1_0() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IParticleManagerApi_1_0() {};

        /**
         * Mark a particle for removal at the end of this timestep.
         *
         * @param particleId The particle id.
         * @return true if particle successfully marked for removal. Otherwise 
         *         false, including if currently read only.
         */
        virtual bool markForRemoval(int particleId) = 0;

        /**
         * Set the scale of a particle. This will increase or decreases the
         * overall scale of the particles, changing the mass, surface radii, etc.
         *
         * @param particleId The particle id.
         * @param scale The new scale of the particle.
         * @return true if particle scale successfully changed. Otherwise 
         *         false, including if currently read only.
         */
        virtual bool setScale(int particleId, double scale) = 0;

        /**
         * Get the scale of a particle. 
         *
         * @param particleId The particle id.
         * @return The scale of the particle if particle exists. Otherwise 0.0.
         */
        virtual double getScale(int particleId) const = 0;

        /**
         * Gets the total number of particles of all types.
         * @return The total number of particles in the simulation.
         */
        virtual int getTotalNumberParticles() const = 0;

        /**
         * Gets pointer to the position of the surfaces of the particle.
         * Enough memory should be reserved for parameter surfacePositions
         * (i.e., memory for as many doubles as the number of surfaces per particle).
         * API code should also be responsible for deleting that memory.
         *
         * @param particleId The particle id.
         * @param surfacePositions Empty pointer where the surface
         *                         position coordinates are going to be stored.
         * @return true if the operation was successful, false in any other case.
         */
        virtual bool getSurfacePositions(int particleId, double* surfacePositions) = 0;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param particleName The particle type name.
         * @param propName Name of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char particleName[NApi::API_BASIC_STRING_LENGTH], const char propName[NApi::API_BASIC_STRING_LENGTH], double value) = 0;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param particleName The particle type name.
         * @param propName Name of the target custom property.
         * @param elementIndex The target index for a multi element property
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char particleName[NApi::API_BASIC_STRING_LENGTH], const char propName[NApi::API_BASIC_STRING_LENGTH], int elementIndex, double value) = 0;
    };
};

#endif // IPARTICLEMANAGERAPI_1_0_H
