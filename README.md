## Code implementation explanation
  Because documentation wasn't too clear for me, I decided to prepare in that way:
* BUTTON_EVT_SINGLE_PRESS - when button pressed once and nothing is happening for 2 secs since the first detection of a button falling slope
* BUTTON_EVT_DOUBLE_PRESS - button pressed two times in less than two seconds and nothing is happening for 3 secs since the first detection of a button falling slope
* BUTTON_EVT_TRIPLE_PRESS - button pressed three times in less than three seconds
* BUTTON_EVT_10SEC_HOLD - button pressed for at least 10 seconds
* BUTTON_EVT_30_SEC_HOLD - button pressed for at least 30 seconds
  
To decide which action should be chosen I used only the mechanism of cyclic function calling from the thread.
Also, I haven't used HW debouncer, because it might be not present on the ECU. I decide to use a simple SW debouncer with the value of 50 ms.
I based the behavior of my app on the behavior of for example wireless earbuds, where is also just one button and many different actions to choose from.

Possible scenarios and behavious in application:
![application_workflow](https://github.com/pkazimierowicz/zephyr-test-assignment/blob/MainFunctionalitiesImpl/Zephyr_Workflow.jpg?raw=true)
