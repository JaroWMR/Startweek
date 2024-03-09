#ifndef LCD_H
#define LCD_H

/* Function prototypes */
void lcd_initialize(void);
void lcd_writeData(char *msg);
void lcd_enable(void);

#endif /* LCD_H */