#ifndef _CONFIG_H_
#define _CONFIG_H_

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <string>
#include <exception>      // std::exception, std::terminate

#include "Logger.h"

class Config {
private:
	boost::property_tree::ptree m_cfg;
	
public:
	char * m_screenshot_address;
	char * m_window_name;
	std::string m_image_path;
	std::string m_image_file_suffix;
	unsigned int m_frame_num;
	int m_window_width;
	int m_window_height;
	int m_cpu_image_id;
	int m_cpu_wave_id;
	int m_cpu_background_id;

	Config(const std::string & cfg_path){
		try{
			boost::property_tree::ini_parser::read_ini(cfg_path,m_cfg);
		} catch(const boost::property_tree::ptree_error &e) {
			logInfo("Read ["<<cfg_path<<"] failed. Error info: "<<e.what());
			std::terminate();
		}

		std::string m_screenshot_address_str, m_window_name_str;
		m_screenshot_address_str = m_cfg.get<std::string>("Section1.SCREENSHOT_ADDRESS");
		m_window_name_str        = m_cfg.get<std::string>("Section1.WINDOW_NAME");

		m_screenshot_address = strdup(m_screenshot_address_str.c_str());

		//For Chinese System
		if(m_window_name_str != "Windows Task Manager"){
			m_window_name = "Windows 任务管理器";
		} else {
			m_window_name = strdup(m_window_name_str.c_str());
		}

		m_image_path         = m_cfg.get<std::string>("Section1.IMAGE_PATH");
		m_image_file_suffix  = m_cfg.get<std::string>("Section1.IMAGE_FILE_SUFFIX");
		m_frame_num          = m_cfg.get<unsigned int>("Section1.FRAME_NUM");
		m_window_width       = m_cfg.get<int>("Section1.WINDOW_WIDTH");
		m_window_height      = m_cfg.get<int>("Section1.WINDOW_HEIGHT");
		m_cpu_image_id       = m_cfg.get<int>("Section1.CPU_IMAGE_ID");
		m_cpu_wave_id        = m_cfg.get<int>("Section1.CPU_WAVE_ID");
		m_cpu_background_id  = m_cfg.get<int>("Section1.CPU_BACKGROUND_ID");

		logInfo("Loading config file        ["<<cfg_path<<"]");
		logInfo("Loading SCREENSHOT_ADDRESS ["<<m_screenshot_address<<"]");
		logInfo("Loading WINDOW_NAME        ["<<m_window_name<<"]");
		logInfo("Loading IMAGE_PATH         ["<<m_image_path<<"]");
		logInfo("Loading IMAGE_FILE_SUFFIX  ["<<m_image_file_suffix<<"]");
		logInfo("Loading FRAME_NUM          ["<<m_frame_num<<"]");
		logInfo("Loading WINDOW_WIDTH       ["<<m_window_width<<"]");
		logInfo("Loading WINDOW_HEIGHT      ["<<m_window_height<<"]");
		logInfo("Loading CPU_IMAGE_ID       ["<<m_cpu_image_id<<"]");
		logInfo("Loading CPU_WAVE_ID        ["<<m_cpu_wave_id<<"]");
		logInfo("Loading CPU_BACKGROUND_ID  ["<<m_cpu_background_id<<"]");
	}
};

#endif // _CONFIG_H_
