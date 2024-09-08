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


#if !defined(HAL_Pic_24fj64ga004_ports_H)
#define HAL_Pic_24fj64ga004_ports_H


#define ENABLE_RA0_PULLUP() _CN2PUE = 1
#define DISABLE_RA0_PULLUP() _CN2PUE = 0
#define ENABLE_RA1_PULLUP() _CN3PUE = 1
#define DISABLE_RA1_PULLUP() _CN3PUE = 0
#define ENABLE_RA2_PULLUP() _CN30PUE = 1
#define DISABLE_RA2_PULLUP() _CN30PUE = 0
#define ENABLE_RA3_PULLUP() _CN29PUE = 1
#define DISABLE_RA3_PULLUP() _CN29PUE = 0
#define ENABLE_RA4_PULLUP() _CN0PUE = 1
#define DISABLE_RA4_PULLUP() _CN0PUE = 0
#define ENABLE_RB0_PULLUP() _CN4PUE = 1
#define DISABLE_RB0_PULLUP() _CN4PUE = 0
#define ENABLE_RB1_PULLUP() _CN5PUE = 1
#define DISABLE_RB1_PULLUP() _CN5PUE = 0
#define ENABLE_RB2_PULLUP() _CN6PUE = 1
#define DISABLE_RB2_PULLUP() _CN6PUE = 0
#define ENABLE_RB3_PULLUP() _CN7PUE = 1
#define DISABLE_RB3_PULLUP() _CN7PUE = 0
#define ENABLE_RB4_PULLUP() _CN1PUE = 1
#define DISABLE_RB4_PULLUP() _CN1PUE = 0
#define ENABLE_RB5_PULLUP() _CN27PUE = 1
#define DISABLE_RB5_PULLUP() _CN27PUE = 0
#define ENABLE_RB6_PULLUP() _CN24PUE = 1
#define DISABLE_RB6_PULLUP() _CN24PUE = 0
#define ENABLE_RB7_PULLUP() _CN23PUE = 1
#define DISABLE_RB7_PULLUP() _CN23PUE = 0
#define ENABLE_RB8_PULLUP() _CN18PUE = 1
#define DISABLE_RB8_PULLUP() _CN18PUE = 0
#define ENABLE_RB9_PULLUP() _CN22PUE = 1
#define DISABLE_RB9_PULLUP() _CN22PUE = 0
#define ENABLE_RB10_PULLUP() _CN16PUE = 1
#define DISABLE_RB10_PULLUP() _CN16PUE = 0
#define ENABLE_RB11_PULLUP() _CN15PUE = 1
#define DISABLE_RB11_PULLUP() _CN15PUE = 0
#define ENABLE_RB12_PULLUP() _CN14PUE = 1
#define DISABLE_RB12_PULLUP() _CN14PUE = 0
#define ENABLE_RB13_PULLUP() _CN13PUE = 1
#define DISABLE_RB13_PULLUP() _CN13PUE = 0
#define ENABLE_RB14_PULLUP() _CN12PUE = 1
#define DISABLE_RB14_PULLUP() _CN12PUE = 0
#define ENABLE_RB15_PULLUP() _CN11PUE = 1
#define DISABLE_RB15_PULLUP() _CN11PUE = 0
#define ENABLE_RC0_PULLUP() _CN8PUE = 1
#define DISABLE_RC0_PULLUP() _CN8PUE = 0
#define ENABLE_RC1_PULLUP() _CN9PUE = 1
#define DISABLE_RC1_PULLUP() _CN9PUE = 0
#define ENABLE_RC2_PULLUP() _CN10PUE = 1
#define DISABLE_RC2_PULLUP() _CN10PUE = 0
#define ENABLE_RC3_PULLUP() _CN28PUE = 1
#define DISABLE_RC3_PULLUP() _CN28PUE = 0
#define ENABLE_RC4_PULLUP() _CN25PUE = 1
#define DISABLE_RC4_PULLUP() _CN25PUE = 0
#define ENABLE_RC5_PULLUP() _CN26PUE = 1
#define DISABLE_RC5_PULLUP() _CN26PUE = 0
#define ENABLE_RC6_PULLUP() _CN18PUE = 1
#define DISABLE_RC6_PULLUP() _CN18PUE = 0
#define ENABLE_RC7_PULLUP() _CN17PUE = 1
#define DISABLE_RC7_PULLUP() _CN17PUE = 0
#define ENABLE_RC8_PULLUP() _CN20PUE = 1
#define DISABLE_RC8_PULLUP() _CN20PUE = 0
#define ENABLE_RC9_PULLUP() _CN19PUE = 1
#define DISABLE_RC9_PULLUP() _CN19PUE = 0
#define ENABLE_RA0_CN_INTERRUPT() _CN2IE = 1
#define DISABLE_RA0_CN_INTERRUPT() _CN2IE = 0
#define ENABLE_RA1_CN_INTERRUPT() _CN3IE = 1
#define DISABLE_RA1_CN_INTERRUPT() _CN3IE = 0
#define ENABLE_RA2_CN_INTERRUPT() _CN30IE = 1
#define DISABLE_RA2_CN_INTERRUPT() _CN30IE = 0
#define ENABLE_RA3_CN_INTERRUPT() _CN29IE = 1
#define DISABLE_RA3_CN_INTERRUPT() _CN29IE = 0
#define ENABLE_RA4_CN_INTERRUPT() _CN0IE = 1
#define DISABLE_RA4_CN_INTERRUPT() _CN0IE = 0
#define ENABLE_RB0_CN_INTERRUPT() _CN4IE = 1
#define DISABLE_RB0_CN_INTERRUPT() _CN4IE = 0
#define ENABLE_RB1_CN_INTERRUPT() _CN5IE = 1
#define DISABLE_RB1_CN_INTERRUPT() _CN5IE = 0
#define ENABLE_RB2_CN_INTERRUPT() _CN6IE = 1
#define DISABLE_RB2_CN_INTERRUPT() _CN6IE = 0
#define ENABLE_RB3_CN_INTERRUPT() _CN7IE = 1
#define DISABLE_RB3_CN_INTERRUPT() _CN7IE = 0
#define ENABLE_RB4_CN_INTERRUPT() _CN1IE = 1
#define DISABLE_RB4_CN_INTERRUPT() _CN1IE = 0
#define ENABLE_RB5_CN_INTERRUPT() _CN27IE = 1
#define DISABLE_RB5_CN_INTERRUPT() _CN27IE = 0
#define ENABLE_RB6_CN_INTERRUPT() _CN24IE = 1
#define DISABLE_RB6_CN_INTERRUPT() _CN24IE = 0
#define ENABLE_RB7_CN_INTERRUPT() _CN23IE = 1
#define DISABLE_RB7_CN_INTERRUPT() _CN23IE = 0
#define ENABLE_RB8_CN_INTERRUPT() _CN18IE = 1
#define DISABLE_RB8_CN_INTERRUPT() _CN18IE = 0
#define ENABLE_RB9_CN_INTERRUPT() _CN22IE = 1
#define DISABLE_RB9_CN_INTERRUPT() _CN22IE = 0
#define ENABLE_RB10_CN_INTERRUPT() _CN16IE = 1
#define DISABLE_RB10_CN_INTERRUPT() _CN16IE = 0
#define ENABLE_RB11_CN_INTERRUPT() _CN15IE = 1
#define DISABLE_RB11_CN_INTERRUPT() _CN15IE = 0
#define ENABLE_RB12_CN_INTERRUPT() _CN14IE = 1
#define DISABLE_RB12_CN_INTERRUPT() _CN14IE = 0
#define ENABLE_RB13_CN_INTERRUPT() _CN13IE = 1
#define DISABLE_RB13_CN_INTERRUPT() _CN13IE = 0
#define ENABLE_RB14_CN_INTERRUPT() _CN12IE = 1
#define DISABLE_RB14_CN_INTERRUPT() _CN12IE = 0
#define ENABLE_RB15_CN_INTERRUPT() _CN11IE = 1
#define DISABLE_RB15_CN_INTERRUPT() _CN11IE = 0
#define ENABLE_RC0_CN_INTERRUPT() _CN8IE = 1
#define DISABLE_RC0_CN_INTERRUPT() _CN8IE = 0
#define ENABLE_RC1_CN_INTERRUPT() _CN9IE = 1
#define DISABLE_RC1_CN_INTERRUPT() _CN9IE = 0
#define ENABLE_RC2_CN_INTERRUPT() _CN10IE = 1
#define DISABLE_RC2_CN_INTERRUPT() _CN10IE = 0
#define ENABLE_RC3_CN_INTERRUPT() _CN28IE = 1
#define DISABLE_RC3_CN_INTERRUPT() _CN28IE = 0
#define ENABLE_RC4_CN_INTERRUPT() _CN25IE = 1
#define DISABLE_RC4_CN_INTERRUPT() _CN25IE = 0
#define ENABLE_RC5_CN_INTERRUPT() _CN26IE = 1
#define DISABLE_RC5_CN_INTERRUPT() _CN26IE = 0
#define ENABLE_RC6_CN_INTERRUPT() _CN18IE = 1
#define DISABLE_RC6_CN_INTERRUPT() _CN18IE = 0
#define ENABLE_RC7_CN_INTERRUPT() _CN17IE = 1
#define DISABLE_RC7_CN_INTERRUPT() _CN17IE = 0
#define ENABLE_RC8_CN_INTERRUPT() _CN20IE = 1
#define DISABLE_RC8_CN_INTERRUPT() _CN20IE = 0
#define ENABLE_RC9_CN_INTERRUPT() _CN19IE = 1
#define DISABLE_RC9_CN_INTERRUPT() _CN19IE = 0
#define DISABLE_RA0_ANALOG() _PCFG0 = 1
#define ENABLE_RA0_ANALOG() _PCFG0 = 0
#define DISABLE_RA1_ANALOG() _PCFG1 = 1
#define ENABLE_RA1_ANALOG() _PCFG1 = 0
#define DISABLE_RB0_ANALOG() _PCFG2 = 1
#define ENABLE_RB0_ANALOG() _PCFG2 = 0
#define DISABLE_RB1_ANALOG() _PCFG3 = 1
#define ENABLE_RB1_ANALOG() _PCFG3 = 0
#define DISABLE_RB2_ANALOG() _PCFG4 = 1
#define ENABLE_RB2_ANALOG() _PCFG4 = 0
#define DISABLE_RB3_ANALOG() _PCFG5 = 1
#define ENABLE_RB3_ANALOG() _PCFG5 = 0
#define DISABLE_RB12_ANALOG() _PCFG12 = 1
#define ENABLE_RB12_ANALOG() _PCFG12 = 0
#define DISABLE_RB13_ANALOG() _PCFG11 = 1
#define ENABLE_RB13_ANALOG() _PCFG11 = 0
#define DISABLE_RB14_ANALOG() _PCFG10 = 1
#define ENABLE_RB14_ANALOG() _PCFG10 = 0
#define DISABLE_RB15_ANALOG() _PCFG9 = 1
#define ENABLE_RB15_ANALOG() _PCFG9 = 0
#define DISABLE_RC0_ANALOG() _PCFG6 = 1
#define ENABLE_RC0_ANALOG() _PCFG6 = 0
#define DISABLE_RC1_ANALOG() _PCFG7 = 1
#define ENABLE_RC1_ANALOG() _PCFG7 = 0
#define DISABLE_RC2_ANALOG() _PCFG8 = 1
#define ENABLE_RC2_ANALOG() _PCFG8 = 0
#define ENABLE_RA0_OPENDRAIN() _ODA0 = 1
#define DISABLE_RA0_OPENDRAIN() _ODA0 = 0
#define ENABLE_RA1_OPENDRAIN() _ODA1 = 1
#define DISABLE_RA1_OPENDRAIN() _ODA1 = 0
#define ENABLE_RA2_OPENDRAIN() _ODA2 = 1
#define DISABLE_RA2_OPENDRAIN() _ODA2 = 0
#define ENABLE_RA3_OPENDRAIN() _ODA3 = 1
#define DISABLE_RA3_OPENDRAIN() _ODA3 = 0
#define ENABLE_RA4_OPENDRAIN() _ODA4 = 1
#define DISABLE_RA4_OPENDRAIN() _ODA4 = 0
#define ENABLE_RA7_OPENDRAIN() _ODA7 = 1
#define DISABLE_RA7_OPENDRAIN() _ODA7 = 0
#define ENABLE_RA8_OPENDRAIN() _ODA8 = 1
#define DISABLE_RA8_OPENDRAIN() _ODA8 = 0
#define ENABLE_RA9_OPENDRAIN() _ODA9 = 1
#define DISABLE_RA9_OPENDRAIN() _ODA9 = 0
#define ENABLE_RA10_OPENDRAIN() _ODA10 = 1
#define DISABLE_RA10_OPENDRAIN() _ODA10 = 0
#define ENABLE_RB0_OPENDRAIN() _ODB0 = 1
#define DISABLE_RB0_OPENDRAIN() _ODB0 = 0
#define ENABLE_RB1_OPENDRAIN() _ODB1 = 1
#define DISABLE_RB1_OPENDRAIN() _ODB1 = 0
#define ENABLE_RB2_OPENDRAIN() _ODB2 = 1
#define DISABLE_RB2_OPENDRAIN() _ODB2 = 0
#define ENABLE_RB3_OPENDRAIN() _ODB3 = 1
#define DISABLE_RB3_OPENDRAIN() _ODB3 = 0
#define ENABLE_RB4_OPENDRAIN() _ODB4 = 1
#define DISABLE_RB4_OPENDRAIN() _ODB4 = 0
#define ENABLE_RB5_OPENDRAIN() _ODB5 = 1
#define DISABLE_RB5_OPENDRAIN() _ODB5 = 0
#define ENABLE_RB6_OPENDRAIN() _ODB6 = 1
#define DISABLE_RB6_OPENDRAIN() _ODB6 = 0
#define ENABLE_RB7_OPENDRAIN() _ODB7 = 1
#define DISABLE_RB7_OPENDRAIN() _ODB7 = 0
#define ENABLE_RB8_OPENDRAIN() _ODB8 = 1
#define DISABLE_RB8_OPENDRAIN() _ODB8 = 0
#define ENABLE_RB9_OPENDRAIN() _ODB9 = 1
#define DISABLE_RB9_OPENDRAIN() _ODB9 = 0
#define ENABLE_RB10_OPENDRAIN() _ODB10 = 1
#define DISABLE_RB10_OPENDRAIN() _ODB10 = 0
#define ENABLE_RB11_OPENDRAIN() _ODB11 = 1
#define DISABLE_RB11_OPENDRAIN() _ODB11 = 0
#define ENABLE_RB12_OPENDRAIN() _ODB12 = 1
#define DISABLE_RB12_OPENDRAIN() _ODB12 = 0
#define ENABLE_RB13_OPENDRAIN() _ODB13 = 1
#define DISABLE_RB13_OPENDRAIN() _ODB13 = 0
#define ENABLE_RB14_OPENDRAIN() _ODB14 = 1
#define DISABLE_RB14_OPENDRAIN() _ODB14 = 0
#define ENABLE_RB15_OPENDRAIN() _ODB15 = 1
#define DISABLE_RB15_OPENDRAIN() _ODB15 = 0
#define ENABLE_RC0_OPENDRAIN() _ODC0 = 1
#define DISABLE_RC0_OPENDRAIN() _ODC0 = 0
#define ENABLE_RC1_OPENDRAIN() _ODC1 = 1
#define DISABLE_RC1_OPENDRAIN() _ODC1 = 0
#define ENABLE_RC2_OPENDRAIN() _ODC2 = 1
#define DISABLE_RC2_OPENDRAIN() _ODC2 = 0
#define ENABLE_RC3_OPENDRAIN() _ODC3 = 1
#define DISABLE_RC3_OPENDRAIN() _ODC3 = 0
#define ENABLE_RC4_OPENDRAIN() _ODC4 = 1
#define DISABLE_RC4_OPENDRAIN() _ODC4 = 0
#define ENABLE_RC5_OPENDRAIN() _ODC5 = 1
#define DISABLE_RC5_OPENDRAIN() _ODC5 = 0
#define ENABLE_RC6_OPENDRAIN() _ODC6 = 1
#define DISABLE_RC6_OPENDRAIN() _ODC6 = 0
#define ENABLE_RC7_OPENDRAIN() _ODC7 = 1
#define DISABLE_RC7_OPENDRAIN() _ODC7 = 0
#define ENABLE_RC8_OPENDRAIN() _ODC8 = 1
#define DISABLE_RC8_OPENDRAIN() _ODC8 = 0
#define ENABLE_RC9_OPENDRAIN() _ODC9 = 1
#define DISABLE_RC9_OPENDRAIN() _ODC9 = 0

