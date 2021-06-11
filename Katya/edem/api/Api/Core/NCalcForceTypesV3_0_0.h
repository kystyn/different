#ifndef NCALCFORCETYPESV3_0_0_H
#define NCALCFORCETYPESV3_0_0_H

#include "HelpersV3_0_0.h"

/** A namespace containing structs that are used when implementing a contact model */
namespace NCalcForceTypesV3_0_0
{
    /**
     * This struct can represent both a sphere of a particle or a triangle mesh of a geometry; both of which are elements.
     */
    struct SDiscreteElement
    {
        int ID; /**< The id of the element. */
        const char* type; /**< Name of this particle or geometry. */

        bool isSphere; /**< True if this element is a sphere, false if geometry */
        
        unsigned int numOfSpheres; /**< The number of spheres of this element. 0 if geometry. */

        double shearModulus; /**< The shear modulus of the element. */
        double poisson;      /**< Poisson's ratio of the element. */

        double area;         /**< If this element is geometry, then this value gives the area of that mesh element. If it is a sphere, then it's zero by default. */

        double mass;                /**< The mass of the particle, to which this sphere belongs. */
        double density;             /**< The density of the material. */
        double volume;              /**< The volume of the particle, to which this sphere belongs. */
        double contactRadius;       /**< The local contact radius of the element at the contact point. */
        double physicalRadius;      /**< The local physical radius of the element at the contact point. */
        NApiHelpersV3_0_0::CSimple3DVector position;   /**< Coordinates of the centroid of the element (NOT the particle). */
        NApiHelpersV3_0_0::CSimple3DVector angVel;     /**< Angular velocity of the element. */
        NApiHelpersV3_0_0::CSimple3DVector MoI;        /**< The moment of inertia of the element. */
        NApiHelpersV3_0_0::CSimple3DVector CoM;        /**< The centre of mass of the particle. */

        NApiHelpersV3_0_0::CSimple3DVector velocityAtContactPoint; /**< Velocity of the element at the contact point, which takes into account the angular velocity as well. */
        NApiHelpersV3_0_0::CSimple3DVector velocity;               /**< Velocity of the element. */

        NApiHelpersV3_0_0::CSimple3x3Matrix orientation; /**< Nine-element array containing the orientation matrix for this element. The elements of the array are in the following order: XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ.*/
    };

    /**Stores a time step's current time and the length of the time step. */
    struct STimeStepData
    {
        double time;      /**< Current simulation time. */
        double timeStep;  /**< Length of current timestep. */
    };

    /** A struct that contains material interaction data */
    struct SInteraction
    {
        double coeffRest;        /**< Coefficient of restitution. */
        double staticFriction;   /**< Coefficient of static friction. */
        double rollingFriction;  /**< Coefficient of rolling friction. */
    };
    
    /** A struct that contains data for contact calculations*/
    struct SContact
    {
        NApiHelpersV3_0_0::CSimple3DVector contactPoint; /**< Coordinates of the location of the centroid of the contact. */
        double       normalContactOverlap;               /**< Magnitude of the normal contact overlap. */
        double       normalPhysicalOverlap;              /**< Magnitude of the normal physical overlap. */

        /*
         * Contact factor is the number of contacts between two particles
         * or between one particle and different mesh elements belonging to the same plane.
         * This can be used to solve the multiple-sphere particle oversampling problem.
         */
        double contactFactor;
    };

    /** Force results, used to return information to the caller of calculateForce */
    struct SContactResult
    {
        NApiHelpersV3_0_0::CSimple3DVector normalForce;            /**< Normal force on element 1. From Newton III the normal force on element 2 is equal and opposite. */
        NApiHelpersV3_0_0::CSimple3DVector usNormalForce;          /**< The unsymmetrical normal forces on element 1. This represent the portion of the calculated normal force that causes energy loss / gain (eg damping). */
        NApiHelpersV3_0_0::CSimple3DVector tangentialForce;        /**< Tangential force on element 1 From Newton III the normal force on element 2 is equal and opposite. */
        NApiHelpersV3_0_0::CSimple3DVector usTangentialForce;      /**< The unsymmetrical tangential forces on element 1. This represent the portion of calculatedTangentialForce that causes energy loss / gain (eg damping). */
        NApiHelpersV3_0_0::CSimple3DVector additionalTorque1;      /**< Any additional torque on element 1 not accounted for by the above forces (which are deemed to act at the contact point). These can be useful, for example, in a consideration rolling friction. */
        NApiHelpersV3_0_0::CSimple3DVector usAdditionalTorque1;    /**< Any unsymmetrical, additional torque on element 1 not accounted for by the above forces. These represent the component of the additional torque that causes energy loss / gain (eg damping). */
        NApiHelpersV3_0_0::CSimple3DVector additionalTorque2;      /**< Any additional torque on element 2 not accounted for by the above forces (which are deemed to act at the contact point). These can be useful, for example, in a consideration rolling friction. */
        NApiHelpersV3_0_0::CSimple3DVector usAdditionalTorque2;    /**< Any unsymmetrical, additional torque on element 2 not accounted for by the above forces. These represent the component of the additional torque that causes energy loss / gain (eg damping). */
    };

    enum class ETransformSpace : char { LOCAL, GLOBAL };

    /** data that is returned by deformation models per triangle for position deltas */
    struct STriangleDeformationResult
    {
        STriangleDeformationResult() = default;
        STriangleDeformationResult(const STriangleDeformationResult&) = default;
        STriangleDeformationResult& operator=(STriangleDeformationResult&&) = default;
        NApiHelpersV3_0_0::CSimple3DVector vert0position;
        NApiHelpersV3_0_0::CSimple3DVector vert1position;
        NApiHelpersV3_0_0::CSimple3DVector vert2position;
    };

    /** data passed to deformation models describing triangle and the force acting */
    struct SGeomTriangleInfo
    {
        NApiHelpersV3_0_0::CSimple3DVector vert0position;
        NApiHelpersV3_0_0::CSimple3DVector vert1position;
        NApiHelpersV3_0_0::CSimple3DVector vert2position;
        NApiHelpersV3_0_0::CSimple3DVector normal;
        NApiHelpersV3_0_0::CSimple3DVector triangleForce;
        unsigned int id; // requested triangle id
    };
}

#endif // NCALCFORCETYPESV3_0_0_H