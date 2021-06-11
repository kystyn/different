#ifndef APITYPES_H
#define APITYPES_H

/***************************************************************************/
/* This header file contains various constants and declarations used by    */
/* the EDEM generic API.                                                   */
/* It is included automatically by any all API headers                     */
/***************************************************************************/

#include <limits.h>

#include "ApiIds.h"

/**
 * The NApi namespace contains common definitions used by
 * all parts of the API.
 */
namespace NApi
{
    /**
     * The max length (including null terminator)
     * of an API name
     */
    const unsigned int API_NAME_LENGTH = 256;

    /**
     * The max length (including null terminator)
     * of a basic string used for passing to and from
     * the API
     */
    const unsigned int API_BASIC_STRING_LENGTH = 256;

    /**
     * Special value used to indicate an ID that is not yet set
     * or available
     */
    const unsigned int NO_ID = UINT_MAX;

    /**
     * Maximum length (including null terminator) of a custom
     * property name
     */
    const unsigned int CUSTOM_PROP_MAX_NAME_LENGTH = 1024;

    /**
     * Maximum length of a full path to a file
     */
    const unsigned int FILE_PATH_MAX_LENGTH = 1024;

    /**
     * Max length for buffer to hold custom property initial values
     */
    const unsigned int BUFF_SIZE = 256;

    /**
     * Max length of custom error message.
     */
    const unsigned int ERROR_MSG_MAX_LENGTH = 256;

    const unsigned int TIMESTEP_LIMIT = 1024;

    /**
     * Type for the API major version number.
     */
    typedef unsigned char tApiMajorVersion;

    /**
     * Type for the API minor version number.
     */
    typedef unsigned char tApiMinorVersion;

    /**
     * Various different interpolation types for field data
     */
    enum EInterpolationType
    {
        /**
         * IDW (Inverse Distance Weighting) interpolation is a method that
         * assumes the interpolating surface is influenced more by closer
         * points as opposed to points that are further away. The
         * interpolated surface is a weighted average of the field points.
         * The weight assigned to each field point diminishes as the distance
         * from the interpolation point to the field point increases. For
         * full details, see
         *     http://en.wikipedia.org/wiki/Inverse_distance_weighting
         */
        eIdw = 0
    };

    /**
    * Type of model in terms of the importance of force calculation
    */
    enum class EPluginModelType
    {
        /**
         * Handles the force calculations for the interactions that occur during the simulation 
         * The "main" physics model type that defines particle collision behaviour, for the core calculations that make the physics model what it is.
         */
        eBase = 0,
        /**
         * Optional chained behaviour which is not core to the force calculations
         */
        eOptional = 1,
        /**
         * Models to calculate rolling friction specifically
         */
        eRollingFriction = 2
    };

    /**
    * Position that the model should execute in the contact chain
    * Execution order of models is the same as the order of this structure, (first->preBase->base->rollingFric->afterBase->final)
    */
    enum class EPluginExecutionChainPosition
    {
        /**
         * Run first. Before everything else
         */
        eFirstPos = 0,
        /**
        * Run before the base contact model.
        */
        ePreBasePos = 1,
        /**
        * The position of the base contact model. The model which does the main interaction force calculations
        * If a plugin is of EPluginModelType::eBase, is must have position of eBasePos
        */
        eBasePos = 2,
        /**
        * The position of the rolling friction model.
        * Only one rolling friction model should be run at once.
        * If a plugin is of EPluginModelType::eRollingFriction, it must have position of eBaseRollingFrictionPos
        */
        eBaseRollingFrictionPos = 3,
        /**
        * Run after the base contact model. This should be the the go-to for non-base/non-rolling friction models.
        * This position is suitible for multiple chained models
        */
        eAfterBasePos = 4,
        /**
        * Run right at the very end of the chaining-flow
        */
        eFinalPos = 5
    };

    /**
     * Lists the data types that can currently be stored as
     * custom property data.  Only the c double type is supported.
     */
    enum EPluginPropertyDataTypes
    {
        eDouble = 0, /**< Double precision floating point number */
        eBool        /**< Boolean type */
    };

    /**
     * Lists the available categories for a custom property
     */
    enum EPluginPropertyCategory
    {
        eParticle   = 0, /**< Particle */
        eContact    = 1, /**< Contact  */
        eGeometry   = 2, /**< Geometry  */
        eSimulation = 3  /**< Simulation  */
    };

    /**
     * Enum of all supported unit types for use in custom
     * property definitions.
     *
     * Data in all but eOther and eNone must be stored in SI
     * units.  EDEM handles converting to various
     * display units based on the users configuration
     *
     * Use eOther as the unit type when a property has a unit
     * but is not listed as one of the internal units supported
     * by EDEM. eNone should only be used for unitless values
     * such as counts.
     */
    enum EPluginPropertyUnitTypes
    {
        eOther               =  0, /**< Unknown unit */
        eNone                =  1, /**< Unitless */
        eAcceleration        =  2, /**< Acceleration (SI: M/s2)*/
        eAngle               =  3, /**< Angle (SI: rad) */
        eAngularAcceleration =  4, /**< Acceleration (SI: rad/s2)*/
        eAngularVelocity     =  5, /**< Angular Velocity (SI: rad/s) */
        eDensity             =  6, /**< Density (SI: kg/m3) */
        eEnergy              =  7, /**< Energy (SI: J) */
        eWorkFunction        =  8, /**< Work Function (SI: J) */
        eForce               =  9, /**< Force (SI: N) */
        eCharge              = 10, /**< Charge (SI: C) */
        eLength              = 11, /**< Length (SI: m) */
        eMass                = 12, /**< Mass (SI: Kg) */
        eMOI                 = 13, /**< Moment of inertia (SI: Kg/m2) */
        eShearMod            = 14, /**< Shear Mod (SI: pa) */
        eTime                = 15, /**< Time (SI: s) */
        eTorque              = 16, /**< Torque (SI: Nm) */
        eVelocity            = 17, /**< Velocity (SI: m/s) */
        eVolume              = 18, /**< Volume (SI: m3) */
        eFrequency           = 19, /**< Frequency (SI: Hz) */
        eTemperature         = 20, /**< Temperature (SI: K) */
        eHeatFlux            = 21, /**< Heat Flux (SI: W) */
        eStiffness           = 22, /**< Stiffness (SI: N/m) */
        eStress              = 23, /**< Stress (SI: pa) */
        eMassFlow            = 24, /**< Mass Flow (SI: Kg/s) */
        eStiffnessPerUnitArea= 25  /**< Stiffness per unit area (SI: N/m3) */
    };

    /**
     * Return value from the main calculation methods of the
     * Contact Model and Particle Body Force Model plugin
     * APIs
     */
    enum ECalculateResult
    {
        /**
         * Sucess
         */
        eSuccess    = 0,

        /**
         * Plugin encountered an error that it will be able to recover from
         */
        eError      = 1,

        /**
         * Plugin encountered a fatal error that it cannot recover from
         * EDEM will flag this event and stop processing
         */
        eFatalError = 2
    };

    enum EGeometryImportUnits
    {
        eMetres = 0,
        eCentimetres,
        eMillimetres,
        eMicrometres,
        eYards,
        eFeet,
        eInches,
        eCAD
    };

    enum class EValueAccumulationType
    {
        eOverWrite = 0,
        eAdditive,
        eMultiplicative
    };

    /*
     * Helper function that returns the string delimiter used as 
     * as part of the custom property initialisation functionality
     */
    inline const char* delim()  {return ":#:";}
};

#endif // APITYPES_H
