#ifndef IGEOMETRYMANAGERAPI_1_1_H
#define IGEOMETRYMANAGERAPI_1_1_H

#include "IGeometryManagerApi_1_0.h"

namespace NApiCore
{
    /**
     * The IGeometryManagerApi_1_1 interface provides basic access to the
     * configured geometries.
     *
     * NAME:     Geometry Manager
     * ID:       EApiId::eGeometryManager (10)
     * VERSION:  1.1
     */
    class IGeometryManagerApi_1_1 : public IGeometryManagerApi_1_0
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IGeometryManagerApi_1_1() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IGeometryManagerApi_1_1() {};

        using IGeometryManagerApi_1_0::resetCustomProperty;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param geomName The geometry name.
         * @param propIndex Index of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char geomName[NApi::API_BASIC_STRING_LENGTH], unsigned int propIndex, double value) = 0;
    };
};

#endif // IGEOMETRYMANAGERAPI_1_1_H
