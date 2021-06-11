#ifndef CGENERICFILEREADER_H
#define CGENERICFILEREADER_H

#include <ostream>
#include <string>
#include <map>

/**
 * This simple class (which acts as its own factory) provides
 * a generic way to read a config file.  To include it in
 * your own plugins \#include the .h file into any plugin file
 * that wants to use the class.  You will also need to compile
 * the cpp file into your plugins shared library along with
 * your other source files.
 *
 * Create a reader using code of the form:
 *
 * @code
 *     // Create and initialise the reader from a file
 *     CGenericFileReader* reader =
 *         CGenericFileReader::getReader("File Path");
 *     if (0 == reader)
 *     {
 *         // File could not be read or was invalid
 *         return false;
 *     }
 *
 *     // Use the reader
 *
 *     // Delete the reader
 *     delete reader;
 * @endcode
 *
 * It can be used in two ways:
 *
 * @code
 *     // Get value, default to 3.14 if the the value
 *     // was not in the file or is invalid
 *     double val = 3.14;
 *     reader->getDouble("Pi", val);
 * @endcode
 *
 * @code
 *     // Get a value and fail if not in the config file
 *     // or present but could not be converted to a number
 *     int intVal = 0;
 *     if (false = reader->getInt("Initial_Count", intVal))
 *     {
 *         // Data is missing or invalid
 *     }
 * @endcode
 *
 * The supported config file format is key=value pairs.
 * both key and value can have spaces inside them but all
 * leading and trailing white space are trimmed.  It's
 * cleaner to keep white space out of keys.
 *
 * An example config file might look something like this:
 *
 * @code
 *      # Some string keys
 *      A_String_key       = Some value
 *      Another_String_key = Another value
 *
 *      # Some double keys
 *      Double_key_1 = 1.0
 *      Double_key_2 = 3.14786
 *      Double_key_3 = 3.14159E5
 *
 *      # Some integer keys
 *      Int_key_1 = 1
 *      Int_key_2 = 1026
 * @endcode
 *
 * Note the use of lines starting # for comments and the fact that
 * 3.14159E5 is correctly parsed for doubles.
 *
 * The reader considers all entries strings but has methods
 * to let you quickly convert values to either signed ints or
 * doubles.
 */
class CGenericFileReader
{
public:

    /**
     * Parses the supplied file and returns a fully
     * initialised reader.  This is the only way to
     * create an instance of this class
     *
     * @param file The full path to the file
     * @return The fully initialised file reader.
     */
    static CGenericFileReader* getReader(const std::string& file);

    /**
     * Destructor, deletes and cleans up internal storage.
     */
    ~CGenericFileReader();

    /**
     * Checks to see if a key was present in the file
     *
     * @param key The key to check for
     * @return True if a value exists for the supplied key
     */
    bool hasKey(const std::string& key);

    /**
     * Checks to see if a key is present and if so returns
     * it as a string.
     *
     * If no value is prsent for the key then the method returns
     * false and valueToSet is unchanged.  As such you can put a
     * default value in to valueToSet before calling this method and
     * ignore the return result if you do not care if the value comes
     * from the file.
     *
     * @param  key        The key to extract the value for
     * @param  valueToSet Refernce to a string to place the value in
     * @return True if key is present
     */
    bool getString(const std::string& key,
                   std::string& valueToSet);

    /**
     * Checks to see if a key is present and if so returns
     * it as an int.
     *
     * If no conversion is possible (e.g "hello" cant be converted
     * to an int or double) then the method returns false and
     * valueToSet is unchanged.  As such you can put a default
     * value in to valueToSet before calling this method and ignore
     * the return result if you do not care if the value comes from
     * the file.
     *
     * @param  key        The key to extract the value for
     * @param  valueToSet Refernce to an int to place the value in
     * @return True if key is present and can be converted to an int
     */
    bool getInt(const std::string& key,
                int& valueToSet);

    /**
     * Checks to see if a key is present and if so returns
     * it as a double.
     *
     * If no conversion is possible (e.g "hello" cant be converted
     * to an int or double) then the method returns false and
     * valueToSet is unchanged.  As such you can put a default
     * value in to valueToSet before calling this method and ignore
     * the return result if you do not care if the value comes from
     * the file.
     *
     * @param  key        The key to extract the value for
     * @param  valueToSet Refernce to a double to place the value in
     * @return True if key is present and can be converted to a double
     */
    bool getDouble(const std::string& key,
                   double& valueToSet);

    /**
     * dumps the current key=value list to the supplied ostream
     *
     * @param out The stream to write to
     */
    void dump(std::ostream& out);

private:

    /**
     * Typedef for holding data read from file
     */
    typedef std::map<std::string, std::string> tStringStringMap;

    /**
     * Constructor, does nothing.
     *
     * The constructor is private to stop it being directly
     * called.  Instead use the getReader() static method.
     */
    CGenericFileReader();

    /**
     * Initialise to the file and read whole file.
     *
     * @param file The full path to the file
     * @return True if file was read successfully.
     */
    bool init(const std::string& file);

    /**
     * Storage for key->value data
     */
    tStringStringMap m_data;
};

#endif // CGENERICFILEREADER_H
