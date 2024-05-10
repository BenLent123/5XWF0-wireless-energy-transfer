//constants
unsigned long frequency_of_timers = 1000;
unsigned long dead_time_timer_1 = 10;
unsigned long dead_time_timer_2 = 15;  
//variables
unsigned long current_time_timer_1 = 0; 
unsigned long current_time_timer_2 = 0;      

void setup() 
{
    
}

void loop()
{
 timer_1_function(frequency_of_timers);
 timer_2_function(frequency_of_timers);
}


void timer_1_function(unsigned long frequency_of_timer_1)
{
  time_in_miliseconds = (1/(frequency_of_timer_1))*1000+dead_time_timer_1;
 if (millis() - current_time_timer_1 > time_in_miliseconds) 
  {
      current_time_timer_1 =  current_time_timer_1 + time_in_miliseconds;
  }
}

void timer_2_function(unsigned long frequency_of_timer_2)
{
  time_in_miliseconds = (1/(frequency_of_timer_2))*1000-dead_time_timer_2;
 if (millis() - current_time_timer_2 > time_in_miliseconds) 
  {
      current_time_timer_2 =  current_time_timer_2 + time_in_miliseconds;
  }
}