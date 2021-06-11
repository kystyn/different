#ifndef ISIMULATIONMANAGERAPI_1_1_H
#define ISIMULATIONMANAGERAPI_1_1_H

#include "ISimulationManagerApi_1_0.h"

namespace NApiCore
{
    /**
     * The ISimulationManagerApi_1_1 interface provides basic access to the
     * configured simulation properties.
     *
     * NAME:     Simulation Manager
     * ID:       EApiId::eSimulationManager (11)
     * VERSION:  1.1
     */
    class ISimulationManagerApi_1_1 : public ISimulationManagerApi_1_0
    {
    public:
        /**
         * Constructor, does nothing.
         */
        ISimulationManagerApi_1_1() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~ISimulationManagerApi_1_1() {};

        using ISimulationManagerApi_1_0::resetCustomProperty;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param propIndex The index of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded, otherwise false.
         */
        virtual bool resetCustomProperty(unsigned int propIndex, double value) = 0;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param propIndex The index of the target custom property.
         * @param elementIndex The target index for a multi element property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded, otherwise false.
         */
        virtual bool resetCustomProperty(unsigned int propIndex, int elementIndex, double value) = 0;
    };
};

#endif // ISIMULATIONMANAGERAPI_1_1_H
