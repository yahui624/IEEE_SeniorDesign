#include <SPI.h>
#include <Pixy2.h>

Pixy2 pixy;

int button_Values () {
  static int i = 0;
  int j;
  uint16_t blocks;
  int xTOtal_val = 0;

  int desired_button_x = 133;
  int adjust_dist_x;

  bool sampling = true;
  int counter = 0;
  int16_t x_pos;

  while (sampling) {

    blocks = pixy.ccc.getBlocks();
    if (counter == 20) {
      sampling = false;
    }
    if (blocks)
    {
      i++;
      if (i % 6 == 0) //<<<<< ----- Delay each  6
      {
        Serial.print("\n");
        Serial.print("Detected :");
        Serial.print(blocks);
        Serial.print("\t");


        for (j = 0; j < blocks; j++)
        {
          counter ++ ;
          x_pos = pixy.ccc.blocks[j].m_x;
          Serial.print("Loc X =");
          Serial.print(pixy.ccc.blocks[j].m_x);
          Serial.print("\t");

          Serial.print("Loc Y =");
          Serial.print(pixy.ccc.blocks[j].m_y);
          Serial.print("\t");

          xTOtal_val += x_pos;
        }
      }
    }
  }
  Serial.print("The average x-location is: ");
  Serial.print(xTOtal_val / counter);
  Serial.print("\n");


  adjust_dist_x = (xTOtal_val / counter) - desired_button_x;
  // X-coordinate
  if (adjust_dist_x > 0) {
    Serial.print("The button is to the left of the desired location = ");
    Serial.print(adjust_dist_x);
    Serial.print("\t");
  }
  else if (adjust_dist_x < 0) {
    Serial.print("The button is to the right of the desired location = ");
    Serial.print(adjust_dist_x);
    Serial.print("\t");
  }
  else if (adjust_dist_x == 0) {
    Serial.print("The button is at the desired location! ");
    Serial.print("\t");
  }
  Serial.print("\n");
  return adjust_dist_x;
}

void setup()
{
  Serial.begin(115200);
  Serial.print("Starting...\n");
  pixy.init();

  int button_val = button_Values();
  Serial.print("The button needs to be adjust: ");
  Serial.print(button_val);

}

void loop () {

}
