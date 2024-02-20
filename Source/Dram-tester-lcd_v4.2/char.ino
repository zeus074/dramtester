void finish() {
  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_logisoso16_tr);
  u8g2.drawStr(6, 16, "TEST PASSED");
  u8g2.setFont(u8g2_font_gb16st_t_2);
  u8g2.drawStr(12, 30, "press start");
  u8g2.sendBuffer();
}
