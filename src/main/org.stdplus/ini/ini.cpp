#include <org.stdplus/ini/ini.hpp>
#include <org.stdplus/iostreams/getline.hpp>

#include <fstream>
#include <sstream>

namespace
{
  void parse_line(org::stdplus::ini::ini& result, std::string const& line , std::string& section)
  {
    if ( !line.length() )
      return;

    for( std::string::size_type idx = 0 ; idx != line.length() ; ++idx )
      {
        char ch = line[ idx ];
        if ( isspace( ch ) )
          continue;

        if ( ch == ';' )
          return;

        if ( ch == '[' && line[ line.length() - 1 ] == ']' )
          {
            section = line.substr( idx + 1 , line.length() - ( idx + 2 ) );
            return;
          }

        if ( ch == '=' )
          {
            std::string name = line.substr( 0 , idx );
            std::string value = line.substr( idx + 1 );

            result.set( section , name , value );
            return;
          }
      }
  }
}

namespace org
{
  namespace stdplus
  {
    namespace ini
    {
      std::string ini::get(std::string const& section, std::string const& key) const
      {
        return m_map.at(section).at(key);
      }

      std::string ini::get(std::string const& section, std::string const& key, std::string const& defaultValue) const
      {
        try {
          return get(section,key);
        } catch( std::out_of_range& ignore ) {
          return defaultValue;
        }
      }

      ini& ini::set(std::string const& section, std::string const& key, std::string const& value)
      {
        m_map[section][key] = value;
        return *this;
      }

      ini ini::load(std::istream& input)
      {
        ini result;
        std::string section;
        while( input && !input.eof() )
          {
            std::string line = stdplus::iostreams::getline(input,"\r\n");

            parse_line( result, line , section );
          }

        return result;
      }

      ini ini::load(std::string const& contents)
      {
        std::stringstream sstream(contents);
        return load(sstream);
      }

      ini ini::loadWithPath(std::string const& filename)
      {
        std::ifstream fstream(filename.c_str());
        return load(fstream);
      }

      std::string ini::save() const
      {
        std::stringstream result;
        save(result);
        return result.str();
      }

      void ini::save(std::ostream& output) const
      {
        std::string spacer = "";
        for( auto& section : m_map )
          {
            output << spacer << "[" << section.first << "]\n";
            for( auto& entry : section.second )
              output << entry.first << "=" << entry.second << "\n";
            spacer = "\n";
          }
      }

      ini::section_type const& ini::section( std::string name ) const
      {
        return m_map.at(name);
      }

    } // namespace ini
  } // namespace stdplus
} // namespace org
