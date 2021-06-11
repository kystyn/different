#ifndef IAPIMANAGER_1_0_H
#define IAPIMANAGER_1_0_H

#include "ApiTypes.h"
#include "IApi.h"

namespace NApiCore
{
    /**
     * The API manager provides the ability to allocate and initialise
     * various APIs for use by plugins.
     *
     * NAME:    API Manager
     * ID:      EApiId::eApiManager (0)
     * VERSION: 1.0
     *
     * All allocation and de-allocation of API instances is handled
     * by the manager.
     *
     * getApi() is used for retrieving allocated and initialized
     * base API instances.
     *
     * release() is used for releasing API instances back to the manager
     * for de-allocation/re-use. Do not directly delete IApi instances as
     * it may result in system instability and data corruption.
     *
     * NOTE: The API manager is capable of returning instances of
     * any supported version of the API manager.
     */
    class IApiManager_1_0 : public IApi
    {
    public:
        /**
         * Constructor, does nothing.
         */
        IApiManager_1_0() {};

        /**
         * Destructor, does nothing.
         */
        virtual ~IApiManager_1_0() {};

        /**
         * Retrieves a base API instance for use.
         *
         * The Api manager is free to return any minor revision
         * equal to or later than the specified value, as long as the
         * major revision matches.
         *
         * All APIs returned by this method should be released back to
         * the API Manager via its release() method once you have finished
         * with them.  Do not delete API instances directly or it may result
         * in system instability and data corruption.
         *
         * @param apiId The id of the API
         * @param major The major version of the API you wish
         * @param minor The minor version of the API you wish
         * @return An initialised API instance or 0 if none
         */
        virtual IApi* getApi(EApiId                 apiId,
                             NApi::tApiMajorVersion major,
                             NApi::tApiMinorVersion minor) = 0;

        /**
         * Releases an API instance back to the manager
         * for potential re-use or de-allocation.
         * @param apiInstance The instance to release
         */
        virtual void release(IApi* apiInstance) = 0;
    };
};

#endif // IAPIMANAGER_1_0_H
