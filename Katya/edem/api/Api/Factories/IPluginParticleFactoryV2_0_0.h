#ifndef IPLUGINPARTICLEFACTORYV2_0_0
#define IPLUGINPARTICLEFACTORYV2_0_0

/***************************************************************************/
/* This header file contains the V2.0.0 plugin factory API definition.     */
/*                                                                         */
/* Include this header and PluginParticleFactoryCore.h into your plugin    */
/* project then implement the methods from PluginParticleFactoryCore.h and */
/* create a new class derived from IPluginParticleFactoryV2_0_0 that       */
/* implements your desired functionality.                                  */
/***************************************************************************/

// Include ALL required headers.  Do not use forward declarations, this
// makes things easier on the end user
#include "PluginConstants.h"
#include "IPluginParticleFactory.h"
#include "ApiTypes.h"
#include "IApiManager_1_0.h"
#include "ICustomPropertyDataApi_1_0.h"

namespace NApiFactory
{
    /**
     * This interface contains all of the methods required to create a
     * particle factory plugin.  A new class should be created that derives
     * from this interface and implements all of its methods. Additionally
     * the methods from the PluginParticleFactoryCore.h file need to be
     * implemented.
     *
     * NAME:              Particle Factory API
     * VERSION:           2.0.0
     * CUSTOM PROPERTIES: Contact, Geometry, Particle, Simulation
     *
     * The methods required by the interface are:
     *
     * createParticle(...)
     *     Returns the details for a new particle
     *
     * The following additional methods can be over-ridden to change
     * the default implementations:
     *
     * getPreferenceFileName(...)
     *     Returns the name of the config file used by this model (if any)
     *     Default: Indicates that no preference file is used
     *
     * usesCustomProperties(...)
     *     Indicates that the plugin wishes to register and/or receive custom
     *     property data.
     *     Default: Indicates the plugin does not use custom properties
     *
     * setup(...)
     *     Performs any one-off setup and initialization just after the
     *     plugin is loaded.
     *     Default: Performs no work but returns true to indicate plugin
     *              loaded cleanly.
     *
     * starting(...)
     *     Called just before simulation starts to allow API handles to
     *     be acquired, temporary storage allocated, files opened etc.
     *     Default: Performs no work but returns true to indicate plugin
     *              is ready to start processing
     *
     * stopping()
     *     Called just after simulation stops to allow API handles to
     *     be released, temporary storage freed, files closed etc.
     *     Default: Does nothing
     *
     * getNumberOfRequiredProperties(...)
     *     Called by EDEM to query the number of custom properties required
     *     by this plugin to run.
     *     Default: Returns 0 to indicate no properties are required for
     *              any category
     *
     * getDetailsForProperty(...)
     *     Returns the details for the custom properties required for this plugin
     *     to run.
     *     Default: Returns false.
     *
     * If you need per plugin instance data simply add entries to your
     * plugin's class definition as you would with any other C++ class
     * definition.
     */
    class IPluginParticleFactoryV2_0_0 : public IPluginParticleFactory
    {
        public:
            /**
             * Constructor, does nothing
             */
            IPluginParticleFactoryV2_0_0() {}

            /**
             * Destructor, does nothing
             */
            virtual ~IPluginParticleFactoryV2_0_0() {}

            /**
             * Retrieves the name of the config file used by the plugin.
             *
             * If the plugin does not need a config file then prefFileName
             * should be set to the empty string.
             *
             * @param prefFileName (RETURN VALUE)
             *                     A character array to be populated with the
             *                     config file name. This path is relative to
             *                     the directory the plugin is stored in.
             *                     EDEM will prepend the full directory the plugin
             *                     is stored in and pass it back to the setup method.
             */
            virtual void getPreferenceFileName(char prefFileName[NApi::FILE_PATH_MAX_LENGTH]) {prefFileName[0] = '\0';}

            /**
             * Indicates whether the plugin wishes to register or receive custom
             * property data.
             *
             * If a plugin factory does not register to receive custom property
             * data then any particles will start with the initial value configured
             * in the EDEM property manager GUI.
             *
             * @return Bool to indicate if custom properties are to be registered
             *         or should be supplied to the createParticle(...) method.
             */
            virtual bool usesCustomProperties() {return false;}

            /**
             * Initializes the plugin by giving it a chance to read any config
             * files, open temporary files, generate data structures or any other
             * one-off setup work.
             *
             * This method is called once, shortly after the plugin is first loaded.
             * If this method returns false EDEM will immediately delete the plugin
             * and an error message will be reported.
             *
             * IMPORTANT: Plugins should not cache API handles in this
             * method.  See the starting(...) and stopping(...) methods.
             *
             * @param apiManager The api manager for use by plugin models
             * @param prefFile Full path to optional preferences file or empty
             *                 string if none
             * @return Bool to say if setup was a success
             */
            virtual bool setup(NApiCore::IApiManager_1_0& apiManager,
                               const char                 prefFile[]) {return true;}

            /**
             * Called to indicate processing is about to begin and the
             * model should allocate any temporary storage and retrieve any
             * file/api/socket handles it may need
             *
             * If the method returns false then processing will not start.
             *
             * IMPORTANT: Plugins should only retrieve API handles in this
             * method. API handles may change between one processing
             * run and another. Attempting to keep and re-use handles
             * will cause system instability.
             *
             * @param apiManager The api manager for use by plugin models
             * @return true if model is ready to start, else false
             */
            virtual bool starting(NApiCore::IApiManager_1_0& apiManager) {return true;}

