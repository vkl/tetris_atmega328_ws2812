/*
 * common.h
 *
 * Created: 3/13/2022 6:08:17 PM
 *  Author: vklad
 */ 


#ifndef COMMON_H_
#define COMMON_H_

#define F_CPU 16000000UL

#include <avr/io.h>


#define HUNDRED(N) ((N/100))
#define TEN(N) ((N/10%10))
#define ONE(N) ((N%10))

#endif /* COMMON_H_ */