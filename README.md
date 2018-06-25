# MyLineWobbler
ぼくのかんがえたさいきょうのらいんわぶらー。This is my line-wobbler clone I think the best.

# 設計

Display:
  GameMaster->MapToArray->DisplayManager->LEDManager
  GameMaster->SoundManager->ToneACWrapper
Input:
  WobbleController->InputManager->GameMaster
Logic:
  Init:
  Main loop:
    Get Input
    Move Object
    Move Enemy
    Move Player
    Set to Display
    Check end or not
      Set to Display
      Set next level
      
Library:
  I2Cdev/MPU6050
  FastLED
  
Hardware:
  MPU6050
  APA102
  ESP32?Teensy?Mega2560?
