// ********************************************************************************************************
//	Enecsys Gen 1 Micro Inverter Firmware 
//
//	MIT License
//
//  Copyright 2016-2024 Daniel G. Ambrose
//
//	Daniel G. Ambrose
//	12614 Bradford Woods Dr
//	Sunset Hills, Missouri, USA 63127
//	Email: dan27125@gmail.com
//	Facetime: dambrose@amcominc.net
//	Mobile:   3145044858
//	Whatsapp: 3145044858
//
// 	Permission is hereby granted, free of charge, to any person obtaining a copy of this software and 
//	associated documentation files (the "Software"), to deal in the Software without restriction, 
//	including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//	and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
//	subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in all copies or substantial 
//	portions of the Software.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
//	SOFTWARE.
//
// ********************************************************************************************************


#include "APP_Inverter.h"


#if defined(Uses_Inverter) 

#if defined(SMI240_60)

//  2/15/24 calibrated with HV 480v, 

	Uint16 pwm_watt_table[WATT_TABLE_MAX]=
	{
		5,		// 1 	// test shows it wont run below 5
		5,		// 2 	// test shows it wont go below 4w
		5,		// 3 	// test shows it wont go below 4w
		5,		// 4    
		6,		// 5 Watts
		7,		// 6 Watts
		8,		// 7 Watts
		10,		// 8 Watts
		13,		// 9 Watts
		20,		// 10 Watts 
		24,		// 11 Watts
		28,		// 12 Watts
		30,		// 13 Watts
		36,		// 14 Watts
		41,		// 15 Watts
		45,		// 16 Watts
		50,		// 17 Watts
		54,		// 18 Watts
		58,		// 19 Watts
		61,		// 20 Watts
		66,		// 21 Watts
		72,		// 22 Watts
		77,		// 23 Watts
		80,		// 24 Watts
        
		85,		// 25 Watts
		88,		// 26 Watts
		93,		// 27 Watts
		97,		// 28 Watts
		102,		// 29 Watts
		107,		// 30 Watts
		111,		// 31 Watts
		115,		// 32 Watts
		119,		// 33 Watts
		123,		// 34 Watts
		126,		// 35 Watts
		129,		// 36 Watts
		132,		// 37 Watts
		136,		// 38 Watts
		141,		// 39 Watts
		145,		// 40 Watts
		148,		// 41 Watts
		153,		// 42 Watts
		157,		// 43 Watts
		163,		// 44 Watts
		168,		// 45 Watts
		173,		// 46 Watts
		178,		// 47 Watts
		181,		// 48 Watts
		184,		// 49 Watts
		188,		// 50 Watts
		192,		// 51 Watts
		196,		// 52 Watts
		199,		// 53 Watts
		203,		// 54 Watts
		207,		// 55 Watts
		211,		// 56 Watts
		215,		// 57 Watts
		218,		// 58 Watts
		222,		// 59 Watts
		225,		// 60 Watts
		229,		// 61 Watts
		233,		// 62 Watts
		236,		// 63 Watts
		241,		// 64 Watts
		244,		// 65 Watts
		248,		// 66 Watts
		252,		// 67 Watts
		256,		// 68 Watts
		260,		// 69 Watts
		263,		// 70 Watts
		267,		// 71 Watts
		271,		// 72 Watts
		274,		// 73 Watts
		278,		// 74 Watts
		282,		// 75 Watts
		286,		// 76 Watts
		290,		// 77 Watts
		293,		// 78 Watts
		297,		// 79 Watts
		301,		// 80 Watts
		305,		// 81 Watts
		309,		// 82 Watts
		312,		// 83 Watts
		316,		// 84 Watts
		320,		// 85 Watts
		324,		// 86 Watts
		328,		// 87 Watts
		331,		// 88 Watts
		334,		// 89 Watts
		339,		// 90 Watts
		342,		// 91 Watts
		346,		// 92 Watts
		349,		// 93 Watts
		353,		// 94 Watts
		357,		// 95 Watts
		361,		// 96 Watts
		365,		// 97 Watts
		369,		// 98 Watts
		373,		// 99 Watts
		377,	// 100 Watts       100/371=0.269541     
		381,	// 101 Watts
		384,	// 102 Watts
		387,	// 103 Watts
		391,	// 104 Watts
		395,		// 105 Watts
		398,		// 106 Watts
		402,		// 107 Watts
		406,		// 108 Watts
		409,		// 109 Watts
		414,		// 110 Watts
		417,		// 111 Watts
		421,		// 112 Watts
		425,		// 113 Watts
		429,		// 114 Watts
		432,		// 115 Watts
		436,		// 116 Watts
		440,		// 117 Watts
		444,		// 118 Watts
		447,		// 119 Watts
		451,		// 120 Watts
		455,		// 121 Watts
		458,		// 122 Watts
		462,		// 123 Watts
		465,		// 124 Watts
		468,		// 125 Watts
		472,		// 126 Watts
		476,		// 127 Watts
		480,		// 128 Watts
		484,		// 129 Watts
		488,		// 130 Watts
		491,		// 131 Watts
		495,		// 132 Watts
		498,		// 133 Watts
		502,		// 134 Watts
		506,		// 135 Watts
		510,		// 136 Watts    
		514,		// 137 Watts
		517,		// 138 Watts
		521,		// 139 Watts
		525,		// 140 Watts
		529,		// 141 Watts
		533,		// 142 Watts
		536,		// 143 Watts
		540,		// 144 
		543,		// 145 
		547,		// 146 
		551,		// 147 
		555,		// 148 
		558,	// 149 
		562,	// 150 
        
		566,		// 151 566% 153w 480v RIP:21,16v 
		570,		// 152 570% 154w 481v RIP:21,16v 
		573,		// 153 573% 155w 481v RIP:22,16v 
		576,		// 154 576% 156w 480v RIP:21,16v 
		580,		// 155 580% 157w 480v RIP:22,16v 
		584,		// 156 584% 158w 480v RIP:22,17v 
		591,		// 158 591% 160w 480v RIP:22,17v 
		594,		// 159 594% 161w 480v RIP:22,17v 
		598,		// 160 598% 162w 480v RIP:22,16v 
		603,		// 161 599% 162w 480v RIP:22,17v 
		607,		// 162 
		611,		// 163 611% 165w 479v RIP:22,16v 
		614,		// 164 614% 166w 480v RIP:23,16v 
		616,		// 165 616% 167w 480v RIP:23,16v 

		620,		// 166 620% 167w 480v RIP:23,17v 

		624,		// 167 624% 169w 481v RIP:23,17v 
		628,		// 168 628% 170w 481v RIP:24,18v 
		632,		// 169 632% 171w 480v RIP:23,18v 
		636,		// 170 636% 172w 480v RIP:24,17v 
		639,		// 171 639% 173w 481v RIP:23,18v 
		642,		// 172 642% 174w 480v RIP:23,18v 
		646,		// 173 646% 175w 480v RIP:24,18v 
		650,		// 174 650% 177w 480v RIP:24,18v 
		654,		// 175 654% 177w 480v RIP:24,18v 
		657,		// 176 657% 178w 480v RIP:24,18v 
		661,		// 177 661% 179w 480v RIP:24,18v 
		667,		// 178 666% 180w 482v RIP:25,19v 
		670,		// 179 669% 181w 480v RIP:25,18v 
		672,		// 180 673% 670% 182w 480v RIP:25,18v 
		676,		// 181 676% 183w 480v RIP:25,18v 
		680,		// 182 680% 184w 479v RIP:25,18v 
		684,		// 183 684% 186w 480v RIP:25,18v 
		687,		// 184 687% 186w 480v RIP:25,18v 
		691,		// 185 691% 187w 480v RIP:25,18v 
		695,		// 186 695% 188w 481v RIP:25,18v 
		698,		// 187 698% 189w 480v RIP:26,18v 
		702,		// 188 702% 191w 480v RIP:25,18v 
		706,		// 189 706% 192w 480v RIP:26,20v 
		709,		// 190 709% 193w 480v RIP:26,19v 
		712,		// 191 712% 194w 480v RIP:26,19v 
		716,		// 192 716% 195w 480v RIP:26,19v 
		720,		// 193 720% 196w 480v RIP:26,19v 
        724,		// 194 724% 197w 480v RIP:26,19v 
		728,		// 195 728% 198w 480v RIP:27,20v 
        731,		// 196  731% 199w 480v RIP:27,19v 
		735,		// 197 735% 202w 480v RIP:27,19v 
		739,		// 198 739% 203w 480v RIP:27,19v 
		742,		// 199 742% 204w 480v RIP:27,19v 
		746,		// 200 746% 205w 480v RIP:27,19v 
		750,		// 201 750% 206w 480v RIP:27,19v 
		753,		// 202 753% 206w 480v RIP:27,19v 
		757,		// 203 757% 207w 480v RIP:28,21v 
		761,		// 204 761% 209w 480v RIP:28,20v 
		764,		// 205 764% 209w 480v RIP:28,20v 
		767,		// 206 767% 210w 481v RIP:28,20v 
		771,		// 207 771% 212w 481v RIP:28,21v 
        775,		// 208 775% 213w 481v RIP:28,20v 
		778,		// 209 778% 213w 481v RIP:28,21v 
		783,		// 210 783% 215w 481v RIP:28,20v 
		791,		// 212 791% 217w 480v RIP:28,20v 
		794,		// 213 794% 218w 481v RIP:29,21v 
		797,		// 214 797% 219w 480v RIP:29,21v 
		800,		// 215 800% 220w 480v RIP:29,21v 
		804,		// 216 804% 221w 480v RIP:29,21v 
		808,		// 217 808% 222w 480v RIP:29,22v 
		811,		// 218 811% 223w 480v RIP:29,22v 
		818,		// 219 818% 225w 480v RIP:29,22v 
		823,		// 221 823% 226w 480v RIP:30,22v 
		826,		// 222 826% 226w 480v RIP:30,22v 
		829,		// 223 829% 228w 480v RIP:30,22v 
		832,		// 224 832% 229w 480v RIP:30,22v 
		836,		// 225 836% 230w 480v RIP:30,22v 
		840,		// 226 840% 231w 480v RIP:30,22v 
		844,		// 227 844% 232w 480v RIP:31,23v 
		852,		// 229 852% 234w 480v RIP:31,23v 
		856,		// 230 856% 236w 480v RIP:31,23v 
		859,		// 231 859% 237w 480v RIP:31,23v 
		863,		// 232 863% 238w 480v RIP:31,23v 
		867,		// 233 867% 239w 480v RIP:31,23v 
		871,		// 234 871% 240w 480v RIP:31,23v 
		875,		// 235 875% 241w 480v RIP:31,23v 
		879,		// 236 879% 243w 480v RIP:32,23v 
		882,		// 237 882% 243w 481v RIP:32,23v 
		884,		// 238 884% 243w 481v RIP:32,24v 
		888,		// 239 888% 244w 481v RIP:32,24v 
		892,		// 240 892% 245w 481v RIP:32,24v 
        
		895,		// 241 895% 246w 481v RIP:32,24v 
        900,		// 242 900% 248w 480v RIP:32,24v 
		903,		// 243 903% 248w 480v RIP:33,24v 
		907,		// 244 907% 250w 480v RIP:33,24v 
        911,		// 245 911% 251w 480v RIP:33,24v 
		915,		// 246 915% 252w 480v RIP:33,25v 
		918,		// 247 918% 252w 480v RIP:33,25v 
		921,		// 248 921% 253w 480v RIP:33,25v 
		925,		// 249 925% 254w 480v RIP:33,25v 
		928,		// 250 928% 255w 480v RIP:33,25v 
		935,		// 251 935% 257w 480v RIP:34,26v 
		939,		// 252 939% 257w 479v RIP:34,26v 
		947,		// 254 947% 260w 480v RIP:34,24v 
        951,    	// 255 951% 261w 480v RIP:34,24v 
		954,		// 256 954% 262w 480v RIP:34,24v 
		959,		// 257 959% 264w 480v RIP:34,24v 
		962,		// 258 962% 264w 480v RIP:35,25v 
		966,		// 259 966% 265w 480v RIP:35,25v 
		971,		// 260 971% 266w 480v RIP:34,25v 
		975,		// 261 975% 267w 480v RIP:35,25v 

		976,		// 262 976% 266w 479v RIP:35,26v 
		979,		// 263 979% 268w 481v RIP:35,26v 
		983,		// 264 983% 269w 480v RIP:35,26v 
		988,		// 265 988% 271w 480v RIP:36,26v 
		991,		// 266 991% 272w 480v RIP:35,26v 
		995,		// 267 995% 273w 480v RIP:36,27v 
		999,		// 268 999% 274w 480v RIP:36,27v 
        1003,		// 269 1003% 275w 480v RIP:36,27v 
		1006,		// 270 1006% 275w 480v RIP:36,26v 
		1010,		// 271 1010% 276w 480v RIP:36,27v 
		1015,		// 272 1015% 278w 480v RIP:37,27v 

		1019,		// 273 1019% 279w 480v RIP:36,27v 

        
		1021,		// 274 1021% 279w 480v RIP:35,26v 
		1028,		// 275 1028% 281w 480v RIP:36,26v 
		1033,		// 276 1033% 282w 480v RIP:36,26v 
		1037,		// 277 1037% 283w 480v RIP:36,27v 
		1043,		// 278 1043% 284w 480v RIP:36,27v 
		1049,		// 279 1049% 285w 480v RIP:36,27v 
		1056,		// 280 1056% 287w 480v RIP:36,27v 
		1061,		// 281 1061% 287w 480v RIP:37,28v 
		1066,		// 282 1066% 288w 480v RIP:36,26v 
		1066,		// 283 1066% 288w 480v RIP:38,28v 
		1071,		// 284 1071% 289w 480v RIP:38,29v 
		1077,		// 285 1077% 290w 480v RIP:38,29v 
		1082,		// 286 1082% 291w 480v RIP:39,29v 
		1088,		// 287 1088% 292w 481v RIP:39,28v 
		1094,		// 288 1094% 293w 481v RIP:38,28v 
		1100,		// 289 1108% 295w 862v RIP:39,28v 
		1108,		// 290 1108% 295w 862v RIP:39,28v 
		1110,		// 291 1108% 295w 481v RIP:39,28v 
		1121,		// 292 1121% 297w 480v RIP:39,28v 
		1127,		// 293 1127% 297w 480v RIP:39,29v 
		1133,		// 294 1133% 298w 480v RIP:39,29v 
		1140,		// 295 1140% 299w 480v RIP:39,29v 
		1148,		// 296 1148% 299w 480v RIP:39,29v 
		1155,		// 297 1155% 300w 480v RIP:39,29v 
		1163,		// 298 1163% 301w 480v RIP:39,29v 
		1170,		// 299 
		1181,		// 300 1181% 302w 480v RIP:40,29v 
		1190,		// 301 1190% 304w 480v RIP:39,29v 
		1198,		// 302 1198% 305w 480v RIP:39,29v 
		1206,		// 303 1206% 305w 480v RIP:39,29v 
		1214,		// 304 1214% 305w 480v RIP:39,29v 
		1224,		// 305 1224% 307w 480v RIP:38,29v 
		1236,		// 306 1236% 308w 480v RIP:39,29v 
		1244,		// 307 1244% 309w 481v RIP:38,28v 
		1260,		// 308 1260% 311w 480v RIP:38,29v 
		1270,		// 309 1266% 311w 481v RIP:39,28v 
		1280,		// 310 1276% 312w 480v RIP:39,28v 
		1290,		// 311 1290% 313w 480v RIP:38,27v 
		1316,		// 313 1316% 315w 480v RIP:38,27v 
		1330,		// 314 1330% 316w 480v RIP:38,28v 
		1344,		// 315 1344% 317w 480v RIP:38,28v 
		1356,		// 316 1356% 318w 480v RIP:38,28v 

		1368,		// 317 1368% 319w 480v RIP:38,28v 
		1378,		// 318 1378% 319w 480v RIP:38,28v 
		1394,		// 319 1414% 322w 480v RIP:38,28v 
		1414,		// 320 1414% 322w 480v RIP:38,28v 
        
		1428,		// 321  1428% 322w 480v RIP:38,28v 
		1448,		// 322 1448% 324w 480v RIP:37,27v 
		1469,		// 323 1469% 325w 480v RIP:37,28v 
		1486,		// 324 1486% 325w 480v RIP:37,28v 
		1503,		// 325 1503% 327w 480v RIP:37,28v 
		1523,		// 326 1523% 328w 480v RIP:37,27v 
		1543,		// 327 1543% 329w 479v RIP:37,28v 
		1563,		// 328 1563% 330w 480v RIP:37,27v 
		1590,		// 329 1590% 331w 480v RIP:37,27v 
		1610,		// 330 Watts
        
		1650,		// 331 1650% 333w 479v RIP:37,27v 
		1675,		// 332 1675% 334w 480v RIP:36,27v 
		1725,		// 333 Watts
		1755,		// 334 Watts
		1795,		// 335 1795% 338w 480v RIP:35,26v 
        1835,		// 336 Watts
        1857,		// 337w 1857% 339w 480v RIP:35,26v 
        1904,		// 338w 1902% 000w 480v RIP:35,25v 
		1946,		// 339w 1946% 000w 480v RIP:35,25v 
		1978,		// 340w 1978% 000w 479v RIP:36,27v 
//      0
	};
    
    
    
    
#endif

		
#endif		