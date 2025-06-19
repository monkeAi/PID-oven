void handleFlags() {

  if (upFlag) {
    menu.process(UP);
    upFlag = false;
  }
  if (downFlag) {
    menu.process(DOWN);
    downFlag = false;
  }
  if (enterFlag) {
    menu.process(ENTER);
    enterFlag = false;
  }
  if (backFlag) {
    menu.process(BACK);
    backFlag = false;
  }

}

// Flags the process command for LcdMenu to execute because it messes up with interupts from rotary encoder, it's executed in loop()
void flagProcess(const unsigned char command) {
  switch (command) {
    case UP:
      upFlag = true;
      return;
    case DOWN:
      downFlag = true;
      return;
    case ENTER:
      enterFlag = true;
      return;
    case BACK:
      backFlag = true;
      return;
  }
}