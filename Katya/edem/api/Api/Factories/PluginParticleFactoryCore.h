#ifndef PLUGINPARTICLEFACTORYCORE_H
#define PLUGINPARTICLEFACTORYCORE_H

/***************************************************************************/
/* This header file contains methods that must be implemented by           */
/* all V2.0.0 and later particle factory plugins.                          */
/***************************************************************************/

#include <Api/Core/PluginConstants.h>

namespace NApiFactory
{
    class IPluginParticleFactory;
};

/**
 * This method should return a newly allocated instance of the plugin
 * each time it is called.
 *
 * TO BE IMPLEMENTED BY END USER IN EACH PARTICLE BODY FORCE PLUGIN
 *
 * @return A newly allocated plugin instance
 */
EXPORT_MACRO NApiFactory::IPluginParticleFactory* GETFACTORYINSTANCE();

/**
 * This method should de-allocate the supplied instance.
 *
 * A plugin will only ever be supplied instances to
 * de-allocate that it created via it's GETFACTORYINSTANCE() method.
 *
 * TO BE IMPLEMENTED BY END USER IN EACH PARTICLE BODY FORCE PLUGIN
 *
 * @param instance The instance to be released
 */
EXPORT_MACRO void RELEASEFACTORYINSTANCE(NApiFactory::IPluginParticleFactory* instance);

/**
 * Returns the version of the plugin interface implemented
 * by this plugin file.
 *
 * TO BE IMPLEMENTED BY END USER
 *
 * The version number must be packed in to a single
 * 32 bit value.  Consider the following example code:
 *
 *  static const int INTERFACE_VERSION_MAJOR = 0x02;
 *  static const int INTERFACE_VERSION_MINOR = 0x00;
 *  static const int INTERFACE_VERSION_PATCH = 0x00;
 *
 *  return (INTERFACE_VERSION_MAJOR << 16 |
 *          INTERFACE_VERSION_MINOR << 8 |
 *          INTERFACE_VERSION_PATCH);
 *
 * @return Interface version packed in to a single 32bit value
 */
EXPORT_MACRO int GETFACTINTERFACEVERSION();

#endif // PLUGINPARTICLEFACTORYCORE_H
