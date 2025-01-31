/******************************************************************************
License: GNU General Public License v3.0 only
Copyright:
	(C) 2008 - 2022 Alexander Shaduri <ashaduri@gmail.com>
******************************************************************************/
/// \file
/// \author Alexander Shaduri
/// \ingroup applib
/// \weakgroup applib
/// @{

// #include "local_glibmm.h"

#include "hz/string_algo.h"  // string_*
#include "hz/string_num.h"  // string_is_numeric, number_to_string
#include "hz/debug.h"  // debug_*

#include "app_pcrecpp.h"
#include "smartctl_version_parser.h"




bool SmartctlVersionParser::parse_version(const std::string& s, std::string& version_only, std::string& version_full)
{
	// e.g.
	// "smartctl version 5.37"
	// "smartctl 5.39"
	// "smartctl 5.39 2009-06-03 20:10" (cvs versions)
	// "smartctl 5.39 2009-08-08 r2873" (svn versions)
	// "smartctl 7.3 (build date Feb 11 2022)" (git versions)
	if (!app_pcre_match(R"(/^smartctl (?:version )?(([0-9][^ \t\n\r]+)(?: [0-9 r:-]+)?)/mi)", s, &version_full, &version_only)) {
		debug_out_error("app", DBG_FUNC_MSG << "No smartctl version information found in supplied string.\n");
		return false;
	}

	hz::string_trim(version_only);
	hz::string_trim(version_full);

	return true;
}



std::optional<double> SmartctlVersionParser::get_numeric_version(const std::string& version_only)
{
	double numeric_version = 0;
	if (!hz::string_is_numeric_nolocale<double>(version_only, numeric_version, false)) {
		return std::nullopt;
	}
	return numeric_version;
}



bool SmartctlVersionParser::check_parsed_version(SmartctlOutputParserType parser_type, const std::string& version_only)
{
	if (auto numeric_version = get_numeric_version(version_only); numeric_version.has_value()) {
		switch(parser_type) {
			case SmartctlOutputParserType::Auto:
				return numeric_version.value() >= minimum_req_text_version || numeric_version.value() >= minimum_req_json_version;
			case SmartctlOutputParserType::Json:
				return numeric_version.value() >= minimum_req_json_version;
			case SmartctlOutputParserType::Text:
				return numeric_version.value() >= minimum_req_text_version;
		}
	}
	return false;
}



std::optional<SmartctlOutputParserType> SmartctlVersionParser::detect_supported_parser_type(const std::string& version_only)
{
	if (check_parsed_version(SmartctlOutputParserType::Json, version_only)) {
		return SmartctlOutputParserType::Json;
	}
	if (check_parsed_version(SmartctlOutputParserType::Text, version_only)) {
		return SmartctlOutputParserType::Text;
	}
	return std::nullopt;
}





/// @}
