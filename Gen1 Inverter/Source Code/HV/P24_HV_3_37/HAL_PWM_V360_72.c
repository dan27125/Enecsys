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
#if defined(SMI360_72)

	// DGA Feb 23, 2024 - wattage table 484a 350v
    // 20k 1209% 300,448w 484a 350v 1280m RIP:56 C:47,23 

	Uint16 pwm_watt_table[WATT_TABLE_MAX]=
	{
		7,		// 1 Watt
		7,		// 2 Watts
		7,		// 3 Watts
		8,		// 4 Watts
		9,		// 5 Watts
		12,		// 6 Watts
		13,		// 7 Watts
		15,		// 8 Watts
		17,		// 9 Watts
		21,		// 10 Watts
		23,		// 11 Watts
		29,		// 12 Watts
		33,		// 13 Watts
		37,		// 14 Watts
		41,		// 15 Watts
		44,		// 16 Watts
		49,		// 17 Watts
		53,		// 18 Watts
		57,		// 19 Watts
		60,		// 20 Watts
		65,		// 21 Watts
		69,		// 22 Watts
		72,		// 23 Watts
		75,		// 24 Watts
		78,		// 25 Watts
		81,		// 26 Watts
		84,		// 27 Watts
		86,		// 28 Watts
		89,		// 29 Watts
		91,		// 30 Watts
		94,		// 31 Watts
		96,		// 32 Watts
		99,		// 33 Watts
		101,		// 34 Watts
		104,		// 35 Watts
		106,		// 36 Watts
		109,		// 37 Watts
		111,		// 38 Watts
		114,		// 39 Watts
		117,		// 40 Watts
		119,		// 41 Watts
		121,		// 42 Watts
		124,		// 43 Watts
		126,		// 44 Watts
		129,		// 45 Watts
		131,		// 46 Watts
		134,		// 47 Watts
		136,		// 48 Watts
		138,		// 49 Watts
		141,		// 50 Watts
        
		143,		// 51 Watts
		146,		// 52 Watts
		148,		// 53 Watts
		151,		// 54 Watts
		153,		// 55 Watts
		156,		// 56 Watts
		158,		// 57 Watts
		160,		// 58 Watts
		163,		// 59 Watts
		165,		// 60 Watts
		168,		// 61 Watts
		170,		// 62 Watts
		173,		// 63 Watts
		175,		// 64 Watts
		177,		// 65 Watts
		179,		// 66 Watts
		182,		// 67 Watts
		184,		// 68 Watts
		187,		// 69 Watts
		189,		// 70 Watts
		192,		// 71 Watts
		194,		// 72 Watts
		197,		// 73 Watts
		199,		// 74 Watts
		201,		// 75 Watts
		203,		// 76 Watts
		206,		// 77 Watts
		208,		// 78 Watts
		211,		// 79 Watts
		213,		// 80 Watts
		215,		// 81 Watts
		218,		// 82 Watts
		220,		// 83 Watts
		223,		// 84 Watts
		225,		// 85 Watts
		228,		// 86 Watts
		230,		// 87 Watts
		232,		// 88 Watts
		235,		// 89 Watts
		237,		// 90 Watts
		240,		// 91 Watts
		242,		// 92 Watts
		244,		// 93 Watts
		247,		// 94 Watts
		249,		// 95 Watts
		251,		// 96 Watts
		254,		// 97 Watts
		256,		// 98 Watts
		258,		// 99 Watts
		260,	// 100 Watts
        
		263,		// 101 Watts
		265,		// 102 Watts
		267,		// 103 Watts
		270,		// 104 Watts
		273,		// 105 Watts
		274,		// 106 Watts
		277,		// 107 Watts
		279,		// 108 Watts
		282,		// 109 Watts
		284,		// 110 Watts
		287,		// 111 Watts
		289,		// 112 Watts
		291,		// 113 Watts
		294,		// 114 Watts
		297,		// 115 Watts
		299,		// 116 Watts
		301,		// 117 Watts
		304,		// 118 Watts
		306,		// 119 Watts
		308,		// 120 Watts
		311,		// 121 Watts
		313,		// 122 Watts
		315,		// 123 Watts
		318,		// 124 Watts
		320,		// 125 Watts
		322,		// 126 Watts
		325,		// 127 Watts
		327,		// 128 Watts
		329,		// 129 Watts
		331,		// 130 Watts
		334,		// 131 Watts
		336,		// 132 Watts
		339,		// 133 Watts
		341,		// 134 Watts
		343,		// 135 Watts
		345,		// 136 Watts
		347,		// 137 Watts
		350,		// 138 Watts
		352,		// 139 Watts
		355,		// 140 Watts
		357,		// 141 Watts
		359,		// 142 Watts
		361,		// 143 Watts
		363,		// 144 Watts
		366,		// 145 Watts
		368,		// 146 Watts
		370,		// 147 Watts
		372,		// 148 Watts
		374,		// 149 Watts
		376,		// 150 Watts
		379,		// 151 Watts
		381,		// 152 Watts
		383,		// 153 Watts
		385,		// 154 Watts
		388,		// 155 Watts
		390,		// 156 Watts
		393,		// 157 Watts
		395,		// 158 Watts
		397,		// 159 Watts
		399,		// 160 Watts
		401,		// 161 Watts
		404,		// 162 Watts
		406,		// 163 Watts
		408,		// 164 Watts
		410,		// 165 Watts
		412,		// 166 Watts
		414,		// 167 Watts
		416,		// 168 Watts
		419,		// 169 Watts
		421,		// 170 Watts
		423,		// 171 Watts
		425,		// 172 Watts
		428,		// 173 Watts
		430,		// 174 Watts
		432,		// 175 Watts
		434,		// 176 Watts
		436,		// 177 Watts
		438,		// 178 Watts
		441,		// 179 Watts
		443,		// 180 Watts
		446,		// 181 Watts
		448,		// 182 Watts
		450,		// 183 Watts
		452,		// 184 Watts
		455,		// 185 Watts
		457,		// 186 Watts
		460,		// 187 Watts
		462,		// 188 Watts
		465,		// 189 Watts
		467,		// 190 Watts
		470,		// 191 Watts
		473,		// 192 Watts
		475,		// 193 Watts
		478,		// 194 Watts
		488,		// 195 Watts
		490,		// 196 Watts
		493,		// 197 Watts
		495,		// 198 Watts
		498,		// 199 Watts
		500,		// 200 Watts
        
		503,		// 201 Watts
		506,		// 202 Watts
		508,		// 203 Watts
		510,		// 204 Watts
		512,		// 205 Watts
		515,		// 206 Watts
		517,		// 207 Watts
		519,		// 208 Watts
		522,		// 209 Watts
		524,		// 210 Watts
		527,		// 211 Watts
		529,		// 212 Watts
		531,		// 213 Watts
		534,		// 214 Watts
		536,		// 215 Watts
		538,		// 216 Watts
		540,		// 217 Watts
		543,		// 218 Watts
		545,		// 219 Watts
		547,		// 220 Watts
		549,		// 221 Watts
		551,		// 222 Watts
		554,		// 223 Watts
		556,		// 224 Watts
		558,		// 225 Watts
		560,		// 226 Watts
		563,		// 227 Watts
		565,		// 228 Watts
		567,		// 229 Watts
		571,		// 230 Watts
		575,		// 231 Watts
		578,		// 232 Watts
		580,		// 233 Watts
		582,		// 234 Watts
		584,		// 235 Watts
		588,		// 236 Watts
		589,		// 237 Watts
		592,		// 238 Watts
		594,		// 239 Watts
		596,		// 240 Watts
		599,		// 241 Watts
		602,		// 242 Watts
		604,		// 243 Watts
		606,		// 244 Watts
		608,		// 245 Watts
		610,		// 246 Watts
		612,		// 247 Watts
		614,		// 248 Watts
		616,		// 249 Watts
		619,		// 250 Watts
		621,		// 251 Watts
		623,		// 252 Watts
		625,		// 253 Watts
		628,		// 254 Watts
		632,		// 255 Watts
		634,		// 256 Watts
		636,		// 257 Watts
		639,		// 258 Watts
		641,		// 259 Watts
		643,		// 260 Watts
		646,		// 261 Watts
		648,		// 262 Watts
		651,		// 263 Watts
		652,		// 264 Watts
		654,		// 265 Watts
		658,		// 266 Watts
		660,		// 267 Watts
		662,		// 268 Watts
		665,		// 269 Watts
		667,		// 270 Watts
		670,		// 271 Watts
		672,		// 272 Watts
		674,		// 273 Watts
		677,		// 274 Watts
		679,		// 275 Watts
		682,		// 276 Watts
		684,		// 277 Watts
		685,		// 278 Watts
		687,		// 279 Watts
		689,		// 280 Watts
		691,		// 281 Watts
		694,		// 282 Watts
		696,		// 283 Watts
		698,		// 284 Watts
		700,		// 285 Watts
		702,		// 286 Watts
		706,		// 287 Watts
		711,		// 288 Watts
		713,		// 289 Watts
		715,		// 290 Watts
		718,		// 291 Watts
		720,		// 292 Watts
		723,		// 293 Watts
		725,		// 294 Watts
		727,		// 295 Watts
		730,		// 296 Watts
		733,		// 297 Watts
		735,		// 298 Watts
		737,		// 299 Watts
		739,		// 300 Watts
        
		742,		// 301 Watts
		745,		// 302 Watts
		747,		// 303 Watts
		750,		// 304 Watts
		752,		// 305 Watts
		754,		// 306 Watts
		757,		// 307 Watts
		759,		// 308 Watts
		762,		// 309 Watts
		764,		// 310 Watts
		766,		// 311 Watts
		768,		// 312 Watts
		772,		// 313 Watts
		774,		// 314 Watts
		776,		// 315 Watts
		778,		// 316 Watts
		780,		// 317 Watts
		783,		// 318 Watts
		786,		// 319 Watts
		791,		// 320 Watts
		793,		// 321 Watts
		796,		// 322 Watts
		798,		// 323 Watts
		800,		// 324 Watts
		803,		// 325 Watts
		806,		// 326 Watts
		808,		// 327 Watts
		810,		// 328 Watts
		813,		// 329 Watts
		815,		// 330 Watts
		817,		// 331 Watts
		820,		// 332 Watts
		822,		// 333 Watts
		826,		// 334 Watts
		828,		// 335 Watts
		830,		// 336 Watts
		833,		// 337 Watts
		835,		// 338 Watts
		837,		// 339 Watts
		839,		// 340 Watts
		841,		// 341 Watts
		844,		// 342 Watts
		847,		// 343 Watts
		850,		// 344 Watts
		852,		// 345 Watts
		854,		// 346 Watts
		856,		// 347 Watts
		858,		// 348 Watts
		863,		// 349 Watts
		866,		// 350 Watts
		870,		// 351 Watts
		872,		// 352 Watts
		874,		// 353 Watts
		877,		// 354 Watts
		880,		// 355 Watts
		882,		// 356 Watts
		885,		// 357 Watts
		887,		// 358 Watts
		890,		// 359 Watts
		892,		// 360 Watts
		894,		// 361 Watts
		896,		// 362 Watts
		899,		// 363 Watts
		903,		// 364 Watts
		906,		// 365 Watts
		909,		// 366 Watts
		911,		// 367 Watts
		914,		// 368 Watts
		916,		// 369 Watts
		918,		// 370 Watts
		920,		// 371 Watts
		923,		// 372 Watts
		926,		// 373 Watts
		928,		// 374 Watts
		932,		// 375 Watts
		934,		// 376 Watts
		939,		// 377 Watts
		941,		// 378 Watts
		943,		// 379 Watts
		946,		// 380 Watts
		948,		// 381 Watts
		950,		// 382 Watts
		954,		// 383 Watts
		958,		// 384 Watts
		961,		// 385 Watts
		963,		// 386 Watts
		966,		// 387 Watts
		970,		// 388 Watts
		973,		// 389 Watts
		977,		// 390 Watts
		980,		// 391 Watts
		982,		// 392 Watts
		986,		// 393 Watts
		989,		// 394 Watts
		991,		// 395 Watts
		992,		// 396 Watts
		994,		// 397 Watts
		996,		// 398 Watts
		997,		// 399 Watts
		1002,		// 400 Watts
		1005,		// 401 Watts
		1006,		// 402 Watts
		1008,		// 403 Watts
		1014,		// 404 Watts
		1018,		// 405 Watts
		1022,		// 406 Watts
		1024,		// 407 Watts
		1028,		// 408 Watts
		1032,		// 409 Watts
		1036,		// 410 Watts
		1038,		// 411 Watts
		1042,		// 412 Watts
		1046,		// 413 Watts
		1058,		// 414 Watts
		1063,		// 415 Watts
		1067,		// 416 Watts
		1070,		// 417 Watts
		1075,		// 418 Watts
		1084,		// 419 Watts
		1088,		// 420 Watts
		1094,		// 421 Watts
		1099,		// 422 Watts
		1105,		// 423 Watts
		1109,		// 424 Watts
		1116,		// 425 Watts
		1120,		// 426 Watts
		1124,		// 427 Watts
		1128,		// 428 Watts
		1132,		// 429 Watts
		1137,		// 430 Watts
		1144,		// 431 Watts
		1151,		// 432 Watts
		1155,		// 433 Watts
		1160,		// 434 Watts
		1164,		// 435 Watts
		1169,		// 436 Watts
		1175,		// 437 Watts
		1179,		// 438 Watts
		1186,		// 439 Watts
		1193,		// 440 Watts
		1200,		// 441 Watts
		1207,		// 442 Watts
		1218,		// 443 Watts
        
		1222,		// 444 Watts
		1229,		// 445 Watts
		1236,		// 446 Watts
		1242,		// 447 Watts
		1249,		// 448 Watts
		1258,		// 449 Watts
		1264,		// 450 Watts
        
		1274,		// 451 Watts
		1280,		// 452 Watts
		1286,		// 453 Watts
		1294,		// 454 Watts
		1300,		// 455 Watts
		1306,		// 456 Watts
		1315,		// 457 Watts
		1322,		// 458 Watts
		1332,		// 459 Watts
		1340,		// 460 Watts
		1355,		// 461 Watts
		1380,		// 462 Watts
		1400,		// 463 Watts
		1415,		// 464 Watts
		1420,		// 465 Watts
		1445,		// 466 Watts
		1460,		// 467 Watts
		1480,		// 468 Watts
		1499,		// 469 Watts
		1513,		// 470 Watts
		1500,		// 471 Watts
		1529,		// 472 Watts
		1534,		// 473 Watts
		1539,		// 474 Watts
		1552,		// 475 Watts
		1572,		// 476 Watts
		1576,		// 477 Watts
		1583,		// 478 Watts
		1591,		// 479 Watts
		1595		// 480 Watts
	};
   
    
    
#endif	// defined(SMI360_72)
		
		
#endif		