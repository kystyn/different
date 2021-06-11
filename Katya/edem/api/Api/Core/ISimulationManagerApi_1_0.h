#ifndef ISIMULATIONMANAGERAPI_1_0_H
#define ISIMULATIONMANAGERAPI_1_0_H

#include "IApi.h"

namespace NApiCore
{
    /**
     * The ISimulationManagerApi_1_0 interface provides basic access to the
     * configured simulation properties.
     *
     * NAME:     Simulation Manager
     * ID:       EApiId::eSimulationManager (11)
     * VERSION:  1.0
     */
    class ISimulationManagerApi_1_0 : public IApi
    {
    public:
        /**
         * Constructor, does nothing.
         */
        ISimulationManagerApi_1_0() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~ISimulationManagerApi_1_0() {};

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param propName Name of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded, otherwise false.
         */
        virtual bool resetCustomProperty(const char propName[NApi::API_BASIC_STRING_LENGTH], double value) = 0;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param propName Name of the target custom property.
         * @param elementIndex The target index for a multi element property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded, otherwise false.
         */
        virtual bool resetCustomProperty(const char propName[NApi::API_BASIC_STRING_LENGTH], int elementIndex, double value) = 0;
    };
};

#endif // ISIMULATIONMANAGERAPI_1_0_H
