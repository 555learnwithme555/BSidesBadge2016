
void registerWrite(int whichPin, int whichState) {
// the bits you want to send
  byte bitsToSend = 0;
  //Serial.println("Setting " + String(whichPin) + " to High");
  // turn off the output so the pins don't light up
  // while you're shifting bits:
  digitalWrite(latchPin, LOW);
 
  // turn on the next highest bit in bitsToSend:
  bitWrite(bitsToSend, whichPin, whichState);

  // shift the bits out:
  shiftOut(dataPin, clockPin, MSBFIRST, bitsToSend);

    // turn on the output so the LEDs can light up:
  digitalWrite(latchPin, HIGH);
  //delay(100);

}

void setOutShift(byte shift)
{
  digitalWrite(latchPin, LOW); 
  shiftOut(dataPin, clockPin,MSBFIRST,  shift); 
  digitalWrite(latchPin, HIGH);

}

void darkness()
{
  byte shift = 0;
  //Serial.println(shift,BIN);
  setOutShift(shift);
}



void twirl(int numTimes = 1)
{
  for(int x=0;x<numTimes;x++)
  {
    for(int i=0;i<=8;i++)
    {
      registerWrite(i,1);
      delay(100);
    }
  }
}


void readShift()
{
   int currentFrame = ui.getUiState()->currentFrame;
String thisBut = "";
  int inputPin = 1;
  int buttonPressedVal = 1; //Depending on how buttons are wired
  digitalWrite(pinStcp, LOW);
  delayMicroseconds(20);
  digitalWrite(pinStcp, HIGH);
  byte buttonVals = 0;
  
  
  for (int i=0; i<8; i++)
  {
    digitalWrite(pinShcp,LOW);
    delayMicroseconds(20);
    inputPin = digitalRead(pinDataIn);
    if(inputPin == buttonPressedVal)
     {
      Serial.println("[.] Button " + String(i) + " pressed!");
      buttonVals = buttonVals | (1 << i);
     }
    
    digitalWrite(pinShcp,HIGH);
  }

   
  if(buttonVals == ((1 << P1_Bottom) | (1<<P2_Bottom)))
  {
    lowPowerMode = true;
    Serial.println("[+] Turning Power Off!");
    display.displayOff();
    darkness();
    
  }
  else if(buttonVals & (1 << P1_Right))
  {
    thisBut = "R";
    display.displayOn();
    ui.nextFrame();
    lowPowerMode = false;
    lastAction = millis();
    
  }
  else if(buttonVals & (1 << P1_Left))
  {
    thisBut = "L";
    display.displayOn();
    ui.previousFrame();
    lowPowerMode = false;
    lastAction = millis();
  }
  else if(buttonVals & (1 << P1_Top))
  {
    thisBut = "U";
    lowPowerMode = false;
    display.displayOn();
    lastAction = millis();
  }
  else if(buttonVals & (1 << P1_Bottom))
  {
    thisBut = "D";
    lowPowerMode = false;
    display.displayOn();
    lastAction = millis();
  }
  else if(buttonVals & (1 << P2_Top))
  {
    thisBut = "B";
    lowPowerMode = false;
    display.displayOn();
    if (currentFrame == 3)
    {
      if(Challenges[currentListedChallenge] == "Konami")
      {
        konamiCode();
      }
      else if(Challenges[currentListedChallenge] == "Alias")
      {
        playAlias();
      }
      else if(completedChallenges > 0)
      {
        playText(Challenges[currentListedChallenge]);
      }
    }
    
    lastAction = millis();
    Serial.print("Free Memory:");
    Serial.println(ESP.getFreeHeap());
    
  }
  else if(buttonVals & (1 << P2_Left))
  {
    thisBut = "A";
    if (currentFrame == 2)
    {
      if(currentScheduleItem < 1)
      {
        currentScheduleItem = numScheduleItems;
      }
      else
      {
        currentScheduleItem--;
      }
      strcpy_P(currentSpeaker, (char*) pgm_read_dword(&(BSidesSchedule[currentScheduleItem])));
    }

    if (currentFrame == 3)
    {
      if(currentListedChallenge < 1)
      {
        currentListedChallenge = completedChallenges - 1;
      }
      else
      {
        currentListedChallenge--;
      }
      
    }
    
    
    lowPowerMode = false;
    display.displayOn();
    lastAction = millis();
  }
  else if(buttonVals & (1 << P2_Bottom))
  {
    thisBut = "C";
    lowPowerMode = false;
    display.displayOn();
    lastAction = millis();
  }
  else if(buttonVals & (1 << P2_Right))
  {
    thisBut = "D";
    if (currentFrame == 2)
    {
      if(currentScheduleItem > numScheduleItems)
      {
        currentScheduleItem = 0;
      }
      else
      {
        currentScheduleItem++;
      }

      strcpy_P(currentSpeaker, (char*) pgm_read_dword(&(BSidesSchedule[currentScheduleItem])));
    }

    if (currentFrame == 3)
    {
      if(currentListedChallenge > completedChallenges-2)
      {
        currentListedChallenge = 0;
      }
      else
      {
        currentListedChallenge++;
      }
      
    }
    
    
    display.displayOn();
    lastAction = millis();
    lowPowerMode = false;
  }



if (thisBut != "")
{
  lastButtons = lastButtons + thisBut;
  if(lastButtons.length() > 10)
  {
    lastButtons = lastButtons.substring(1);
  }
}
//Serial.println(lastButtons + "!");
if(lastButtons == "UUDDLRLRAB")
{
  if(addChallenge("Konami"))
  {
    playNinja();
  }
  lastButtons = "";
}
  
  
  


}
