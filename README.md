# Simplified Mricrocontroller programming
we want to build a device for distance warning in carports. For this task, we want to use an ultrasonic sensor (Type HC-SR04). The Ultrasonic sensor is mounted on the wall of the carport.
<div>
<img width = "600" src="https://user-images.githubusercontent.com/24769585/60543605-ae147980-9d51-11e9-8345-0e196f460f9f.jpg">
</div>

## Details 
<ol>
<li>Build a functioning sample design on the breadboard for 
<ul>
	<li>measuring</li> 
	<li>logging the distance if the distance is more than 0.2m an less than 2m,</li> 
	<li>displaying distances of the ultrasonic sensor and the car. </li>
	<li>two warning lights(LED) if the distance is less than the warning distance (red) and if the distance between warning distance and 1 Meter (green) </li>
</ul>
</li>

<li>
Write a program which display the following Information to an LCD-Module :
<ol>
<li>the distance between the ultrasonic sensor and the car</li> 
<li>the time of correct parking (distance between warning distance and 1 Meter) </li>
</ol>
</li>

<li>Write a program for logging data , time, measured distance to the SD-Card instide the data-logging-shield. Logging on time only if the car is in the correct position. </li>

<li>
Write a program that indicates with LEDs when the car is in the correct position. If there is no car in the carport (garage) both LED should be off. The green LED should switch on if the car arrives the correct position (between warning distance and 1 Meter), the red one should be switched on if the car is closer than the warning distance.</li>

For programing this excercise, you need two additional libraries, "RTClib" for the RTC-Data-logging shield (Chip DS1307), and "UltraDistSensor" for the Ultrasonic-Module (SR004). 
</ol>

## Results
<div>
<img width = "600" src="https://user-images.githubusercontent.com/24769585/60541827-5d028680-9d4d-11e9-8c1b-388034ec25b0.JPG">
</div>
