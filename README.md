# Hello World Example

## ESP32 CPU load test without special configuration.

  I wanted a non intrusive CPU load test that I can casually print 
the CPU load on the terminal. And I did not want to enable the
FreeRTOS task stat subsystem.

  The result is extracted to this project.
  
  
 Theory:
 
   We create a low priority (igle) task, and a higher priority 
 (monitor) task.
 
  The low priority task delays 0 ms. We know, the task surrenders its control
of the CPU, will not get it back until the next time slice. (10 msec) But ...
the time value of zero, the scheduler tries to give it as soon as slice is 
available. That is how we know the processor is doing 'stuff'.

 The core of the measuring technique is in three lines:
 
        int64_t now = esp_timer_get_time();     // Time anchor
        vTaskDelay(0 / portTICK_RATE_MS);
        int64_t now2 = esp_timer_get_time();    // The diff is the time away.
        

  Testing:
   
   Every 10 seconds we started a heavy loop. Here is what the console shows:
   
ESP32, 2 CPU cores, WiFi/BT/BLE, silicon revision 1, 4MB external flash
CPU Load Demo.

100%  2%  -0%  -0%  -0%  -0%  -0%  -0%  -0%   work [[ -0%  99%   ]] rest 70%  -0%  -0%  
-0%  -0%  -0%  -0%  -0%  -0%  -0%   work [[ 31%  100%   ]] rest 38%  -0%  -0%  -0%  -0%  
-0%  -0%  -0%  -0%  -0%   work [[ 63%  100%   ]] rest 6%  -0%  -0%  -0%  -0%  -0%  -0%  
-0%  -0%  -0%   work [[ 95%   ]] rest 74%  -0%  -0%  -0%  -0%  -0%  -0%  -0%  -0%  
-0%   work [[ 27%  100%   ]] rest 42%  -0% 

 Note how nicely the sliding time overlap shows.
 
 Peter Glen
 
 Free to copy.