static inline void CONFIG_RA0_AS_DIG_OUTPUT() {
  DISABLE_RA0_PULLUP();
  DISABLE_RA0_OPENDRAIN();
  _TRISA0 = 0;
  _PCFG0 = 1;
}
static inline void CONFIG_RA1_AS_DIG_OUTPUT() {
  DISABLE_RA1_PULLUP();
  DISABLE_RA1_OPENDRAIN();
  _TRISA1 = 0;
  _PCFG1 = 1;
}
static inline void CONFIG_RA2_AS_DIG_OUTPUT() {
  DISABLE_RA2_PULLUP();
  DISABLE_RA2_OPENDRAIN();
  _TRISA2 = 0;
}
static inline void CONFIG_RA3_AS_DIG_OUTPUT() {
  DISABLE_RA3_PULLUP();
  DISABLE_RA3_OPENDRAIN();
  _TRISA3 = 0;
}
static inline void CONFIG_RA4_AS_DIG_OUTPUT() {
  DISABLE_RA4_PULLUP();
  DISABLE_RA4_OPENDRAIN();
  _TRISA4 = 0;
}
static inline void CONFIG_RA7_AS_DIG_OUTPUT() {
  DISABLE_RA7_OPENDRAIN();
  _TRISA7 = 0;
}
static inline void CONFIG_RA8_AS_DIG_OUTPUT() {
  DISABLE_RA8_OPENDRAIN();
  _TRISA8 = 0;
}
static inline void CONFIG_RA9_AS_DIG_OUTPUT() {
  DISABLE_RA9_OPENDRAIN();
  _TRISA9 = 0;
}
static inline void CONFIG_RA10_AS_DIG_OUTPUT() {
  DISABLE_RA10_OPENDRAIN();
  _TRISA10 = 0;
}
static inline void CONFIG_RB0_AS_DIG_OUTPUT() {
  DISABLE_RB0_PULLUP();
  DISABLE_RB0_OPENDRAIN();
  _TRISB0 = 0;
  _PCFG2 = 1;
}
static inline void CONFIG_RB1_AS_DIG_OUTPUT() {
  DISABLE_RB1_PULLUP();
  DISABLE_RB1_OPENDRAIN();
  _TRISB1 = 0;
  _PCFG3 = 1;
}
static inline void CONFIG_RB2_AS_DIG_OUTPUT() {
  DISABLE_RB2_PULLUP();
  DISABLE_RB2_OPENDRAIN();
  _TRISB2 = 0;
  _PCFG4 = 1;
}
static inline void CONFIG_RB3_AS_DIG_OUTPUT() {
  DISABLE_RB3_PULLUP();
  DISABLE_RB3_OPENDRAIN();
  _TRISB3 = 0;
  _PCFG5 = 1;
}
static inline void CONFIG_RB4_AS_DIG_OUTPUT() {
  DISABLE_RB4_PULLUP();
  DISABLE_RB4_OPENDRAIN();
  _TRISB4 = 0;
}
static inline void CONFIG_RB5_AS_DIG_OUTPUT() {
  DISABLE_RB5_PULLUP();
  DISABLE_RB5_OPENDRAIN();
  _TRISB5 = 0;
}
static inline void CONFIG_RB6_AS_DIG_OUTPUT() {
  DISABLE_RB6_PULLUP();
  DISABLE_RB6_OPENDRAIN();
  _TRISB6 = 0;
}
static inline void CONFIG_RB7_AS_DIG_OUTPUT() {
  DISABLE_RB7_PULLUP();
  DISABLE_RB7_OPENDRAIN();
  _TRISB7 = 0;
}
static inline void CONFIG_RB8_AS_DIG_OUTPUT() {
  DISABLE_RB8_PULLUP();
  DISABLE_RB8_OPENDRAIN();
  _TRISB8 = 0;
}
static inline void CONFIG_RB9_AS_DIG_OUTPUT() {
  DISABLE_RB9_PULLUP();
  DISABLE_RB9_OPENDRAIN();
  _TRISB9 = 0;
}
static inline void CONFIG_RB10_AS_DIG_OUTPUT() {
  DISABLE_RB10_PULLUP();
  DISABLE_RB10_OPENDRAIN();
  _TRISB10 = 0;
}
static inline void CONFIG_RB11_AS_DIG_OUTPUT() {
  DISABLE_RB11_PULLUP();
  DISABLE_RB11_OPENDRAIN();
  _TRISB11 = 0;
}
static inline void CONFIG_RB12_AS_DIG_OUTPUT() {
  DISABLE_RB12_PULLUP();
  DISABLE_RB12_OPENDRAIN();
  _TRISB12 = 0;
  _PCFG12 = 1;
}
static inline void CONFIG_RB13_AS_DIG_OUTPUT() {
  DISABLE_RB13_PULLUP();
  DISABLE_RB13_OPENDRAIN();
  _TRISB13 = 0;
  _PCFG11 = 1;
}
static inline void CONFIG_RB14_AS_DIG_OUTPUT() {
  DISABLE_RB14_PULLUP();
  DISABLE_RB14_OPENDRAIN();
  _TRISB14 = 0;
  _PCFG10 = 1;
}
static inline void CONFIG_RB15_AS_DIG_OUTPUT() {
  DISABLE_RB15_PULLUP();
  DISABLE_RB15_OPENDRAIN();
  _TRISB15 = 0;
  _PCFG9 = 1;
}
static inline void CONFIG_RC0_AS_DIG_OUTPUT() {
  DISABLE_RC0_PULLUP();
  DISABLE_RC0_OPENDRAIN();
  _TRISC0 = 0;
  _PCFG6 = 1;
}
static inline void CONFIG_RC1_AS_DIG_OUTPUT() {
  DISABLE_RC1_PULLUP();
  DISABLE_RC1_OPENDRAIN();
  _TRISC1 = 0;
  _PCFG7 = 1;
}
static inline void CONFIG_RC2_AS_DIG_OUTPUT() {
  DISABLE_RC2_PULLUP();
  DISABLE_RC2_OPENDRAIN();
  _TRISC2 = 0;
  _PCFG8 = 1;
}
static inline void CONFIG_RC3_AS_DIG_OUTPUT() {
  DISABLE_RC3_PULLUP();
  DISABLE_RC3_OPENDRAIN();
  _TRISC3 = 0;
}
static inline void CONFIG_RC4_AS_DIG_OUTPUT() {
  DISABLE_RC4_PULLUP();
  DISABLE_RC4_OPENDRAIN();
  _TRISC4 = 0;
}
static inline void CONFIG_RC5_AS_DIG_OUTPUT() {
  DISABLE_RC5_PULLUP();
  DISABLE_RC5_OPENDRAIN();
  _TRISC5 = 0;
}
static inline void CONFIG_RC6_AS_DIG_OUTPUT() {
  DISABLE_RC6_PULLUP();
  DISABLE_RC6_OPENDRAIN();
  _TRISC6 = 0;
}
static inline void CONFIG_RC7_AS_DIG_OUTPUT() {
  DISABLE_RC7_PULLUP();
  DISABLE_RC7_OPENDRAIN();
  _TRISC7 = 0;
}
static inline void CONFIG_RC8_AS_DIG_OUTPUT() {
  DISABLE_RC8_PULLUP();
  DISABLE_RC8_OPENDRAIN();
  _TRISC8 = 0;
}
static inline void CONFIG_RC9_AS_DIG_OUTPUT() {
  DISABLE_RC9_PULLUP();
  DISABLE_RC9_OPENDRAIN();
  _TRISC9 = 0;
}
static inline void CONFIG_RA0_AS_DIG_OD_OUTPUT() {
  DISABLE_RA0_PULLUP();
  ENABLE_RA0_OPENDRAIN();
  _TRISA0 = 0;
  _PCFG0 = 1;
}
static inline void CONFIG_RA1_AS_DIG_OD_OUTPUT() {
  DISABLE_RA1_PULLUP();
  ENABLE_RA1_OPENDRAIN();
  _TRISA1 = 0;
  _PCFG1 = 1;
}
static inline void CONFIG_RA2_AS_DIG_OD_OUTPUT() {
  DISABLE_RA2_PULLUP();
  ENABLE_RA2_OPENDRAIN();
  _TRISA2 = 0;
}
static inline void CONFIG_RA3_AS_DIG_OD_OUTPUT() {
  DISABLE_RA3_PULLUP();
  ENABLE_RA3_OPENDRAIN();
  _TRISA3 = 0;
}
static inline void CONFIG_RA4_AS_DIG_OD_OUTPUT() {
  DISABLE_RA4_PULLUP();
  ENABLE_RA4_OPENDRAIN();
  _TRISA4 = 0;
}
static inline void CONFIG_RA7_AS_DIG_OD_OUTPUT() {
  ENABLE_RA7_OPENDRAIN();
  _TRISA7 = 0;
}
static inline void CONFIG_RA8_AS_DIG_OD_OUTPUT() {
  ENABLE_RA8_OPENDRAIN();
  _TRISA8 = 0;
}
static inline void CONFIG_RA9_AS_DIG_OD_OUTPUT() {
  ENABLE_RA9_OPENDRAIN();
  _TRISA9 = 0;
}
static inline void CONFIG_RA10_AS_DIG_OD_OUTPUT() {
  ENABLE_RA10_OPENDRAIN();
  _TRISA10 = 0;
}
static inline void CONFIG_RB0_AS_DIG_OD_OUTPUT() {
  DISABLE_RB0_PULLUP();
  ENABLE_RB0_OPENDRAIN();
  _TRISB0 = 0;
  _PCFG2 = 1;
}
static inline void CONFIG_RB1_AS_DIG_OD_OUTPUT() {
  DISABLE_RB1_PULLUP();
  ENABLE_RB1_OPENDRAIN();
  _TRISB1 = 0;
  _PCFG3 = 1;
}
static inline void CONFIG_RB2_AS_DIG_OD_OUTPUT() {
  DISABLE_RB2_PULLUP();
  ENABLE_RB2_OPENDRAIN();
  _TRISB2 = 0;
  _PCFG4 = 1;
}
static inline void CONFIG_RB3_AS_DIG_OD_OUTPUT() {
  DISABLE_RB3_PULLUP();
  ENABLE_RB3_OPENDRAIN();
  _TRISB3 = 0;
  _PCFG5 = 1;
}
static inline void CONFIG_RB4_AS_DIG_OD_OUTPUT() {
  DISABLE_RB4_PULLUP();
  ENABLE_RB4_OPENDRAIN();
  _TRISB4 = 0;
}
static inline void CONFIG_RB5_AS_DIG_OD_OUTPUT() {
  DISABLE_RB5_PULLUP();
  ENABLE_RB5_OPENDRAIN();
  _TRISB5 = 0;
}
static inline void CONFIG_RB6_AS_DIG_OD_OUTPUT() {
  DISABLE_RB6_PULLUP();
  ENABLE_RB6_OPENDRAIN();
  _TRISB6 = 0;
}
static inline void CONFIG_RB7_AS_DIG_OD_OUTPUT() {
  DISABLE_RB7_PULLUP();
  ENABLE_RB7_OPENDRAIN();
  _TRISB7 = 0;
}
static inline void CONFIG_RB8_AS_DIG_OD_OUTPUT() {
  DISABLE_RB8_PULLUP();
  ENABLE_RB8_OPENDRAIN();
  _TRISB8 = 0;
}
static inline void CONFIG_RB9_AS_DIG_OD_OUTPUT() {
  DISABLE_RB9_PULLUP();
  ENABLE_RB9_OPENDRAIN();
  _TRISB9 = 0;
}
static inline void CONFIG_RB10_AS_DIG_OD_OUTPUT() {
  DISABLE_RB10_PULLUP();
  ENABLE_RB10_OPENDRAIN();
  _TRISB10 = 0;
}
static inline void CONFIG_RB11_AS_DIG_OD_OUTPUT() {
  DISABLE_RB11_PULLUP();
  ENABLE_RB11_OPENDRAIN();
  _TRISB11 = 0;
}
static inline void CONFIG_RB12_AS_DIG_OD_OUTPUT() {
  DISABLE_RB12_PULLUP();
  ENABLE_RB12_OPENDRAIN();
  _TRISB12 = 0;
  _PCFG12 = 1;
}
static inline void CONFIG_RB13_AS_DIG_OD_OUTPUT() {
  DISABLE_RB13_PULLUP();
  ENABLE_RB13_OPENDRAIN();
  _TRISB13 = 0;
  _PCFG11 = 1;
}
static inline void CONFIG_RB14_AS_DIG_OD_OUTPUT() {
  DISABLE_RB14_PULLUP();
  ENABLE_RB14_OPENDRAIN();
  _TRISB14 = 0;
  _PCFG10 = 1;
}
static inline void CONFIG_RB15_AS_DIG_OD_OUTPUT() {
  DISABLE_RB15_PULLUP();
  ENABLE_RB15_OPENDRAIN();
  _TRISB15 = 0;
  _PCFG9 = 1;
}
static inline void CONFIG_RC0_AS_DIG_OD_OUTPUT() {
  DISABLE_RC0_PULLUP();
  ENABLE_RC0_OPENDRAIN();
  _TRISC0 = 0;
  _PCFG6 = 1;
}
static inline void CONFIG_RC1_AS_DIG_OD_OUTPUT() {
  DISABLE_RC1_PULLUP();
  ENABLE_RC1_OPENDRAIN();
  _TRISC1 = 0;
  _PCFG7 = 1;
}
static inline void CONFIG_RC2_AS_DIG_OD_OUTPUT() {
  DISABLE_RC2_PULLUP();
  ENABLE_RC2_OPENDRAIN();
  _TRISC2 = 0;
  _PCFG8 = 1;
}
static inline void CONFIG_RC3_AS_DIG_OD_OUTPUT() {
  DISABLE_RC3_PULLUP();
  ENABLE_RC3_OPENDRAIN();
  _TRISC3 = 0;
}
static inline void CONFIG_RC4_AS_DIG_OD_OUTPUT() {
  DISABLE_RC4_PULLUP();
  ENABLE_RC4_OPENDRAIN();
  _TRISC4 = 0;
}
static inline void CONFIG_RC5_AS_DIG_OD_OUTPUT() {
  DISABLE_RC5_PULLUP();
  ENABLE_RC5_OPENDRAIN();
  _TRISC5 = 0;
}
static inline void CONFIG_RC6_AS_DIG_OD_OUTPUT() {
  DISABLE_RC6_PULLUP();
  ENABLE_RC6_OPENDRAIN();
  _TRISC6 = 0;
}
static inline void CONFIG_RC7_AS_DIG_OD_OUTPUT() {
  DISABLE_RC7_PULLUP();
  ENABLE_RC7_OPENDRAIN();
  _TRISC7 = 0;
}
static inline void CONFIG_RC8_AS_DIG_OD_OUTPUT() {
  DISABLE_RC8_PULLUP();
  ENABLE_RC8_OPENDRAIN();
  _TRISC8 = 0;
}
static inline void CONFIG_RC9_AS_DIG_OD_OUTPUT() {
  DISABLE_RC9_PULLUP();
  ENABLE_RC9_OPENDRAIN();
  _TRISC9 = 0;
}
static inline void CONFIG_RA0_AS_DIG_INPUT() {
  DISABLE_RA0_PULLUP();
  _TRISA0 = 1;
  _PCFG0 = 1;
}
static inline void CONFIG_RA1_AS_DIG_INPUT() {
  DISABLE_RA1_PULLUP();
  _TRISA1 = 1;
  _PCFG1 = 1;
}
static inline void CONFIG_RA2_AS_DIG_INPUT() {
  DISABLE_RA2_PULLUP();
  _TRISA2 = 1;
}
static inline void CONFIG_RA3_AS_DIG_INPUT() {
  DISABLE_RA3_PULLUP();
  _TRISA3 = 1;
}
static inline void CONFIG_RA4_AS_DIG_INPUT() {
  DISABLE_RA4_PULLUP();
  _TRISA4 = 1;
}
static inline void CONFIG_RA7_AS_DIG_INPUT() {
  _TRISA7 = 1;
}
static inline void CONFIG_RA8_AS_DIG_INPUT() {
  _TRISA8 = 1;
}
static inline void CONFIG_RA9_AS_DIG_INPUT() {
  _TRISA9 = 1;
}
static inline void CONFIG_RA10_AS_DIG_INPUT() {
  _TRISA10 = 1;
}
static inline void CONFIG_RB0_AS_DIG_INPUT() {
  DISABLE_RB0_PULLUP();
  _TRISB0 = 1;
  _PCFG2 = 1;
}
static inline void CONFIG_RB1_AS_DIG_INPUT() {
  DISABLE_RB1_PULLUP();
  _TRISB1 = 1;
  _PCFG3 = 1;
}
static inline void CONFIG_RB2_AS_DIG_INPUT() {
  DISABLE_RB2_PULLUP();
  _TRISB2 = 1;
  _PCFG4 = 1;
}
static inline void CONFIG_RB3_AS_DIG_INPUT() {
  DISABLE_RB3_PULLUP();
  _TRISB3 = 1;
  _PCFG5 = 1;
}
static inline void CONFIG_RB4_AS_DIG_INPUT() {
  DISABLE_RB4_PULLUP();
  _TRISB4 = 1;
}
static inline void CONFIG_RB5_AS_DIG_INPUT() {
  DISABLE_RB5_PULLUP();
  _TRISB5 = 1;
}
static inline void CONFIG_RB6_AS_DIG_INPUT() {
  DISABLE_RB6_PULLUP();
  _TRISB6 = 1;
}
static inline void CONFIG_RB7_AS_DIG_INPUT() {
  DISABLE_RB7_PULLUP();
  _TRISB7 = 1;
}
static inline void CONFIG_RB8_AS_DIG_INPUT() {
  DISABLE_RB8_PULLUP();
  _TRISB8 = 1;
}
static inline void CONFIG_RB9_AS_DIG_INPUT() {
  DISABLE_RB9_PULLUP();
  _TRISB9 = 1;
}
static inline void CONFIG_RB10_AS_DIG_INPUT() {
  DISABLE_RB10_PULLUP();
  _TRISB10 = 1;
}
static inline void CONFIG_RB11_AS_DIG_INPUT() {
  DISABLE_RB11_PULLUP();
  _TRISB11 = 1;
}
static inline void CONFIG_RB12_AS_DIG_INPUT() {
  DISABLE_RB12_PULLUP();
  _TRISB12 = 1;
  _PCFG12 = 1;
}
static inline void CONFIG_RB13_AS_DIG_INPUT() {
  DISABLE_RB13_PULLUP();
  _TRISB13 = 1;
  _PCFG11 = 1;
}
static inline void CONFIG_RB14_AS_DIG_INPUT() {
  DISABLE_RB14_PULLUP();
  _TRISB14 = 1;
  _PCFG10 = 1;
}
static inline void CONFIG_RB15_AS_DIG_INPUT() {
  DISABLE_RB15_PULLUP();
  _TRISB15 = 1;
  _PCFG9 = 1;
}
static inline void CONFIG_RC0_AS_DIG_INPUT() {
  DISABLE_RC0_PULLUP();
  _TRISC0 = 1;
  _PCFG6 = 1;
}
static inline void CONFIG_RC1_AS_DIG_INPUT() {
  DISABLE_RC1_PULLUP();
  _TRISC1 = 1;
  _PCFG7 = 1;
}
static inline void CONFIG_RC2_AS_DIG_INPUT() {
  DISABLE_RC2_PULLUP();
  _TRISC2 = 1;
  _PCFG8 = 1;
}
static inline void CONFIG_RC3_AS_DIG_INPUT() {
  DISABLE_RC3_PULLUP();
  _TRISC3 = 1;
}
static inline void CONFIG_RC4_AS_DIG_INPUT() {
  DISABLE_RC4_PULLUP();
  _TRISC4 = 1;
}
static inline void CONFIG_RC5_AS_DIG_INPUT() {
  DISABLE_RC5_PULLUP();
  _TRISC5 = 1;
}
static inline void CONFIG_RC6_AS_DIG_INPUT() {
  DISABLE_RC6_PULLUP();
  _TRISC6 = 1;
}
static inline void CONFIG_RC7_AS_DIG_INPUT() {
  DISABLE_RC7_PULLUP();
  _TRISC7 = 1;
}
static inline void CONFIG_RC8_AS_DIG_INPUT() {
  DISABLE_RC8_PULLUP();
  _TRISC8 = 1;
}
static inline void CONFIG_RC9_AS_DIG_INPUT() {
  DISABLE_RC9_PULLUP();
  _TRISC9 = 1;
}
#define CONFIG_RP0_AS_DIG_PIN()  _PCFG2 = 1
#define CONFIG_RP1_AS_DIG_PIN()  _PCFG3 = 1
#define CONFIG_RP2_AS_DIG_PIN()  _PCFG4 = 1
#define CONFIG_RP3_AS_DIG_PIN()  _PCFG5 = 1
#define CONFIG_RP4_AS_DIG_PIN()
#define CONFIG_RP5_AS_DIG_PIN()
#define CONFIG_RP6_AS_DIG_PIN()
#define CONFIG_RP7_AS_DIG_PIN()
#define CONFIG_RP8_AS_DIG_PIN()
#define CONFIG_RP9_AS_DIG_PIN()
#define CONFIG_RP10_AS_DIG_PIN()
#define CONFIG_RP11_AS_DIG_PIN()
#define CONFIG_RP12_AS_DIG_PIN()  _PCFG12 = 1
#define CONFIG_RP13_AS_DIG_PIN()  _PCFG11 = 1
#define CONFIG_RP14_AS_DIG_PIN()  _PCFG10 = 1
#define CONFIG_RP15_AS_DIG_PIN()  _PCFG9 = 1
#define CONFIG_RP16_AS_DIG_PIN()  _PCFG6 = 1
#define CONFIG_RP17_AS_DIG_PIN()  _PCFG7 = 1
#define CONFIG_RP18_AS_DIG_PIN()  _PCFG8 = 1
#define CONFIG_RP19_AS_DIG_PIN()
#define CONFIG_RP20_AS_DIG_PIN()
#define CONFIG_RP21_AS_DIG_PIN()
#define CONFIG_RP22_AS_DIG_PIN()
#define CONFIG_RP23_AS_DIG_PIN()
#define CONFIG_RP24_AS_DIG_PIN()
#define CONFIG_RP25_AS_DIG_PIN()
#define CONFIG_RP26_AS_DIG_PIN()
#define CONFIG_RP27_AS_DIG_PIN()
#define CONFIG_RP28_AS_DIG_PIN()
#define CONFIG_RP29_AS_DIG_PIN()
#define CONFIG_RP30_AS_DIG_PIN()
#define CONFIG_RP31_AS_DIG_PIN()
#define CONFIG_RP32_AS_DIG_PIN()
#define CONFIG_RP33_AS_DIG_PIN()
#define CONFIG_RP34_AS_DIG_PIN()
#define CONFIG_RP35_AS_DIG_PIN()
#define CONFIG_RP36_AS_DIG_PIN()
#define CONFIG_RP37_AS_DIG_PIN()
#define CONFIG_RP38_AS_DIG_PIN()
#define CONFIG_RP39_AS_DIG_PIN()
#define CONFIG_RP40_AS_DIG_PIN()
#define CONFIG_RP41_AS_DIG_PIN()
#define CONFIG_RP42_AS_DIG_PIN()
#define CONFIG_RP43_AS_DIG_PIN()
#define CONFIG_RP44_AS_DIG_PIN()
#define CONFIG_RP45_AS_DIG_PIN()

