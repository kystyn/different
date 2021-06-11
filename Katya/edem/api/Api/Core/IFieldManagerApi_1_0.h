#ifndef IFIELDMANAGERAPI_1_0_H
#define IFIELDMANAGERAPI_1_0_H

#include "IApi.h"

namespace NApiCore
{
    /**
     * The IFieldManagerApi_1_0 provides basic access to the field manager
     * via a series of query methods.
     *
     * NAME:    Field Manager
     * ID:      EApiId::eFieldManager (1)
     * VERSION: 1.0
     *
     * At this time only read methods are supplied.
     */
    class IFieldManagerApi_1_0 : public IApi
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IFieldManagerApi_1_0() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IFieldManagerApi_1_0() {};

        /**
         * Returns a Field API instance for the specified field ID.
         *
         * The Field Manager is free to return any minor revision
         * equal to or later than the specified value, as long as the
         * major revision matches.
         *
         * All APIs returned by this method should be released back to
         * the API Manager via its release() method once you have finished
         * with them.  Do not delete API instances directly or it may result
         * in system instability and data corruption.
         *
         * @param apiId The API the user is requesting
         *              (currently API_MANAGER_API is supported)
         * @param major The required major version
         * @param minor The minimum required minor version
         * @param fieldId ID of the field to bind the API instance to
         * @return An initialised API instance or 0 if none
         */
        virtual IApi* getApi(EApiId                 apiId,
                             NApi::tApiMajorVersion major,
                             NApi::tApiMinorVersion minor,
                             unsigned int           fieldId) = 0;

        /**
         * Returns a Field API instance for the specified field ID.
         *
         * The Field Manager is free to return any minor revision
         * equal to or later than the specified value, as long as the
         * major revision matches.
         *
         * All APIs returned by this method should be released back to
         * the API Manager via its release() method once you have finished
         * with them.  Do not delete API instances directly or it may result
         * in system instability and data corruption.
         *
         * @param apiId The API the user is requesting
         *              (currently API_MANAGER_API is supported)
         * @param major The required major version
         * @param minor The minimum required minor version
         * @param fieldName Name of the field to bind the API instance to
         * @return An initialised API instance or 0 if none
         */
        virtual IApi* getApi(EApiId                 apiId,
                             NApi::tApiMajorVersion major,
                             NApi::tApiMinorVersion minor,
                             char                   fieldName[NApi::API_BASIC_STRING_LENGTH]) = 0;
    };
};

#endif // IFIELDMANAGERAPI_1_0_H
