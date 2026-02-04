#ifndef ALDEVICELIST_H
#define ALDEVICELIST_H

#pragma warning(disable: 4786)  //disable warning "identifier was truncated to '255' characters in the browser information"
#include <vector>
#include <string>
#include "LoadOAL.h"

//using namespace std;

typedef struct
{
	std::string			strDeviceName;
	int				iMajorVersion;
	int				iMinorVersion;
	unsigned int	uiSourceCount;//支持的最大source个数
	std::vector<std::string>	vstrExtensions;
	bool			bSelected;
} ALDEVICEINFO, *LPALDEVICEINFO;

extern void clear(ALDEVICEINFO& deviceInfo);

class ALDeviceList
{
private:
	//类似opengl的glad,包含所有导出的函数指针
	OPENALFNTABLE	ALFunction;
	std::vector<ALDEVICEINFO> vDeviceInfo;
	int defaultDeviceIndex;
	int filterIndex;

public:
	ALDeviceList ();
	~ALDeviceList ();
	int GetNumDevices();
	char *GetDeviceName(int index);
	void GetDeviceVersion(int index, int *major, int *minor);
	unsigned int GetMaxNumSources(int index);
	bool IsExtensionSupported(int index, char *szExtName);
	int GetDefaultDevice();
	void FilterDevicesMinVer(int major, int minor);
	void FilterDevicesMaxVer(int major, int minor);
	void FilterDevicesExtension(char *szExtName);
	void ResetFilters();
	int GetFirstFilteredDevice();
	int GetNextFilteredDevice();

	ALDEVICEINFO GetDeviceInfo(int index) const;

private:
	unsigned int GetMaxNumSources();
};

#endif // ALDEVICELIST_H
