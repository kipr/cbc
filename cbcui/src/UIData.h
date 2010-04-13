#ifndef __UIDATA_H__
#define __UIDATA_H__

typedef struct __UIData
{
  int a_button;
  int b_button;
  int up_button;
  int down_button;
  int left_button;
  int right_button;

  int state;    // 1 = beep, 2 = play sound file, 3 = is playing sound?, 4 = stop playing, 5 = record from mic, 6 = stop recording
  int recording;
  int playing;
  int recordTime;
  char filename[80];
} UIData;

#endif
