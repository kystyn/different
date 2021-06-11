#ifndef IGEOMETRYMANAGERAPI_1_2_H
#define IGEOMETRYMANAGERAPI_1_2_H

#include "IGeometryManagerApi_1_1.h"

#include <vector>

#include "HelpersV3_0_0.h"
#include "NCalcForceTypesV3_0_0.h"


namespace NApiCore
{
    /**
     * The IGeometryManagerApi_1_2 interface provides additional access to the configured geometries mesh coordinates,
     * in addition to the normal custom property access
     *
     * NAME:     Geometry Manager
     * ID:       EApiId::eGeometryManager (10)
     * VERSION:  1.2
     */
    class IGeometryManagerApi_1_2 : public IGeometryManagerApi_1_1
    {
    public:
        /** Default constructor */
        IGeometryManagerApi_1_2() = default;

        /** Default destructor */
        virtual ~IGeometryManagerApi_1_2() = default;

        /**
        * Provides access into geometry mesh data. 
        * - Can only be used in API Starting calls. Must be copied from. -
        * @param geomName
        * @param in_transformSpace local or global space to retrieve coords in
        * @return pointer to start of geometry mesh in format [x, y, z, x, y, z..]
        */
        virtual const double* getGeometryMesh(const char geomName[NApi::API_BASIC_STRING_LENGTH],
                                              NCalcForceTypesV3_0_0::ETransformSpace transformSpace) const = 0;

        /**
        * @param geomName
        * @return length of the geometry mesh buffer returned by getGeometryMesh(..)
        */
        virtual unsigned int getSizeMeshBuffer(const char geomName[NApi::API_BASIC_STRING_LENGTH]) const = 0;
                
        /**
        * Provides access into triangle node data.
        * - Can only be used in API Starting calls. Must be copied. -
        * @param geomName
        * @return pointer to start of triangle nodes in format [t0i0, t0i1, t0i2, t1i0, t1i1, t1i2, ..]
        */
        virtual const unsigned int* getAllTriangleNodes(const char geomName[NApi::API_BASIC_STRING_LENGTH]) const = 0;

        /**
        * @param geomName
        * @return length of the triangle nodes buffer returned by getAllTriangleNodes(..)
        */
        virtual unsigned int getSizeTriangleNodeBuffer(const char geomName[NApi::API_BASIC_STRING_LENGTH]) const = 0;

        /** 
        * Resets the given custom property to the given value for all geometries
        * @param propIndex custom property index
        * @param value
        */
        virtual void resetCustomPropertyForAllGeometries(unsigned int propIndex, double value) = 0;

        /** Reset internally stored triangle node/mesh data that can be requested by api implementers. */
        virtual void resetTransferredData() = 0;


        /**
        * Helper function to safely retrieve geometry triangle nodes from the geometry api manager.
        * @param geometryManager
        * @param geomName
        * @return triangle nodes for the given geometry */
        std::vector<NApiHelpersV3_0_0::SGeomTriangleNode> getGeometryTriangleNodes(const NApiCore::IGeometryManagerApi_1_2* geometryManager,
                                                                                   const char* geomName) const
        {
            const unsigned int* nodesDescription(geometryManager->getAllTriangleNodes(geomName));
            const auto sizeBuffer(geometryManager->getSizeTriangleNodeBuffer(geomName));

            std::vector< NApiHelpersV3_0_0::SGeomTriangleNode> nodes;

            if (nodesDescription == nullptr)
                return nodes;

            nodes.reserve(sizeBuffer / 3);

            for (unsigned int i{ 0 }; i < sizeBuffer; i += 3)
            {
                nodes.emplace_back(nodesDescription[i], nodesDescription[i + 1], nodesDescription[i + 2]);
            }

            return nodes;
        }

        /**
        * Helper function to safely retrieve geometry mesh coordinates from the geometry api manager.
        * @param geometryManager
        * @param geomName
        * @param transformSpace global or local space to get mesh coordinates in
        * @return vertex coordinates for the given geometry */
        std::vector<NApiHelpersV3_0_0::CSimple3DVector> getGeometryMesh(const NApiCore::IGeometryManagerApi_1_2* geometryManager,
                                                                        const char* geomName,
                                                                        NCalcForceTypesV3_0_0::ETransformSpace transformSpace) const
        {
            const double* meshDescription(geometryManager->getGeometryMesh(geomName, transformSpace));
            const auto sizeMeshBuffer(geometryManager->getSizeMeshBuffer(geomName));

            std::vector< NApiHelpersV3_0_0::CSimple3DVector> coords;

            if (meshDescription == nullptr)
                return coords;

            coords.reserve(sizeMeshBuffer / 3);

            for (unsigned int i{ 0 }; i < sizeMeshBuffer; i += 3)
            {
                coords.emplace_back(meshDescription[i], meshDescription[i + 1], meshDescription[i + 2]);
            }

            return coords;
        }
    };
};

#endif // IGEOMETRYMANAGERAPI_1_2_H
