unsigned long current_time_timer_1 = 0;        // will store last time LED was updated
unsigned long frequency_of_timer_1 = 1000;
unsigned long current_time_timer_1 = 0;        // will store last time LED was updated
unsigned long frequency_of_timer_1 = 2000; // interval at which to blink (milliseconds)

void setup() 
{
    
}

void loop()
{
 timer_1_function(frequency_of_timer_1);
}


void timer_1_function(unsigned long frequency_of_timer_1 )
{
  time_in_miliseconds = (1/(frequency_of_timer_1))*1000;
 if (millis() - current_time_timer_1 > time_in_miliseconds) 
  {
      current_time_timer_1 =  current_time_timer_1 + time_in_miliseconds;
  }
}

void timer_2_function(unsigned long frequency_of_timer_2 )
{
  time_in_miliseconds = (1/(frequency_of_timer_1))*1000;
 if (millis() - current_time_timer_2 > time_in_miliseconds) 
  {
      current_time_timer_2 =  current_time_timer_2 + time_in_miliseconds;
  }
}