#ifdef _PCFG0
static inline void CONFIG_AN0_AS_ANALOG() {
  CONFIG_RA0_AS_DIG_INPUT();
  _PCFG0 = 0;
}
#endif

#ifdef _PCFG1
static inline void CONFIG_AN1_AS_ANALOG() {
  CONFIG_RA1_AS_DIG_INPUT();
  _PCFG1 = 0;
}
#endif


#ifdef _PCFG2
static inline void CONFIG_AN2_AS_ANALOG() {
  CONFIG_RB0_AS_DIG_INPUT();
  _PCFG2 = 0;
}
#endif


#ifdef _PCFG3
static inline void CONFIG_AN3_AS_ANALOG() {
  CONFIG_RB1_AS_DIG_INPUT();
  _PCFG3 = 0;
}
#endif

#ifdef _PCFG4
static inline void CONFIG_AN4_AS_ANALOG() {
  CONFIG_RB2_AS_DIG_INPUT();
  _PCFG4 = 0;
}
#endif


#ifdef _PCFG5
static inline void CONFIG_AN5_AS_ANALOG() {
  CONFIG_RB3_AS_DIG_INPUT();
  _PCFG5 = 0;
}
#endif

#ifdef _PCFG6
static inline void CONFIG_AN6_AS_ANALOG() {
  CONFIG_RC0_AS_DIG_INPUT();
  _PCFG6 = 0;
}
#endif


