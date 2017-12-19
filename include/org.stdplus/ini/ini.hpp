#ifndef he674ccb3_dd18_4160_9808_8a6c3db62ccf
#define he674ccb3_dd18_4160_9808_8a6c3db62ccf

#include <org.stdplus/strings/fromString.hpp>

#include <iostream>
#include <map>
#include <stdexcept>
#include <string>

namespace org
{
namespace stdplus
{
  namespace ini
  {
    class ini
    {
    public:
      typedef std::map< std::string , std::string > section_type;

      std::string get( std::string const& section, std::string const& key , std::string const& defaultValue ) const; // returns defaultValue if section or key does not exist
      std::string get( std::string const& section, std::string const& key ) const; // throws std::out_of_range if section or key does not exist.

      template< typename RESULT_TYPE >
      RESULT_TYPE read( std::string const& section, std::string const& key, RESULT_TYPE defaultValue ) const
      {
        try
          {
            return read<RESULT_TYPE>(section,key);
          }
        catch( std::out_of_range& ignore )
          {
            return defaultValue;
          }
      }

      template< typename RESULT_TYPE >
      RESULT_TYPE read( std::string const& section, std::string const& key) const
      {
        std::string sValue = get(section,key);
        return strings::fromString<RESULT_TYPE>(sValue);
      }

      ini& set( std::string const& section, std::string const& key , std::string const& value ); // returns `*this`.

      static ini load(std::string const& contents);
      static ini loadWithPath(std::string const& filename);
      static ini load(std::istream& input);
      void save(std::ostream& output) const;
      std::string save() const;

      section_type const& section( std::string name ) const;

    private:
      typedef std::map< std::string , section_type > map_type;
      map_type m_map;
    };
  } // namespace ini
} // namespace stdplus
} // namespace org
#endif
