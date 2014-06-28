#ifndef LUT_TABLE_H
#define LUT_TABLE_H

#include <map>
#include <boost/optional/optional.hpp>
#include <QRgb>
#include <QVector>

class LUT_table
{
public:
	LUT_table();


//	std::map<double, int>& operator*();
	int get_temp_from_value(double pixel_value); //or from pixel to pixel factor
	void add_data_from_profile(int temperature, double value);


	std::map<int, double> data_from_profile;
	std::map<double, int> actual_LUT;

	//TODO refactor
	static QVector<QRgb> false_color_qcolortable;

private:
	boost::optional<int> temperature_rounded;


	//TODO refactor
	void fill_the_false_color_qcolortable();
};

#endif // LUT_TABLE_H
