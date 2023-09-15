//constant holding the value of the desired pin used in place of the mechanical switch
#define ACTPIN 2

//Current Cycles of Current Set
int cycles = 0;
//Total Cylces Over Duration Of Test
int total_cycles = 0;

//The Number Of Cycles Per Set
int set_cycles = 10;

//True if cycles >= set_cycles (test for if the set is complete)
bool isMax = false;

//Used to define the end of overall testing
bool isEnd = false;
bool endSignaled = false;

//Interval and duration in Miliseconds
int interval = 100;

//The cycle "ON" duration in Miliseconds
int duration = 1000;

//Cooldown duration in Miliseconds
int cooldown = 60000;


//Use this int to set a firm limit on test cycles. if test_limit_cycles is set to -1 then test will continue as long as board is supplied power (and int overflow does not ouccur)
int test_limit_cycles = -1;

void setup() {

  //Set ACTPIN to output mode
  pinMode(ACTPIN, OUTPUT);

  //Start logging data via USB serial
  Serial.begin(9600);
}

void loop() {
  if (!isEnd) {
    //Check if the cycle count has been met
    if (cycles >= set_cycles) {
      isMax = true;
      //reset current set cylces to 0
      cycles = 0;
      Serial.println("start cooldown");

      //Init Cooldown
      delay(60000);

      Serial.println("finish cooldown");

      //Basically pointless since its unreachable during delay
      isMax = false;
    }

    //Check if current cycles is greater than or equal to max cycles per set (basically pointless because its unreachable too)
    if (!isMax) {
      //Set the actuation pin to HIGH or tool "ON" emulates closed switch
      digitalWrite(ACTPIN, HIGH);
      //Keep tool in HIGH or "ON" for the on duratiuon
      delay(duration);

      //Set the actuation pin to LOW or tool "OFF" emulates an open switch
      digitalWrite(ACTPIN, LOW);
      //Keep tool in LOW or "OFF" for the off duration
      delay(interval);

      //Count up current cycles and total cycles (Note this does not account for the acutal condition of the tool or if a cycle has actually been completed)
      cycles += 1;
      total_cycles += 1;

      //Serial outputs for
      Serial.println(String(cycles) + "< Cycles This Set");
      Serial.println(String(total_cycles) + " Total Cycles Complete");
    }


    //Check for end of testing
    if(test_limit_cycles != -1 && !isEnd) {
      if(total_cycles >= test_limit_cycles) {
        isEnd = true;
      }
    }


  } else {
    if(!endSignaled) {
      Serial.println("-------------------------");
      Serial.println("Test Complete");
      Serial.println("-------------------------");
      endSignaled = true;
    }
  }
}
