#include "lut_table.h"
#include "QColor"
#include <iostream>
#include "log.h"
using namespace std;

QVector<QRgb> LUT_table::false_color_qcolortable;

LUT_table::LUT_table()
{
	fill_the_false_color_qcolortable();
}

int LUT_table::get_temp_from_value(double pixel_value)
{
	temperature_rounded = boost::none;

	std::map <double, int>::iterator it = actual_LUT.find(pixel_value);

	if (it != actual_LUT.end())
	{
		//log_debug here for check
		return it->second;
	}

	std::map <int, double>::iterator it_prev;
	std::map <int, double>::iterator it_current;
	boost::optional <int> lower_bound_temp;
	boost::optional <int> upper_bound_temp;
	boost::optional <double> lower_bound_value;
	boost::optional <double> upper_bound_value;

	for ( it_prev = data_from_profile.begin() , (it_current = data_from_profile.begin())++;
		  it_current != data_from_profile.end();
		  it_prev ++, it_current++)
	{
		if (  it_prev->second <= pixel_value && pixel_value <=it_current->second)
		{
			upper_bound_temp = it_current->first;
			if (!lower_bound_temp )
			{
				lower_bound_temp = it_prev->first;
				lower_bound_value = it_prev->second;
				upper_bound_value = it_current->second;
			}
			else
			{
				if ( it_prev->second < lower_bound_value)
				{
					lower_bound_value = it_prev->second;
				}
				if ( it_current->second > upper_bound_value )
				{
					upper_bound_value = it_current->second;
				}
			}
		}
		else if ( it_prev->second >= pixel_value && pixel_value >=it_current->second)
		{
			upper_bound_temp = it_current->first;
			if (!lower_bound_temp )
			{
				lower_bound_temp = it_prev->first;
				lower_bound_value = it_current->second;
				upper_bound_value = it_prev->second;
			}
			else
			{
				if ( it_prev->second > upper_bound_value)
				{
					upper_bound_value = it_prev->second;
				}
				if ( it_current->second < lower_bound_value )
				{
					lower_bound_value = it_current->second;
				}
			}
		}
	}

	if (!temperature_rounded)
	{
		std::map<int, double>::iterator it = data_from_profile.begin();
		if ( pixel_value < it->second )
		{
//			log_error("1");
			temperature_rounded = it->first;
			actual_LUT[pixel_value] = temperature_rounded.get();
			return temperature_rounded.get(); //rounding
		}

		std::map<int, double>::reverse_iterator r_it = data_from_profile.rbegin();
		if ( pixel_value > r_it->second )
		{
//			log_error("2");
			temperature_rounded = r_it->first;
			actual_LUT[pixel_value] = temperature_rounded.get();
			return temperature_rounded.get(); //rounding
		}
	}

	double factor_of_distance_from_lower_bound = (pixel_value - lower_bound_value.get()) / (upper_bound_value.get() - lower_bound_value.get());

	double temperature_floating_number = lower_bound_temp.get() + factor_of_distance_from_lower_bound*(upper_bound_temp.get() - lower_bound_temp.get());
	temperature_rounded = (temperature_floating_number+0.5);

	actual_LUT[pixel_value] = temperature_rounded.get();
	return temperature_rounded.get(); //rounding
}

void LUT_table::add_data_from_profile(int temperature, double value)
{
	data_from_profile[temperature] = value;
}

