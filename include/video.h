#ifndef VIDEO_H
#define VIDEO_H

void put_char(char c);
void printk(const char *msg);
void screen_clear();
void panic(const char *panic_message);
void update_cursor();

extern int cursor;
extern int prompt_limit;

#endif 