            /**
             * Called to indicate processing is finished and that
             * the model should free any temporary storage and close/release
             * file/api/socket handles.
             *
             * The implementation must be able to handle this method being
             * called multiple times in a row without intervening calls
             * to starting.  This can occur when one or more loaded models
             * abort processing.
             *
             * IMPORTANT: Plugins must release all API handles in this
             * method. API handles may change between one processing
             * run and another. Attempting to keep and re-use handles
             * will cause system instability.
             */
            virtual void stopping(NApiCore::IApiManager_1_0& apiManager) {}

            /**
             * The createParticle(...) method returns details for a single
             * new particle to be added to the system.
             *
             * This method is called at least once per timestep.  If the
             * factory does not wish to create any particles it should return
             * eSuccess and set particleCreated to false, if it wishes to
             * create at least one particle it should return eSuccess, set
             * particleCreated to true and populate the remaining parameters
             * with the details of the particle.
             *
             * If the factory wishes to create more than one particle in a
             * timestep then the additionalParticleRequired parameter should
             * be set to true.  The factory code will continue to call the
             * plugins createParticle(...) method until either
             * particleCreated or additionalParticleRequired are set to
             * false.
             *
             * @param time
             *            Current time
             * @param particleCreated
             *            (RETURN VALUE)
             *            Indicates if the factory has created a particle for
             *            this timestep (true) in which case all following
             *            parameters indicate the configuration of that
             *            particle.  If false then no particle is to
             *            be created and no more are required.
             *            This parameter defaults to false on each call
             *            it must tbe explicitly set to true to indicate
             *            a particle is to be created.
             * @param additionalParticleRequired
             *            (RETURN VALUE)
             *            If particleCreated is true this parameter indicates
             *            whether the factory wishes to create at least one
             *            more particle this timestep (true).
             *            This parameter defaults to false on each call
             *            it must be explicitly set to true if you wish
             *            to create another particle
             * @param type
             *            (RETURN VALUE)
             *            The name of the particle type that the new particle
             *            should be a member of.
             * @param scale
             *            (RETURN VALUE)
             *            Scaling factor for the new particle
             * @param posX
             *            (RETURN VALUE)
             *            X coordinate to place the particle at
             * @param posY
             *            (RETURN VALUE)
             *            Y coordinate to place the particle at
             * @param posZ
             *            (RETURN VALUE)
             *            Z coordinate to place the particle at
             * @param velX
             *            (RETURN VALUE)
             *            X component of the particles starting velocity
             * @param velY
             *            (RETURN VALUE)
             *            Ycomponent of the particles starting velocity
             * @param velZ
             *            (RETURN VALUE)
             *            Z component of the particles starting velocity
             * @param angVelX
             *            (RETURN VALUE)
             *            X component of the particles starting angular velocity
             * @param angVelY
             *            (RETURN VALUE)
             *            Y component of the particles starting angular velocity
             * @param angVelZ
             *            (RETURN VALUE)
             *            Z component of the particles starting angular velocity
             * @param orientation
             *            (RETURN VALUE)
             *            Nine-element array containing the orientation matrix for
             *            this element.
             *            The elements of the array are in the following order:
             *                XX, XY, XZ, YX, YY, YZ, ZX, ZY, ZZ
             * @param propData
             *            Versioned interface providing access to the initial
             *            value for all particle custom properties (as configured
             *            in the EDEM GUI) and a corresponding changeset.
             */
            virtual NApi::ECalculateResult createParticle(
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
                                               NApiCore::ICustomPropertyDataApi_1_0* propData) = 0;

            /**
             * Returns the number of custom properties this plugin wants to
             * register with the system for the supplied category.
             *
             * This version of the API supports the following property
             * categories:
             *     Contact Properties
             *     Geometry Properties
             *     Particle Properties
             *     Simulation Properties
             *
             * The method will be called once for each category at load time.
             * The implementation should return how many properties of that
             * category it wishes to register.
             *
             * If the plugin does not use custom properties this method should
             * return 0 for all categories.
             *
             * @param category The category of the custom property.
             * @return The number of custom properties the plugin wishes to
             *         register.
             */
            virtual unsigned int getNumberOfRequiredProperties(
                                     const NApi::EPluginPropertyCategory category) {return 0;}

            /**
             * Retrieves details for a given property.  This method will be
             * called for each category for propertyIndex values
             * 0...(getNumberOfRequiredProperties(category) - 1) to retrieve
             * the details for that property from the plugin.  These properties
             * will then be registered with the system if they do not clash
             * with any existing properties.
             *
             * This version of the API supports the following property
             * categories:
             *     Contact Properties
             *     Geometry Properties
             *     Particle Properties
             *     Simulation Properties
             *
             * If the plugin does not use custom properties this method should
             * allways return false.
             *
             * @param propertyIndex    The index of the property to retrieve data
             *                         for
             * @param category         The category of the custom property to return
             *                         details for.
             * @param name             (RETURN VALUE)
             *                         A CUSTOM_PROP_MAX_NAME_LENGTH char array
             *                         is supplied to be populated with the name
             *                         of the property
             * @param dataType         (RETURN VALUE)
             *                         The data type of the property should allways
             *                         be set to eDouble
             * @param numberOfElements (RETURN VALUE)
             *                         The number of elements (min 1)
             * @param unitType         (RETURN VALUE)
             *                         The unit type of the property
             * @return bool to say if data exists for the property
             */
            virtual bool getDetailsForProperty(
                             unsigned int                    propertyIndex,
                             NApi::EPluginPropertyCategory   category,
                             char                            name[NApi::CUSTOM_PROP_MAX_NAME_LENGTH],
                             NApi::EPluginPropertyDataTypes& dataType,
                             unsigned int&                   numberOfElements,
                             NApi::EPluginPropertyUnitTypes& unitType) {return false;}
    };
};

#endif // IPLUGINPARTICLEFACTORYV2_0_0

