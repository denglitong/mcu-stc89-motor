#include <8051.h>
#include <common.h>

unsigned char beat_code[] = {
    0x0F ^ 0b0001, 0x0F ^ 0b0011, 0x0F ^ 0b0010, 0x0F ^ 0b0110,
    0x0F ^ 0b0100, 0x0F ^ 0b1100, 0x0F ^ 0b1000, 0x0F ^ 0b1001,
};

// 启动电机进行转动
void turn_on_28BYJ() {
  unsigned char P1_HIGH_4BIT = P1 & 0xF0;
  unsigned char i = 0;
  while (1) {
    P1 = P1_HIGH_4BIT | beat_code[i];
    i++;
    // 对 8 取模（只取前 3 位）
    i &= 0x07;
    // 这里延迟的时间间隔需要大于电机的驱动频率对应的单次耗时
    // 550HZ ->1~2ms -> 选用 2ms
    delay_ms(2);
  }
}

// 启动电机进行转动，转过固定角度
//
// 角度 -> 节拍 的转换
// 4 相 8 拍模式下，转子转过一圈需要 8*8 拍
// 28BYJ 减速电机，减速比 1:64，即转子转数 / 64 = 外面转轴转速
void turn_on_28BYJ_with_specific_angle(unsigned long angle) {
  // (64 * angle) / 360 * 64
  unsigned long beats = (angle * 4096) / 360;
  unsigned char P1_HIGH_4BIT = P1 & 0xF0;
  unsigned char i = 0;
  while (beats--) {
    P1 = P1_HIGH_4BIT | beat_code[i];
    i++;
    // 对 8 取模（只取前 3 位）
    i &= 0x07;
    delay_ms(2);
  }
  // stop 28BYJ
  P1 = P1 | 0x0F;
}

int main() {
  // turn_on_28BYJ();
  turn_on_28BYJ_with_specific_angle(90);
  // 需要加这个主循环，不然主板总是不断循环运行 main 函数（可以理解为裸系统）
  while (1) {
  }
}
