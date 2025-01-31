/******************************************************************************
License: GNU General Public License v3.0 only
Copyright:
	(C) 2008 - 2021 Alexander Shaduri <ashaduri@gmail.com>
******************************************************************************/
/// \file
/// \author Alexander Shaduri
/// \ingroup applib
/// \weakgroup applib
/// @{

#ifndef SMARTCTL_ATA_TEXT_PARSER_H
#define SMARTCTL_ATA_TEXT_PARSER_H

#include <string>
#include <vector>

#include "ata_storage_property.h"



/// Smartctl (S)ATA text output parser.
/// Note: ALL parse_* functions (except parse_full() and parse_version())
/// expect data in unix-newline format!
class SmartctlAtaTextParser {
	public:

		/// Parse full "smartctl -x" output
		bool parse_full(const std::string& full, AtaStorageAttribute::DiskType disk_type);


	private:

		/// Parse the section part (with "=== .... ===" header) - info or data sections.
		bool parse_section(const std::string& header, const std::string& body);


		/// Parse the info section (without "===" header).
		/// This includes --info and --get=all.
		bool parse_section_info(const std::string& body);

		/// Parse a component (one line) of the info section
		bool parse_section_info_property(AtaStorageProperty& p);


		/// Parse the Data section (without "===" header)
		bool parse_section_data(const std::string& body);

		/// Parse subsections of Data section
		bool parse_section_data_subsection_health(const std::string& sub);
		bool parse_section_data_subsection_capabilities(const std::string& sub);
		bool parse_section_data_subsection_attributes(const std::string& sub);
		bool parse_section_data_subsection_directory_log(const std::string& sub);
		bool parse_section_data_subsection_error_log(const std::string& sub);
		bool parse_section_data_subsection_selftest_log(const std::string& sub);
		bool parse_section_data_subsection_selective_selftest_log(const std::string& sub);
		bool parse_section_data_subsection_scttemp_log(const std::string& sub);
		bool parse_section_data_subsection_scterc_log(const std::string& sub);
		bool parse_section_data_subsection_devstat(const std::string& sub);
		bool parse_section_data_subsection_sataphy(const std::string& sub);

		/// Check the capabilities for internal properties we can use.
		bool parse_section_data_internal_capabilities(AtaStorageProperty& cap_prop);


		/// Clear parsed data
		void clear();


	public:

		/// Get "full" data, as passed to parse_full().
		[[nodiscard]] std::string get_data_full() const;

		/// Get parse error message. Call this only if parsing doesn't succeed,
		/// to get a friendly error message.
		[[nodiscard]] std::string get_error_msg() const;


		/// Get parse result properties
		[[nodiscard]] const std::vector<AtaStorageProperty>& get_properties() const;



	private:


		/// Add a property into property list, look up and set its description
		void add_property(AtaStorageProperty p);


		/// Set "full" data ("smartctl -x" output)
		void set_data_full(const std::string& s);


		/// Set "info" section data ("smartctl -i" output, or the first part of "smartctl -x" output)
		void set_data_section_info(const std::string& s);


		/// Parse "data" section data (the second part of "smartctl -x" output).
		void set_data_section_data(const std::string& s);


		/// Set error message
		void set_error_msg(const std::string& s);



		std::vector<AtaStorageProperty> properties_;  ///< Parsed data properties

		std::string data_full_;  ///< full data, filled by parse_full()
		std::string data_section_info_;  ///< "info" section data, filled by parse_section_info()
		std::string data_section_data_;  ///< "data" section data, filled by parse_section_data()

		std::string error_msg_;  ///< This will be filled with some displayable message on error

		AtaStorageAttribute::DiskType disk_type_ = AtaStorageAttribute::DiskType::Any;  ///< Disk type (HDD, SSD)

};






#endif

/// @}