//TODO: REFACTOR!!!
void LUT_table::fill_the_false_color_qcolortable()
{
	uchar lut[256][3];
	lut[0][0]=0; lut[0][1]=0; lut[0][2]=0;
	lut[1][0]=0; lut[1][1]=0; lut[1][2]=7;
	lut[2][0]=0; lut[2][1]=0; lut[2][2]=15;
	lut[3][0]=0; lut[3][1]=0; lut[3][2]=22;
	lut[4][0]=0; lut[4][1]=0; lut[4][2]=30;
	lut[5][0]=0; lut[5][1]=0; lut[5][2]=38;
	lut[6][0]=0; lut[6][1]=0; lut[6][2]=45;
	lut[7][0]=0; lut[7][1]=0; lut[7][2]=53;
	lut[8][0]=0; lut[8][1]=0; lut[8][2]=61;
	lut[9][0]=0; lut[9][1]=0; lut[9][2]=65;
	lut[10][0]=0; lut[10][1]=0; lut[10][2]=69;
	lut[11][0]=0; lut[11][1]=0; lut[11][2]=74;
	lut[12][0]=0; lut[12][1]=0; lut[12][2]=78;
	lut[13][0]=0; lut[13][1]=0; lut[13][2]=82;
	lut[14][0]=0; lut[14][1]=0; lut[14][2]=87;
	lut[15][0]=0; lut[15][1]=0; lut[15][2]=91;
	lut[16][0]=1; lut[16][1]=0; lut[16][2]=96;
	lut[17][0]=4; lut[17][1]=0; lut[17][2]=100;
	lut[18][0]=7; lut[18][1]=0; lut[18][2]=104;
	lut[19][0]=10; lut[19][1]=0; lut[19][2]=108;
	lut[20][0]=13; lut[20][1]=0; lut[20][2]=113;
	lut[21][0]=16; lut[21][1]=0; lut[21][2]=117;
	lut[22][0]=19; lut[22][1]=0; lut[22][2]=121;
	lut[23][0]=22; lut[23][1]=0; lut[23][2]=125;
	lut[24][0]=25; lut[24][1]=0; lut[24][2]=130;
	lut[25][0]=28; lut[25][1]=0; lut[25][2]=134;
	lut[26][0]=31; lut[26][1]=0; lut[26][2]=138;
	lut[27][0]=34; lut[27][1]=0; lut[27][2]=143;
	lut[28][0]=37; lut[28][1]=0; lut[28][2]=147;
	lut[29][0]=40; lut[29][1]=0; lut[29][2]=151;
	lut[30][0]=43; lut[30][1]=0; lut[30][2]=156;
	lut[31][0]=46; lut[31][1]=0; lut[31][2]=160;
	lut[32][0]=49; lut[32][1]=0; lut[32][2]=165;
	lut[33][0]=52; lut[33][1]=0; lut[33][2]=168;
	lut[34][0]=55; lut[34][1]=0; lut[34][2]=171;
	lut[35][0]=58; lut[35][1]=0; lut[35][2]=175;
	lut[36][0]=61; lut[36][1]=0; lut[36][2]=178;
	lut[37][0]=64; lut[37][1]=0; lut[37][2]=181;
	lut[38][0]=67; lut[38][1]=0; lut[38][2]=185;
	lut[39][0]=70; lut[39][1]=0; lut[39][2]=188;
	lut[40][0]=73; lut[40][1]=0; lut[40][2]=192;
	lut[41][0]=76; lut[41][1]=0; lut[41][2]=195;
	lut[42][0]=79; lut[42][1]=0; lut[42][2]=199;
	lut[43][0]=82; lut[43][1]=0; lut[43][2]=202;
	lut[44][0]=85; lut[44][1]=0; lut[44][2]=206;
	lut[45][0]=88; lut[45][1]=0; lut[45][2]=209;
	lut[46][0]=91; lut[46][1]=0; lut[46][2]=213;
	lut[47][0]=94; lut[47][1]=0; lut[47][2]=216;
	lut[48][0]=98; lut[48][1]=0; lut[48][2]=220;
	lut[49][0]=101; lut[49][1]=0; lut[49][2]=220;
	lut[50][0]=104; lut[50][1]=0; lut[50][2]=221;
	lut[51][0]=107; lut[51][1]=0; lut[51][2]=222;
	lut[52][0]=110; lut[52][1]=0; lut[52][2]=223;
	lut[53][0]=113; lut[53][1]=0; lut[53][2]=224;
	lut[54][0]=116; lut[54][1]=0; lut[54][2]=225;
	lut[55][0]=119; lut[55][1]=0; lut[55][2]=226;
	lut[56][0]=122; lut[56][1]=0; lut[56][2]=227;
	lut[57][0]=125; lut[57][1]=0; lut[57][2]=224;
	lut[58][0]=128; lut[58][1]=0; lut[58][2]=222;
	lut[59][0]=131; lut[59][1]=0; lut[59][2]=220;
	lut[60][0]=134; lut[60][1]=0; lut[60][2]=218;
	lut[61][0]=137; lut[61][1]=0; lut[61][2]=216;
	lut[62][0]=140; lut[62][1]=0; lut[62][2]=214;
	lut[63][0]=143; lut[63][1]=0; lut[63][2]=212;
	lut[64][0]=146; lut[64][1]=0; lut[64][2]=210;
	lut[65][0]=148; lut[65][1]=0; lut[65][2]=206;
	lut[66][0]=150; lut[66][1]=0; lut[66][2]=202;
	lut[67][0]=152; lut[67][1]=0; lut[67][2]=199;
	lut[68][0]=154; lut[68][1]=0; lut[68][2]=195;
	lut[69][0]=156; lut[69][1]=0; lut[69][2]=191;
	lut[70][0]=158; lut[70][1]=0; lut[70][2]=188;
	lut[71][0]=160; lut[71][1]=0; lut[71][2]=184;
	lut[72][0]=162; lut[72][1]=0; lut[72][2]=181;
	lut[73][0]=163; lut[73][1]=0; lut[73][2]=177;
	lut[74][0]=164; lut[74][1]=0; lut[74][2]=173;
	lut[75][0]=166; lut[75][1]=0; lut[75][2]=169;
	lut[76][0]=167; lut[76][1]=0; lut[76][2]=166;
	lut[77][0]=168; lut[77][1]=0; lut[77][2]=162;
	lut[78][0]=170; lut[78][1]=0; lut[78][2]=158;
	lut[79][0]=171; lut[79][1]=0; lut[79][2]=154;
	lut[80][0]=173; lut[80][1]=0; lut[80][2]=151;
	lut[81][0]=174; lut[81][1]=0; lut[81][2]=147;
	lut[82][0]=175; lut[82][1]=0; lut[82][2]=143;
	lut[83][0]=177; lut[83][1]=0; lut[83][2]=140;
	lut[84][0]=178; lut[84][1]=0; lut[84][2]=136;
	lut[85][0]=179; lut[85][1]=0; lut[85][2]=132;
	lut[86][0]=181; lut[86][1]=0; lut[86][2]=129;
	lut[87][0]=182; lut[87][1]=0; lut[87][2]=125;
	lut[88][0]=184; lut[88][1]=0; lut[88][2]=122;
	lut[89][0]=185; lut[89][1]=0; lut[89][2]=118;
	lut[90][0]=186; lut[90][1]=0; lut[90][2]=114;
	lut[91][0]=188; lut[91][1]=0; lut[91][2]=111;
	lut[92][0]=189; lut[92][1]=0; lut[92][2]=107;
	lut[93][0]=190; lut[93][1]=0; lut[93][2]=103;
	lut[94][0]=192; lut[94][1]=0; lut[94][2]=100;
	lut[95][0]=193; lut[95][1]=0; lut[95][2]=96;
	lut[96][0]=195; lut[96][1]=0; lut[96][2]=93;
	lut[97][0]=196; lut[97][1]=1; lut[97][2]=89;
	lut[98][0]=198; lut[98][1]=3; lut[98][2]=85;
	lut[99][0]=199; lut[99][1]=5; lut[99][2]=82;
	lut[100][0]=201; lut[100][1]=7; lut[100][2]=78;
	lut[101][0]=202; lut[101][1]=8; lut[101][2]=74;
	lut[102][0]=204; lut[102][1]=10; lut[102][2]=71;
	lut[103][0]=205; lut[103][1]=12; lut[103][2]=67;
	lut[104][0]=207; lut[104][1]=14; lut[104][2]=64;
	lut[105][0]=208; lut[105][1]=16; lut[105][2]=60;
	lut[106][0]=209; lut[106][1]=19; lut[106][2]=56;
	lut[107][0]=210; lut[107][1]=21; lut[107][2]=53;
	lut[108][0]=212; lut[108][1]=24; lut[108][2]=49;
	lut[109][0]=213; lut[109][1]=27; lut[109][2]=45;
	lut[110][0]=214; lut[110][1]=29; lut[110][2]=42;
	lut[111][0]=215; lut[111][1]=32; lut[111][2]=38;
	lut[112][0]=217; lut[112][1]=35; lut[112][2]=35;
	lut[113][0]=218; lut[113][1]=37; lut[113][2]=31;
	lut[114][0]=220; lut[114][1]=40; lut[114][2]=27;
	lut[115][0]=221; lut[115][1]=43; lut[115][2]=23;
	lut[116][0]=223; lut[116][1]=46; lut[116][2]=20;
	lut[117][0]=224; lut[117][1]=48; lut[117][2]=16;
	lut[118][0]=226; lut[118][1]=51; lut[118][2]=12;
	lut[119][0]=227; lut[119][1]=54; lut[119][2]=8;
	lut[120][0]=229; lut[120][1]=57; lut[120][2]=5;
	lut[121][0]=230; lut[121][1]=59; lut[121][2]=4;
	lut[122][0]=231; lut[122][1]=62; lut[122][2]=3;
	lut[123][0]=233; lut[123][1]=65; lut[123][2]=3;
	lut[124][0]=234; lut[124][1]=68; lut[124][2]=2;
	lut[125][0]=235; lut[125][1]=70; lut[125][2]=1;
	lut[126][0]=237; lut[126][1]=73; lut[126][2]=1;
	lut[127][0]=238; lut[127][1]=76; lut[127][2]=0;
	lut[128][0]=240; lut[128][1]=79; lut[128][2]=0;
	lut[129][0]=241; lut[129][1]=81; lut[129][2]=0;
	lut[130][0]=243; lut[130][1]=84; lut[130][2]=0;
	lut[131][0]=244; lut[131][1]=87; lut[131][2]=0;
	lut[132][0]=246; lut[132][1]=90; lut[132][2]=0;
	lut[133][0]=247; lut[133][1]=92; lut[133][2]=0;
	lut[134][0]=249; lut[134][1]=95; lut[134][2]=0;
	lut[135][0]=250; lut[135][1]=98; lut[135][2]=0;
	lut[136][0]=252; lut[136][1]=101; lut[136][2]=0;
	lut[137][0]=252; lut[137][1]=103; lut[137][2]=0;
	lut[138][0]=252; lut[138][1]=105; lut[138][2]=0;
	lut[139][0]=253; lut[139][1]=107; lut[139][2]=0;
	lut[140][0]=253; lut[140][1]=109; lut[140][2]=0;
	lut[141][0]=253; lut[141][1]=111; lut[141][2]=0;
	lut[142][0]=254; lut[142][1]=113; lut[142][2]=0;
	lut[143][0]=254; lut[143][1]=115; lut[143][2]=0;
	lut[144][0]=255; lut[144][1]=117; lut[144][2]=0;
	lut[145][0]=255; lut[145][1]=119; lut[145][2]=0;
	lut[146][0]=255; lut[146][1]=121; lut[146][2]=0;
	lut[147][0]=255; lut[147][1]=123; lut[147][2]=0;
	lut[148][0]=255; lut[148][1]=125; lut[148][2]=0;
	lut[149][0]=255; lut[149][1]=127; lut[149][2]=0;
	lut[150][0]=255; lut[150][1]=129; lut[150][2]=0;
	lut[151][0]=255; lut[151][1]=131; lut[151][2]=0;
	lut[152][0]=255; lut[152][1]=133; lut[152][2]=0;
	lut[153][0]=255; lut[153][1]=134; lut[153][2]=0;
	lut[154][0]=255; lut[154][1]=136; lut[154][2]=0;
	lut[155][0]=255; lut[155][1]=138; lut[155][2]=0;
	lut[156][0]=255; lut[156][1]=140; lut[156][2]=0;
	lut[157][0]=255; lut[157][1]=141; lut[157][2]=0;
	lut[158][0]=255; lut[158][1]=143; lut[158][2]=0;
	lut[159][0]=255; lut[159][1]=145; lut[159][2]=0;
	lut[160][0]=255; lut[160][1]=147; lut[160][2]=0;
	lut[161][0]=255; lut[161][1]=148; lut[161][2]=0;
	lut[162][0]=255; lut[162][1]=150; lut[162][2]=0;
	lut[163][0]=255; lut[163][1]=152; lut[163][2]=0;
	lut[164][0]=255; lut[164][1]=154; lut[164][2]=0;
	lut[165][0]=255; lut[165][1]=155; lut[165][2]=0;
	lut[166][0]=255; lut[166][1]=157; lut[166][2]=0;
	lut[167][0]=255; lut[167][1]=159; lut[167][2]=0;
	lut[168][0]=255; lut[168][1]=161; lut[168][2]=0;
	lut[169][0]=255; lut[169][1]=162; lut[169][2]=0;
	lut[170][0]=255; lut[170][1]=164; lut[170][2]=0;
	lut[171][0]=255; lut[171][1]=166; lut[171][2]=0;
	lut[172][0]=255; lut[172][1]=168; lut[172][2]=0;
	lut[173][0]=255; lut[173][1]=169; lut[173][2]=0;
	lut[174][0]=255; lut[174][1]=171; lut[174][2]=0;
	lut[175][0]=255; lut[175][1]=173; lut[175][2]=0;
	lut[176][0]=255; lut[176][1]=175; lut[176][2]=0;
	lut[177][0]=255; lut[177][1]=176; lut[177][2]=0;
	lut[178][0]=255; lut[178][1]=178; lut[178][2]=0;
	lut[179][0]=255; lut[179][1]=180; lut[179][2]=0;
	lut[180][0]=255; lut[180][1]=182; lut[180][2]=0;
	lut[181][0]=255; lut[181][1]=184; lut[181][2]=0;
	lut[182][0]=255; lut[182][1]=186; lut[182][2]=0;
	lut[183][0]=255; lut[183][1]=188; lut[183][2]=0;
	lut[184][0]=255; lut[184][1]=190; lut[184][2]=0;
	lut[185][0]=255; lut[185][1]=191; lut[185][2]=0;
	lut[186][0]=255; lut[186][1]=193; lut[186][2]=0;
	lut[187][0]=255; lut[187][1]=195; lut[187][2]=0;
	lut[188][0]=255; lut[188][1]=197; lut[188][2]=0;
	lut[189][0]=255; lut[189][1]=199; lut[189][2]=0;
	lut[190][0]=255; lut[190][1]=201; lut[190][2]=0;
	lut[191][0]=255; lut[191][1]=203; lut[191][2]=0;
	lut[192][0]=255; lut[192][1]=205; lut[192][2]=0;
	lut[193][0]=255; lut[193][1]=206; lut[193][2]=0;
	lut[194][0]=255; lut[194][1]=208; lut[194][2]=0;
	lut[195][0]=255; lut[195][1]=210; lut[195][2]=0;
	lut[196][0]=255; lut[196][1]=212; lut[196][2]=0;
	lut[197][0]=255; lut[197][1]=213; lut[197][2]=0;
	lut[198][0]=255; lut[198][1]=215; lut[198][2]=0;
	lut[199][0]=255; lut[199][1]=217; lut[199][2]=0;
	lut[200][0]=255; lut[200][1]=219; lut[200][2]=0;
	lut[201][0]=255; lut[201][1]=220; lut[201][2]=0;
	lut[202][0]=255; lut[202][1]=222; lut[202][2]=0;
	lut[203][0]=255; lut[203][1]=224; lut[203][2]=0;
	lut[204][0]=255; lut[204][1]=226; lut[204][2]=0;
	lut[205][0]=255; lut[205][1]=228; lut[205][2]=0;
	lut[206][0]=255; lut[206][1]=230; lut[206][2]=0;
	lut[207][0]=255; lut[207][1]=232; lut[207][2]=0;
	lut[208][0]=255; lut[208][1]=234; lut[208][2]=0;
	lut[209][0]=255; lut[209][1]=235; lut[209][2]=4;
	lut[210][0]=255; lut[210][1]=237; lut[210][2]=8;
	lut[211][0]=255; lut[211][1]=239; lut[211][2]=13;
	lut[212][0]=255; lut[212][1]=241; lut[212][2]=17;
	lut[213][0]=255; lut[213][1]=242; lut[213][2]=21;
	lut[214][0]=255; lut[214][1]=244; lut[214][2]=26;
	lut[215][0]=255; lut[215][1]=246; lut[215][2]=30;
	lut[216][0]=255; lut[216][1]=248; lut[216][2]=35;
	lut[217][0]=255; lut[217][1]=248; lut[217][2]=42;
	lut[218][0]=255; lut[218][1]=249; lut[218][2]=50;
	lut[219][0]=255; lut[219][1]=250; lut[219][2]=58;
	lut[220][0]=255; lut[220][1]=251; lut[220][2]=66;
	lut[221][0]=255; lut[221][1]=252; lut[221][2]=74;
	lut[222][0]=255; lut[222][1]=253; lut[222][2]=82;
	lut[223][0]=255; lut[223][1]=254; lut[223][2]=90;
	lut[224][0]=255; lut[224][1]=255; lut[224][2]=98;
	lut[225][0]=255; lut[225][1]=255; lut[225][2]=105;
	lut[226][0]=255; lut[226][1]=255; lut[226][2]=113;
	lut[227][0]=255; lut[227][1]=255; lut[227][2]=121;
	lut[228][0]=255; lut[228][1]=255; lut[228][2]=129;
	lut[229][0]=255; lut[229][1]=255; lut[229][2]=136;
	lut[230][0]=255; lut[230][1]=255; lut[230][2]=144;
	lut[231][0]=255; lut[231][1]=255; lut[231][2]=152;
	lut[232][0]=255; lut[232][1]=255; lut[232][2]=160;
	lut[233][0]=255; lut[233][1]=255; lut[233][2]=167;
	lut[234][0]=255; lut[234][1]=255; lut[234][2]=175;
	lut[235][0]=255; lut[235][1]=255; lut[235][2]=183;
	lut[236][0]=255; lut[236][1]=255; lut[236][2]=191;
	lut[237][0]=255; lut[237][1]=255; lut[237][2]=199;
	lut[238][0]=255; lut[238][1]=255; lut[238][2]=207;
	lut[239][0]=255; lut[239][1]=255; lut[239][2]=215;
	lut[240][0]=255; lut[240][1]=255; lut[240][2]=223;
	lut[241][0]=255; lut[241][1]=255; lut[241][2]=227;
	lut[242][0]=255; lut[242][1]=255; lut[242][2]=231;
	lut[243][0]=255; lut[243][1]=255; lut[243][2]=235;
	lut[244][0]=255; lut[244][1]=255; lut[244][2]=239;
	lut[245][0]=255; lut[245][1]=255; lut[245][2]=243;
	lut[246][0]=255; lut[246][1]=255; lut[246][2]=247;
	lut[247][0]=255; lut[247][1]=255; lut[247][2]=251;
	lut[248][0]=255; lut[248][1]=255; lut[248][2]=255;
	lut[249][0]=255; lut[249][1]=255; lut[249][2]=255;
	lut[250][0]=255; lut[250][1]=255; lut[250][2]=255;
	lut[251][0]=255; lut[251][1]=255; lut[251][2]=255;
	lut[252][0]=255; lut[252][1]=255; lut[252][2]=255;
	lut[253][0]=255; lut[253][1]=255; lut[253][2]=255;
	lut[254][0]=255; lut[254][1]=255; lut[254][2]=255;
	lut[255][0]=255; lut[255][1]=255; lut[255][2]=255;
//	false_color_qcolortable.resize(256);

	for (int i = 0 ; i < 256 ; i++)
	{
		false_color_qcolortable.append( qRgb(lut[i][0], lut[i][1], lut[i][2]) );
	}


}
