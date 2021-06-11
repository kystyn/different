#ifndef NEXTERNALFORCETYPESV3_0_0_H
#define NEXTERNALFORCETYPESV3_0_0_H

#include "HelpersV3_0_0.h"

namespace NExternalForceTypesV3_0_0
{
    /**
     * This struct represents particle element, which is used to modify particle body forces (such as electromagnetic or drag forces).
     */
    struct SParticle
    {
        int ID;           /**< The id of the particle. */
        const char* type; /**< Name of the particle template, from which this particle was created. */

        double mass;                                     /**< The mass of the particle. */
        double volume;                                   /**< The volume of the particle. */
        double density;                                  /**< The density of the particle. */
        unsigned int NumOfSpheres;                       /**< The number of spheres of the particle. */
        NApiHelpersV3_0_0::CSimple3DVector position;     /**< The centroid of the particle. */
        NApiHelpersV3_0_0::CSimple3DVector velocity;     /**< The velocity the particle. */
        NApiHelpersV3_0_0::CSimple3DVector angVel;       /**< The angular velocity of the particle. */
        NApiHelpersV3_0_0::CSimple3x3Matrix orientation; /**< Nine element array containing the orientation matrix for this particle. The elements of the array are in the following order: XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ.*/
    };

    /**
     * Return values.
     */
    struct SResults
    {
        NApiHelpersV3_0_0::CSimple3DVector force;  /**< The particle body force on the particle. This force is taken to act at the particle centroid. */
        NApiHelpersV3_0_0::CSimple3DVector torque; /**< The particle body torque on the particle. This torque is in global coordinates. */
    };

    /**Stores a time step's current time and the length of the time step.*/
    struct STimeStepData
    {
        double time;      /**< Current simulation time. */
        double timeStep;  /**< Length of current timestep. */
    };
}

#endif // NEXTERNALFORCETYPESV3_0_0_H