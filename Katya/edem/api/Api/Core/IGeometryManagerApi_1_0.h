#ifndef IGEOMETRYMANAGERAPI_1_0_H
#define IGEOMETRYMANAGERAPI_1_0_H

#include "IApi.h"

namespace NApiCore
{
    /**
     * The IGeometryManagerApi_1_0 interface provides basic access to the
     * configured geometries.
     *
     * NAME:     Geometry Manager
     * ID:       EApiId::eGeometryManager (10)
     * VERSION:  1.0
     */
    class IGeometryManagerApi_1_0 : public IApi
    {
    public:
        /** */
        IGeometryManagerApi_1_0() = default;

        /** */
        virtual ~IGeometryManagerApi_1_0() = default;

        /**
         * Enables the custom property data entries to be set to the supplied value
         * @param geomName The geometry name.
         * @param propName Name of the target custom property.
         * @param value The value to reset the property value to.
         * @return true if reset operation succeeded
         */
        virtual bool resetCustomProperty(const char geomName[NApi::API_BASIC_STRING_LENGTH], const char propName[NApi::API_BASIC_STRING_LENGTH], double value) = 0;
    };
};

#endif // IGEOMETRYMANAGERAPI_1_0_H
