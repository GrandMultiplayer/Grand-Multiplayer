#include <string>
#include <algorithm>

#include "ini.h"

#include "IniReader.h"

INIReader::INIReader(const std::string& filename)
{
	_error = ini_parse(filename.c_str(), ValueHandler, this);
}

int INIReader::ParseError() const
{
	return _error;
}

std::string INIReader::Get(const std::string& section, const std::string& name, const std::string& default_value) const
{
	std::string key = MakeKey(section, name);
	// Use _values.find() here instead of _values.at() to support pre C++11 compilers
	return _values.count(key) ? _values.find(key)->second : default_value;
}

long INIReader::GetInteger(const std::string& section, const std::string& name, long default_value) const
{
	std::string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	// This parses "1234" (decimal) and also "0x4D2" (hex)
	long n = strtol(value, &end, 0);
	return end > value ? n : default_value;
}

double INIReader::GetReal(const std::string& section, const std::string& name, double default_value) const
{
	std::string valstr = Get(section, name, "");
	const char* value = valstr.c_str();
	char* end;
	double n = strtod(value, &end);
	return end > value ? n : default_value;
}

bool INIReader::GetBoolean(const std::string& section, const std::string& name, bool default_value) const
{
	std::string valstr = Get(section, name, "");
	// Convert to lower case to make string comparisons case-insensitive
	transform(valstr.begin(), valstr.end(), valstr.begin(), ::tolower);
	if (valstr == "true" || valstr == "yes" || valstr == "on" || valstr == "1")
		return true;
	else if (valstr == "false" || valstr == "no" || valstr == "off" || valstr == "0")
		return false;
	else
		return default_value;
}

std::string INIReader::MakeKey(const std::string& section, const std::string& name)
{
	std::string key = section + "=" + name;
	// Convert to lower case to make section/name lookups case-insensitive
	transform(key.begin(), key.end(), key.begin(), ::tolower);
	return key;
}

int INIReader::ValueHandler(void* user, const char* section, const char* name,
	const char* value)
{
	INIReader* reader = (INIReader*)user;
	std::string key = MakeKey(section, name);
	if (reader->_values[key].size() > 0)
		reader->_values[key] += "\n";
	reader->_values[key] += value;
	return 1;
}