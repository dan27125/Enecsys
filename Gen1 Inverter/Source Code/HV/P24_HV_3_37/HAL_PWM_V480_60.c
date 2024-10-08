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
#if defined(SMI480_60)

	// DGA March 6,2023 - wattage amplitude lookup table for 480 DUO
	
	Uint16 pwm_watt_table[WATT_TABLE_MAX]=
	{
		5,		// 1 Watts
		5,		// 2 Watts
		5,		// 3 Watts
		5,		// 4 Watts
		5,		// 5 Watts
		5,		// 6 Watts
		5,		// 7 Watts
		5,		// 8 Watts
		5,		// 9 Watts
		5,		// 10 Watts
		6,		// 11 Watts
		6,		// 12 Watts
		6,		// 13 Watts
		6,		// 14 Watts
		7,		// 15 Watts
		7,		// 16 Watts
		8,		// 17 Watts
		8,		// 18 Watts
		9,		// 19 Watts
		10,		// 20 Watts
		11,		// 21 Watts
		12,		// 22 Watts
		13,		// 23 Watts
		14,		// 24 Watts
		16,		// 25 Watts
		28,		// 26 Watts
		34,		// 27 Watts
		37,		// 28 Watts
		38,		// 29 Watts
		40,		// 30 Watts
		42,		// 31 Watts
		45,		// 32 Watts
		48,		// 33 Watts
		50,		// 34 Watts
		51,		// 35 Watts
		52,		// 36 Watts
		53,		// 37 Watts
		55,		// 38 Watts
		56,		// 39 Watts
		57,		// 40 Watts
		58,		// 41 Watts
		60,		// 42 Watts
		62,		// 43 Watts
		64,		// 44 Watts
		65,		// 45 Watts
		67,		// 46 Watts
		69,		// 47 Watts
		71,		// 48 Watts
		72,		// 49 Watts
		74,		// 50 Watts
		77,		// 51 Watts
		79,		// 52 Watts
		81,		// 53 Watts
		82,		// 54 Watts
		84,		// 55 Watts
		85,		// 56 Watts
		86,		// 57 Watts
		87,		// 58 Watts
		89,		// 59 Watts
		90,		// 60 Watts
		92,		// 61 Watts
		93,		// 62 Watts
		94,		// 63 Watts
		96,		// 64 Watts
		97,		// 65 Watts
		98,		// 66 Watts
		100,		// 67 Watts
		101,		// 68 Watts
		103,		// 69 Watts
		106,		// 70 Watts
		107,		// 71 Watts
		108,		// 72 Watts
		109,		// 73 Watts
		110,		// 74 Watts
		112,		// 75 Watts
		113,		// 76 Watts
		115,		// 77 Watts
		116,		// 78 Watts
		117,		// 79 Watts
		119,		// 80 Watts
		121,		// 81 Watts
		122,		// 82 Watts
		124,		// 83 Watts
		126,		// 84 Watts
		127,		// 85 Watts
		129,		// 86 Watts
		130,		// 87 Watts
		131,		// 88 Watts
		133,		// 89 Watts
		134,		// 90 Watts
		136,		// 91 Watts
		137,		// 92 Watts
		139,		// 93 Watts
		141,		// 94 Watts
		142,		// 95 Watts
		143,		// 96 Watts
		145,		// 97 Watts
		146,		// 98 Watts
		148,		// 99 Watts
		150,		// 100 Watts
		151,		// 101 Watts
		153,		// 102 Watts
		154,		// 103 Watts
		155,		// 104 Watts
		157,		// 105 Watts
		158,		// 106 Watts
		161,		// 107 Watts
		163,		// 108 Watts
		164,		// 109 Watts
		165,		// 110 Watts
		166,		// 111 Watts
		168,		// 112 Watts
		170,		// 113 Watts
		171,		// 114 Watts
		173,		// 115 Watts
		174,		// 116 Watts
		175,		// 117 Watts
		177,		// 118 Watts
		178,		// 119 Watts
		180,		// 120 Watts
		182,		// 121 Watts
		183,		// 122 Watts
		184,		// 123 Watts
		186,		// 124 Watts
		188,		// 125 Watts
		189,		// 126 Watts
		190,		// 127 Watts
		192,		// 128 Watts
		194,		// 129 Watts
		195,		// 130 Watts
		197,		// 131 Watts
		198,		// 132 Watts
		200,		// 133 Watts
		201,		// 134 Watts
		202,		// 135 Watts
		204,		// 136 Watts
		206,		// 137 Watts
		208,		// 138 Watts
		209,		// 139 Watts
		210,		// 140 Watts
		212,		// 141 Watts
		213,		// 142 Watts
		214,		// 143 Watts
		216,		// 144 Watts
		218,		// 145 Watts
		219,		// 146 Watts
		221,		// 147 Watts
		222,		// 148 Watts
		224,		// 149 Watts
		226,		// 150 Watts
		227,		// 151 Watts
		229,		// 152 Watts
		230,		// 153 Watts
		232,		// 154 Watts
		233,		// 155 Watts
		235,		// 156 Watts
		237,			// 157 Watts
		238,		// 158 Watts
		240,		// 159 Watts
		241,		// 160 Watts
		243,		// 161 Watts
		245,		// 162 Watts
		246,		// 163 Watts
		248,		// 164 Watts
		249,		// 165 Watts
		250,		// 166 Watts
		252,		// 167 Watts
		253,		// 168 Watts
		255,		// 169 Watts
		256,		// 170 Watts
		258,		// 171 Watts
		260,		// 172 Watts
		261,		// 173 Watts
		263,		// 174 Watts
		264,		// 175 Watts
		266,		// 176 Watts
		267,		// 177 Watts
		269,		// 178 Watts
		270,		// 179 Watts
		271,		// 180 Watts
		273,		// 181 Watts
		274,		// 182 Watts
		276,		// 183 Watts
		278,		// 184 Watts
		279,		// 185 Watts
		281,		// 186 Watts
		282,		// 187 Watts
		283,		// 188 Watts
		285,		// 189 Watts
		287,		// 190 Watts
		288,		// 191 Watts
		290,		// 192 Watts
		291,		// 193 Watts
		293,		// 194 Watts
		294,		// 195 Watts
		296,		// 196 Watts
		297,		// 197 Watts
		299,		// 198 Watts
		301,		// 199 Watts
		302,		// 200 Watts
		304,		// 201 Watts
		305,		// 202 Watts
		306,		// 203 Watts
		308,		// 204 Watts
		310,		// 205 Watts
		311,		// 206 Watts
		313,		// 207 Watts
		314,		// 208 Watts
		315,		// 209 Watts
		317,		// 210 Watts
		319,		// 211 Watts
		320,		// 212 Watts
		322,		// 213 Watts
		323,		// 214 Watts
		324,		// 215 Watts
		326,		// 216 Watts
		327,		// 217 Watts
		329,		// 218 Watts
		330,		// 219 Watts
		332,		// 220 Watts
		333,		// 221 Watts
		335,		// 222 Watts
		337,		// 223 Watts
		338,		// 224 Watts
		340,		// 225 Watts
		341,		// 226 Watts
		343,		// 227 Watts
		345,		// 228 Watts
		346,		// 229 Watts
		348,		// 230 Watts
		349,		// 231 Watts
		351,		// 232 Watts
		352,		// 233 Watts
		354,		// 234 Watts
		355,		// 235 Watts
		356,		// 236 Watts
		357,		// 237 Watts
		359,		// 238 Watts
		361,		// 239 Watts
		363,		// 240 Watts
		364,		// 241 Watts
		365,		// 242 Watts
		367,		// 243 Watts
		368,		// 244 Watts
		370,		// 245 Watts
		371,		// 246 Watts
		373,		// 247 Watts
		374,		// 248 Watts
		376,		// 249 Watts
		377,		// 250 Watts
		379,		// 251 Watts
		381,		// 252 Watts
		382,		// 253 Watts
		384,		// 254 Watts
		385,		// 255 Watts
		387,		// 256 Watts
		388,		// 257 Watts
		390,		// 258 Watts
		391,		// 259 Watts
		393,		// 260 Watts
		395,		// 261 Watts
		396,		// 262 Watts
		398,		// 263 Watts
		400,		// 264 Watts
		401,		// 265 Watts
		403,		// 266 Watts
		404,		// 267 Watts
		406,		// 268 Watts
		407,		// 269 Watts
		408,		// 270 Watts
		410,		// 271 Watts
		412,		// 272 Watts
		413,		// 273 Watts
		414,		// 274 Watts
		416,		// 275 Watts
		417,		// 276 Watts
		418,		// 277 Watts
		420,		// 278 Watts
		422,		// 279 Watts
		424,		// 280 Watts
		425,		// 281 Watts
		426,		// 282 Watts
		428,		// 283 Watts
		430,		// 284 Watts
		431,		// 285 Watts
		432,		// 286 Watts
		434,		// 287 Watts
		436,		// 288 Watts
		438,		// 289 Watts
		439,		// 290 Watts
		440,		// 291 Watts
		441,		// 292 Watts
		443,		// 293 Watts
		445,		// 294 Watts
		446,		// 295 Watts
		447,		// 296 Watts
		449,		// 297 Watts
		450,		// 298 Watts
		452,		// 299 Watts
		453,		// 300 Watts
		455,		// 301 Watts
		456,		// 302 Watts
		457,		// 303 Watts
		459,		// 304 Watts
		461,		// 305 Watts
		462,		// 306 Watts
		463,		// 307 Watts
		466,		// 308 Watts
		467,		// 309 Watts
		469,		// 310 Watts
		470,		// 311 Watts
		471,		// 312 Watts
		473,		// 313 Watts
		474,		// 314 Watts
		475,		// 315 Watts
		477,		// 316 Watts
		479,		// 317 Watts
		480,		// 318 Watts
		482,		// 319 Watts
		484,		// 320 Watts
		485,		// 321 Watts
		486,		// 322 Watts
		488,		// 323 Watts
		490,		// 324 Watts
		491,		// 325 Watts
		493,		// 326 Watts
		494,		// 327 Watts
		495,		// 328 Watts
		497,		// 329 Watts
		499,		// 330 Watts
		500,		// 331 Watts
		501,		// 332 Watts
		503,		// 333 Watts
		505,		// 334 Watts
		507,		// 335 Watts
		508,		// 336 Watts
		509,		// 337 Watts
		510,		// 338 Watts
		511,		// 339 Watts
		513,		// 340 Watts
		515,		// 341 Watts
		517,		// 342 Watts
		519,		// 343 Watts
		520,		// 344 Watts
		521,		// 345 Watts
		522,		// 346 Watts
		524,		// 347 Watts
		526,		// 348 Watts
		527,		// 349 Watts
		528,		// 350 Watts
		530,		// 351 Watts
		532,		// 352 Watts
		533,		// 353 Watts
		534,		// 354 Watts
		535,		// 355 Watts
		537,		// 356 Watts
		539,		// 357 Watts
		541,		// 358 Watts
		542,		// 359 Watts
		544,		// 360 Watts
		545,		// 361 Watts
		546,		// 362 Watts
		547,		// 363 Watts
		549,		// 364 Watts
		551,		// 365 Watts
		552,		// 366 Watts
		554,		// 367 Watts
		555,		// 368 Watts
		556,		// 369 Watts
		558,		// 370 Watts
		560,		// 371 Watts
		561,		// 372 Watts
		563,		// 373 Watts
		564,		// 374 Watts
		565,		// 375 Watts
		567,		// 376 Watts
		568,		// 377 Watts
		570,		// 378 Watts
		571,		// 379 Watts
		572,		// 380 Watts
		574,		// 381 Watts
		576,		// 382 Watts
		577,		// 383 Watts
		578,		// 384 Watts
		580,		// 385 Watts
		581,		// 386 Watts
		583,		// 387 Watts
		584,		// 388 Watts
		585,		// 389 Watts
		587,		// 390 Watts
		590,		// 391 Watts
		591,		// 392 Watts
		593,		// 393 Watts
		594,		// 394 Watts
		596,		// 395 Watts
		598,		// 396 Watts
		599,		// 397 Watts
		600,		// 398 Watts
		602,		// 399 Watts
		603,		// 400 Watts
		605,		// 401 Watts
		607,		// 402 Watts
		608,		// 403 Watts
		609,		// 404 Watts
		610,		// 405 Watts
		612,		// 406 Watts
		613,		// 407 Watts
		614,		// 408 Watts
		616,		// 409 Watts
		618,		// 410 Watts
		619,		// 411 Watts
		620,		// 412 Watts
		622,		// 413 Watts
		623,		// 414 Watts
		625,		// 415 Watts
		627,		// 416 Watts
		628,		// 417 Watts
		629,		// 418 Watts
		631,		// 419 Watts
		633,		// 420 Watts
		634,		// 421 Watts
		636,		// 422 Watts
		637,		// 423 Watts
		639,		// 424 Watts
		640,		// 425 Watts
		643,		// 426 Watts
		644,		// 427 Watts
		445,		// 428 Watts
		646,		// 429 Watts
		648,		// 430 Watts
		649,		// 431 Watts
		651,		// 432 Watts
		652,		// 433 Watts
		653,		// 434 Watts
		655,		// 435 Watts
		656,		// 436 Watts
		658,		// 437 Watts
		659,		// 438 Watts
		661,		// 439 Watts
		662,		// 440 Watts
		664,		// 441 Watts
		665,		// 442 Watts
		667,		// 443 Watts
		669,		// 444 Watts
		671,		// 445 Watts
		671,		// 446 Watts
		672,		// 447 Watts
		673,		// 448 Watts
		675,		// 449 Watts
		677,		// 450 Watts
		679,		// 451 Watts
		680,		// 452 Watts
		682,		// 453 Watts
		683,		// 454 Watts
		685,		// 455 Watts
		686,		// 456 Watts
		688,		// 457 Watts
		689,		// 458 Watts
		690,		// 459 Watts
		692,		// 460 Watts
		693,		// 461 Watts
		695,		// 462 Watts
		696,		// 463 Watts
		698,		// 464 Watts
		700,		// 465 Watts
		701,		// 466 Watts
		702,		// 467 Watts
		703,		// 468 Watts
		705,		// 469 Watts
		706,		// 470 Watts
		708,		// 471 Watts
		709,		// 472 Watts
		711,		// 473 Watts
		712,		// 474 Watts
		713,		// 475 Watts
		715,		// 476 Watts
		716,		// 477 Watts
		718,		// 478 Watts
		719,		// 479 Watts
		721,		// 480 Watts
		723,		// 481 Watts
		724,		// 482 Watts
		725,		// 483 Watts
		727,		// 484 Watts
		729,		// 485 Watts
		730,		// 486 Watts
		732,		// 487 Watts
		733,		// 488 Watts
		735,		// 489 Watts
		737,		// 490 Watts
		738,		// 491 Watts
		739,		// 492 Watts
		741,		// 493 Watts
		742,		// 494 Watts
		744,		// 495 Watts
		746,		// 496 Watts
		747,		// 497 Watts
		749,		// 498 Watts
		750,		// 499 Watts
		752,		// 500 Watts
		753,		// 501 Watts		
		754,		// 502 Watts		
		755,		// 503 Watts		
		757,		// 504 Watts		
		758,		// 505 Watts		
		759,		// 506 Watts		
		761,		// 507 Watts		
		762,		// 508 Watts		
		763,		// 509 Watts		
		765,		// 510 Watts		
		767,		// 511 Watts		
		768,		// 512 Watts		
		772,		// 513 Watts
		774,		// 514 Watts		
		775,		// 515 Watts		
		776,		// 516 Watts		
		777,		// 517 Watts		
		780,		// 518 Watts		
		782,		// 519 Watts		
		783,		// 520 Watts		
		785,		// 521 Watts		
		787,		// 522 Watts		
		788,		// 523 Watts		
		790,		// 524 Watts		
		791,		// 525 Watts		
		792,		// 526 Watts		
		793,		// 527 Watts		
		795,		// 528 Watts		
		797,		// 529 Watts		
		798,		// 530 Watts		
		799,		// 531 Watts		
		800,		// 532 Watts		
		802,		// 533 Watts		
		804,		// 534 Watts		
		806,		// 535 Watts		
		808,		// 536 Watts		
		810,		// 537 Watts		
		812,		// 538 Watts		
		814,		// 539 Watts		
		816,		// 540 Watts		
		818,		// 541 Watts		
		820,		// 542 Watts		
		822,		// 543 Watts		
		824,		// 544 Watts		
		825,		// 545 Watts		
		827,		// 546 Watts		
		829,		// 547 Watts		
		830,		// 548 Watts		
		833,		// 549 Watts		
		835,		// 550 Watts		
		837,		// 551 Watts		
		839,		// 552 Watts		
		840,		// 553 Watts		
		843,		// 554 Watts		
		845,		// 555 Watts		
		846,		// 556 Watts		
		848,		// 557 Watts		
		849,		// 558 Watts		
		852,		// 559 Watts		
		854,		// 560 Watts		
		856,		// 561 Watts		
		858,		// 562 Watts		
		861,		// 563 Watts		
		862,		// 564 Watts		
		864,		// 565 Watts		
		866,		// 566 Watts		
		868,		// 567 Watts		
		870,		// 568 Watts		
		872,		// 569 Watts		
		874,		// 570 Watts		
		876,		// 571 Watts		
		878,		// 572 Watts		
		879,		// 573 Watts		
		881,		// 574 Watts		
		883,		// 575 Watts		
		885,		// 576 Watts		
		887,		// 577 Watts		
		890,		// 578 Watts		
		893,		// 579 Watts		
		895,		// 580 Watts		
		897,		// 581 Watts		
		899,		// 582 Watts		
		902,		// 583 Watts		
		904,		// 584 Watts		
		906,		// 585 Watts		
		908,		// 586 Watts		
		910,		// 587 Watts		
		912,		// 588 Watts		
		914,		// 589 Watts		
		916,		// 590 Watts		
		918,		// 591 Watts		
		920,		// 592 Watts		
		922,		// 593 Watts		
		924,		// 594 Watts		
		927,		// 595 Watts		
		929,		// 596 Watts		
		931,		// 597 Watts		
		933,		// 598 Watts		
		934,		// 599 Watts		
		936		// 600 Watts
};
	

#endif	// defined(SMI480_60)
		
		
#endif		