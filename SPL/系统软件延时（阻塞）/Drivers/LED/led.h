#ifndef __LED_H
#define __LED_H

#define LED_PIN     GPIO_Pin_13
#define LED_PORT    GPIOC

void LED_Init(void);
void LED_ON(void);
void LED_OFF(void);
void LED_Toggle(void);

#endif
