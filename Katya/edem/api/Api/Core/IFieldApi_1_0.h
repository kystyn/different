#ifndef IFIELDAPI_1_0_H
#define IFIELDAPI_1_0_H

#include "IApi.h"

namespace NApiCore
{
    /**
     * The IFieldApi_1_0 provides basic access to field
     * data via a series of query methods.
     *
     * NAME:    Field
     * ID:      EApiId::eField (2)
     * VERSION: 1.0
     *
     * At this time only read methods are supplied.
     */
    class IFieldApi_1_0 : public IApi
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IFieldApi_1_0() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IFieldApi_1_0() {};

        /**
         * Checks whether the field represents scalar data or
         * vector data.
         * @return true if the field contains scalar data, false
         *         if it is a vector field.
         */
        virtual bool fieldIsScalar() = 0;

        /**
         * Queries a vector field for the interpolated
         * field value at a given point in EDEM's global
         * co-ordinate system.
         *
         * EDEM will map from the supplied point to the fields
         * internal co-ordinates using the orientation set in
         * the creator's field manager dialog.
         *
         * For a 2D vector field, results in one dimension (depending
         * on the plane's orientation) will always be zero
         *
         * @param pointX X Co-ordinate of the query point
         * @param pointY Y Co-ordinate of the query point
         * @param pointZ Z Co-ordinate of the query point
         * @param numberOfPoints How many points to use when calculating
         *                       the interpolated result
         * @param interpolationType The type of interpolation to use.
         * @param resultX
         *            [RETURN VALUE]
         *            X dimension of the vector field strength
         * @param resultY
         *            [RETURN VALUE]
         *            Y dimension of the vector field strength
         * @param resultZ
         *            [RETURN VALUE]
         *            Z dimension of the vector field strength
         * @return true if an interpolated value could be calculated,
         *         else false.
         */
        virtual bool queryVectorField(double                   pointX,
                                      double                   pointY,
                                      double                   pointZ,
                                      unsigned int             numberOfPoints,
                                      NApi::EInterpolationType interpolationType,
                                      double&                  resultX,
                                      double&                  resultY,
                                      double&                  resultZ) = 0;


        /**
         * Queries a scalar field for the interpolated
         * field value at a given point in EDEM's global
         * co-ordinate system.
         *
         * EDEM will map from the supplied point to the fields
         * internal co-ordinates using the orientation set in
         * the creator's field manager dialog.
         *
         * @param pointX X Co-ordinate of the query point
         * @param pointY Y Co-ordinate of the query point
         * @param pointZ Z Co-ordinate of the query point
         * @param numberOfPoints How many points to use when calculating
         *                       the interpolated result
         * @param interpolationType The type of interpolation to use.
         * @param result
         *            [RETURN VALUE]
         *            Strength of the scalar field
         * @return true if an interpolated value could be calculated,
         *         else false.
         */
        virtual bool queryScalarField(double                   pointX,
                                      double                   pointY,
                                      double                   pointZ,
                                      unsigned int             numberOfPoints,
                                      NApi::EInterpolationType interpolationType,
                                      double&                  result) = 0;
    };
};

#endif // IFIELDAPI_1_0_H
