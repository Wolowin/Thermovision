#ifndef GAIN_EXPOSURE_TEMP_MAP_H
#define GAIN_EXPOSURE_TEMP_MAP_H

#include <map>
#include <Qstring>
#include <vector>

typedef std::map <QString, std::vector<QString> > exposure_temp_map;
typedef exposure_temp_map::iterator ex_te_map_iterator;
typedef std::map < QString, exposure_temp_map > gain_exposure_temp_map;
typedef gain_exposure_temp_map::iterator ga_ex_te_map_iterator;

#endif // GAIN_EXPOSURE_TEMP_MAP_H