#ifdef _PCFG7
static inline void CONFIG_AN7_AS_ANALOG() {
  CONFIG_RC1_AS_DIG_INPUT();
  _PCFG7 = 0;
}
#endif


#ifdef _PCFG8
static inline void CONFIG_AN8_AS_ANALOG() {
  CONFIG_RC2_AS_DIG_INPUT();
  _PCFG8 = 0;
}
#endif


#ifdef _PCFG9
static inline void CONFIG_AN9_AS_ANALOG() {
  CONFIG_RB15_AS_DIG_INPUT();
  _PCFG9 = 0;
}
#endif


#ifdef _PCFG10
static inline void CONFIG_AN10_AS_ANALOG() {
  CONFIG_RB14_AS_DIG_INPUT();
  _PCFG10 = 0;
}
#endif

#ifdef _PCFG11
static inline void CONFIG_AN11_AS_ANALOG() {
  CONFIG_RB13_AS_DIG_INPUT();
  _PCFG11 = 0;
}
#endif

#ifdef _PCFG12
static inline void CONFIG_AN12_AS_ANALOG() {
  CONFIG_RB12_AS_DIG_INPUT();
  _PCFG12 = 0;
}
#endif


//	#define _PIC24_DIGIO_DEFINED

#endif	//	HAL_Pic_24fj64ga004_ports.h