#pragma once

#pragma pack(1)

#define WM_UPDATE_DETECTOR_SHOW_MSG (WM_USER + 100)

class Detector;
struct tagDetectorInfo {
	int id;

	Detector* detector;
	std::string szCfgPath;
	std::string szWeightPath;
	std::vector<std::string> vecName;

	tagDetectorInfo()
	{
		id = 0;
		detector = NULL;
		szCfgPath = "";
		szWeightPath = "";
	}
};

#pragma pack()