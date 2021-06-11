#include "CGenericFileReader.h"

#include <fstream>
#include <sstream>
#include <iostream>


using namespace std;

/**
 * This simple template function provides a
 * typesafe method of converting a string to
 * a numeric type (t).  It will also
 * report a failure to parse to the caller
 *
 * EXAMPLE:
 *
 * @tparam T         Type to convert to
 * @param toValue    The value to store the result in
 * @param fromString The string to convert from
 * @param base       Stream options.  Typically std:dec or std:hex
 *                   or one of the other representation flags
 */
template <class T>
bool from_string(T& toValue,
                 const std::string& fromString,
                 std::ios_base& (*base)(std::ios_base&))
{
  std::istringstream iss(fromString);
  return !(iss >> base >> toValue).fail();
}

/**
 * Simple function to removes leading and trailing whitespace from a string
 *
 * @param toTrim Reference to string to trim
 */
void trim(std::string& toTrim)
{
    // List of characters to remove
    static const std::string WHITESPACE = " \t\r\n\v\f";

    // Find first character that is not whitespace
    size_t pos = toTrim.find_first_not_of(WHITESPACE);
    if (string::npos == pos)
    {
        // Whole string is whitespace
        toTrim.clear();
        return;
    }
    else
    {
        // Strip space to this point
        toTrim.erase(0, pos);
    }

    // Find last character that is not whitespace
    pos = toTrim.find_last_not_of(WHITESPACE);
    if (pos != string::npos &&
        (pos + 1) < toTrim.length())
    {
        toTrim.erase(pos+1);
    }
}

CGenericFileReader::CGenericFileReader()
{
    // Nothing to do
}

CGenericFileReader::~CGenericFileReader()
{
    // Nothing to do, map clears itself
}

CGenericFileReader* CGenericFileReader::getReader(const std::string& file)
{
    CGenericFileReader* tmp = new CGenericFileReader();

    if (0 != tmp)
    {
        if (false == tmp->init(file))
        {
            delete tmp;
            tmp = 0;
        }
    }

    // Return the new reader (or 0 if anything went wrong)
    return tmp;
}

bool CGenericFileReader::hasKey(const std::string& key)
{
    // Find the key
    tStringStringMap::iterator idx = m_data.find(key);

    // If the iterator is not at the end of the map
    // the key exists
    return m_data.end() != idx;
}

bool CGenericFileReader::getString(const std::string& key,
                                   std::string& valueToSet)
{
    // Find the key
    tStringStringMap::iterator idx = m_data.find(key);

    // Check to see if the key exists in the map
    if (m_data.end() != idx)
    {
        valueToSet = idx->second;
        return true;
    }
    else
    {
        return false;
    }
}

bool CGenericFileReader::getInt(const std::string& key,
                                int& valueToSet)
{
    string strValue;
    if (false == getString(key, strValue))
    {
        // Key not present
        return false;
    }

    // Check to see if value can be converted to a number
    int number;
    if (false == from_string<int>(number, strValue, std::dec))
    {
        // Could not convert string to int
        return false;
    }

    // Store converted value and return
    valueToSet = number;
    return true;
}

bool CGenericFileReader::getDouble(const std::string& key,
                                   double& valueToSet)
{
    string strValue;
    if (false == getString(key, strValue))
    {
        // Key not present
        return false;
    }

    // Check to see if value can be converted to a number
    double number;
    if (false == from_string<double>(number, strValue, std::dec))
    {
        // Could not convert string to int
        return false;
    }

    // Store converted value and return
    valueToSet = number;
    return true;
}

void CGenericFileReader::dump(std::ostream& out)
{
    // send everything to the out
    for (tStringStringMap::iterator idx = m_data.begin();
         idx != m_data.end();
         ++idx)
    {
        out << idx->first << "=" << idx->second << endl;
    }
}

bool CGenericFileReader::init(const std::string& file)
{
    // Check to see if we can open the file
    ifstream fileStream(file.c_str(), ios::in);
    if (!fileStream)
    {
        // Stream didnt open well so we cant read file
        return false;
    }

    // We have a file.  Read it line by line.
    //
    // This is using some clever code:
    // 1. This is the global std::getline NOT the ifstream::getline
    // 2. The return of getLine is ifstream (in this case)
    // 3. We query the stream to see if its failbit or badbit are set.
    //    These get set when we try to read when we've already reached
    //    the end of the file.  We dont stop on eofbit being set
    //    as it may be set when we read the last line of the file
    //    and we need to parse that
    string line;
    while (false == getline(fileStream, line).fail())
    {
        // First remove all leading/trailing white space
        trim(line);

        // Skip empty lines and comments
        if (0   != line.length() &&
            '#' != line[0])
        {
            // Find the first '=' character
            size_t pos = line.find('=');

            // We can rejec tlines with no '=' or those
            // where it is the first character (no key name)
            if (string::npos != pos &&
                0 != pos)
            {
                //extract the key
                string key = line.substr(0, pos);

                // Move position past the '='
                pos++;

                // Extract the value
                // If '=' is the last character then we have
                // an empty value, this is allowed
                string value;
                if (pos < line.length())
                {
                    value = line.substr(pos);
                }

                // Trim the key and value again incase there was space
                // arround the '='
                trim(key);
                trim(value);

                // Store value
                if (false == m_data.insert(std::make_pair(key,value)).second)
                {
                    // could not insert due to duplicate key
                    // the file is malformed
                    return false;
                }
            }
            else
            {
                return false;
            }

        }
    }

    return true;
}